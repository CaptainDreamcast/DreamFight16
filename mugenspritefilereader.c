#include "mugenspritefilereader.h"

#include <assert.h>
#include <string.h>

#include <tari/file.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/texture.h>
#include <tari/math.h>

#include "lz5.h"

typedef struct {
	char mSignature[12];
	uint8_t mVersion[4];
} SFFSharedHeader;

typedef struct {
	char mSignature[12];
	char mVersion[4];
	
	uint32_t mReserved1;
	uint32_t mReserved2;

	uint8_t mCompatibleVersion[4];

	uint32_t mReserved3;
	uint32_t mReserved4;

	uint32_t mSpriteOffset;
	uint32_t mSpriteTotal;

	uint32_t mPaletteOffset;
	uint32_t mPaletteTotal;

	uint32_t mLDataOffset;
	uint32_t mLDataLength;

	uint32_t mTDataOffset;
	uint32_t mTDataLength;

	uint32_t mReserved5;
	uint32_t mReserved6;

	char mComments[436];
} SFFHeader2;

typedef struct {
	uint16_t mGroupNo;
	uint16_t mItemNo;
	uint16_t mNumCols;
	uint16_t mIndex;

	uint32_t mDataOffset;
	uint32_t mDataLength;
} SFFPalette2;

typedef struct {
	uint16_t mGroupNo;
	uint16_t mItemNo;
	uint16_t mWidth;
	uint16_t mHeight;
	
	uint16_t mAxisX;
	uint16_t mAxisY;

	uint16_t mIndex;

	uint8_t mFormat;
	uint8_t mColorDepth;

	uint32_t mDataOffset;
	uint32_t mDataLength;

	uint16_t mPaletteIndex;
	uint16_t mFlags;
} SFFSprite2;

typedef struct {
	char mSignature[12];
	char mVersion[4];
	int32_t mGroupAmount;
	int32_t mImageAmount;
	int32_t mFirstFileOffset;
	int32_t mSubheaderSize;
	int8_t mPaletteType;
	char mBlank[3];
	char mComments[476];
} SFFHeader;


typedef struct {
	int32_t mNextFilePosition;
	int32_t mSubfileLength;
	int16_t mImageAxisXCoordinate;
	int16_t mImageAxisYCoordinate;
	int16_t mGroup;
	int16_t mImage;
	int16_t mIndexOfPreciousSpriteCopy;
	int8_t mHasSamePaletteAsPreviousImage;
	char mComments[12];
} SFFSubFileHeader;

typedef struct {
	uint8_t mZSoftID;
	uint8_t mVersion;
	uint8_t mEncoding;
	uint8_t mBitsPerPixel;

	int16_t mMinX;
	int16_t mMinY;
	int16_t mMaxX;
	int16_t mMaxY;

	int16_t mHorizontalResolution;
	int16_t mVerticalResolution;

	uint8_t mHeaderPalette[48];
	uint8_t mReserved;
	uint8_t mPlaneAmount;
	uint16_t mBytesPerLine;
	uint16_t mPaletteInfo;
	char mFiller[58];
} PCXHeader;

static uint32_t get2DBufferIndex(uint32_t i, uint32_t j, uint32_t w) {
	return j * w + i;
}

static Buffer makeBufferWithCorrectImageSize(Buffer b, int w, int h, int fw, int fh) {
	char* output = allocMemory(fw*fh);
	


	int i, j;
	for (j = 0; j < fh; j++) {
		for (i = 0; i < fw; i++) {
			if (j < h && i < w) {
				assert(get2DBufferIndex(i, j, fw) < (uint32_t)(fw*fh));
				assert(get2DBufferIndex(i, j, w) < (uint32_t)b.mLength);
				output[get2DBufferIndex(i, j, fw)] = ((char*)b.mData)[get2DBufferIndex(i, j, w)];
			}
			else {
				assert(get2DBufferIndex(i, j, fw) < (uint32_t)(fw*fh));
				output[get2DBufferIndex(i, j, fw)] = 0;
			}
		}
	}

	return makeBufferOwned(output, fw*fh);
}

