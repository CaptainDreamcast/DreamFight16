#include "mugenspritefilereader.h"

#include <assert.h>

#include <tari/file.h>
#include <tari/log.h>

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

static MugenSpriteFileSprite* loadTextureFromPCXBuffer(Buffer b) {
	PCXHeader header;
	

	BufferPointer p = getBufferPointer(b);

	readFromBufferPointer(&header, &p, sizeof(PCXHeader));

	int bytesPerPixel = header.mBitsPerPixel / 8;
	int32_t pcxImageSize = bytesPerPixel*header.mHorizontalResolution*header.mVerticalResolution;

	int encodedSize = b.mLength - sizeof(PCXHeader) - 256 * 3;
	Buffer encodedImageBuffer = makeBuffer(p, encodedSize);
	Buffer rawImageBuffer = decodeRLEBufferAndReturnOwnedBuffer(encodedImageBuffer, pcxImageSize);

	int w = header.mHorizontalResolution;
	int h = header.mVerticalResolution;
	int finalWidth = getFinalImageSize(w);
	int finalHeight = getFinalImageSize(h);

	Buffer pcxImageBuffer = makeBufferWithCorrectImageSize(rawImageBuffer, w, h, finalWidth, finalHeight);
	freeBuffer(rawImageBuffer);

	Buffer paletteBuffer = makeBuffer(p + encodedSize, 256 * 3);
	TextureData ret = loadTextureFromPalettedImageData1bpp(pcxImageBuffer, paletteBuffer, finalWidth, finalHeight);
	freeBuffer(pcxImageBuffer);

	MugenSpriteFileSprite* e = allocMemory(sizeof(MugenSpriteFileSprite));
	e->mTexture = ret;
	e->mOriginalTextureSize.x = w;
	e->mOriginalTextureSize.y = h;

	return e;
}

static void insertTextureIntoSpriteFile(MugenSpriteFile* tDst, MugenSpriteFileSprite* tTexture, int tGroup, int tSprite) {

	if (!int_map_contains(&tDst->mGroups, tGroup)) {
		MugenSpriteFileGroup* group = allocMemory(sizeof(MugenSpriteFileGroup));
		group->mSprites = new_int_map();
		int_map_push_owned(&tDst->mGroups, tGroup, group);
	}

	MugenSpriteFileGroup* g = int_map_get(&tDst->mGroups, tGroup);
	
	int_map_push_owned(&g->mSprites, tSprite, tTexture);
}

static void loadSingleSFFFile(Buffer b, BufferPointer* p, MugenSpriteFile* tDst) {
	SFFSubFileHeader subHeader;

	readFromBufferPointer(&subHeader, p, sizeof(SFFSubFileHeader));

	debugInteger(sizeof(SFFSubFileHeader));
	debugPointer(subHeader.mNextFilePosition);
	debugInteger(subHeader.mSubfileLength);
	debugInteger(subHeader.mGroup);
	debugInteger(subHeader.mImage);
	debugString(subHeader.mComments);

	Buffer pcxBuffer;
	pcxBuffer.mData = *p;
	pcxBuffer.mLength = subHeader.mSubfileLength;
	pcxBuffer.mIsOwned = 0;

	MugenSpriteFileSprite* texture = loadTextureFromPCXBuffer(pcxBuffer);
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
	return ret;
}

MugenSpriteFile loadMugenSpriteFile(char * tPath)
{
	debugLog("Loading sprite file.");
	debugString(tPath);
	MugenSpriteFile ret = makeEmptySpriteFile();
	SFFHeader header;

	Buffer b = fileToBuffer(tPath);
	BufferPointer p = getBufferPointer(b);
	
	loadSFFHeader(&p, &header);
	
	debugInteger(header.mGroupAmount);
	debugInteger(header.mImageAmount);
	debugPointer(header.mFirstFileOffset);

	p = ((char*)b.mData) + header.mFirstFileOffset;
	while (p) {
		loadSingleSFFFile(b, &p, &ret);
	}

	freeBuffer(b);

	return ret;
}

MugenSpriteFileSprite* getMugenSpriteFileTextureReference(MugenSpriteFile* tFile, int tGroup, int tSprite)
{
	assert(int_map_contains(&tFile->mGroups, tGroup));

	MugenSpriteFileGroup* g = int_map_get(&tFile->mGroups, tGroup);

	assert(int_map_contains(&g->mSprites, tSprite));

	MugenSpriteFileSprite* e = int_map_get(&g->mSprites, tSprite);
	return e;
}
