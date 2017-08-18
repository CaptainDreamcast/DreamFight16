#include "stage.h"

#include <stdio.h>
#include <assert.h>

#include <tari/memoryhandler.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/animation.h>
#include <tari/stagehandler.h>
#include <tari/math.h>
#include <tari/input.h>

#include "mugendefreader.h"
#include "mugenspritefilereader.h"
#include "playerdefinition.h"
#include "mugenstagehandler.h"

typedef struct {
	char mName[1024];
	char mDisplayName[1024];
	char mVersionDate[100];
	char mMugenVersion[100];
	char mAuthor[100];
} StageInfo;

typedef struct {
	Position mStartPosition;
	double mBoundLeft;
	double mBoundRight;
	double mBoundHigh;
	double mBoundLow;

	double mTension;
	double mTensionHigh;
	double mTensionLow;

	double mVerticalFollow;
	double mFloorTension;

	int mOverdrawHigh;
	int mOverdrawLow;
	int mCutHigh;
	int mCutLow;

	double mStartZoom;
	double mZoomOut;
	double mZoomIn;

} StageCamera;

typedef struct {
	Position mP1Start;
	Position mP2Start;

	int mP1Facing;
	int mP2Facing;

	double mLeftBound;
	double mRightBound;

} StagePlayerInfo;

typedef struct {
	double mScreenLeft;
	double mScreenRight;
} StageBound;

typedef struct {
	double mZOffset;
	int mZOffsetLink;
	int mAutoturn;
	int mResetBG;
	Vector3DI mLocalCoordinates;
	Vector3D mScale;

} StageStageInfo;

typedef struct {
	int mIntensity;
	Vector3D mColor;
	double mScaleY;
	Vector3DI mFadeRange;
	double mXShear;

} StageShadow;

typedef struct {
	int mReflect;
} StageReflection;

typedef struct {
	char mBGMusic[1024];
	int mBGVolume;
} StageMusic;

typedef struct {
	char mSpritePath[1024];
	int mDebugBG;

} StageBackgroundDefinition;

typedef enum {
	STAGE_BACKGROUND_STATIC,
	STAGE_BACKGROUND_ANIMATED,
} StageBackgroundType;

typedef struct {
	StageBackgroundType mType;
	Vector3DI mSpriteNo;
	int mLayerNo;
	Position mStart;
	Position mDelta;
	int mMask;
	Vector3DI mTile;
	Vector3DI mTileSpacing;
	int mListPosition;

	int mActionNumber;
} StageBackgroundElement;

static struct {
	StageInfo mInfo;
	StageCamera mCamera;
	StagePlayerInfo mPlayerInfo;
	StageBound mBound;
	StageStageInfo mStageInfo;
	StageShadow mShadow;
	StageReflection mReflection;
	StageMusic mMusic;
	StageBackgroundDefinition mBackgroundDefinition;

	List mBackgroundElements;

	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	char mDefinitionPath[1024];
} gData;

static void loadStageInfo(MugenDefScript* s) {
	loadStringOrDefault(gData.mInfo.mName, s, "Info", "name", "");
	loadStringOrDefault(gData.mInfo.mDisplayName, s, "Info", "displayname", gData.mInfo.mName);
	loadStringOrDefault(gData.mInfo.mVersionDate, s, "Info", "versiondate", "1.0");
	loadStringOrDefault(gData.mInfo.mMugenVersion, s, "Info", "mugenversion", "1.0");
	loadStringOrDefault(gData.mInfo.mAuthor, s, "Info", "author", "John Doe");
}