static int getFinalImageSize(int w) {
	int s = 1;
	while (s < w) {
		s *= 2;
	}

	return s;
}

static TextureData loadTextureFromPalettedImageData1bpp(Buffer tPCXImageBuffer, Buffer tPaletteBuffer, int w, int h) {
	uint8_t* output = allocMemory(w*h*4);
	uint8_t* img = (uint8_t*)tPCXImageBuffer.mData;
	uint8_t* pal = (uint8_t*)tPaletteBuffer.mData;

	int i, j;
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			uint8_t pid = img[get2DBufferIndex(i, j, w)];
			output[get2DBufferIndex(i, j, w) * 4 + 0] = pal[pid * 3 + 2];
			output[get2DBufferIndex(i, j, w) * 4 + 1] = pal[pid * 3 + 1];
			output[get2DBufferIndex(i, j, w) * 4 + 2] = pal[pid * 3 + 0];
			output[get2DBufferIndex(i, j, w) * 4 + 3] = pid == 0 ? 0 : 0xFF;
		}
	}

	Buffer b = makeBufferOwned(output, w*h*4);

	TextureData ret = loadTextureFromARGB32Buffer(b, w, h);

	freeBuffer(b);

	return ret;
}

static Buffer decodeRLEBufferAndReturnOwnedBuffer(Buffer b, int tFinalSize) {
	uint8_t* output = allocMemory(tFinalSize+10);
	uint8_t* input = b.mData;

	int ip;
	int op = 0;
	for (ip = 0; ip < (int)b.mLength; ip++) {
		uint8_t cur = input[ip];
	
		if ((cur & 0xC0) == 0xC0) { // decode
			int steps = cur & 0x3F;
			ip++;
			uint8_t val = input[ip];
			int k;
			for (k = 0; k < steps; k++) output[op++] = val;
		}
		else {
			output[op++] = cur;
		}

		
	}

	return makeBufferOwned(output, tFinalSize);
}

static Buffer decodeRLE8BufferAndReturnOwnedBuffer(Buffer b, int tFinalSize) {
	uint8_t* output = allocMemory(tFinalSize + 10);
	uint8_t* input = b.mData;

	uint32_t dstpos = 0;
	uint32_t srcpos = 0;

	while (srcpos < (int)b.mLength)
	{
		if (((input[srcpos] & 0xC0) == 0x40))
		{
			int run;
			for (run = 0; run < (input[srcpos] & 0x3F); run++)
			{
				output[dstpos] = input[srcpos + 1];
				dstpos++;
			}
			srcpos += 2;
		}
		else
		{
			output[dstpos] = input[srcpos];
			dstpos++;
			srcpos++;
		}
	}

	return makeBufferOwned(output, tFinalSize);
}

static MugenSpriteFileSprite* makeMugenSpriteFileSprite(TextureData tTexture, TextureSize tOriginalTextureSize, Vector3D tAxisOffset) {
	MugenSpriteFileSprite* e = allocMemory(sizeof(MugenSpriteFileSprite));
	e->mTexture = tTexture;
	e->mOriginalTextureSize = tOriginalTextureSize;
	e->mIsLinked = 0;
	e->mIsLinkedTo = 0;
	e->mAxisOffset = tAxisOffset;
	return e;
}

static MugenSpriteFileSprite* makeLinkedMugenSpriteFileSprite(int tIsLinkedTo) {
	MugenSpriteFileSprite* e = allocMemory(sizeof(MugenSpriteFileSprite));
	e->mIsLinked = 1;
	e->mIsLinkedTo = tIsLinkedTo;
	return e;
}

static Buffer* loadPCXPaletteToAllocatedBuffer(BufferPointer p, int tEncodedSize) {
	Buffer* insertPal = allocMemory(sizeof(Buffer));
	
	int size = 256 * 3;
	char* data = allocMemory(size);
	memcpy(data, p + tEncodedSize, size);
	
	*insertPal = makeBufferOwned(data, size);
	return insertPal;
}

