#include "fightui.h"

#include <assert.h>

#include <tari/file.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/input.h>

#include "mugendefreader.h"
#include "mugenspritefilereader.h"
#include "mugenanimationreader.h"
#include "mugenanimationhandler.h"
#include "stage.h"
#include "playerdefinition.h"
#include "mugenstagehandler.h"

#define COORD_P 240

typedef struct {
	Position mBG0Position;
	int mBG0AnimationID;

	Position mBG1Position;
	int mBG1AnimationID;

	Position mMidPosition;
	int mMidAnimationID;

	Position mFrontPosition;
	int mFrontAnimationID;

	Vector3D mHealthRangeX;

	double mPercentage;
	double mDisplayedPercentage;

	int mIsPaused;
	Duration mPauseNow;

} HealthBar;

typedef struct {
	Position mBG0Position;
	int mBG0AnimationID;

	Position mBG1Position;
	int mBG1AnimationID;

	Position mMidPosition;
	int mMidAnimationID;

	Position mFrontPosition;
	int mFrontAnimationID;

	Vector3D mPowerRangeX;
	Position mCounterOffset;
} PowerBar;

typedef struct {
	Position mBGPosition;
	int mBGAnimationID;

	Position mBG0Position;
	int mBG0AnimationID;

	Position mBG1Position;
	int mBG1AnimationID;

	Position mFacePosition;
	int mFaceAnimationID;
} Face;

typedef struct {
	Position mBGPosition;
	int mBGAnimationID;

	Position mTextPosition;
	int mTextID;
} DisplayName;

typedef struct {
	int mIsActive;

	int mValue;
	Position mPosition;

	Position mBGPosition;
	int mBGAnimationID;

	int mTextID;
	int mFramesPerCount;
	int mNow;

	void(*mFinishedCB)();
} TimeCounter;

typedef struct {
	int mRoundTime;
	Position mPosition;
	MugenAnimation* mDefaultAnimation;
	int mDefaultFaceDirection;
	int mHasDefaultAnimation;
	int mDefaultTextSize;
	int mDefaultBreakSize;

	int mHasCustomRoundAnimation[10];
	MugenAnimation* mCustomRoundAnimations[10];
	Position mCustomRoundPositions[10];
	int mCustomRoundFaceDirection[10];

	char mText[1024];
	char mDisplayedText[1024];
	int mDisplayTime;

	int mIsDisplayingRound;
	int mDisplayNow;

	int mHasActiveAnimation;
	int mAnimationID;
	int mTextID;

	void(*mCB)();
} Round;

typedef struct {
	Position mPosition;
	MugenAnimation* mAnimation;
	int mFaceDirection;

	int mIsDisplayingFight;

	int mAnimationID;

	void(*mCB)();

} FightDisplay;

typedef struct {
	Position mPosition;
	MugenAnimation* mAnimation;
	int mFaceDirection;

	int mIsDisplaying;

	int mAnimationID;

	void(*mCB)();

} KO;

typedef struct {
	int mTextID;
	
	Position mDisplayPosition;
	Position mPosition;
	char mText[1024];
	char mDisplayedText[1024];
	int mNow;
	int mDisplayTime;

	int mTextSize;
	int mBreakSize;

	int mIsDisplaying;
	void(*mCB)();
} WinDisplay;

typedef struct {
	int mIsCountingDownControl;
	int mControlReturnTime;
	int mNow;
} ControlCountdown;

typedef struct {
	int mIsActive;

	int mContinueTextID;
	int mValueTextID;

	int mValue;
	int mNow;
	int mDuration;

	void(*mFinishedCB)();
	void(*mPressedContinueCB)();

} Continue;

static struct {
	MugenSpriteFile mFightSprites;
	MugenAnimations mFightAnimations;

	MugenSpriteFile mFightFXSprites;
	MugenAnimations mFightFXAnimations;

	HealthBar mHealthBars[2];
	PowerBar mPowerBars[2];
	Face mFaces[2];
	DisplayName mDisplayName[2];
	TimeCounter mTime;
	Round mRound;
	FightDisplay mFight;
	KO mKO;
	WinDisplay mWin;
	Continue mContinue;

	ControlCountdown mControl;


	List mHitSparks;
} gData;

static void loadFightDefFilesFromScript(MugenDefScript* tScript, char* tDefPath) {
	char directory[1024];
	getPathToFile(directory, tDefPath);

	char fileName[1024], fullPath[1024];
	loadStringOrDefault(fileName, tScript, "Files", "sff", "NO_FILE");
	sprintf(fullPath, "%s%s", directory, fileName);
	gData.mFightSprites = loadMugenSpriteFileWithoutPalette(fullPath);

	gData.mFightAnimations = loadMugenAnimationFile(tDefPath);

	loadStringOrDefault(fileName, tScript, "Files", "fightfx.sff", "NO_FILE");
	sprintf(fullPath, "%s%s", directory, fileName);
	gData.mFightFXSprites = loadMugenSpriteFileWithoutPalette(fullPath);

	loadStringOrDefault(fileName, tScript, "Files", "fightfx.air", "NO_FILE");
	sprintf(fullPath, "%s%s", directory, fileName);
	gData.mFightFXAnimations = loadMugenAnimationFile(fullPath);

}

