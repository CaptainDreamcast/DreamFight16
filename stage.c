#include "stage.h"

#include <stdio.h>
#include <assert.h>

#include <tari/memoryhandler.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/animation.h>
#include <tari/stagehandler.h>

#include "mugendefreader.h"
#include "mugenspritefilereader.h"

Stage gStage;

static void loadStringOrDefault(char* tDst, MugenDefScript* s, char* tGroup, char* tVariable, char* tDefault) {
	if (isMugenDefStringVariable(s, tGroup, tVariable)) {
		char* res = getAllocatedMugenDefStringVariable(s, tGroup, tVariable);
		strcpy(tDst, res);
		freeMemory(res);
	}
	else {
		strcpy(tDst, tDefault);
	}
}

static void loadFloatOrDefault(double* tDst, MugenDefScript* s, char* tGroup, char* tVariable, double tDefault) {
	if (isMugenDefFloatVariable(s, tGroup, tVariable)) {
		*tDst = getMugenDefFloatVariable(s, tGroup, tVariable);
	}
	else {
		*tDst = tDefault;
	}
}

static void loadIntegerOrDefault(int* tDst, MugenDefScript* s, char* tGroup, char* tVariable, int tDefault) {
	if (isMugenDefNumberVariable(s, tGroup, tVariable)) {
		*tDst = getMugenDefNumberVariable(s, tGroup, tVariable);
	}
	else {
		*tDst = tDefault;
	}
}

static void loadVectorOrDefault(Vector3D* tDst, MugenDefScript* s, char* tGroup, char* tVariable, Vector3D tDefault) {
	if (isMugenDefVectorVariable(s, tGroup, tVariable)) {
		*tDst = getMugenDefVectorVariable(s, tGroup, tVariable);
	}
	else {
		*tDst = tDefault;
	}
}

static void loadVectorIOrDefault(Vector3DI* tDst, MugenDefScript* s, char* tGroup, char* tVariable, Vector3DI tDefault) {
	if (isMugenDefVectorIVariable(s, tGroup, tVariable)) {
		*tDst = getMugenDefVectorIVariable(s, tGroup, tVariable);
	}
	else {
		*tDst = tDefault;
	}
}

static void loadStageInfo(MugenDefScript* s) {
	loadStringOrDefault(gStage.mInfo.mName, s, "Info", "name", "");
	loadStringOrDefault(gStage.mInfo.mDisplayName, s, "Info", "displayname", gStage.mInfo.mName);
	loadStringOrDefault(gStage.mInfo.mVersionDate, s, "Info", "versiondate", "1.0");
	loadStringOrDefault(gStage.mInfo.mMugenVersion, s, "Info", "mugenversion", "1.0");
	loadStringOrDefault(gStage.mInfo.mAuthor, s, "Info", "author", "John Doe");
}

static void loadStageCamera(MugenDefScript* s) {
	gStage.mCamera.mStartPosition = makePosition(0, 0, 0);
	loadFloatOrDefault(&gStage.mCamera.mStartPosition.x, s, "Camera", "startx", 0);
	loadFloatOrDefault(&gStage.mCamera.mStartPosition.y, s, "Camera", "starty", 0);
	loadFloatOrDefault(&gStage.mCamera.mBoundLeft, s, "Camera", "boundleft", 0);
	loadFloatOrDefault(&gStage.mCamera.mBoundRight, s, "Camera", "boundright", 0);
	loadFloatOrDefault(&gStage.mCamera.mBoundHigh, s, "Camera", "boundhigh", 0);
	loadFloatOrDefault(&gStage.mCamera.mBoundLow, s, "Camera", "boundlow", 0);
	loadFloatOrDefault(&gStage.mCamera.mTension, s, "Camera", "tension", 0);
	loadFloatOrDefault(&gStage.mCamera.mTensionHigh, s, "Camera", "tensionhigh", 0);
	loadFloatOrDefault(&gStage.mCamera.mTensionLow, s, "Camera", "tensionlow", 0);
	loadFloatOrDefault(&gStage.mCamera.mVerticalFollow, s, "Camera", "verticalfollow", 0);
	loadFloatOrDefault(&gStage.mCamera.mFloorTension, s, "Camera", "floortension", 0);
	loadIntegerOrDefault(&gStage.mCamera.mOverdrawHigh, s, "Camera", "overdrawhigh", 0);
	loadIntegerOrDefault(&gStage.mCamera.mOverdrawLow, s, "Camera", "overdrawlow", 0);
	loadIntegerOrDefault(&gStage.mCamera.mCutHigh, s, "Camera", "cuthigh", 0);
	loadIntegerOrDefault(&gStage.mCamera.mCutLow, s, "Camera", "cutlow", 0);
	loadFloatOrDefault(&gStage.mCamera.mStartZoom, s, "Camera", "startzoom", 1);
	loadFloatOrDefault(&gStage.mCamera.mZoomOut, s, "Camera", "zoomout", 1);
	loadFloatOrDefault(&gStage.mCamera.mZoomIn, s, "Camera", "zoomin", 1);
}