static MugenSpriteFileSprite* loadTextureFromPCXBuffer(MugenSpriteFile* tDst, int mIsUsingOwnPalette, Buffer b, Vector3D tAxisOffset) { // TODO: refactor
	PCXHeader header;
	

	BufferPointer p = getBufferPointer(b);

	readFromBufferPointer(&header, &p, sizeof(PCXHeader));

	int bytesPerPixel = header.mBitsPerPixel / 8;
	int w = header.mMaxX - header.mMinX + 1;
	int h = header.mMaxY - header.mMinY + 1;
	int finalWidth = getFinalImageSize(w);
	int finalHeight = getFinalImageSize(h);
	int32_t pcxImageSize = bytesPerPixel*w*h;

	printf("load sprite1: %d/%d %d/%d\n", w, finalWidth, h, finalHeight);

	assert(header.mBitsPerPixel == 8);
	assert(header.mEncoding == 1);
	assert(header.mPlaneAmount == 1);
	assert(header.mBytesPerLine == w);

	int encodedSize = b.mLength - sizeof(PCXHeader) - 256 * 3;
	Buffer encodedImageBuffer = makeBuffer(p, encodedSize);
	Buffer rawImageBuffer = decodeRLEBufferAndReturnOwnedBuffer(encodedImageBuffer, pcxImageSize);

	Buffer pcxImageBuffer = makeBufferWithCorrectImageSize(rawImageBuffer, w, h, finalWidth, finalHeight);
	freeBuffer(rawImageBuffer);


	if (mIsUsingOwnPalette) {
		Buffer* insertPalette = loadPCXPaletteToAllocatedBuffer(p, encodedSize);
		vector_push_back_owned(&tDst->mPalettes, insertPalette);
	}

	Buffer* paletteBuffer = vector_get_back(&tDst->mPalettes);

	TextureData ret = loadTextureFromPalettedImageData1bpp(pcxImageBuffer, *paletteBuffer, finalWidth, finalHeight);
	freeBuffer(pcxImageBuffer);

	return makeMugenSpriteFileSprite(ret, makeTextureSize(w, h), tAxisOffset);
}

static void insertTextureIntoSpriteFile(MugenSpriteFile* tDst, MugenSpriteFileSprite* tTexture, int tGroup, int tSprite) {

	if (!int_map_contains(&tDst->mGroups, tGroup)) {
		MugenSpriteFileGroup* group = allocMemory(sizeof(MugenSpriteFileGroup));
		group->mSprites = new_int_map();
		int_map_push_owned(&tDst->mGroups, tGroup, group);
	}

	MugenSpriteFileGroup* g = int_map_get(&tDst->mGroups, tGroup);
	
	int_map_push_owned(&g->mSprites, tSprite, tTexture);
	vector_push_back(&tDst->mAllSprites, tTexture);
}

static int gPreviousGroup;

static void loadSingleSFFFile(Buffer b, BufferPointer* p, MugenSpriteFile* tDst) {
	SFFSubFileHeader subHeader;

	readFromBufferPointer(&subHeader, p, sizeof(SFFSubFileHeader));

	debugInteger(sizeof(SFFSubFileHeader));
	debugPointer(subHeader.mNextFilePosition);
	debugInteger(subHeader.mSubfileLength);
	debugInteger(subHeader.mGroup);
	debugInteger(subHeader.mImage);
	debugString(subHeader.mComments);

	MugenSpriteFileSprite* texture;

	if (subHeader.mSubfileLength) {
		Buffer pcxBuffer;
		pcxBuffer.mData = *p;
		pcxBuffer.mLength = subHeader.mSubfileLength;
		pcxBuffer.mIsOwned = 0;

		int isUsingOwnPalette = !subHeader.mHasSamePaletteAsPreviousImage;
		gPreviousGroup = subHeader.mGroup;

		texture = loadTextureFromPCXBuffer(tDst, isUsingOwnPalette, pcxBuffer, makePosition(subHeader.mImageAxisXCoordinate, subHeader.mImageAxisYCoordinate, 0));
	}
	else {
		texture = makeLinkedMugenSpriteFileSprite(subHeader.mIndexOfPreciousSpriteCopy);
	}

	insertTextureIntoSpriteFile(tDst, texture, subHeader.mGroup, subHeader.mImage);

	if (!subHeader.mNextFilePosition) {
		*p = NULL;
	}
	else {
		*p = ((char*)b.mData) + subHeader.mNextFilePosition;
		if (*p >= ((char*)b.mData) + b.mLength) {
			*p = NULL;
		}
	}

}