static void loadStageCamera(MugenDefScript* s) {
	gData.mCamera.mStartPosition = makePosition(0, 0, 0);
	loadFloatOrDefault(&gData.mCamera.mStartPosition.x, s, "Camera", "startx", 0);
	loadFloatOrDefault(&gData.mCamera.mStartPosition.y, s, "Camera", "starty", 0);
	loadFloatOrDefault(&gData.mCamera.mBoundLeft, s, "Camera", "boundleft", 0);
	loadFloatOrDefault(&gData.mCamera.mBoundRight, s, "Camera", "boundright", 0);
	loadFloatOrDefault(&gData.mCamera.mBoundHigh, s, "Camera", "boundhigh", 0);
	loadFloatOrDefault(&gData.mCamera.mBoundLow, s, "Camera", "boundlow", 0);
	loadFloatOrDefault(&gData.mCamera.mTension, s, "Camera", "tension", 0);
	loadFloatOrDefault(&gData.mCamera.mTensionHigh, s, "Camera", "tensionhigh", 0);
	loadFloatOrDefault(&gData.mCamera.mTensionLow, s, "Camera", "tensionlow", 0);
	loadFloatOrDefault(&gData.mCamera.mVerticalFollow, s, "Camera", "verticalfollow", 0);
	loadFloatOrDefault(&gData.mCamera.mFloorTension, s, "Camera", "floortension", 0);
	loadIntegerOrDefault(&gData.mCamera.mOverdrawHigh, s, "Camera", "overdrawhigh", 0);
	loadIntegerOrDefault(&gData.mCamera.mOverdrawLow, s, "Camera", "overdrawlow", 0);
	loadIntegerOrDefault(&gData.mCamera.mCutHigh, s, "Camera", "cuthigh", 0);
	loadIntegerOrDefault(&gData.mCamera.mCutLow, s, "Camera", "cutlow", 0);
	loadFloatOrDefault(&gData.mCamera.mStartZoom, s, "Camera", "startzoom", 1);
	loadFloatOrDefault(&gData.mCamera.mZoomOut, s, "Camera", "zoomout", 1);
	loadFloatOrDefault(&gData.mCamera.mZoomIn, s, "Camera", "zoomin", 1);
}

static void loadStagePlayerInfo(MugenDefScript* s) {

	gData.mPlayerInfo.mP1Start = makePosition(0, 0, 0);
	loadFloatOrDefault(&gData.mPlayerInfo.mP1Start.x, s, "PlayerInfo", "p1startx", 0);
	loadFloatOrDefault(&gData.mPlayerInfo.mP1Start.y, s, "PlayerInfo", "p1starty", 0);

	gData.mPlayerInfo.mP2Start = makePosition(0, 0, 0);
	loadFloatOrDefault(&gData.mPlayerInfo.mP2Start.x, s, "PlayerInfo", "p2startx", 0);
	loadFloatOrDefault(&gData.mPlayerInfo.mP2Start.y, s, "PlayerInfo", "p2starty", 0);

	loadIntegerOrDefault(&gData.mPlayerInfo.mP1Facing, s, "PlayerInfo", "p1facing", 0);
	loadIntegerOrDefault(&gData.mPlayerInfo.mP2Facing, s, "PlayerInfo", "p2facing", 0);

	loadFloatOrDefault(&gData.mPlayerInfo.mLeftBound, s, "PlayerInfo", "leftbound", 0);
	loadFloatOrDefault(&gData.mPlayerInfo.mRightBound, s, "PlayerInfo", "rightbound", 0);
}

static void loadStageBound(MugenDefScript* s) {
	loadFloatOrDefault(&gData.mBound.mScreenLeft, s, "Bound", "screenleft", 0);
	loadFloatOrDefault(&gData.mBound.mScreenRight, s, "Bound", "screenright", 0);
}

static void loadStageStageInfo(MugenDefScript* s) {
	loadFloatOrDefault(&gData.mStageInfo.mZOffset, s, "StageInfo", "zoffset", 0);
	loadIntegerOrDefault(&gData.mStageInfo.mZOffsetLink, s, "StageInfo", "zoffsetlink", 0);
	loadIntegerOrDefault(&gData.mStageInfo.mAutoturn, s, "StageInfo", "autoturn", 1);
	loadIntegerOrDefault(&gData.mStageInfo.mResetBG, s, "StageInfo", "resetBG", 0);
	loadVectorIOrDefault(&gData.mStageInfo.mLocalCoordinates, s, "StageInfo", "localcoord", makeVector3DI(320, 240, 0));

	gData.mStageInfo.mScale = makePosition(1, 1, 1);
	loadFloatOrDefault(&gData.mStageInfo.mScale.x, s, "StageInfo", "xscale", 1);
	loadFloatOrDefault(&gData.mStageInfo.mScale.y, s, "StageInfo", "yscale", 1);
}