static int loadSingleUIComponentWithFullComponentNameForStorageAndReturnIfLegit(MugenDefScript* tScript, MugenAnimations* tAnimations, Position tBasePosition, char* tGroupName, char* tComponentName, double tZ, MugenAnimation** oAnimation, Position* oPosition, int* oFaceDirection) {
	char name[1024];

	int animation;
	sprintf(name, "%s.anim", tComponentName);
	loadIntegerOrDefault(&animation, tScript, tGroupName, name, -1);

	if (animation == -1) {
		Vector3DI sprite;
		sprintf(name, "%s.spr", tComponentName);
		loadVectorIOrDefault(&sprite, tScript, tGroupName, name, makeVector3DI(-1, -1, 0));
		if (sprite.x == -1 && sprite.y == -1) return 0;

		*oAnimation = createOneFrameMugenAnimationForSprite(sprite.x, sprite.y);
	}
	else {
		*oAnimation = getMugenAnimation(tAnimations, animation);
	}

	sprintf(name, "%s.offset", tComponentName);
	loadVectorOrDefault(oPosition, tScript, tGroupName, name, makePosition(0, 0, 0));
	oPosition->z = tZ;
	*oPosition = vecAdd(*oPosition, tBasePosition);
	*oPosition = vecSub(*oPosition, getStageCoordinateSystemOffset(COORD_P)); // TODO: think better

	sprintf(name, "%s.facing", tComponentName);
	loadIntegerOrDefault(oFaceDirection, tScript, tGroupName, name, 1);

	return 1;
}

static void loadSingleUIComponentWithFullComponentName(MugenDefScript* tScript, MugenSpriteFile* tSprites, MugenAnimations* tAnimations, Position tBasePosition, char* tGroupName, char* tComponentName, double tZ, int* oAnimationID, Position* oPosition, int tScaleCoordinateP) {
	MugenAnimation* anim;
	int faceDirection;

	if (!loadSingleUIComponentWithFullComponentNameForStorageAndReturnIfLegit(tScript, tAnimations, tBasePosition, tGroupName, tComponentName, tZ, &anim, oPosition, &faceDirection)) {
		*oAnimationID = -1;
		return;
	}

	*oAnimationID = addMugenAnimation(NULL, anim, tSprites, oPosition, COORD_P, tScaleCoordinateP); // TODO non-hardcoded
	setRegisteredAnimationToUseFixedZ(*oAnimationID);

	if (faceDirection == -1) {
		setRegisteredAnimationFaceDirection(*oAnimationID, FACE_DIRECTION_LEFT);
	}

}

static void loadSingleUIComponent(int i, MugenDefScript* tScript, MugenSpriteFile* tSprites, MugenAnimations* tAnimations, Position tBasePosition, char* tGroupName, char* tComponentName, double tZ, int* oAnimationID, Position* oPosition, int tScaleCoordinateP) {
	char name[1024];

	sprintf(name, "p%d.%s", i + 1, tComponentName);
	loadSingleUIComponentWithFullComponentName(tScript, tSprites, tAnimations, tBasePosition, tGroupName, name, tZ, oAnimationID, oPosition, tScaleCoordinateP);
}

static void loadSingleUITextWithFullComponentNameForStorage(MugenDefScript* tScript, Position tBasePosition, char* tGroupName, char* tComponentName, double tZ, Position* oPosition, int tIsReadingText, char* tText, int tTextSize, int tBreakSize) {
	char name[1024];

	sprintf(name, "%s.offset", tComponentName);
	loadVectorOrDefault(oPosition, tScript, tGroupName, name, makePosition(0, 0, 0));
	oPosition->z = tZ;
	*oPosition = vecAdd(*oPosition, tBasePosition);
	*oPosition = vecScale(*oPosition, 480.0 / COORD_P); // TODO: non-hardcoded

	if (tIsReadingText) {
		sprintf(name, "%s.text", tComponentName);
		char* text = getAllocatedMugenDefStringVariable(tScript, tGroupName, name);
		strcpy(tText, text);
		freeMemory(text);
	}

	Vector3DI fontData;
	sprintf(name, "%s.font", tComponentName);
	loadVectorIOrDefault(&fontData, tScript, tGroupName, name, makeVector3DI(0, 0, 1));

	*oPosition = vecSub(*oPosition, makePosition(0, tTextSize + tBreakSize, 0));
	if (fontData.z == -1) {
		int n = strlen(tText);
		double len = tTextSize*n + tBreakSize*(n - 1);
		*oPosition = vecSub(*oPosition, makePosition(len, 0, 0));
	} else if (fontData.z == 0) {
		int n = strlen(tText);
		double len = tTextSize*n + tBreakSize*(n - 1);
		*oPosition = vecSub(*oPosition, makePosition(len / 2, 0, 0));
	}
}