static void loadSFFHeader(BufferPointer* p, SFFHeader* tHeader) {
	readFromBufferPointer(tHeader, p, sizeof(SFFHeader));
}

static MugenSpriteFile makeEmptySpriteFile() {
	MugenSpriteFile ret;
	ret.mGroups = new_int_map();
	ret.mAllSprites = new_vector();
	ret.mPalettes = new_vector();
	return ret;
}

static void loadMugenSpriteFilePaletteFile(MugenSpriteFile* ret, char* tPath) {
	assert(isFile(tPath));

	Buffer* b = allocMemory(sizeof(Buffer));
	*b = fileToBuffer(tPath);
	assert(b->mLength == 256 * 3);

	vector_push_back_owned(&ret->mPalettes, b);
}

static MugenSpriteFile loadMugenSpriteFile1(Buffer b, int tHasPaletteFile, char* tOptionalPaletteFile) {
	MugenSpriteFile ret = makeEmptySpriteFile();
	
	if (tHasPaletteFile) {
		loadMugenSpriteFilePaletteFile(&ret, tOptionalPaletteFile);
	}
	
	BufferPointer p = getBufferPointer(b);

	SFFHeader header;
	loadSFFHeader(&p, &header);

	debugInteger(header.mGroupAmount);
	debugInteger(header.mImageAmount);
	debugPointer(header.mFirstFileOffset);

	gPreviousGroup = -1;

	p = ((char*)b.mData) + header.mFirstFileOffset;
	while (p) {
		loadSingleSFFFile(b, &p, &ret);
	}

	return ret;
}

static void loadSFFHeader2(BufferPointer* p, SFFHeader2* tHeader) {
	readFromBufferPointer(tHeader, p, sizeof(SFFHeader2));
}

static Buffer processRawPalette2(Buffer tRaw) {
	int n = tRaw.mLength / 4;

	assert(n <= 256);
	char* raw = (char*)tRaw.mData;
	char* out = allocMemory(256 * 3);
	memset(out, 0, 256 * 3);

	int i = 0;
	for (i = 0; i < n; i++) {
		out[3 * i + 0] = raw[4 * i +  0];
		out[3 * i + 1] = raw[4 * i + 1];
		out[3 * i + 2] = raw[4 * i + 2];
	}


	return makeBufferOwned(out, 256 * 3);
}

static void loadSinglePalette2(Buffer b, BufferPointer* p, SFFHeader2* tHeader, MugenSpriteFile* tDst) {
	SFFPalette2 palette;
	readFromBufferPointer(&palette, p, sizeof(SFFPalette2));

	debugPointer(palette.mDataOffset);
	debugInteger(palette.mDataLength);
	debugInteger(palette.mIndex);

	BufferPointer data = getBufferPointer(b);
	data += tHeader->mLDataOffset + palette.mDataOffset;

	Buffer rawPalette = makeBuffer(data, palette.mDataLength);
	Buffer* processedPalette = allocMemory(sizeof(Buffer));
	*processedPalette = processRawPalette2(rawPalette);

	vector_push_back_owned(&tDst->mPalettes, processedPalette);
}