static void loadStageShadow(MugenDefScript* s) {
	loadIntegerOrDefault(&gData.mShadow.mIntensity, s, "Shadow", "intensity", 128);
	loadVectorOrDefault(&gData.mShadow.mColor, s, "StageInfo", "color", makePosition(1, 1, 1));
	loadFloatOrDefault(&gData.mShadow.mScaleY, s, "StageInfo", "yscale", 1);
	loadVectorIOrDefault(&gData.mShadow.mFadeRange, s, "StageInfo", "fade.range", makeVector3DI(0, 0, 0));
	loadFloatOrDefault(&gData.mShadow.mXShear, s, "StageInfo", "xshear", 0);
}

static void loadStageReflection(MugenDefScript* s) {
	loadIntegerOrDefault(&gData.mReflection.mReflect, s, "Reflection", "reflect", 0);
}

static void loadStageMusic(MugenDefScript* s) {
	loadStringOrDefault(gData.mMusic.mBGMusic, s, "Music", "bgmusic", "");
	loadIntegerOrDefault(&gData.mMusic.mBGVolume, s, "Music", "bgvolume", 0);
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

	loadStringOrDefault(gData.mBackgroundDefinition.mSpritePath, s, name, "spr", "");
	loadIntegerOrDefault(&gData.mBackgroundDefinition.mDebugBG, s, name, "debugbg", 0);

	return bgdef;
}

static int isBackgroundElementGroup(MugenDefScriptGroup* tGroup) {
	return tGroup->mName[0] == 'B' && tGroup->mName[1] == 'G';
}

static int isActionGroup(MugenDefScriptGroup* tGroup) {
	char firstW[100], secondW[100];
	int items = sscanf(tGroup->mName, "%s %s", firstW, secondW);
	if (items < 2) return 0;

	turnStringLowercase(firstW);
	turnStringLowercase(secondW);
	return !strcmp("begin", firstW) && !strcmp("action", secondW);
}

static void addBackgroundElementToStageHandler(StageBackgroundElement* e) {
	e->mStart.z = e->mListPosition + e->mLayerNo * 30; // TODO
	if (e->mType == STAGE_BACKGROUND_STATIC) {
		addMugenStageHandlerStaticBackgroundElement(e->mStart, e->mSpriteNo.x, e->mSpriteNo.y, &gData.mSprites, e->mDelta, e->mTile, e->mTileSpacing);
	} else if (e->mType == STAGE_BACKGROUND_ANIMATED) {
		addMugenStageHandlerAnimatedBackgroundElement(e->mStart, e->mActionNumber, &gData.mAnimations, &gData.mSprites, e->mDelta, e->mTile, e->mTileSpacing);
	}
	else {
		logError("Unable to determine bg element type");
		logErrorInteger(e->mType);
		abortSystem();
	}
}