static void loadStagePlayerInfo(MugenDefScript* s) {

	gStage.mPlayerInfo.mP1Start = makePosition(0, 0, 0);
	loadFloatOrDefault(&gStage.mPlayerInfo.mP1Start.x, s, "PlayerInfo", "p1startx", 0);
	loadFloatOrDefault(&gStage.mPlayerInfo.mP1Start.y, s, "PlayerInfo", "p1starty", 0);

	gStage.mPlayerInfo.mP2Start = makePosition(0, 0, 0);
	loadFloatOrDefault(&gStage.mPlayerInfo.mP2Start.x, s, "PlayerInfo", "p2startx", 0);
	loadFloatOrDefault(&gStage.mPlayerInfo.mP2Start.y, s, "PlayerInfo", "p2starty", 0);

	loadIntegerOrDefault(&gStage.mPlayerInfo.mP1Facing, s, "PlayerInfo", "p1facing", 0);
	loadIntegerOrDefault(&gStage.mPlayerInfo.mP2Facing, s, "PlayerInfo", "p2facing", 0);

	loadFloatOrDefault(&gStage.mPlayerInfo.mLeftBound, s, "PlayerInfo", "leftbound", 0);
	loadFloatOrDefault(&gStage.mPlayerInfo.mRightBound, s, "PlayerInfo", "rightbound", 0);
}

static void loadStageBound(MugenDefScript* s) {
	loadFloatOrDefault(&gStage.mBound.mScreenLeft, s, "Bound", "screenleft", 0);
	loadFloatOrDefault(&gStage.mBound.mScreenRight, s, "Bound", "screenright", 0);
}

static void loadStageStageInfo(MugenDefScript* s) {
	loadFloatOrDefault(&gStage.mStageInfo.mZOffset, s, "StageInfo", "zoffset", 0);
	loadIntegerOrDefault(&gStage.mStageInfo.mZOffsetLink, s, "StageInfo", "zoffsetlink", 0);
	loadIntegerOrDefault(&gStage.mStageInfo.mAutoturn, s, "StageInfo", "autoturn", 1);
	loadIntegerOrDefault(&gStage.mStageInfo.mResetBG, s, "StageInfo", "resetBG", 0);
	loadVectorOrDefault(&gStage.mStageInfo.mLocalCoordinates, s, "StageInfo", "localcoord", makePosition(320, 240, 0));
	
	gStage.mStageInfo.mScale = makePosition(1, 1, 1);
	loadFloatOrDefault(&gStage.mStageInfo.mScale.x, s, "StageInfo", "xscale", 1);
	loadFloatOrDefault(&gStage.mStageInfo.mScale.y, s, "StageInfo", "yscale", 1);
}

static void loadStageShadow(MugenDefScript* s) {
	loadIntegerOrDefault(&gStage.mShadow.mIntensity, s, "Shadow", "intensity", 128);
	loadVectorOrDefault(&gStage.mShadow.mColor, s, "StageInfo", "color", makePosition(1, 1, 1));
	loadFloatOrDefault(&gStage.mShadow.mScaleY, s, "StageInfo", "yscale", 1);
	loadVectorIOrDefault(&gStage.mShadow.mFadeRange, s, "StageInfo", "fade.range", makeVector3DI(0, 0, 0));
	loadFloatOrDefault(&gStage.mShadow.mXShear, s, "StageInfo", "xshear", 0);
}

static void loadStageReflection(MugenDefScript* s) {
	loadIntegerOrDefault(&gStage.mReflection.mReflect, s, "Reflection", "reflect", 0);
}

static void loadStageMusic(MugenDefScript* s) {
	loadStringOrDefault(gStage.mMusic.mBGMusic, s, "Music", "bgmusic", "");
	loadIntegerOrDefault(&gStage.mMusic.mBGVolume, s, "Music", "bgvolume", 0);
}

static MugenDefScriptGroup* loadStageBackgroundDefinitionAndReturnGroup(MugenDefScript* s) {
	MugenDefScriptGroup* bgdef;
	char name[100];
	if (string_map_contains(&s->mGroups, "BGDef")) {	
		strcpy(name, "BGDef");
	} else if(string_map_contains(&s->mGroups, "BGdef")) {
		strcpy(name, "BGdef");
	}
	else {
		strcpy(name, "");
		logError("No background definition found.");
		abortSystem();
	}

	bgdef = string_map_get(&s->mGroups, name);

	loadStringOrDefault(gStage.mBackgroundDefinition.mSpritePath, s, name, "spr", "");
	loadIntegerOrDefault(&gStage.mBackgroundDefinition.mDebugBG, s, name, "debugbg", 0);

	return bgdef;
}