static void loadPalettes2(Buffer b, SFFHeader2* tHeader, MugenSpriteFile* tDst) {
	int i = 0;

	BufferPointer p = getBufferPointer(b);
	p += tHeader->mPaletteOffset;
	for (i = 0; i < (int)tHeader->mPaletteTotal; i++) {
		loadSinglePalette2(b, &p, tHeader, tDst);
	}
}


static Buffer readRawRLE8Sprite2(BufferPointer p, uint32_t tSize) {
	uint32_t decompressedSize = 0;
	readFromBufferPointer(&decompressedSize, &p, sizeof(uint32_t));

	Buffer b = makeBuffer(p, tSize - 4);
	return decodeRLE8BufferAndReturnOwnedBuffer(b, decompressedSize);
}

static Buffer decodeLZ5BufferAndReturnOwnedBuffer(Buffer b, uint32_t tFinalSize) {
	uint8_t* out = allocMemory(tFinalSize + 10);
	uint8_t* src = b.mData;

	decompressLZ5(out, src, b.mLength);

	return makeBufferOwned(out, tFinalSize);
}

static Buffer readRawLZ5Sprite2(BufferPointer p, uint32_t tSize) {
	uint32_t decompressedSize = 0;
	readFromBufferPointer(&decompressedSize, &p, sizeof(uint32_t));

	Buffer b = makeBuffer(p, tSize - 4);
	return decodeLZ5BufferAndReturnOwnedBuffer(b, decompressedSize);

}


static Buffer readRawSprite2(Buffer b, SFFSprite2* tSprite, SFFHeader2* tHeader) {
	BufferPointer p = getBufferPointer(b);
	uint32_t realOffset;
	if (tSprite->mFlags) realOffset = tHeader->mTDataOffset;
	else realOffset = tHeader->mLDataOffset;

	p += realOffset + tSprite->mDataOffset;

	if (tSprite->mFormat == 2) {		
		return readRawRLE8Sprite2(p, tSprite->mDataLength);
	} else if (tSprite->mFormat == 4) {
		return readRawLZ5Sprite2(p, tSprite->mDataLength);
	}
	else {
		logError("Unable to parse sprite format.");
		logErrorInteger(tSprite->mFormat);
		abortSystem();
		return b;
	}
	


}

static void insertLinkedTextureIntoSpriteFile2(MugenSpriteFile* tDst, SFFSprite2* tSprite) {
	MugenSpriteFileSprite* e = makeLinkedMugenSpriteFileSprite(tSprite->mIndex);
	insertTextureIntoSpriteFile(tDst, e, tSprite->mGroupNo, tSprite->mItemNo);
}

static void loadSingleSprite2(Buffer b, BufferPointer* p, SFFHeader2* tHeader, MugenSpriteFile* tDst, int tPreferredPalette) {
	SFFSprite2 sprite;
	readFromBufferPointer(&sprite, p, sizeof(SFFSprite2));

	debugLog("Load sprite2");
	debugInteger(sprite.mGroupNo);
	debugInteger(sprite.mItemNo);
	debugInteger(sprite.mFormat);

	if (!sprite.mDataLength) {
		insertLinkedTextureIntoSpriteFile2(tDst, &sprite);
		return;
	}

	
	int finalWidth = getFinalImageSize(sprite.mWidth);
	int finalHeight = getFinalImageSize(sprite.mHeight);

	Buffer rawBuffer = readRawSprite2(b, &sprite, tHeader);

	Buffer imageBuffer = makeBufferWithCorrectImageSize(rawBuffer, sprite.mWidth, sprite.mHeight, finalWidth, finalHeight);
	freeBuffer(rawBuffer);

	int palette;
	tPreferredPalette = -1; // TODO: fix
	if (tPreferredPalette == -1) palette = sprite.mPaletteIndex;
	else palette = tPreferredPalette;

	Buffer* paletteBuffer = vector_get(&tDst->mPalettes, palette);

	TextureData ret = loadTextureFromPalettedImageData1bpp(imageBuffer, *paletteBuffer, finalWidth, finalHeight);
	freeBuffer(imageBuffer);


	MugenSpriteFileSprite* e = makeMugenSpriteFileSprite(ret, makeTextureSize(sprite.mWidth, sprite.mHeight), makePosition(sprite.mAxisX, sprite.mAxisY, 0));
	insertTextureIntoSpriteFile(tDst, e, sprite.mGroupNo, sprite.mItemNo);
}