static void loadBackgroundElement(MugenDefScript* s, char* tName, int i) {
	StageBackgroundElement* e = allocMemory(sizeof(StageBackgroundElement));

	debugLog("Load background element.");
 	debugString(tName);

	char type[100];
	loadStringOrDefault(type, s, tName, "type", "normal");
	if (!strcmp("normal", type) || !strcmp("parallax", type)) { // TODO: parallax
		e->mType = STAGE_BACKGROUND_STATIC;
	} else if (!strcmp("anim", type)) {
		e->mType = STAGE_BACKGROUND_ANIMATED;
		loadIntegerOrDefault(&e->mActionNumber, s, tName, "actionno", -1);
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

	list_push_back_owned(&gData.mBackgroundElements, e);
}

static void loadBackgroundDefinitionGroup(MugenDefScript* s, MugenDefScriptGroup* tGroup, int i) {
	if (isBackgroundElementGroup(tGroup)) {
		loadBackgroundElement(s, tGroup->mName, i);
	}
	else if (isActionGroup(tGroup)) {
		// TODO: ignore properly
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
	sprintf(sffFile, "%s%s", path, gData.mBackgroundDefinition.mSpritePath);
	if (!isFile(sffFile)) {
		sprintf(sffFile, "assets/%s", gData.mBackgroundDefinition.mSpritePath);
		if (!isFile(sffFile)) {
			logError("Unable to locate sff file.");
			logErrorString(gData.mBackgroundDefinition.mSpritePath);
			abortSystem();
		}
	}
	gData.mSprites = loadMugenSpriteFileWithoutPalette(sffFile);
}

static void loadStageBackgroundElements(char* tPath, MugenDefScript* s) {
	MugenDefScriptGroup* bgdef = loadStageBackgroundDefinitionAndReturnGroup(s);

	loadStageTextures(tPath);

	gData.mBackgroundElements = new_list();
	int i = 0;
	MugenDefScriptGroup* cur = bgdef->mNext;
	while (cur != NULL) {
		loadBackgroundDefinitionGroup(s, cur, i);
		i++;
		cur = cur->mNext;
	}

}

static void setStageCamera() {
	double sizeX = gData.mCamera.mBoundRight - gData.mCamera.mBoundLeft;
	double sizeY = gData.mCamera.mBoundLow - gData.mCamera.mBoundHigh;
	setMugenStageHandlerCameraRange(makeGeoRectangle(gData.mCamera.mBoundLeft, gData.mCamera.mBoundHigh, sizeX, sizeY));
}

static void loadStage(void* tData)
{
	(void)tData;
	instantiateActor(MugenStageHandler);

	gData.mAnimations = loadMugenAnimationFile(gData.mDefinitionPath);

	MugenDefScript s = loadMugenDefScript(gData.mDefinitionPath);

	loadStageInfo(&s);
	loadStageCamera(&s);
	loadStagePlayerInfo(&s);
	loadStageBound(&s);
	loadStageStageInfo(&s);
	loadStageShadow(&s);
	loadStageReflection(&s);
	loadStageMusic(&s);
	setMugenStageHandlerCameraCoordinates(makeVector3DI(320, 240, 0)); // TODO

	setStageCamera();
	loadStageBackgroundElements(gData.mDefinitionPath, &s);

	unloadMugenDefScript(s);
}

static void updateCameraMovement() {
	double x1 = getPlayerPositionX(getRootPlayer(0), gData.mStageInfo.mLocalCoordinates.y);
	double x2 = getPlayerPositionX(getRootPlayer(1), gData.mStageInfo.mLocalCoordinates.y);
	double minX = min(x1, x2);
	double maxX = max(x1, x2);
	minX -= gData.mStageInfo.mLocalCoordinates.x / 2;
	maxX -= gData.mStageInfo.mLocalCoordinates.x / 2;
	minX -= getCameraPositionX(gData.mStageInfo.mLocalCoordinates.y);
	maxX -= getCameraPositionX(gData.mStageInfo.mLocalCoordinates.y);

	double right = getCameraPositionX(gData.mStageInfo.mLocalCoordinates.y) + gData.mStageInfo.mLocalCoordinates.x / 2;
	double left = getCameraPositionX(gData.mStageInfo.mLocalCoordinates.y) - gData.mStageInfo.mLocalCoordinates.x / 2;
	
	double lx = (left + gData.mCamera.mTension) - minX;
	double rx = maxX - (right - gData.mCamera.mTension);

	if (lx > 0 && rx > 0) {
		// TODO
	}
	else if (rx > 0) {
		double delta = min(rx, -lx);
		addMugenStageHandlerCameraPositionX(delta);
	}
	else if (lx > 0) {
		double delta = min(lx, -rx);
		addMugenStageHandlerCameraPositionX(-delta);
	}
}

static void updateStage(void* tData) {
	(void)tData;

	updateCameraMovement();
}

ActorBlueprint StageBP = {
	.mLoad = loadStage,
	.mUpdate = updateStage,
};


void setStageMugenDefinition(char * tPath)
{
	strcpy(gData.mDefinitionPath, tPath);
}

double parseCoordinatesToLocalCoordinateSystem(double tCoordinate, int tOtherCoordinateSystemAsP)
{
	ScreenSize sz = getScreenSize();
	int currentP = sz.y; 
	double fac = currentP / (double)tOtherCoordinateSystemAsP;

	return tCoordinate*fac;
}

Position getPlayerStartingPosition(int i, int tCoordinateP)
{
	Position ret;
	if (!i) ret = gData.mPlayerInfo.mP1Start;
	else ret = gData.mPlayerInfo.mP2Start;

	ret = vecAdd(ret, makePosition(gData.mStageInfo.mLocalCoordinates.x / 2,0,0));

	return vecScale(ret, tCoordinateP / (double)gData.mStageInfo.mLocalCoordinates.y);
}

Position getStageCoordinateSystemOffset(int tCoordinateP)
{
	Position ret = makePosition(0, gData.mStageInfo.mZOffset, 0);
	return vecScale(ret, tCoordinateP / (double)gData.mStageInfo.mLocalCoordinates.y);
}

int doesPlayerStartFacingLeft(int i)
{
	if (!i) return gData.mPlayerInfo.mP1Facing == -1;
	else return gData.mPlayerInfo.mP2Facing == -1;
}

double getCameraPositionX(int tCoordinateP)
{
	Position p = *getMugenStageHandlerCameraPositionReference();
	ScreenSize sz = getScreenSize();
	p = transformCoordinatesVector(p, sz.y, tCoordinateP);

	return p.x;
}

double getCameraPositionY(int tCoordinateP)
{
	Position p = *getMugenStageHandlerCameraPositionReference();
	ScreenSize sz = getScreenSize();
	p = transformCoordinatesVector(p, sz.y, tCoordinateP);

	return p.y;
}

double getScreenFactorFromCoordinateP(int tCoordinateP)
{
	ScreenSize sz = getScreenSize();
	return sz.y / (double)tCoordinateP;
}

int getStageCoordinateP()
{
	return gData.mStageInfo.mLocalCoordinates.y;
}

double transformCoordinates(double tVal, int tSrcP, int tDstP)
{
	return tVal * (tDstP / (double) tSrcP);
}

Vector3D transformCoordinatesVector(Vector3D tVal, int tSrcP, int tDstP)
{
	return vecScale(tVal, (tDstP / (double)tSrcP));
}

double getStageTopOfScreenBasedOnPlayer(int tCoordinateP)
{
	return getCameraPositionY(tCoordinateP);
}

double getStageLeftOfScreenBasedOnPlayer(int tCoordinateP)
{
	Position p = *getMugenStageHandlerCameraPositionReference();
	ScreenSize sz = getScreenSize();
	p = transformCoordinatesVector(p, sz.y, tCoordinateP);

	return p.x;
}

double getStageRightOfScreenBasedOnPlayer(int tCoordinateP)
{
	Position p = *getMugenStageHandlerCameraPositionReference();
	ScreenSize sz = getScreenSize();
	p = transformCoordinatesVector(p, sz.y, tCoordinateP);
	double screenSize = transformCoordinates(sz.x, sz.y, tCoordinateP);

	return p.x + screenSize;
}

Position getStageCenterOfScreenBasedOnPlayer(int tCoordinateP)
{
	Position ret = *getMugenStageHandlerCameraPositionReference();

	ret = vecAdd(ret, makePosition(gData.mStageInfo.mLocalCoordinates.x / 2, 0, 0));

	return vecScale(ret, tCoordinateP / (double)gData.mStageInfo.mLocalCoordinates.y);
}

int getGameWidth(int tCoordinateP)
{
	return (int)transformCoordinates(640, 480, tCoordinateP); // TODO: non-hardcoded
}

int getStageLeftEdgeMinimumPlayerDistance(int tCoordinateP)
{
	return (int)transformCoordinates(gData.mBound.mScreenLeft, getStageCoordinateP(), tCoordinateP);
}

int getStageRightEdgeMinimumPlayerDistance(int tCoordinateP)
{
	return (int)transformCoordinates(gData.mBound.mScreenRight, getStageCoordinateP(), tCoordinateP);
}