static void loadSingleUITextWithFullComponentName(MugenDefScript* tScript, Position tBasePosition, char* tGroupName, char* tComponentName, double tZ, int* oTextID, Position* oPosition, int tIsReadingText, char* tText, int tTextSize, int tBreakSize) {
	loadSingleUITextWithFullComponentNameForStorage(tScript, tBasePosition, tGroupName, tComponentName, tZ, oPosition, tIsReadingText, tText, tTextSize, tBreakSize);

	*oTextID = addHandledText(*oPosition, tText, 0, COLOR_WHITE, makePosition(tTextSize, tTextSize, 1), makePosition(tBreakSize, tBreakSize, 0), makePosition(640, 480, 1), INF);
}


static void loadSingleUIText(int i, MugenDefScript* tScript, Position tBasePosition, char* tGroupName, char* tComponentName, double tZ, int* oTextID, Position* oPosition, int tIsReadingText, char* tText, int tTextSize, int tBreakSize) {
	char name[1024];

	sprintf(name, "p%d.%s", i + 1, tComponentName);
	loadSingleUITextWithFullComponentName(tScript, tBasePosition, tGroupName, name, tZ, oTextID, oPosition, tIsReadingText, tText, tTextSize, tBreakSize);
}

static void loadSingleHealthBar(int i, MugenDefScript* tScript) {
	char name[1024];

	HealthBar* bar = &gData.mHealthBars[i];


	Position basePosition;
	sprintf(name, "p%d.pos", i + 1);
	loadVectorOrDefault(&basePosition, tScript, "Lifebar", name, makePosition(0,0,0));
	basePosition.z = 20;

	int coordP = COORD_P; // TODO
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Lifebar", "bg0", 0, &bar->mBG0AnimationID, &bar->mBG0Position, coordP);
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Lifebar", "bg1", 1, &bar->mBG1AnimationID, &bar->mBG1Position, coordP);
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Lifebar", "mid", 2,  &bar->mMidAnimationID, &bar->mMidPosition, coordP);
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Lifebar", "front", 3, &bar->mFrontAnimationID, &bar->mFrontPosition, coordP);

	sprintf(name, "p%d.range.x", i + 1);
	loadVectorOrDefault(&bar->mHealthRangeX, tScript, "Lifebar", name, makePosition(0, 0, 0));

	bar->mPercentage = 1;
	bar->mDisplayedPercentage = 1;
	bar->mIsPaused = 0;
}

static void loadSinglePowerBar(int i, MugenDefScript* tScript) {
	char name[1024];

	PowerBar* bar = &gData.mPowerBars[i];

	Position basePosition;
	sprintf(name, "p%d.pos", i + 1);
	loadVectorOrDefault(&basePosition, tScript, "Powerbar", name, makePosition(0, 0, 0));
	basePosition.z = 20;

	int coordP = COORD_P; // TODO
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Powerbar", "bg0", 0, &bar->mBG0AnimationID, &bar->mBG0Position, coordP);
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Powerbar", "bg1", 1, &bar->mBG1AnimationID, &bar->mBG1Position, coordP);
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Powerbar", "mid", 2, &bar->mMidAnimationID, &bar->mMidPosition, coordP);
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Powerbar", "front", 3, &bar->mFrontAnimationID, &bar->mFrontPosition, coordP);

	sprintf(name, "p%d.range.x", i + 1);
	loadVectorOrDefault(&bar->mPowerRangeX, tScript, "Powerbar", name, makePosition(0, 0, 0));

	sprintf(name, "p%d.counter.offset", i + 1);
	loadVectorOrDefault(&bar->mCounterOffset, tScript, "Powerbar", name, makePosition(0, 0, 0));
}

static void loadSingleFace(int i, MugenDefScript* tScript) {
	char name[1024];

	Face* face = &gData.mFaces[i];

	Position basePosition;
	sprintf(name, "p%d.pos", i + 1);
	loadVectorOrDefault(&basePosition, tScript, "Face", name, makePosition(0, 0, 0));
	basePosition.z = 20;

	Player* p = getRootPlayer(i);
	int coordP = COORD_P; // TODO
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Face", "bg", 0, &face->mBGAnimationID, &face->mBGPosition, coordP);
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Face", "bg0", 0, &face->mBG0AnimationID, &face->mBG0Position, coordP);
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Face", "bg1", 1, &face->mBG1AnimationID, &face->mBG1Position, coordP);
	loadSingleUIComponent(i, tScript, getPlayerSprites(p), getPlayerAnimations(p), basePosition, "Face", "face", 2, &face->mFaceAnimationID, &face->mFacePosition, getPlayerCoordinateP(p));

}

