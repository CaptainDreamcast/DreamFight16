#include "pausecontrollers.h"

#include <assert.h>

#include "mugenanimationreader.h"
#include "playerdefinition.h"
#include "fightui.h"
#include "mugenanimationhandler.h"
#include "mugenstagehandler.h"

typedef struct {
	int mIsActive;
	Player* mPlayer;

	int mNow;
	int mDuration;

	int mBufferTimeForCommandsDuringPauseEnd;
	int mMoveTime;
	int mIsPausingBG;

	int mHasAnimation;
	int mMugenAnimationID;
	// TODO: sound

	Position mAnimationReferencePosition;

	int mIsDarkening;
	double mPlayer2DefenseMultiplier;

	int mIsSettingPlayerUnhittable;

} SuperPauseHandler;

static struct {
	SuperPauseHandler mSuperPause;

} gData;

static void loadSuperPauseHandler() {
	gData.mSuperPause.mIsActive = 0;
}

static void loadPauseHandler(void* tData) {
	(void)tData;
	loadSuperPauseHandler();
}

void setSuperPauseActive()
{
	assert(!gData.mSuperPause.mIsActive);

	setPlayerSuperPaused(gData.mSuperPause.mPlayer);
	// TODO: stuff

	gData.mSuperPause.mIsActive = 1;
}


static void setSuperPauseInactive() {
	if (gData.mSuperPause.mHasAnimation) {
		removeRegisteredAnimation(gData.mSuperPause.mMugenAnimationID);
	}

	setPlayerUnSuperPaused(gData.mSuperPause.mPlayer);

	gData.mSuperPause.mIsActive = 0;
}

static void updateSuperPause() {
	if (!gData.mSuperPause.mIsActive) return;

	gData.mSuperPause.mNow++;
	if (gData.mSuperPause.mNow >= gData.mSuperPause.mDuration)  // TODO: fix
	{
		setSuperPauseInactive();
	}
}

static void updatePauseHandler(void* tData) {
	(void)tData;
	updateSuperPause();
}

ActorBlueprint PauseHandler = {
	.mLoad = loadPauseHandler,
	.mUpdate = updatePauseHandler,
};

void setSuperPausePlayer(Player* tPlayer)
{
	gData.mSuperPause.mPlayer = tPlayer;
}

void setSuperPauseTime(int tTime)
{
	gData.mSuperPause.mNow = 0;
	gData.mSuperPause.mDuration = tTime;
}

void setSuperPauseBufferTimeForCommandsDuringPauseEnd(int tBufferTime)
{
	gData.mSuperPause.mBufferTimeForCommandsDuringPauseEnd = tBufferTime;
}

void setSuperPauseMoveTime(int tMoveTime)
{
	gData.mSuperPause.mMoveTime = tMoveTime; // TODO: do something with this
}

void setSuperPauseIsPausingBG(int tIsPausingBG)
{
	gData.mSuperPause.mIsPausingBG = tIsPausingBG;
}

void setSuperPauseAnimation(int tIsInPlayerFile, int tAnimationNumber)
{
	if (tAnimationNumber == -1) {
		gData.mSuperPause.mHasAnimation = 0;
		return;
	}

	MugenAnimation* animation;
	MugenSpriteFile* sprites;
	if (tIsInPlayerFile) {
		animation = getPlayerAnimation(gData.mSuperPause.mPlayer, tAnimationNumber);
		sprites = getPlayerSprites(gData.mSuperPause.mPlayer);
	}
	else {
		animation = getFightEffectAnimation(tAnimationNumber);
		sprites = getFightEffectSprites();
	}

	gData.mSuperPause.mHasAnimation = 1;
	gData.mSuperPause.mMugenAnimationID = addMugenAnimation(NULL, animation, sprites, &gData.mSuperPause.mAnimationReferencePosition, getPlayerCoordinateP(gData.mSuperPause.mPlayer), getPlayerCoordinateP(gData.mSuperPause.mPlayer));
	setRegisteredAnimationCameraPositionReference(gData.mSuperPause.mMugenAnimationID, getMugenStageHandlerCameraPositionReference());
	setRegisteredAnimationToUseFixedZ(gData.mSuperPause.mMugenAnimationID);
}

void setSuperPauseSound(int tIsInPlayerFile, int tSoundGroup, int tSoundItem)
{
	(void)tIsInPlayerFile;
	(void)tSoundGroup;
	(void)tSoundItem;
	// TODO
}

void setSuperPausePosition(Position tPosition)
{
	int isPlayerFacingRight = getPlayerIsFacingRight(gData.mSuperPause.mPlayer);
	if (!isPlayerFacingRight) tPosition.x *= -1;

	Position mPlayerPosition = getPlayerPosition(gData.mSuperPause.mPlayer, getPlayerCoordinateP(gData.mSuperPause.mPlayer));
	gData.mSuperPause.mAnimationReferencePosition = vecAdd(tPosition, mPlayerPosition);
	gData.mSuperPause.mAnimationReferencePosition.z = 20; // TODO: better
}

void setSuperPauseDarkening(int tIsDarkening)
{
	gData.mSuperPause.mIsDarkening = tIsDarkening;
}

void setSuperPausePlayer2DefenseMultiplier(double tMultiplier)
{
	gData.mSuperPause.mPlayer2DefenseMultiplier = tMultiplier;
}

void setSuperPausePowerToAdd(int tPowerToAdd)
{
	addPlayerPower(gData.mSuperPause.mPlayer, tPowerToAdd);
}

void setSuperPausePlayerUnhittability(int tIsUnhittable)
{
	gData.mSuperPause.mIsSettingPlayerUnhittable = tIsUnhittable;
}