static int isBackgroundElementGroup(MugenDefScriptGroup* tGroup) {
	return tGroup->mName[0] == 'B' && tGroup->mName[1] == 'G';
}

static void addBackgroundElementToStageHandler(StageBackgroundElement* e) {
	MugenSpriteFileSprite* tex = getMugenSpriteFileTextureReference(&gStage.mSprites, e->mSpriteNo.x, e->mSpriteNo.y);
	
	double overreach = max(0, tex->mOriginalTextureSize.y - 480);
	Position start = makePosition(0, -overreach, 0);
	Position p = vecAdd(start, e->mStart);

	int i = addScrollingBackgroundWithMovementIn2D(e->mDelta.x, e->mDelta.y, e->mListPosition);
	addBackgroundElementWithTextureData(i, p, &tex->mTexture, createOneFrameAnimation());
}

static void loadBackgroundElement(MugenDefScript* s, char* tName, int i) {
	StageBackgroundElement* e = allocMemory(sizeof(StageBackgroundElement));

	debugLog("Load background element.");
 	debugString(tName);

	char type[100];
	loadStringOrDefault(type, s, tName, "type", "normal");
	if (strcmp("normal", type)) {
		e->mType = STAGE_BACKGROUND_STATIC;
	} else if (strcmp("anim", type)) {
		e->mType = STAGE_BACKGROUND_ANIMATED;
	}
	else {
		logError("Unknown type.");
		logErrorString(type);
		abortSystem();
	}

	loadVectorIOrDefault(&e->mSpriteNo, s, tName, "spriteno", makeVector3DI(0, 0, 0));
	loadIntegerOrDefault(&e->mLayerNo, s, tName, "layerno", 0);
	loadVectorOrDefault(&e->mStart, s, tName, "start", makePosition(0, 0, 0));
	loadVectorOrDefault(&e->mDelta, s, tName, "delta", makePosition(1, 1, 1));
	loadIntegerOrDefault(&e->mMask, s, tName, "mask", 0);
	loadVectorIOrDefault(&e->mTile, s, tName, "tile", makeVector3DI(0, 0, 0));
	loadVectorIOrDefault(&e->mTileSpacing, s, tName, "tilespacing", makeVector3DI(0, 0, 0));
	e->mListPosition = i;

	addBackgroundElementToStageHandler(e);

	list_push_back_owned(&gStage.mBackgroundElements, e);
}

static void loadBackgroundDefinitionGroup(MugenDefScript* s, MugenDefScriptGroup* tGroup, int i) {
	if (isBackgroundElementGroup(tGroup)) {
		loadBackgroundElement(s, tGroup->mName, i);
	}
	else {
		logError("Unknown background definition group.");
		logErrorString(tGroup->mName);
		abortSystem();
	}
}

static void loadStageTextures(char* tPath) {
	char path[1024];
	getPathToFile(path, tPath);
	char sffFile[1024];
	sprintf(sffFile, "%s%s", path, gStage.mBackgroundDefinition.mSpritePath);
	gStage.mSprites = loadMugenSpriteFile(sffFile);
}

static void loadStageBackgroundElements(char* tPath, MugenDefScript* s) {
	MugenDefScriptGroup* bgdef = loadStageBackgroundDefinitionAndReturnGroup(s);

	loadStageTextures(tPath);

	gStage.mBackgroundElements = new_list();
	int i = 0;
	MugenDefScriptGroup* cur = bgdef->mNext;
	while (cur != NULL) {
		loadBackgroundDefinitionGroup(s, cur, i);
		i++;
		cur = cur->mNext;
	}

}

static void setStageCamera() {
	double offX = 160;
	double offY = 0;
	double sizeX = gStage.mCamera.mBoundRight - gStage.mCamera.mBoundLeft;
	double sizeY = gStage.mCamera.mBoundLow - gStage.mCamera.mBoundHigh;
	setStageCameraRange(makeGeoRectangle(gStage.mCamera.mBoundLeft+offX, gStage.mCamera.mBoundHigh+offY, sizeX, sizeY));
}


void loadStageFromMugenDefinition(char * tPath)
{
	MugenDefScript s = loadMugenDefScript(tPath);

	loadStageInfo(&s);
	loadStageCamera(&s);
	loadStagePlayerInfo(&s);
	loadStageBound(&s);
	loadStageStageInfo(&s);
	loadStageShadow(&s);
	loadStageReflection(&s);
	loadStageMusic(&s);

	loadStageBackgroundElements(tPath, &s);

	setStageCamera();
}
