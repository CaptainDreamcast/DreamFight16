#pragma once

#include <tari/datastructures.h>
#include <tari/texture.h>


typedef struct {
	TextureData mTexture;
	TextureSize mOriginalTextureSize;
	int mIsLinked;
	int mIsLinkedTo;

	Vector3D mAxisOffset;
} MugenSpriteFileSprite;

typedef struct {
	IntMap mSprites;

} MugenSpriteFileGroup;

typedef struct {
	IntMap mGroups;
	Vector mAllSprites;
	Vector mPalettes;
} MugenSpriteFile;


MugenSpriteFile loadMugenSpriteFile(char * tPath, int tPreferredPalette, int tHasPaletteFile, char* tOptionalPaletteFile);
MugenSpriteFile loadMugenSpriteFileWithoutPalette(char* tPath);
MugenSpriteFileSprite* getMugenSpriteFileTextureReference(MugenSpriteFile* tFile, int tGroup, int tSprite);