static void loadSingleName(int i, MugenDefScript* tScript) {
	char name[1024];

	DisplayName* displayName = &gData.mDisplayName[i];

	Position basePosition;
	sprintf(name, "p%d.pos", i + 1);
	loadVectorOrDefault(&basePosition, tScript, "Name", name, makePosition(0, 0, 0));
	basePosition.z = 20;

	int coordP = COORD_P; // TODO
	loadSingleUIComponent(i, tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Name", "bg", 0, &displayName->mBGAnimationID, &displayName->mBGPosition, coordP);
	loadSingleUIText(i, tScript, basePosition, "Name", "name", 0, &displayName->mTextID, &displayName->mTextPosition, 0, getPlayerDisplayName(getRootPlayer(i)), 15, -5);
}

static void loadPlayerUIs(MugenDefScript* tScript) {
	int i;
	for (i = 0; i < 2; i++) {
		loadSingleHealthBar(i, tScript);
		loadSinglePowerBar(i, tScript);
		loadSingleFace(i, tScript);
		loadSingleName(i, tScript);
		setLifeBarPercentage(getRootPlayer(i), 1);
		setPowerBarPercentage(getRootPlayer(i), 0);
	}
}

static void loadTimer(MugenDefScript* tScript) {
	Position basePosition;
	loadVectorOrDefault(&basePosition, tScript, "Time", "pos", makePosition(0, 0, 0));
	basePosition.z = 20;

	int coordP = COORD_P; // TODO
	loadSingleUIComponentWithFullComponentName(tScript, &gData.mFightSprites, &gData.mFightAnimations, basePosition, "Time", "bg", 0, &gData.mTime.mBGAnimationID, &gData.mTime.mBGPosition, coordP);

	loadVectorOrDefault(&gData.mTime.mPosition, tScript, "Time", "counter.offset", makePosition(0, 0, 0));
	gData.mTime.mPosition = vecAdd(gData.mTime.mPosition, basePosition);
	gData.mTime.mPosition = vecScale(gData.mTime.mPosition, 480.0 / coordP);
	gData.mTime.mPosition.z++;

	loadIntegerOrDefault(&gData.mTime.mFramesPerCount, tScript, "Time", "framespercount", 60);

	int textSize = 25;
	int breakSize = -5;

	gData.mTime.mPosition = vecSub(gData.mTime.mPosition, makePosition(textSize + breakSize / 2, textSize + breakSize, 0));
	gData.mTime.mIsActive = 0;

	gData.mTime.mTextID = addHandledText(gData.mTime.mPosition, "99", 0, COLOR_WHITE, makePosition(textSize, textSize, 1), makePosition(breakSize, breakSize, 1), makePosition(640, 640, 1), INF);
	gData.mTime.mValue = 99;
	gData.mTime.mNow = 0;
	resetTimer();
}

static void loadRound(MugenDefScript* tScript) {
	Position basePosition;
	loadVectorOrDefault(&basePosition, tScript, "Round", "pos", makePosition(0,0,0));
	basePosition.z = 20;

	loadIntegerOrDefault(&gData.mRound.mRoundTime, tScript, "Round", "round.time", 0);
	loadIntegerOrDefault(&gData.mRound.mDisplayTime, tScript, "Round", "round.default.displaytime", 0);

	gData.mRound.mDefaultTextSize = 40;
	gData.mRound.mDefaultBreakSize = -5;
	if (isMugenDefVariable(tScript, "Round", "round.default.anim")) {
		gData.mRound.mHasDefaultAnimation = 1;
		assert(loadSingleUIComponentWithFullComponentNameForStorageAndReturnIfLegit(tScript, &gData.mFightAnimations, basePosition, "Round", "round.default", 1, &gData.mRound.mDefaultAnimation, &gData.mRound.mPosition, &gData.mRound.mDefaultFaceDirection));
	}
	else {
		gData.mRound.mHasDefaultAnimation = 0;
		loadSingleUITextWithFullComponentNameForStorage(tScript, basePosition, "Round", "round.default", 1, &gData.mRound.mPosition, 1, gData.mRound.mText, gData.mRound.mDefaultTextSize, gData.mRound.mDefaultBreakSize);
	}


	int i;
	for (i = 0; i < 9; i++) {
		char name[1024];
		sprintf(name, "round%d.anim", i + 1);
		if (!isMugenDefVariable(tScript, "Round", name)) {
			gData.mRound.mHasCustomRoundAnimation[i] = 0;
		}
		else {
			gData.mRound.mHasCustomRoundAnimation[i] = 1;
			sprintf(name, "round%d", i + 1);
			assert(loadSingleUIComponentWithFullComponentNameForStorageAndReturnIfLegit(tScript, &gData.mFightAnimations, basePosition, "Round", name, 1, &gData.mRound.mCustomRoundAnimations[i], &gData.mRound.mCustomRoundPositions[i], &gData.mRound.mCustomRoundFaceDirection[i]));
		}
	}

	gData.mRound.mIsDisplayingRound = 0;
}

static void loadFight(MugenDefScript* tScript) {
	Position basePosition;
	loadVectorOrDefault(&basePosition, tScript, "Round", "pos", makePosition(0, 0, 0));
	basePosition.z = 20;

	assert(loadSingleUIComponentWithFullComponentNameForStorageAndReturnIfLegit(tScript, &gData.mFightAnimations, basePosition, "Round", "fight", 1, &gData.mFight.mAnimation, &gData.mFight.mPosition, &gData.mFight.mFaceDirection));

	gData.mFight.mIsDisplayingFight = 0;
}

static void loadKO(MugenDefScript* tScript) {
	Position basePosition;
	loadVectorOrDefault(&basePosition, tScript, "Round", "pos", makePosition(0, 0, 0));
	basePosition.z = 20;

	assert(loadSingleUIComponentWithFullComponentNameForStorageAndReturnIfLegit(tScript, &gData.mFightAnimations, basePosition, "Round", "ko", 1, &gData.mKO.mAnimation, &gData.mKO.mPosition, &gData.mKO.mFaceDirection));

	gData.mKO.mIsDisplaying = 0;
}

static void loadWinDisplay(MugenDefScript* tScript) {
	Position basePosition;
	loadVectorOrDefault(&basePosition, tScript, "Round", "pos", makePosition(0, 0, 0));
	basePosition.z = 20;

	gData.mWin.mTextSize = 20;
	gData.mWin.mBreakSize = -5;
	loadSingleUITextWithFullComponentNameForStorage(tScript, basePosition, "Round", "win", 1, &gData.mWin.mPosition, 1, gData.mWin.mText, gData.mWin.mTextSize, gData.mWin.mBreakSize);

	loadIntegerOrDefault(&gData.mWin.mDisplayTime, tScript, "Round", "win.displaytime", 0);

	gData.mWin.mIsDisplaying = 0;
}

static void loadControl(MugenDefScript* tScript) {
	loadIntegerOrDefault(&gData.mControl.mControlReturnTime, tScript, "Round", "ctrl.time", 0);

	gData.mControl.mIsCountingDownControl = 0;
}

static void loadHitSparks() {
	gData.mHitSparks = new_list();
}

static void loadContinue() {
	gData.mContinue.mIsActive = 0;
}

static void loadFightUI(void* tData) {
	(void)tData;

	char defPath[1024];
	strcpy(defPath, "assets/data/fight.def"); // TODO
	MugenDefScript script = loadMugenDefScript(defPath);
	
	loadFightDefFilesFromScript(&script, defPath);
	loadPlayerUIs(&script);
	loadTimer(&script);
	loadRound(&script);
	loadFight(&script);
	loadKO(&script);
	loadWinDisplay(&script);
	loadControl(&script);
	loadContinue();
	unloadMugenDefScript(script);

	loadHitSparks();
}


typedef struct {
	Position mPosition;
	int mAnimationID;
} HitSpark;

static int updateSingleHitSpark(void* tCaller, void* tData) {
	(void)tCaller;

	HitSpark* e = tData;

	if (!getRegisteredAnimationRemainingAnimationTime(e->mAnimationID)) {
		removeRegisteredAnimation(e->mAnimationID);
		return 1;
	}

	return 0;
}

static void updateHitSparks() {
	list_remove_predicate(&gData.mHitSparks, updateSingleHitSpark, NULL);
}

static void setBarToPercentage(int tAnimationID, Vector3D tRange, double tPercentage) {
	double fullSize = fabs(tRange.y - tRange.x);
	int newSize = (int)(fullSize * tPercentage);
	setRegisteredAnimationRectangleWidth(tAnimationID, newSize);
}

static void updateSingleHealthBar(int i) {
	HealthBar* bar = &gData.mHealthBars[i];

	double dx = bar->mPercentage - bar->mDisplayedPercentage;

	if (fabs(dx) < 1e-6) return;

	if (bar->mIsPaused) {
		if (handleDurationAndCheckIfOver(&bar->mPauseNow, 60)) {
			bar->mIsPaused = 0;
		}
		return;
	}

	bar->mDisplayedPercentage += dx * 0.1;
	setBarToPercentage(bar->mMidAnimationID, bar->mHealthRangeX, bar->mDisplayedPercentage);
}

static void updateHealthBars() {
	int i;
	for (i = 0; i < 2; i++) {
		updateSingleHealthBar(i);
	}
}


static void playDisplayAnimation(int* oAnimationID, MugenAnimation* tAnimation, Position* tBasePosition, int tFaceDirection) {
	*oAnimationID = addMugenAnimation(NULL, tAnimation, &gData.mFightSprites, tBasePosition, COORD_P, COORD_P); // TODO non-hardcoded
	setRegisteredAnimationToUseFixedZ(*oAnimationID);

	if (tFaceDirection == -1) {
		setRegisteredAnimationFaceDirection(*oAnimationID, FACE_DIRECTION_LEFT);
	}
}

static void removeDisplayedAnimation(int tAnimationID) {
	removeRegisteredAnimation(tAnimationID);
}


static void playDisplayText(int* oTextID, char* tText, Position tPosition, int tTextSize, int tBreakSize) {
	*oTextID = addHandledText(tPosition, tText, 0, COLOR_WHITE, makePosition(tTextSize, tTextSize, 1), makePosition(tBreakSize, tBreakSize, 1), makePosition(640, 640, 1), INF);
}

static void removeDisplayedText(int tTextID) {
	removeHandledText(tTextID);
}

static void updateRoundDisplay() {
	if (!gData.mRound.mIsDisplayingRound) return;

	gData.mRound.mDisplayNow++;
	if (gData.mRound.mDisplayNow >= gData.mRound.mDisplayTime) {
	
		if (gData.mRound.mHasActiveAnimation) {
			removeDisplayedAnimation(gData.mRound.mAnimationID);
		}
		else {
			removeDisplayedText(gData.mRound.mTextID);
		}

		gData.mRound.mCB();
		gData.mRound.mIsDisplayingRound = 0;
	}
}

static void startControlCountdown() {
	gData.mControl.mNow = 0;
	gData.mControl.mIsCountingDownControl = 1;
}

static void updateFightDisplay() {
	if (!gData.mFight.mIsDisplayingFight) return;

	if (!getRegisteredAnimationRemainingAnimationTime(gData.mFight.mAnimationID)) {
		removeDisplayedAnimation(gData.mFight.mAnimationID);
		gData.mFight.mCB();
		startControlCountdown();
		gData.mFight.mIsDisplayingFight = 0;
	}
}

static void updateKODisplay() {
	if (!gData.mKO.mIsDisplaying) return;

	if (!getRegisteredAnimationRemainingAnimationTime(gData.mKO.mAnimationID)) {
		removeDisplayedAnimation(gData.mKO.mAnimationID);
		gData.mKO.mCB();
		gData.mKO.mIsDisplaying = 0;
	}
}


static void updateWinDisplay() {
	if (!gData.mWin.mIsDisplaying) return;

	gData.mWin.mNow++;
	if (gData.mWin.mNow >= gData.mWin.mDisplayTime || hasPressedStartFlank()) {
		removeDisplayedText(gData.mWin.mTextID);
		gData.mWin.mCB();
		gData.mWin.mIsDisplaying = 0;
	}
}


static void updateControlCountdown() {
	if (!gData.mControl.mIsCountingDownControl) return;

	gData.mControl.mNow++;
	if (gData.mControl.mNow >= gData.mControl.mControlReturnTime) {
		setPlayerControl(getRootPlayer(0), 1);
		setPlayerControl(getRootPlayer(1), 1);
		gData.mControl.mIsCountingDownControl = 0;
	}
}

static void updateTimeDisplayText() {
	char text[10];
	if(gData.mTime.mValue < 10) sprintf(text, "0%d", gData.mTime.mValue);
	else sprintf(text, "%d", gData.mTime.mValue);
	setHandledText(gData.mTime.mTextID, text);
}

static void updateTimeDisplay() {
	if (!gData.mTime.mIsActive) return;

	gData.mTime.mNow++;
	if (gData.mTime.mNow >= gData.mTime.mFramesPerCount) {
		gData.mTime.mNow = 0;
		gData.mTime.mValue--;
		if (gData.mTime.mValue < 0) {
			gData.mTime.mValue = 0;
			gData.mTime.mFinishedCB();
		}

		updateTimeDisplayText();
	}
}

static void setContinueInactive() {
	removeHandledText(gData.mContinue.mContinueTextID);
	removeHandledText(gData.mContinue.mValueTextID);
	gData.mContinue.mIsActive = 0;
}


static void decreaseContinueCounter() {

	removeHandledText(gData.mContinue.mValueTextID);

	gData.mContinue.mValue--;
	char text[10];
	sprintf(text, "%d", gData.mContinue.mValue);
	gData.mContinue.mValueTextID = addHandledText(makePosition(307, 200, 20), text, 0, COLOR_WHITE, makePosition(40, 40, 1), makePosition(-5, -5, 0), makePosition(INF, INF, 1), INF);
}

static void updateContinueDisplay() {
	if (!gData.mContinue.mIsActive) return;

	if (hasPressedStartFlank()) {
		setContinueInactive();
		gData.mContinue.mPressedContinueCB();
		return;
	}
	
	gData.mContinue.mNow++;
	if (gData.mContinue.mNow >= gData.mContinue.mDuration || hasPressedAFlank()) {
		gData.mContinue.mNow = 0;
		if (!gData.mContinue.mValue) {
			setContinueInactive();
			gData.mContinue.mFinishedCB();
			return;
		}

		decreaseContinueCounter();
	}
}

static void updateFightUI(void* tData) {
	(void)tData;
	updateHitSparks();
	updateHealthBars();
	updateRoundDisplay();
	updateFightDisplay();
	updateKODisplay();
	updateWinDisplay();
	updateControlCountdown();
	updateTimeDisplay();
	updateContinueDisplay();
}




ActorBlueprint FightUIBP = {
	.mLoad = loadFightUI,
	.mUpdate = updateFightUI,
};

void playHitSpark(Position tPosition, Player* tPlayer, int tIsInPlayerFile, int tNumber, int tIsFacingRight, int tPositionCoordinateP, int tScaleCoordinateP)
{
	MugenAnimation* anim;
	MugenSpriteFile* spriteFile;

	if (tIsInPlayerFile) {
		spriteFile = getPlayerSprites(tPlayer);
		anim = getMugenAnimation(getPlayerAnimations(tPlayer), tNumber);
	}
	else {
		spriteFile = &gData.mFightFXSprites;
		anim = getMugenAnimation(&gData.mFightFXAnimations, tNumber);
	}

	HitSpark* e = allocMemory(sizeof(HitSpark));
	
	e->mPosition = tPosition;
	e->mPosition.z = 16;
	e->mAnimationID = addMugenAnimation(NULL, anim, spriteFile, &e->mPosition, tPositionCoordinateP, tScaleCoordinateP);
	setRegisteredAnimationToUseFixedZ(e->mAnimationID);
	setRegisteredAnimationCameraPositionReference(e->mAnimationID, getMugenStageHandlerCameraPositionReference());
	if (!tIsFacingRight) {
		setRegisteredAnimationFaceDirection(e->mAnimationID, tIsFacingRight);
	}
	list_push_back_owned(&gData.mHitSparks, e);
}

void setLifeBarPercentage(Player* tPlayer, double tPercentage)
{
	HealthBar* bar = &gData.mHealthBars[tPlayer->mRootID];

	bar->mPercentage = tPercentage;

	setBarToPercentage(bar->mFrontAnimationID, bar->mHealthRangeX, bar->mPercentage);
	bar->mIsPaused = 1;
	bar->mPauseNow = 0;
}

void setPowerBarPercentage(Player* tPlayer, double tPercentage)
{
	PowerBar* bar = &gData.mPowerBars[tPlayer->mRootID];
	setBarToPercentage(bar->mFrontAnimationID, bar->mPowerRangeX, tPercentage);
}

void enableTimer()
{
	gData.mTime.mIsActive = 1;
}

void disableTimer()
{
	gData.mTime.mIsActive = 0;
}

void resetTimer()
{
	gData.mTime.mNow = 0;
	gData.mTime.mValue = 99;
	updateTimeDisplayText();
}

MugenAnimation * getFightEffectAnimation(int tNumber)
{
	return getMugenAnimation(&gData.mFightFXAnimations, tNumber);
}

MugenSpriteFile * getFightEffectSprites()
{
	return &gData.mFightFXSprites;
}

static void parseRoundText(char* tDst, char* tSrc, int tRoundNumber) {

	int i, o = 0;
	int n = strlen(tSrc);
	for (i = 0; i < n; i++) {
		if (tSrc[i] == '%' && i < n - 1 && tSrc[i + 1] == 'i') {
			int len = sprintf(&tDst[o], "%d", tRoundNumber);
			o += len;
			i++;
		}
		else {
			tDst[o] = tSrc[i];
			o++;
		}
	}
	tDst = '\0';
}

void playRoundAnimation(int tRound, void(*tFunc)())
{
	
	tRound--;
	if (gData.mRound.mHasCustomRoundAnimation[tRound]) {
		gData.mRound.mHasActiveAnimation = 1;
		playDisplayAnimation(&gData.mRound.mAnimationID, gData.mRound.mCustomRoundAnimations[tRound], &gData.mRound.mCustomRoundPositions[tRound], gData.mRound.mCustomRoundFaceDirection[tRound]);
	}
	else if (gData.mRound.mHasDefaultAnimation) {
		gData.mRound.mHasActiveAnimation = 1;
		playDisplayAnimation(&gData.mRound.mAnimationID, gData.mRound.mDefaultAnimation, &gData.mRound.mPosition, gData.mRound.mDefaultFaceDirection);
	}
	else {
		gData.mRound.mHasActiveAnimation = 0;
		parseRoundText(gData.mRound.mDisplayedText, gData.mRound.mText, tRound + 1);

		playDisplayText(&gData.mRound.mTextID, gData.mRound.mDisplayedText, gData.mRound.mPosition, gData.mRound.mDefaultTextSize, gData.mRound.mDefaultBreakSize);
	}

	gData.mRound.mCB = tFunc;
	gData.mRound.mDisplayNow = 0;
	gData.mRound.mIsDisplayingRound = 1;
}

void playFightAnimation(void(*tFunc)())
{
	playDisplayAnimation(&gData.mFight.mAnimationID, gData.mFight.mAnimation, &gData.mFight.mPosition, gData.mFight.mFaceDirection);

	gData.mFight.mCB = tFunc;
	gData.mFight.mIsDisplayingFight = 1;
}

void playKOAnimation(void(*tFunc)())
{
	playDisplayAnimation(&gData.mKO.mAnimationID, gData.mKO.mAnimation, &gData.mKO.mPosition, gData.mKO.mFaceDirection);

	gData.mKO.mCB = tFunc;
	gData.mKO.mIsDisplaying = 1;
}

static void parseWinText(char* tDst, char* tSrc, char* tName, Position* oDisplayPosition, Position tPosition, int tSize, int tBreakSize) {
	int i, o = 0;
	int bonus = 0;
	int n = strlen(tSrc);
	for (i = 0; i < n; i++) {
		if (tSrc[i] == '%' && i < n - 1 && tSrc[i + 1] == 's') {
			int len = sprintf(&tDst[o], "%s", tName);
			bonus += len - 1;
			o += len;
			i++;
		}
		else {
			tDst[o] = tSrc[i];
			o++;
		}
	}
	tDst = '\0';

	*oDisplayPosition = tPosition;
	oDisplayPosition->x -= (bonus * (double)(tSize + tBreakSize)) / 2;
}

void playWinAnimation(char * tName, void(*tFunc)())
{
	parseWinText(gData.mWin.mDisplayedText, gData.mWin.mText, tName, &gData.mWin.mDisplayPosition, gData.mWin.mPosition, gData.mWin.mTextSize, gData.mWin.mBreakSize);
	playDisplayText(&gData.mWin.mTextID, gData.mWin.mDisplayedText, gData.mWin.mDisplayPosition, gData.mWin.mTextSize, gData.mWin.mBreakSize);
	
	gData.mWin.mNow = 0;
	gData.mWin.mCB = tFunc;
	gData.mWin.mIsDisplaying = 1;
}

void playContinueAnimation(void(*tAnimationFinishedFunc)(), void(*tContinuePressedFunc)())
{

	gData.mContinue.mFinishedCB = tAnimationFinishedFunc;
	gData.mContinue.mPressedContinueCB = tContinuePressedFunc;
	gData.mContinue.mValue = 10;
	gData.mContinue.mNow = 0;
	gData.mContinue.mDuration = 60;

	gData.mContinue.mContinueTextID = addHandledText(makePosition(170, 150, 20), "Continue?", 0, COLOR_WHITE, makePosition(40, 40, 1), makePosition(-5, -5, 0), makePosition(INF, INF, 1), INF);
	gData.mContinue.mValueTextID = addHandledText(makePosition(290, 200, 20), "10", 0, COLOR_WHITE, makePosition(40, 40, 1), makePosition(-5, -5, 0), makePosition(INF, INF, 1), INF);

	gData.mContinue.mIsActive = 1;
}

void setTimeDisplayFinishedCB(void(*tTimeDisplayFinishedFunc)())
{
	gData.mTime.mFinishedCB = tTimeDisplayFinishedFunc;
}

static void setSingleUIComponentInvisibleForOneFrame(int tAnimationID) {
	if(tAnimationID != -1) setRegisteredAnimationInvisibleFlag(tAnimationID);
}

void setBarInvisibleForOneFrame()
{
	int i;
	for (i = 0; i < 2; i++) {
		setSingleUIComponentInvisibleForOneFrame(gData.mHealthBars[i].mBG0AnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mHealthBars[i].mBG1AnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mHealthBars[i].mMidAnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mHealthBars[i].mFrontAnimationID);

		setSingleUIComponentInvisibleForOneFrame(gData.mPowerBars[i].mBG0AnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mPowerBars[i].mBG1AnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mPowerBars[i].mMidAnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mPowerBars[i].mFrontAnimationID);

		setSingleUIComponentInvisibleForOneFrame(gData.mFaces[i].mBGAnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mFaces[i].mBG0AnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mFaces[i].mBG1AnimationID);
		setSingleUIComponentInvisibleForOneFrame(gData.mFaces[i].mFaceAnimationID);
	}
}
