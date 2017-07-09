#pragma once

#include <tari/datastructures.h>
#include <tari/texture.h>


typedef struct {
	TextureData mTexture;
	TextureSize mOriginalTextureSize;
} MugenSpriteFileSprite;

typedef struct {
	IntMap mSprites;

} MugenSpriteFileGroup;

typedef struct {
	IntMap mGroups;
} MugenSpriteFile;


MugenSpriteFile loadMugenSpriteFile(char* tPath);
MugenSpriteFileSprite* getMugenSpriteFileTextureReference(MugenSpriteFile* tFile, int tGroup, int tSprite);