static void loadSprites2(Buffer b, SFFHeader2* tHeader, MugenSpriteFile* tDst, int tPreferredPalette) {
	int i = 0;

	BufferPointer p = getBufferPointer(b);
	p += tHeader->mSpriteOffset;
	for (i = 0; i < (int)tHeader->mSpriteTotal; i++) {
		loadSingleSprite2(b, &p, tHeader, tDst, tPreferredPalette);
	}
}

static MugenSpriteFile loadMugenSpriteFile2(Buffer b, int tPreferredPalette, int tHasPaletteFile, char* tOptionalPaletteFile) {
	MugenSpriteFile ret = makeEmptySpriteFile();

	if (tHasPaletteFile) {
		loadMugenSpriteFilePaletteFile(&ret, tOptionalPaletteFile);
	}

	BufferPointer p = getBufferPointer(b);

	SFFHeader2 header;
	loadSFFHeader2(&p, &header);

	debugPointer(header.mSpriteOffset);
	debugInteger(header.mSpriteTotal);
	debugPointer(header.mPaletteOffset);
	debugInteger(header.mPaletteTotal);

	loadPalettes2(b, &header, &ret);
	tPreferredPalette = min(tPreferredPalette, vector_size(&ret.mPalettes) - 1);
	loadSprites2(b, &header, &ret, tPreferredPalette);

	return ret;
}

MugenSpriteFile loadMugenSpriteFile(char * tPath, int tPreferredPalette, int tHasPaletteFile, char* tOptionalPaletteFile)
{
	debugLog("Loading sprite file.");
	debugString(tPath);

	MugenSpriteFile ret;
	Buffer b = fileToBuffer(tPath);
	BufferPointer p = getBufferPointer(b);

	SFFSharedHeader header;
	readFromBufferPointer(&header, &p, sizeof(SFFSharedHeader));

	if (header.mVersion[1] == 1 && header.mVersion[3] == 1) {
		ret = loadMugenSpriteFile1(b, tHasPaletteFile, tOptionalPaletteFile);
	} else if (header.mVersion[1] == 1 && header.mVersion[3] == 2) {
		ret = loadMugenSpriteFile2(b, tPreferredPalette, tHasPaletteFile, tOptionalPaletteFile);
	}
	else if (header.mVersion[1] == 0 && header.mVersion[3] == 2) {
		ret = loadMugenSpriteFile2(b, tPreferredPalette, tHasPaletteFile, tOptionalPaletteFile);
	}
	else {
		logError("Unrecognized SFF version.");
		logErrorInteger(header.mVersion[0]);
		logErrorInteger(header.mVersion[1])  ;
		logErrorInteger(header.mVersion[2]);
		logErrorInteger(header.mVersion[3]);
		abortSystem();
	}

	freeBuffer(b);
	
	return ret;
}

MugenSpriteFile loadMugenSpriteFileWithoutPalette(char * tPath)
{
	return loadMugenSpriteFile(tPath, -1, 0, NULL);
}

MugenSpriteFileSprite* getMugenSpriteFileTextureReference(MugenSpriteFile* tFile, int tGroup, int tSprite)
{
	if (!int_map_contains(&tFile->mGroups, tGroup)) return NULL;

	MugenSpriteFileGroup* g = int_map_get(&tFile->mGroups, tGroup);

	if (!int_map_contains(&g->mSprites, tSprite)) return NULL;

	MugenSpriteFileSprite* e = int_map_get(&g->mSprites, tSprite);
	while (e->mIsLinked) {
		e = vector_get(&tFile->mAllSprites, e->mIsLinkedTo);
	}
	return e;
}
