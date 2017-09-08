#include "playerdefinition.h"

#include <assert.h>
#include <string.h>

#include <tari/file.h>
#include <tari/physicshandler.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/timer.h>
#include <tari/math.h>
#include <tari/mugendefreader.h>
#include <tari/mugenanimationreader.h>

#include "mugencommandreader.h"
#include "mugenstatereader.h"
#include "mugenanimationhandler.h"
#include "mugencommandhandler.h"
#include "mugenstatehandler.h"
#include "playerhitdata.h"
#include "projectile.h"

#include "stage.h"
#include "fightui.h"
#include "mugenstagehandler.h"
#include "gamelogic.h"
#include "ai.h"

static struct {
	Player mPlayers[2];
} gData;

static void loadPlayerHeaderFromScript(PlayerHeader* tHeader, MugenDefScript* tScript) {
	getMugenDefStringOrDefault(tHeader->mName, tScript, "Info", "name", "Character");
	getMugenDefStringOrDefault(tHeader->mDisplayName, tScript, "Info", "displayname", tHeader->mName);
	getMugenDefStringOrDefault(tHeader->mVersion, tScript, "Info", "versiondate", "09,09,2017");
	getMugenDefStringOrDefault(tHeader->mMugenVersion, tScript, "Info", "mugenversion", "1.1");
	getMugenDefStringOrDefault(tHeader->mAuthor, tScript, "Info", "author", "John Doe");
	getMugenDefStringOrDefault(tHeader->mPaletteDefaults, tScript, "Info", "pal.defaults", "1");

	getMugenDefVectorIOrDefault(&tHeader->mLocalCoordinates, tScript, "Info", "localcoord", makeVector3DI(320, 240, 0));
}

static void loadOptionalStateFiles(MugenDefScript* tScript, char* tPath, Player* tPlayer) {
	char file[200];
	char scriptPath[1024];
	char name[100];

	int i = 1;
	while (i) {
		sprintf(name, "st%d", i);
		getMugenDefStringOrDefault(file, tScript, "Files", name, "");
		sprintf(scriptPath, "%s%s", tPath, file);
		if (!isFile(scriptPath)) return;
		
		loadMugenStateDefinitionsFromFile(&tPlayer->mConstants.mStates, scriptPath);
	
		i++;
	}

	
}

static void setPlayerFaceDirection(Player* p, FaceDirection tDirection);

static void setPlayerExternalDependencies(Player* tPlayer) {
	tPlayer->mPhysicsID = addToPhysicsHandler(getPlayerStartingPosition(tPlayer->mRootID, tPlayer->mHeader.mLocalCoordinates.y));
	setPlayerPhysics(tPlayer, MUGEN_STATE_PHYSICS_STANDING);
	setPlayerStateMoveType(tPlayer, MUGEN_STATE_MOVE_TYPE_IDLE);
	setPlayerStateType(tPlayer, MUGEN_STATE_TYPE_STANDING);

	tPlayer->mAnimationID = addRegisteredAnimation(tPlayer, getMugenAnimation(&tPlayer->mAnimations, 0), &tPlayer->mSprites, getHandledPhysicsPositionReference(tPlayer->mPhysicsID), tPlayer->mHeader.mLocalCoordinates.y, tPlayer->mHeader.mLocalCoordinates.y);
	setRegisteredAnimationCameraPositionReference(tPlayer->mAnimationID, getMugenStageHandlerCameraPositionReference());
	tPlayer->mStateMachineID = registerMugenStateMachine(&tPlayer->mConstants.mStates, tPlayer);
}

static void loadPlayerFiles(char* tPath, Player* tPlayer, MugenDefScript* tScript) {
	char file[200];
	char path[1024];
	char scriptPath[1024];
	char name[100];
	getPathToFile(path, tPath);

	getMugenDefStringOrDefault(file, tScript, "Files", "cns", "");
	assert(strcmp("", file));
	sprintf(scriptPath, "%s%s", path, file);
	tPlayer->mConstants = loadMugenConstantsFile(scriptPath);

	getMugenDefStringOrDefault(file, tScript, "Files", "stcommon", "");
	sprintf(scriptPath, "%s%s", path, file);
	if (isFile(scriptPath)) {
		loadMugenStateDefinitionsFromFile(&tPlayer->mConstants.mStates, scriptPath);
	}

	getMugenDefStringOrDefault(file, tScript, "Files", "st", "");
	sprintf(scriptPath, "%s%s", path, file);
	if (isFile(scriptPath)) {
		loadMugenStateDefinitionsFromFile(&tPlayer->mConstants.mStates, scriptPath);
	}
	
	loadOptionalStateFiles(tScript, path, tPlayer);

	getMugenDefStringOrDefault(file, tScript, "Files", "cmd", "");
	assert(strcmp("", file));
	sprintf(scriptPath, "%s%s", path, file);
	tPlayer->mCommands = loadMugenCommandFile(scriptPath);
	loadMugenStateDefinitionsFromFile(&tPlayer->mConstants.mStates, scriptPath);

	getMugenDefStringOrDefault(file, tScript, "Files", "anim", "");
	assert(strcmp("", file));
	sprintf(scriptPath, "%s%s", path, file);
	tPlayer->mAnimations = loadMugenAnimationFile(scriptPath);

	char palettePath[1024];
	int preferredPalette = tPlayer->mPreferredPalette;
	sprintf(name, "pal%d", preferredPalette + 1);
	getMugenDefStringOrDefault(file, tScript, "Files", name, "");
	int hasPalettePath = strcmp("", file);
	sprintf(palettePath, "%s%s", path, file);

	getMugenDefStringOrDefault(file, tScript, "Files", "sprite", "");
	assert(strcmp("", file));
	sprintf(scriptPath, "%s%s", path, file);
	tPlayer->mSprites = loadMugenSpriteFile(scriptPath, preferredPalette, hasPalettePath, palettePath);

	setPlayerExternalDependencies(tPlayer);
	tPlayer->mCommandID = registerMugenCommands(tPlayer, &tPlayer->mCommands);

	if (getPlayerAILevel(tPlayer)) {
		setAIActive(tPlayer);
	}

	if (doesPlayerStartFacingLeft(tPlayer->mRootID)) {
		setPlayerFaceDirection(tPlayer, FACE_DIRECTION_LEFT);
	}
	else {
		setPlayerFaceDirection(tPlayer, FACE_DIRECTION_RIGHT);
	}
}

static void initHitDefAttributeSlot(HitDefAttributeSlot* tSlot) {
	tSlot->mIsActive = 0;
	tSlot->mNow = 0;
}

static void resetHelperState(Player* p) {
	p->mHelpers = new_list();
	p->mProjectiles = new_int_map();

	p->mNoWalkFlag = 0;
	p->mNoAutoTurnFlag = 0;
	p->mNoLandFlag = 0;
	p->mPushDisabledFlag = 0;

	p->mIsHitOver = 1;
	p->mIsHitShakeOver = 1;
	p->mIsFalling = 0;
	p->mCanRecoverFromFall = 0;

	p->mDefenseMultiplier = 1;

	p->mIsFrozen = 0;

	p->mIsLyingDown = 0;
	p->mIsHitPaused = 0;
	p->mIsSuperPaused = 0;
	p->mMoveContactCounter = 0;

	p->mHitCount = 0;
	p->mFallAmountInCombo = 0;

	p->mAttackMultiplier = 1;

	p->mHitDataID = initPlayerHitDataAndReturnID(p);

	p->mFaceDirection = FACE_DIRECTION_RIGHT;

	p->mHasMoveBeenReversed = 0;
	p->mMoveHit = 0;
	p->mMoveGuarded = 0;

	p->mRoundsExisted = 0;

	p->mIsBound = 0;
	p->mBoundHelpers = new_list();

	p->mComboCounter = 0;

	int i;
	for (i = 0; i < 2; i++) {
		initHitDefAttributeSlot(&p->mNotHitBy[i]);
	}
}

static void loadPlayerState(Player* p) {
	memset(p->mVars, 0, sizeof p->mVars);
	memset(p->mSystemVars, 0, sizeof p->mSystemVars);
	memset(p->mFloatVars, 0, sizeof p->mFloatVars);
	memset(p->mSystemFloatVars, 0, sizeof p->mSystemFloatVars);
	
	p->mID = 0;

	p->mIsInControl = 1;
	p->mIsAlive = 1;
	p->mRoundsWon = 0;

	resetHelperState(p);
	p->mIsHelper = 0;
	p->mParent = NULL;
	p->mHelperIDInParent = -1;

	p->mIsProjectile = 0;
	p->mProjectileID = -1;
	p->mProjectileDataID = -1;

	p->mPower = 0; 
	
	p->mIsBoundToScreen = 1;
}

static void loadPlayerStateWithConstantsLoaded(Player* p) {
	p->mLife = p->mConstants.mHeader.mLife; 
}

static void loadSinglePlayerFromMugenDefinition(Player* p)
{
	MugenDefScript script = loadMugenDefScript(p->mDefinitionPath);

	loadPlayerState(p);
	loadPlayerHeaderFromScript(&p->mHeader, &script);
	loadPlayerFiles(p->mDefinitionPath, p, &script);
	loadPlayerStateWithConstantsLoaded(p);
	unloadMugenDefScript(script);
	

}

void loadPlayers() {
	int i = 0;
	for (i = 0; i < 2; i++) {
		gData.mPlayers[i].mRoot = &gData.mPlayers[i];
		gData.mPlayers[i].mOtherPlayer = &gData.mPlayers[i ^ 1];
		gData.mPlayers[i].mPreferredPalette = 0; // TODO
		gData.mPlayers[i].mRootID = i;
		gData.mPlayers[i].mControllerID = i; // TODO: remove
		loadSinglePlayerFromMugenDefinition(&gData.mPlayers[i]);
	}
}

static void resetSinglePlayer(Player* p) {
	p->mIsAlive = 1;

	p->mLife = p->mConstants.mHeader.mLife;
	setLifeBarPercentage(p, 1);

	setPlayerPosition(p, getPlayerStartingPosition(p->mRootID, getPlayerCoordinateP(p)), getPlayerCoordinateP(p));
	
	if (doesPlayerStartFacingLeft(p->mRootID)) {
		setPlayerFaceDirection(p, FACE_DIRECTION_LEFT);
	}
	else {
		setPlayerFaceDirection(p, FACE_DIRECTION_RIGHT);
	}
}

void resetPlayers()
{
	resetSinglePlayer(&gData.mPlayers[0]);
	resetSinglePlayer(&gData.mPlayers[1]);
}

static void resetSinglePlayerEntirely(Player* p) {
	p->mRoundsWon = 0;
	p->mRoundsExisted = 0;
	p->mPower = 0;
	setPowerBarPercentage(p, 0);
}

void resetPlayersEntirely()
{
	resetPlayers();
	resetSinglePlayerEntirely(&gData.mPlayers[0]);
	resetSinglePlayerEntirely(&gData.mPlayers[1]);
}



static void updateWalking(Player* p) {

	if (p->mNoWalkFlag) {
		p->mNoWalkFlag = 0;
		return;
	}

	if (!p->mIsInControl) return;

	if (getPlayerStateType(p) != MUGEN_STATE_TYPE_STANDING) return;

	if (isPlayerCommandActive(p, "holdfwd") || isPlayerCommandActive(p, "holdback")) {
		if (getPlayerState(p) != 20) {
			changePlayerState(p, 20);
		}
	}
	else if (getPlayerState(p) == 20) {
		changePlayerState(p, 0);
	}
}

static void updateJumping(Player* p) {
	if (!p->mIsInControl) return;
	if (getPlayerStateType(p) != MUGEN_STATE_TYPE_STANDING) return;


	if (isPlayerCommandActive(p, "holdup")) {
		changePlayerState(p, 40);
	}
}

static void updateLanding(Player* p) {
	if (p->mNoLandFlag) {
		p->mNoLandFlag = 0;
		return;
	}

	Position pos = *getHandledPhysicsPositionReference(p->mPhysicsID);
	if (getPlayerPhysics(p) != MUGEN_STATE_PHYSICS_AIR) return;
	if (isPlayerPaused(p)) return;

	Velocity vel = *getHandledPhysicsVelocityReference(p->mPhysicsID);
	if (pos.y >= 0 && vel.y >= 0) {
		setPlayerControl(p, 1);
		changePlayerState(p, 0);
	}
}

static void updateCrouchingDown(Player* p) {
	if (!p->mIsInControl) return;
	if (getPlayerStateType(p) != MUGEN_STATE_TYPE_STANDING) return;


	if (isPlayerCommandActive(p, "holddown")) {
		changePlayerState(p, 10);
	}
}

static void updateStandingUp(Player* p) {
	if (!p->mIsInControl) return;
	if (getPlayerStateType(p) != MUGEN_STATE_TYPE_CROUCHING) return;


	if (!isPlayerCommandActive(p, "holddown")) {
		changePlayerState(p, 12);
	}
}

static void setPlayerFaceDirection(Player* p, FaceDirection tDirection) {
	if (p->mFaceDirection == tDirection) return;

	setRegisteredAnimationFaceDirection(p->mAnimationID, tDirection);

	if (!p->mIsHelper) {
		setMugenCommandFaceDirection(p->mCommandID, tDirection);
	}

	p->mFaceDirection = tDirection;
}

static void updateAutoTurn(Player* p) {
	if (p->mNoAutoTurnFlag) {
		p->mNoAutoTurnFlag = 0;
		return;
	}

	if (!p->mIsInControl) return;

	if (getPlayerStateType(p) == MUGEN_STATE_TYPE_AIR) return;

	Player* p2 = getPlayerOtherPlayer(p);

	double x1 = getHandledPhysicsPositionReference(p->mPhysicsID)->x;
	double x2 = getHandledPhysicsPositionReference(p2->mPhysicsID)->x;

	if (x1 > x2) setPlayerFaceDirection(p, FACE_DIRECTION_LEFT);
	else if (x1 < x2) setPlayerFaceDirection(p, FACE_DIRECTION_RIGHT);
}

static void updatePositionFreeze(Player* p) {
	if (p->mIsFrozen) {
		Position* pos = getHandledPhysicsPositionReference(p->mPhysicsID);
		*pos = p->mFreezePosition;
		p->mIsFrozen = 0;
	}
}

static void updateGettingUp(Player* p) {
	if (getPlayerState(p) != 5110 && !p->mIsLyingDown) return;

	if (!p->mIsLyingDown) {
		increasePlayerFallAmountInCombo(p);
		p->mLyingDownTime = 0;
		p->mIsLyingDown = 1;
		return;
	}

	if (p->mIsLyingDown && getPlayerState(p) != 5110) {
		p->mIsLyingDown = 0;
		return;
	}

	if (handleDurationAndCheckIfOver(&p->mLyingDownTime, p->mConstants.mHeader.mLiedownTime)) {
		p->mIsLyingDown = 0;
		changePlayerState(p, 5120);
	}

}

static void updateHitPause(Player* p) {
	if (!p->mIsHitPaused) return;

	if (handleDurationAndCheckIfOver(&p->mHitPauseNow, p->mHitPauseDuration)) {
		setPlayerUnHitPaused(p);
	}
}

static void updateBindingPosition(Player* p) {
	Player* bindRoot = p->mBoundTarget;
	Position pos = getPlayerPosition(bindRoot, getPlayerCoordinateP(p));

	if (p->mBoundPositionType == PLAYER_BIND_POSITION_TYPE_HEAD) {
		pos = vecAdd(pos, getPlayerHeadPosition(bindRoot));
	}
	else if (p->mBoundPositionType == PLAYER_BIND_POSITION_TYPE_MID) {
		pos = vecAdd(pos, getPlayerMiddlePosition(bindRoot));
	}

	setPlayerPosition(p, pos, getPlayerCoordinateP(p));
}

static void removePlayerBinding(Player* tPlayer) {
	tPlayer->mIsBound = 0;

	Player* boundTo = tPlayer->mBoundTarget;
	list_remove(&boundTo->mBoundHelpers, tPlayer->mBoundID);
}

static void updateBinding(Player* p) {
	if (!p->mIsBound) return;
	if (isPlayerPaused(p)) return;

	Player* bindRoot = p->mBoundTarget;
	updateBindingPosition(p);

	if (p->mBoundFaceSet) {
		if (p->mBoundFaceSet == 1) setPlayerIsFacingRight(p, getPlayerIsFacingRight(bindRoot));
		else setPlayerIsFacingRight(p, !getPlayerIsFacingRight(bindRoot));
	}

	p->mBoundNow++;
	if (p->mBoundNow >= p->mBoundDuration) {
		removePlayerBinding(p);
	}
}

static int isPlayerGuarding(Player* p) {
	(void)p;
	return getPlayerState(p) >= 120 && getPlayerState(p) <= 155; // TODO: properly
}

static void updateGuarding(Player* p) {
	if (isPlayerPaused(p)) return;
	if (!p->mIsInControl) return;
	if (isPlayerGuarding(p)) {
		
		return;
	}

	if (isPlayerCommandActive(p, "holdback") && isPlayerBeingAttacked(p) && isPlayerInGuardDistance(p)) {
		changePlayerState(p, 120);
	}
}

static void setPlayerUnguarding(Player* p) {
	if (getPlayerStateType(p) == MUGEN_STATE_TYPE_STANDING) {
		changePlayerState(p, 0);
	}
	else if (getPlayerStateType(p) == MUGEN_STATE_TYPE_CROUCHING) {
		changePlayerState(p, 11);
	}
	else if (getPlayerStateType(p) == MUGEN_STATE_TYPE_AIR) {
		changePlayerState(p, 51);
	}
	else {
		logError("Unknown player state.");
		logErrorInteger(getPlayerStateType(p));
		abortSystem();
	}
}

static void updateGuardingOver(Player* p) {
	if (isPlayerPaused(p)) return;
	if (!p->mIsInControl) return;
	if (getPlayerState(p) != 140) return;
	if (getRemainingPlayerAnimationTime(p)) return;


	setPlayerUnguarding(p);
}
static void updateSinglePlayer(Player* p);

static void updateSinglePlayerCB(void* tCaller, void* tData) {
	(void)tCaller;
	Player* p = tData;
	updateSinglePlayer(p);
}

static void updateSingleHitAttributeSlot(HitDefAttributeSlot* tSlot) {
	if (!tSlot->mIsActive) return;

	tSlot->mNow++;
	if (tSlot->mNow >= tSlot->mTime) {
		tSlot->mIsActive = 0;
	}
}

static void updateHitAttributeSlots(Player* p) {
	if (isPlayerPaused(p)) return;

	int i;
	for (i = 0; i < 2; i++) {
		updateSingleHitAttributeSlot(&p->mNotHitBy[i]);
	}
}

static void updatePush(Player* p) {
	if (isPlayerPaused(p)) return;
	if (isPlayerHelper(p)) return;
	if (!getPlayerControl(p)) return;
	if (p->mPushDisabledFlag) return;

	Player* otherPlayer = getPlayerOtherPlayer(p);
	if (otherPlayer->mPushDisabledFlag) return;

	double frontX1 = getPlayerFrontX(p);
	double frontX2 = getPlayerFrontX(otherPlayer);

	double x1 = getPlayerPositionX(p, getPlayerCoordinateP(p));
	double x2 = getPlayerPositionX(otherPlayer, getPlayerCoordinateP(p));
	
	double distX = getPlayerDistanceToFrontOfOtherPlayerX(p);
	double distY = getPlayerAxisDistanceY(p);

	if (distY >= 0 && distY >= otherPlayer->mConstants.mSizeData.mHeight) return;
	if (distY <= 0 && distY <= -p->mConstants.mSizeData.mHeight) return;

	if (x1 < x2) {
		if (frontX1 > frontX2) {
			setPlayerPositionX(p, x1 - distX / 2, getPlayerCoordinateP(p));
			setPlayerPositionX(otherPlayer, x2 + distX / 2, getPlayerCoordinateP(p));
		}
	}
	else {
		if (frontX1 < frontX2) {
			setPlayerPositionX(p, x1 + distX / 2, getPlayerCoordinateP(p));
			setPlayerPositionX(otherPlayer, x2 - distX / 2, getPlayerCoordinateP(p));
		}
	}

}

static void updatePushFlags() {
	getRootPlayer(0)->mPushDisabledFlag = 0;
	getRootPlayer(1)->mPushDisabledFlag = 0;
}

static void updateStageBorder(Player* p) {
	if (!p->mIsBoundToScreen) return;

	double left = getStageLeftOfScreenBasedOnPlayer(getPlayerCoordinateP(p));
	double right = getStageRightOfScreenBasedOnPlayer(getPlayerCoordinateP(p));
	int lx = getStageLeftEdgeMinimumPlayerDistance(getPlayerCoordinateP(p));
	int rx = getStageRightEdgeMinimumPlayerDistance(getPlayerCoordinateP(p));
	double x = getPlayerPositionX(p, getPlayerCoordinateP(p));

	if (x < left + lx) {
		setPlayerPositionX(p, left + lx, getPlayerCoordinateP(p));
	}
	else if (x > right - rx) {
		setPlayerPositionX(p, right-rx, getPlayerCoordinateP(p));
	}
}

static void updateSinglePlayer(Player* p) {
	updateWalking(p);
	updateJumping(p);
	updateLanding(p);
	updateCrouchingDown(p);
	updateStandingUp(p);
	updateAutoTurn(p);
	updatePositionFreeze(p);
	updateGettingUp(p);
	updateHitPause(p);
	updateBinding(p);
	updateGuarding(p);
	updateGuardingOver(p);
	updateHitAttributeSlots(p);
	updatePush(p);
	updateStageBorder(p);

	list_map(&p->mHelpers, updateSinglePlayerCB, NULL);
}

void updatePlayers()
{
	int i;
	for (i = 0; i < 2; i++) {
		updateSinglePlayer(&gData.mPlayers[i]);
	}

	updatePushFlags();
}

static void setPlayerHitOver(void* tCaller) {
	Player* p = tCaller;

	p->mIsHitOver = 1;
}

static void setPlayerHitShakeOver(void* tCaller) {
	Player* p = tCaller;

	p->mIsHitShakeOver = 1;


	Duration hitDuration;
	if (isPlayerGuarding(p)) {
		hitDuration = getActiveHitDataGuardHitTime(p);
	}
	else if(getPlayerStateType(p) == MUGEN_STATE_TYPE_AIR){
		hitDuration = getActiveHitDataAirHitTime(p);
	}
	else {
		hitDuration = getActiveHitDataGroundHitTime(p);
	}

	setPlayerVelocityX(p, getActiveHitDataVelocityX(p), getPlayerCoordinateP(p));
	setPlayerVelocityY(p, getActiveHitDataVelocityY(p), getPlayerCoordinateP(p));

	addTimerCB(hitDuration, setPlayerHitOver, p);
}

static void setPlayerHitStates(Player* p, Player* tOtherPlayer) {
	if (getActiveHitDataPlayer1StateNumber(p) != -1) {
		changePlayerState(tOtherPlayer, getActiveHitDataPlayer1StateNumber(p));
	}

	if (getActiveHitDataPlayer2StateNumber(p) == -1) {

		int nextState;

		if (getPlayerStateType(p) == MUGEN_STATE_TYPE_STANDING) {
			if (isPlayerGuarding(p)) {
				nextState = 150;
			}
			else {
				nextState = 5000;
			}
		}
		else if (getPlayerStateType(p) == MUGEN_STATE_TYPE_CROUCHING) {
			if (isPlayerGuarding(p)) {
				nextState = 152;
			}
			else {
				nextState = 5010;
			}
		}
		else if (getPlayerStateType(p) == MUGEN_STATE_TYPE_AIR) {
			if (isPlayerGuarding(p)) {
				nextState = 154;
			}
			else {
				nextState = 5020;
			}
		}
		else {
			logError("Unrecognized player state type.");
			logErrorInteger(getPlayerStateType(p));
			abortSystem();
			nextState = 5000;
		}

		changePlayerState(p, nextState);
	}
	else if (getActiveHitDataPlayer2CapableOfGettingPlayer1State(p)) {
		changePlayerStateToOtherPlayerStateMachine(p, tOtherPlayer, getActiveHitDataPlayer2StateNumber(p));
	}
	else {
		changePlayerState(p, getActiveHitDataPlayer2StateNumber(p));
	}
}

static int checkPlayerHitGuardFlagsAndReturnIfGuardable(Player* tPlayer, char* tFlags) {
	MugenStateType type = getPlayerStateType(tPlayer);
	char test[100];
	strcpy(test, tFlags);
	turnStringLowercase(test);

	if (type == MUGEN_STATE_TYPE_STANDING) {

		return strchr(test, 'h') != NULL || strchr(test, 'm') != NULL;
	} else  if (type == MUGEN_STATE_TYPE_CROUCHING) {
		return strchr(test, 'l') != NULL || strchr(test, 'm') != NULL;
	}
	else  if (type == MUGEN_STATE_TYPE_AIR) {
		return strchr(test, 'a') != NULL;
	}
	else {
		logError("Unrecognized player type.");
		logErrorInteger(type);
		abortSystem();
		return 0;
	}

}

static void setPlayerUnguarding(Player* p);

static void setPlayerHit(Player* p, Player* tOtherPlayer, void* tHitData) {
	setPlayerControl(p, 0);

	copyHitDataToActive(p, tHitData);

	if (isPlayerGuarding(p) && !checkPlayerHitGuardFlagsAndReturnIfGuardable(p, getActiveHitDataGuardFlag(p))) {
		setPlayerUnguarding(p);
	}

	if (isPlayerGuarding(p)) {
		setActiveHitDataVelocityX(p, getActiveHitDataGuardVelocity(p));
		setActiveHitDataVelocityY(p, 0);
		p->mIsFalling = 0; 
	}
	else if (getPlayerStateType(p) == MUGEN_STATE_TYPE_AIR) {
		setActiveHitDataVelocityX(p, getActiveHitDataAirVelocityX(p));
	 	setActiveHitDataVelocityY(p, getActiveHitDataAirVelocityY(p));
		p->mIsFalling = getActiveHitDataAirFall(p);
	}
	else {
		setActiveHitDataVelocityX(p, getActiveHitDataGroundVelocityX(p));
		setActiveHitDataVelocityY(p, getActiveHitDataGroundVelocityY(p));
		p->mIsFalling = getActiveHitDataFall(p);
	}

	int hitShakeDuration, hitPauseDuration;
	int damage;
	int powerUp1;
	int powerUp2;
	if (isPlayerGuarding(p)) {
		damage = (int)(getActiveHitDataGuardDamage(p) * p->mAttackMultiplier);
		setPlayerMoveGuarded(tOtherPlayer);
		powerUp1 = getActiveHitDataPlayer2GuardPowerAdded(p);
		powerUp2 = getActiveHitDataPlayer1GuardPowerAdded(p);
		hitPauseDuration = getActiveHitDataPlayer1GuardPauseTime(p);
		hitShakeDuration = getActiveHitDataPlayer2GuardPauseTime(p);
	}
	else {
		damage = (int)(getActiveHitDataDamage(p) * p->mAttackMultiplier);
		setPlayerMoveHit(tOtherPlayer);
		powerUp1 = getActiveHitDataPlayer2PowerAdded(p);
		powerUp2 = getActiveHitDataPlayer1PowerAdded(p);
		hitPauseDuration = getActiveHitDataPlayer1PauseTime(p);
		hitShakeDuration = getActiveHitDataPlayer2PauseTime(p);
	}

	p->mIsHitShakeOver = 0;
	p->mIsHitOver = 0;
	
	p->mCanRecoverFromFall = getActiveHitDataFallRecovery(p); // TODO: add time

	addPlayerDamage(p, damage);
	addPlayerPower(p, powerUp1);
	addPlayerPower(tOtherPlayer, powerUp2);

	setPlayerHitStates(p, tOtherPlayer);
	if (hitShakeDuration) {
		setPlayerHitPaused(p, hitShakeDuration);
	}

	if (hitPauseDuration) {
		setPlayerHitPaused(tOtherPlayer, hitPauseDuration);
	}

	addTimerCB(hitShakeDuration, setPlayerHitShakeOver, p);
}

static void playPlayerHitSpark(Player* p1, Player* p2, int tIsInPlayerFile, int tNumber, Position tSparkOffset) {
	if (tNumber == -1) return;

	Position pos1 = *getHandledPhysicsPositionReference(p1->mPhysicsID);
	Position pos2 = *getHandledPhysicsPositionReference(p2->mPhysicsID);

	Position base;
	if (pos1.x < pos2.x) {
		int width = p1->mFaceDirection == FACE_DIRECTION_RIGHT ? p1->mConstants.mSizeData.mGroundFrontWidth : p1->mConstants.mSizeData.mGroundBackWidth;
		base = vecAdd(pos1, makePosition(width, 0, 0));
		base.y = pos2.y;
	}
	else {
		int width = p1->mFaceDirection == FACE_DIRECTION_LEFT ? p1->mConstants.mSizeData.mGroundFrontWidth : p1->mConstants.mSizeData.mGroundBackWidth;
		base = vecAdd(pos1, makePosition(-width, 0, 0));
		base.y = pos2.y;
	}

	if (p1->mFaceDirection == FACE_DIRECTION_LEFT) {
		tSparkOffset.x = -tSparkOffset.x;
	}

	tSparkOffset = vecAdd(tSparkOffset, base);

	playHitSpark(tSparkOffset, p2, tIsInPlayerFile, tNumber, getActiveHitDataIsFacingRight(p1), getPlayerCoordinateP(p2), getPlayerCoordinateP(p2));
}

typedef struct {
	int mFound;
	MugenAttackType mType;
	MugenAttackClass mClass;
} HitDefAttributeFlag2Caller;

static void checkSingleFlag2InHitDefAttributeSlot(HitDefAttributeFlag2Caller* tCaller, char* tFlag) {

	if (tCaller->mClass == MUGEN_ATTACK_CLASS_NORMAL) {
		if (tFlag[0] != 'n') return;
	}
	else if (tCaller->mClass == MUGEN_ATTACK_CLASS_SPECIAL) {
		if (tFlag[0] != 's') return;
	}
	else if (tCaller->mClass == MUGEN_ATTACK_CLASS_HYPER) {
		if (tFlag[0] != 'h') return;
	}
	else {
		logError("Unrecognized attack type.");
		logErrorInteger(tCaller->mType);
		abortSystem();
	}

	if (tCaller->mType == MUGEN_ATTACK_TYPE_ATTACK) {
		if (tFlag[1] != 'a') return;
	} 
	else if (tCaller->mType == MUGEN_ATTACK_TYPE_PROJECTILE) {
		if (tFlag[1] != 'p') return;
	}
	else if (tCaller->mType == MUGEN_ATTACK_TYPE_THROW) {
		if (tFlag[1] != 't') return;
	}
	else {
		logError("Unrecognized attack type.");
		logErrorInteger(tCaller->mType);
		abortSystem();
	}

	tCaller->mFound = 1;
}

static int checkSingleNoHitDefSlot(HitDefAttributeSlot* tSlot, Player* p2) {
	if (!tSlot->mIsActive) return 1;

	MugenStateType type = getHitDataType(p2);

	if (type == MUGEN_STATE_TYPE_STANDING) {
		if (strchr(tSlot->mFlag1, 's') != NULL) return tSlot->mIsHitBy;
	} 
	else if (type == MUGEN_STATE_TYPE_CROUCHING) {
		if (strchr(tSlot->mFlag1, 'c') != NULL) return tSlot->mIsHitBy;
	}
	else if (type == MUGEN_STATE_TYPE_AIR) {
		if (strchr(tSlot->mFlag1, 'a') != NULL) return tSlot->mIsHitBy;
	}
	else {
		logError("Invalid hitdef type.");
		logErrorInteger(type);
		abortSystem();
		return 0;
	}

	HitDefAttributeFlag2Caller caller;
	caller.mFound = 0;
	caller.mType = getHitDataAttackType(p2);
	caller.mClass = getHitDataAttackClass(p2);

	int i;
	for (i = 0; i < tSlot->mFlag2Amount; i++) {
		checkSingleFlag2InHitDefAttributeSlot(&caller, tSlot->mFlag2[i]);
	}

	if (caller.mFound) return tSlot->mIsHitBy;
	else return !tSlot->mIsHitBy;
}

static int checkActiveHitDefAttributeSlots(Player* p, Player* p2) {
	int i;
	for (i = 0; i < 2; i++) {
		if (!checkSingleNoHitDefSlot(&p->mNotHitBy[i], p2)) return 0;
	}

	return 1;
}

void playerHitCB(Player* p, void* tHitData)
{
	// TOOD: reversaldef

	Player* otherPlayer = getReceivedHitDataPlayer(tHitData);

	if (getPlayerStateType(p) == MUGEN_STATE_TYPE_LYING) return;
	if (!isReceivedHitDataActive(tHitData)) return;
	if (!checkActiveHitDefAttributeSlots(p, otherPlayer)) return;

	setPlayerHit(p, otherPlayer, tHitData);
	setReceivedHitDataInactive(tHitData);
	
	if (isPlayerGuarding(p)) {
		playPlayerHitSpark(p, otherPlayer, isActiveHitDataGuardSparkInPlayerFile(p), getActiveHitDataGuardSparkNumber(p), getActiveHitDataSparkXY(p));
	}
	else {
		increasePlayerHitCount(otherPlayer);
		playPlayerHitSpark(p, otherPlayer, isActiveHitDataSparkInPlayerFile(p), getActiveHitDataSparkNumber(p), getActiveHitDataSparkXY(p));
	}
	
	setPlayerMoveContactCounterActive(otherPlayer);

	if (isPlayerProjectile(otherPlayer)) {
		handleProjectileHit(otherPlayer);
	}
}

void setPlayerDefinitionPath(int i, char * tDefinitionPath)
{
	strcpy(gData.mPlayers[i].mDefinitionPath, tDefinitionPath);
}

Player * getRootPlayer(int i)
{
	return &gData.mPlayers[i];
}

Player * getPlayerRoot(Player * p)
{
	return p->mRoot;
}

Player * getPlayerParent(Player * p)
{
	return p->mParent;
}

int getPlayerState(Player* p)
{
	return getRegisteredStateState(p->mStateMachineID);
}

int getPlayerPreviousState(Player* p)
{
	return getRegisteredStatePreviousState(p->mStateMachineID);
}

MugenStateType getPlayerStateType(Player* p)
{
	return p->mStateType;
}

void setPlayerStateType(Player* p, MugenStateType tType)
{
	if (tType == MUGEN_STATE_TYPE_UNCHANGED) return;

	p->mStateType = tType;
}

MugenStateMoveType getPlayerStateMoveType(Player* p)
{
	return p->mMoveType;
}

void setPlayerStateMoveType(Player* p, MugenStateMoveType tType)
{
	if (tType == MUGEN_STATE_MOVE_TYPE_UNCHANGED) return;

	p->mMoveType = tType;
}

int getPlayerControl(Player* p)
{
	return p->mIsInControl;
}

void setPlayerControl(Player* p, int tNewControl)
{
	p->mIsInControl = tNewControl;
}


MugenStatePhysics getPlayerPhysics(Player* p) {
	return p->mStatePhysics;
}

void setPlayerPhysics(Player* p, MugenStatePhysics tNewPhysics)
{

	if (tNewPhysics == MUGEN_STATE_PHYSICS_UNCHANGED) {
		return;
	}
	else if (tNewPhysics == MUGEN_STATE_PHYSICS_STANDING) {
		setHandledPhysicsDragCoefficient(p->mPhysicsID, makePosition(p->mConstants.mMovementData.mStandFiction, 0, 0));
		setHandledPhysicsGravity(p->mPhysicsID, makePosition(0, 0, 0));
		Position* pos = getHandledPhysicsPositionReference(p->mPhysicsID);
		Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
		Acceleration* acc = getHandledPhysicsAccelerationReference(p->mPhysicsID);
		pos->y = 0;
		vel->y = 0;
		acc->y = 0;
	}
	else if (tNewPhysics == MUGEN_STATE_PHYSICS_CROUCHING) {
		setHandledPhysicsDragCoefficient(p->mPhysicsID, makePosition(p->mConstants.mMovementData.mCrouchFriction, 0, 0));
		setHandledPhysicsGravity(p->mPhysicsID, makePosition(0, 0, 0));
		Position* pos = getHandledPhysicsPositionReference(p->mPhysicsID);
		Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
		Acceleration* acc = getHandledPhysicsAccelerationReference(p->mPhysicsID);
		pos->y = 0;
		vel->y = 0;
		acc->y = 0;
	}
	else if (tNewPhysics == MUGEN_STATE_PHYSICS_NONE) {
		setHandledPhysicsDragCoefficient(p->mPhysicsID, makePosition(0, 0, 0));
		setHandledPhysicsGravity(p->mPhysicsID, makePosition(0, 0, 0));


		if (getPlayerStateType(p) == MUGEN_STATE_TYPE_LYING) {
			Position* pos = getHandledPhysicsPositionReference(p->mPhysicsID);
			Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
			Acceleration* acc = getHandledPhysicsAccelerationReference(p->mPhysicsID);
			pos->y = 0;
			vel->y = 0;
			acc->y = 0;
		}
	}
	else if (tNewPhysics == MUGEN_STATE_PHYSICS_AIR) {
		setHandledPhysicsDragCoefficient(p->mPhysicsID, makePosition(0, 0, 0));
		setHandledPhysicsGravity(p->mPhysicsID, makePosition(0, p->mConstants.mMovementData.mVerticalAcceleration, 0));

		if (tNewPhysics != p->mStatePhysics) {
			setPlayerNoLandFlag(p);
		}
	}
	else {
		logError("Unrecognized physics state.");
		logErrorInteger(tNewPhysics);
		abortSystem();
	}

	p->mStatePhysics = tNewPhysics;
}

int getPlayerMoveContactCounter(Player* p)
{
	return p->mMoveContactCounter;
}

static void increaseComboCounter(Player* p) {

	p->mComboCounter++;
}

static void resetComboCounter(Player* p) {
	if (!getPlayerControl(p)) return;

	p->mComboCounter = 0;
}

void resetPlayerMoveContactCounter(Player* p)
{
	p->mMoveContactCounter = 0;
	resetComboCounter(p);
}

void setPlayerMoveContactCounterActive(Player* p) {
	p->mMoveContactCounter = 1;
	increaseComboCounter(p);
}

int getPlayerVariable(Player* p, int tIndex)
{
	// assert(tIndex < 100); // TODO: figure out
	return p->mVars[tIndex];
}

void setPlayerVariable(Player* p, int tIndex, int tValue)
{
	// assert(tIndex < 100); // TODO: figure out
	p->mVars[tIndex] = tValue;
}

void addPlayerVariable(Player * p, int tIndex, int tValue)
{
	int cur = getPlayerVariable(p, tIndex);
	cur += tValue;
	setPlayerVariable(p, tIndex, cur);
}

int getPlayerSystemVariable(Player* p, int tIndex)
{
	// assert(tIndex < 100); // TODO: figure out
	return p->mSystemVars[tIndex];
}

void setPlayerSystemVariable(Player* p, int tIndex, int tValue)
{
	// assert(tIndex < 100); // TODO: figure out
	p->mSystemVars[tIndex] = tValue;
}

void addPlayerSystemVariable(Player * p, int tIndex, int tValue)
{
	int cur = getPlayerSystemVariable(p, tIndex);
	cur += tValue;
	setPlayerSystemVariable(p, tIndex, cur);
}

double getPlayerFloatVariable(Player* p, int tIndex)
{
	// assert(tIndex < 100); // TODO: figure out
	return p->mFloatVars[tIndex];
}

void setPlayerFloatVariable(Player* p, int tIndex, double tValue)
{
	// assert(tIndex < 100); // TODO: figure out
	p->mFloatVars[tIndex] = tValue;
}

void addPlayerFloatVariable(Player * p, int tIndex, double tValue)
{
	double cur = getPlayerFloatVariable(p, tIndex);
	cur += tValue;
	setPlayerFloatVariable(p, tIndex, cur);
}

double getPlayerSystemFloatVariable(Player* p, int tIndex)
{
	// assert(tIndex < 100); // TODO: figure out
	return p->mSystemFloatVars[tIndex];
}

void setPlayerSystemFloatVariable(Player* p, int tIndex, double tValue)
{
	// assert(tIndex < 100); // TODO: figure out
	p->mSystemFloatVars[tIndex] = tValue;
}

void addPlayerSystemFloatVariable(Player * p, int tIndex, double tValue)
{
	double cur = getPlayerSystemFloatVariable(p, tIndex);
	cur += tValue;
	setPlayerSystemFloatVariable(p, tIndex, cur);
}

int getPlayerTimeInState(Player* p)
{
	return getRegisteredStateTimeInState(p->mStateMachineID);
}

int getPlayerAnimationNumber(Player* p)
{
	return getRegisteredAnimationAnimationNumber(p->mAnimationID);
}

int getRemainingPlayerAnimationTime(Player* p)
{
	// printf("%d %d remaining time %d\n", p->mRootID, p->mID, getRegisteredAnimationRemainingAnimationTime(p->mAnimationID));
	return getRegisteredAnimationRemainingAnimationTime(p->mAnimationID);
}

Vector3D getPlayerPosition(Player* p, int tCoordinateP)
{
	return makePosition(getPlayerPositionX(p, tCoordinateP), getPlayerPositionY(p, tCoordinateP), 0);
}

double getPlayerPositionBasedOnScreenCenterX(Player * p, int tCoordinateP)
{
	Position pos = getStageCenterOfScreenBasedOnPlayer(tCoordinateP);
	Position ret = vecSub(getPlayerPosition(p, tCoordinateP), pos);
	return ret.x;
}

double getPlayerPositionX(Player* p, int tCoordinateP)
{
	double scale = tCoordinateP / getPlayerCoordinateP(p);
	return getHandledPhysicsPositionReference(p->mPhysicsID)->x * scale;
}

double getPlayerPositionBasedOnStageFloorY(Player * p, int tCoordinateP)
{
	Position pos = getStageCenterOfScreenBasedOnPlayer(tCoordinateP);
	Position ret = vecSub(getPlayerPosition(p, tCoordinateP), pos);
	return ret.y;
}

double getPlayerPositionY(Player* p, int tCoordinateP)
{
	double scale = tCoordinateP / getPlayerCoordinateP(p);
	return getHandledPhysicsPositionReference(p->mPhysicsID)->y * scale;
}

double getPlayerVelocityX(Player* p, int tCoordinateP)
{
	double scale = tCoordinateP / getPlayerCoordinateP(p);
	double x = getHandledPhysicsVelocityReference(p->mPhysicsID)->x * scale;
	if (p->mFaceDirection == FACE_DIRECTION_LEFT) x *= -1;

	return  x;
}

double getPlayerVelocityY(Player* p, int tCoordinateP)
{
	double scale = tCoordinateP / getPlayerCoordinateP(p);
	return getHandledPhysicsVelocityReference(p->mPhysicsID)->y * scale;
}

double getPlayerStandFrictionThreshold(Player* p)
{
	return p->mConstants.mMovementData.mStandFrictionThreshold;
}

double getPlayerCrouchFrictionThreshold(Player* p)
{
	return p->mConstants.mMovementData.mCrouchFrictionThreshold;
}

double getPlayerAirGetHitGroundLevelY(Player* p)
{
	return p->mConstants.mMovementData.mAirGetHitGroundLevelY;
}

double getPlayerAirGetHitGroundRecoveryGroundLevelY(Player * p)
{
	return p->mConstants.mMovementData.mAirGetHitGroundRecoveryGroundGoundLevelY;
}

double getPlayerAirGetHitGroundRecoveryGroundYTheshold(Player* p)
{
	return p->mConstants.mMovementData.mAirGetHitGroundRecoveryGroundYTheshold;
}

double getPlayerAirGetHitAirRecoveryVelocityYThreshold(Player* p)
{
	return p->mConstants.mMovementData.mAirGetHitAirRecoveryVelocityYThreshold;
}

double getPlayerAirGetHitTripGroundLevelY(Player* p)
{
	return p->mConstants.mMovementData.mAirGetHitTripGroundLevelY;
}

double getPlayerDownBounceOffsetX(Player* p)
{
	return p->mConstants.mMovementData.mBounceOffset.x;
}

double getPlayerDownBounceOffsetY(Player* p)
{
	return p->mConstants.mMovementData.mBounceOffset.y;
}

double getPlayerDownVerticalBounceAcceleration(Player* p)
{
	return p->mConstants.mMovementData.mVerticalBounceAcceleration;
}

double getPlayerDownBounceGroundLevel(Player* p)
{
	return p->mConstants.mMovementData.mBounceGroundLevel;
}

double getPlayerLyingDownFrictionThreshold(Player* p)
{
	return p->mConstants.mMovementData.mLyingDownFrictionThreshold;
}

double getPlayerVerticalAcceleration(Player* p)
{
	return p->mConstants.mMovementData.mVerticalAcceleration;
}

double getPlayerForwardWalkVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mWalkForward.x;
}

double getPlayerBackwardWalkVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mWalkBackward.x;
}

double getPlayerForwardRunVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mRunForward.x;
}

double getPlayerForwardRunVelocityY(Player* p)
{
	return p->mConstants.mVelocityData.mRunForward.y;
}

double getPlayerBackwardRunVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mRunBackward.x;
}

double getPlayerBackwardRunVelocityY(Player* p)
{
	return p->mConstants.mVelocityData.mRunBackward.y;
}

double getPlayerForwardRunJumpVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mRunJumpForward.x;
}

double getPlayerNeutralJumpVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mJumpNeutral.x;
}

double getPlayerForwardJumpVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mJumpForward.x;
}

double getPlayerBackwardJumpVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mJumpBackward.x;
}

double getPlayerJumpVelocityY(Player* p)
{
	return p->mConstants.mVelocityData.mJumpNeutral.y;
}

double getPlayerNeutralAirJumpVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mAirJumpNeutral.x;
}

double getPlayerForwardAirJumpVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mAirJumpForward.x;
}

double getPlayerBackwardAirJumpVelocityX(Player* p)
{
	return p->mConstants.mVelocityData.mAirJumpBackward.x;
}

double getPlayerAirJumpVelocityY(Player* p)
{
	return p->mConstants.mVelocityData.mAirJumpNeutral.y;
}


int isPlayerAlive(Player* p)
{
	return p->mIsAlive;
}

void setPlayerVelocityX(Player* p, double x, int tCoordinateP)
{
	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
	double fx = x*scale;
	if (p->mFaceDirection == FACE_DIRECTION_LEFT) fx *= -1;
	vel->x = fx;
}

void setPlayerVelocityY(Player* p, double y, int tCoordinateP)
{
	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
	vel->y = y*scale;
}

void multiplyPlayerVelocityX(Player* p, double x, int tCoordinateP)
{
	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
	double fx = x * scale;
	vel->x *= fx;
}

void multiplyPlayerVelocityY(Player* p, double y, int tCoordinateP)
{
	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
	vel->y *= y*scale;
}

void addPlayerVelocityX(Player* p, double x, int tCoordinateP)
{
	if (isPlayerPaused(p)) return;

	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
	double fx = x*scale;
	if (p->mFaceDirection == FACE_DIRECTION_LEFT) fx *= -1;
	vel->x += fx;
}

void addPlayerVelocityY(Player* p, double y, int tCoordinateP)
{
	if (isPlayerPaused(p)) return;

	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Velocity* vel = getHandledPhysicsVelocityReference(p->mPhysicsID);
	vel->y += y*scale;
}

void setPlayerPosition(Player * p, Position tPosition, int tCoordinateP)
{
	setPlayerPositionX(p, tPosition.x, tCoordinateP);
	setPlayerPositionY(p, tPosition.y, tCoordinateP);
}

void setPlayerPositionX(Player* p, double x, int tCoordinateP)
{
	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Position* pos = getHandledPhysicsPositionReference(p->mPhysicsID);
	pos->x = x*scale;
}

void setPlayerPositionY(Player* p, double y, int tCoordinateP)
{
	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Position* pos = getHandledPhysicsPositionReference(p->mPhysicsID);
	pos->y = y*scale;
}

void addPlayerPositionX(Player* p, double x, int tCoordinateP)
{
	if (isPlayerPaused(p)) return;

	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	Position* pos = getHandledPhysicsPositionReference(p->mPhysicsID);
	double fx = x*scale;
	if (p->mFaceDirection == FACE_DIRECTION_LEFT) fx *= -1;
	pos->x += fx;
}

void addPlayerPositionY(Player* p, double y, int tCoordinateP)
{
	if (isPlayerPaused(p)) return;

	Position* pos = getHandledPhysicsPositionReference(p->mPhysicsID);

	double scale = getPlayerCoordinateP(p) / tCoordinateP;
	pos->y += y*scale;
}

int isPlayerCommandActive(Player* p, char * tCommandName)
{
	return isCommandActive(p->mCommandID, tCommandName);
}



int hasPlayerState(Player * p, int mNewState)
{
	return hasHandledStateMachineState(p->mStateMachineID, mNewState);
}

int hasPlayerStateSelf(Player * p, int mNewState)
{
	return hasHandledStateMachineStateSelf(p->mStateMachineID, mNewState);
}

void changePlayerState(Player* p, int mNewState)
{
	if (!hasPlayerState(p, mNewState)) {
		logWarning("Trying to change into state that does not exist");
		logWarningInteger(mNewState);
		return;
	}
	changeHandledStateMachineState(p->mStateMachineID, mNewState);
	setRegisteredStateTimeInState(p->mStateMachineID, -1);
	updateSingleStateMachineByID(p->mStateMachineID); // TODO: think
}

void changePlayerStateToOtherPlayerStateMachine(Player * p, Player * tOtherPlayer, int mNewState)
{
	changeHandledStateMachineStateToOtherPlayerStateMachine(p->mStateMachineID, tOtherPlayer->mStateMachineID, mNewState);
	setRegisteredStateTimeInState(p->mStateMachineID, -1);
	updateSingleStateMachineByID(p->mStateMachineID); // TODO: think
}

void changePlayerStateBeforeImmediatelyEvaluatingIt(Player * p, int mNewState)
{
	if (!hasPlayerState(p, mNewState)) {
		logWarning("Trying to change into state that does not exist");
		logWarningInteger(mNewState);
		return;
	}
	changeHandledStateMachineState(p->mStateMachineID, mNewState);

}

void changePlayerStateToSelfBeforeImmediatelyEvaluatingIt(Player * p, int tNewState)
{
	changeHandledStateMachineStateToOwnStateMachine(p->mStateMachineID, tNewState);
}

void changePlayerAnimation(Player* p, int tNewAnimation)
{
	changePlayerAnimationWithStartStep(p, tNewAnimation, 0);
}

void changePlayerAnimationWithStartStep(Player* p, int tNewAnimation, int tStartStep)
{
	MugenAnimation* newAnimation = getMugenAnimation(&p->mAnimations, tNewAnimation);
	changeGameMugenAnimationWithStartStep(p->mAnimationID, newAnimation, tStartStep);
}

void changePlayerAnimationToPlayer2AnimationWithStartStep(Player * p, int tNewAnimation, int tStartStep)
{
	Player* otherPlayer = getPlayerOtherPlayer(p);
	MugenAnimation* newAnimation = getMugenAnimation(&otherPlayer->mAnimations, tNewAnimation);
	changeGameMugenAnimationWithStartStep(p->mAnimationID, newAnimation, tStartStep);
}

int isPlayerStartingAnimationElementWithID(Player* p, int tStepID)
{
	return isStartingHandledAnimationElementWithID(p->mAnimationID, tStepID);
}

int getPlayerTimeFromAnimationElement(Player* p, int tStep)
{
	return getTimeFromHandledAnimationElement(p->mAnimationID, tStep);
}

int getPlayerAnimationElementFromTimeOffset(Player* p, int tTime)
{
	return getHandledAnimationElementFromTimeOffset(p->mAnimationID, tTime);
}

void setPlayerSpritePriority(Player* p, int tPriority)
{
	setRegisteredAnimationSpritePriority(p->mAnimationID, tPriority);
}

void setPlayerNoWalkFlag(Player* p)
{
	p->mNoWalkFlag = 1;
}

void setPlayerNoAutoTurnFlag(Player* p)
{
	p->mNoAutoTurnFlag = 1;
}

void setPlayerInvisibleFlag(Player * p)
{
	setRegisteredAnimationInvisibleFlag(p->mAnimationID);
}

void setPlayerNoLandFlag(Player* p)
{
	p->mNoLandFlag = 1;
}

void setPlayerNoShadow(Player * p)
{
	(void)p;
	// TODO: shadow
}

void setPlayerPushDisabledFlag(Player * p, int tIsDisabled)
{
	p->mPushDisabledFlag = tIsDisabled;
}

void setPlayerNoJuggleCheckFlag(Player * p)
{
	p->mNoJuggleCheckFlag = 1;
}

void setPlayerIntroFlag(Player * p)
{
	p->mIntroFlag = 1; // TODO: use
}

void setPlayerNoAirGuardFlag(Player * p)
{
	(void)p;
	// TODO: guard stuff
}

int isPlayerInIntro(Player * p)
{
	int ret = p->mIntroFlag;
	p->mIntroFlag = 0;
	return ret;
}

int doesPlayerHaveAnimationHimself(Player* p, int tAnimation)
{
	return hasMugenAnimation(&p->mAnimations, tAnimation);
}

int isPlayerHitShakeOver(Player* p)
{
	return p->mIsHitShakeOver;
}

int isPlayerHitOver(Player* p)
{
	return p->mIsHitOver;
}

int isPlayerFalling(Player* p) {
	return p->mIsFalling;
}

int canPlayerRecoverFromFalling(Player* p)
{
	return isPlayerFalling(p) && p->mCanRecoverFromFall; // TODO: add time
}

int getPlayerSlideTime(Player* p)
{
	return getActiveHitDataGroundSlideTime(p); // TODO: for guarding
}

void setPlayerDefenseMultiplier(Player* p, double tValue)
{
	p->mDefenseMultiplier = tValue;
}

void setPlayerPositionFrozen(Player* p)
{
	p->mIsFrozen = 1;
	p->mFreezePosition = *getHandledPhysicsPositionReference(p->mPhysicsID);
}

MugenSpriteFile * getPlayerSprites(Player* p)
{
	return &p->mSprites;
}

MugenAnimations * getPlayerAnimations(Player* p)
{
	return &p->mAnimations;
}

MugenAnimation * getPlayerAnimation(Player* p, int tNumber)
{
	return getMugenAnimation(&p->mAnimations, tNumber);
}

int getPlayerCoordinateP(Player* p)
{
	return p->mHeader.mLocalCoordinates.y;
}

char * getPlayerDisplayName(Player* p)
{
	return p->mHeader.mDisplayName;
}

char * getPlayerName(Player * p)
{
	return p->mHeader.mName;
}

char * getPlayerAuthorName(Player * p)
{
	return p->mHeader.mAuthor;
}

int isPlayerPaused(Player* p)
{
	return p->mIsHitPaused || p->mIsSuperPaused;
}

static void pausePlayer(Player* p) {
	if (isPlayerPaused(p)) return;

	pausePhysics(p->mPhysicsID);
	pauseRegisteredAnimation(p->mAnimationID);
	pauseRegisteredStateMachine(p->mStateMachineID);
}

static void unpausePlayer(Player* p) {
	if (isPlayerPaused(p)) return;

	resumePhysics(p->mPhysicsID);
	unpauseRegisteredAnimation(p->mAnimationID);
	unpauseRegisteredStateMachine(p->mStateMachineID);
}

void setPlayerHitPaused(Player* p, int tDuration)
{
	pausePlayer(p);

	p->mIsHitPaused = 1;
	p->mHitPauseNow = 0;
	p->mHitPauseDuration = tDuration;
}

void setPlayerUnHitPaused(Player* p)
{
	p->mIsHitPaused = 0;

	unpausePlayer(p);
}

void setPlayerSuperPaused(Player* p)
{
	pausePlayer(p);

	p->mIsSuperPaused = 1;
}

void setPlayerUnSuperPaused(Player* p)
{
	p->mIsSuperPaused = 0;

	unpausePlayer(p);

	if (!isPlayerPaused(p)) {
		advanceRegisteredAnimationOneTick(p->mAnimationID); // TODO: fix somehow
	}
}

void addPlayerDamage(Player* p, int tDamage)
{
	p->mLife -= tDamage;
	if (p->mLife <= 0) {
		p->mIsAlive = 0;
		p->mLife = 0;
	}

	double perc = p->mLife / (double)p->mConstants.mHeader.mLife;
	setLifeBarPercentage(p, perc);

}

int getPlayerTargetAmount(Player* p)
{
	(void)p;
	return 0; // TODO
}

int getPlayerTargetAmountWithID(Player* p, int tID)
{
	(void)p;
	(void)tID;
	return 0; // TODO
}

int getPlayerHelperAmount(Player* p)
{
	return list_size(&p->mHelpers);
}

typedef struct {
	int mSearchID;
	int mFoundAmount;
} PlayerHelperCountCaller;

static void countHelperByIDCB(void* tCaller, void* tData) {
	PlayerHelperCountCaller* caller = tCaller;
	Player* helper = tData;

	if (helper->mID == caller->mSearchID) {
		caller->mFoundAmount++;
	}
}

int getPlayerHelperAmountWithID(Player* p, int tID)
{
	PlayerHelperCountCaller caller;
	caller.mFoundAmount = 0;
	caller.mSearchID = tID;
	list_map(&p->mHelpers, countHelperByIDCB, &caller);

	return caller.mFoundAmount;
}

typedef struct {
	Player* mFoundHelper;
	int mSearchID;

} PlayerHelperFindCaller;

static void findHelperByIDCB(void* tCaller, void* tData) {
	PlayerHelperFindCaller* caller = tCaller;
	Player* helper = tData;

	if (helper->mID == caller->mSearchID) {
		caller->mFoundHelper = helper;
	}
}

Player * getPlayerHelperOrNullIfNonexistant(Player * p, int tID)
{
	PlayerHelperFindCaller caller;
	caller.mFoundHelper = NULL;
	caller.mSearchID = tID;
	list_map(&p->mHelpers, findHelperByIDCB, &caller);

	return caller.mFoundHelper;
}

int getPlayerProjectileAmount(Player* p)
{
	(void)p;
	return 0; // TODO
}

int getPlayerProjectileAmountWithID(Player * p, int tID)
{
	(void)p;
	(void)tID;
	return 0; // TODO
}


int getPlayerTimeLeftInHitPause(Player* p)
{
	if (!p->mIsHitPaused) return 0;

	return (int)(p->mHitPauseDuration - p->mHitPauseNow);
}

double getPlayerFrontAxisDistanceToScreen(Player* p)
{
	double x = getPlayerPositionX(p, getPlayerCoordinateP(p));
	double screenX = getPlayerScreenEdgeInFrontX(p);

	return fabs(screenX - x);
}

double getPlayerBackAxisDistanceToScreen(Player* p)
{
	double x = getPlayerPositionX(p, getPlayerCoordinateP(p));
	double screenX = getPlayerScreenEdgeInBackX(p);

	return fabs(screenX - x);
}

double getPlayerFrontDistanceToScreen(Player* p)
{
	double x = getPlayerFrontX(p);
	double screenX = getPlayerScreenEdgeInFrontX(p);

	return fabs(screenX - x);
}

double getPlayerBackDistanceToScreen(Player* p)
{
	double x = getPlayerBackX(p);
	double screenX = getPlayerScreenEdgeInBackX(p);

	return fabs(screenX - x);
}

double getPlayerFrontX(Player* p)
{
	double x = getPlayerPositionX(p, getPlayerCoordinateP(p));
	if (p->mFaceDirection == FACE_DIRECTION_RIGHT) return x + p->mConstants.mSizeData.mGroundFrontWidth; // TODO: air
	else return x - p->mConstants.mSizeData.mGroundFrontWidth; // TODO: air
}

double getPlayerBackX(Player* p)
{
	double x = getPlayerPositionX(p, getPlayerCoordinateP(p));
	if (p->mFaceDirection == FACE_DIRECTION_RIGHT) return x - p->mConstants.mSizeData.mGroundBackWidth; // TODO: air
	else return x + p->mConstants.mSizeData.mGroundBackWidth; // TODO: air
}

double getPlayerScreenEdgeInFrontX(Player* p)
{
	double x = getCameraPositionX(getPlayerCoordinateP(p));

	if (p->mFaceDirection == FACE_DIRECTION_RIGHT) return x + p->mHeader.mLocalCoordinates.x / 2;
	else return  x - p->mHeader.mLocalCoordinates.x / 2;
}

double getPlayerScreenEdgeInBackX(Player* p)
{
	double x = getCameraPositionX(getPlayerCoordinateP(p));

	if (p->mFaceDirection == FACE_DIRECTION_RIGHT) return x - p->mHeader.mLocalCoordinates.x / 2;
	else return  x + p->mHeader.mLocalCoordinates.x / 2;
}

double getPlayerDistanceToFrontOfOtherPlayerX(Player* p)
{
	Player* otherPlayer = getPlayerOtherPlayer(p);
	double x1 = getPlayerFrontX(p);
	double x2 = getPlayerFrontX(otherPlayer);

	return fabs(x2-x1);
}

static double getPlayerAxisDistanceForTwoReferencesX(Player* p1, Player* p2) {

	double x1 = getPlayerPositionX(p1, getPlayerCoordinateP(p1));
	double x2 = getPlayerPositionX(p2, getPlayerCoordinateP(p1));

	return x2 - x1;
}

double getPlayerAxisDistanceForTwoReferencesY(Player* p1, Player* p2)
{
	double y1 = getPlayerPositionY(p1, getPlayerCoordinateP(p1));
	double y2 = getPlayerPositionY(p2, getPlayerCoordinateP(p1));

	return y2 - y1;
}

double getPlayerAxisDistanceX(Player* p)
{
	Player* otherPlayer = getPlayerOtherPlayer(p);
	return getPlayerAxisDistanceForTwoReferencesX(p, otherPlayer);
}

double getPlayerAxisDistanceY(Player* p)
{
	Player* otherPlayer = getPlayerOtherPlayer(p);
	return getPlayerAxisDistanceForTwoReferencesY(p, otherPlayer);
}

double getPlayerDistanceToRootX(Player * p)
{
	Player* otherPlayer = p->mRoot;
	return getPlayerAxisDistanceForTwoReferencesX(p, otherPlayer);
}

double getPlayerDistanceToRootY(Player * p)
{
	Player* otherPlayer = p->mRoot;
	return getPlayerAxisDistanceForTwoReferencesY(p, otherPlayer);
}

int getPlayerGroundSizeFront(Player* p)
{
	return p->mConstants.mSizeData.mGroundFrontWidth;
}

void setPlayerGroundSizeFront(Player * p, int tGroundSizeFront)
{
	p->mConstants.mSizeData.mGroundFrontWidth = tGroundSizeFront;
}

int getPlayerGroundSizeBack(Player* p)
{
	return p->mConstants.mSizeData.mGroundBackWidth;
}

void setPlayerGroundSizeBack(Player * p, int tGroundSizeBack)
{
	p->mConstants.mSizeData.mGroundBackWidth = tGroundSizeBack;
}

int getPlayerAirSizeFront(Player * p)
{
	return p->mConstants.mSizeData.mAirFrontWidth;
}

void setPlayerAirSizeFront(Player * p, int tAirSizeFront)
{
	p->mConstants.mSizeData.mAirFrontWidth = tAirSizeFront;
}

int getPlayerAirSizeBack(Player * p)
{
	return p->mConstants.mSizeData.mAirBackWidth;
}

void setPlayerAirSizeBack(Player * p, int tAirSizeBack)
{
	p->mConstants.mSizeData.mAirBackWidth = tAirSizeBack;
}

int getPlayerHeight(Player* p)
{
	return p->mConstants.mSizeData.mHeight;
}

void setPlayerHeight(Player * p, int tHeight)
{
	p->mConstants.mSizeData.mHeight = tHeight;
}

static void increaseSinglePlayerRoundsExisted(Player * p);
static void increasePlayerRoundsExistedCB(void* tCaller, void* tData) {
	(void)tCaller;
	Player* p = tData;
	increaseSinglePlayerRoundsExisted(p);
}

static void increaseSinglePlayerRoundsExisted(Player * p)
{
	p->mRoundsExisted++;
	list_map(&p->mHelpers, increasePlayerRoundsExistedCB, NULL);
}

void increasePlayerRoundsExisted()
{
	increaseSinglePlayerRoundsExisted(&gData.mPlayers[0]);
	increaseSinglePlayerRoundsExisted(&gData.mPlayers[1]);
}


void increasePlayerRoundsWon(Player * p)
{
	p->mRoundsWon++;
}

int hasPlayerWonByKO(Player* p)
{
	(void)p;
	return 0; // TODO
}

int hasPlayerLostByKO(Player * p)
{
	(void)p;
	return 0; // TODO
}

int hasPlayerWonPerfectly(Player* p)
{
	return hasPlayerWon(p) && p->mLife == p->mConstants.mHeader.mLife;
}

int hasPlayerWon(Player* p)
{
	return p->mRoundsWon == 2; // TODO: getRoundNumber
}

int hasPlayerLost(Player* p)
{
	Player* otherPlayer = getPlayerOtherPlayer(p);
	return hasPlayerWon(otherPlayer); 
}

int hasPlayerMoveHitOtherPlayer(Player* p)
{
	MugenStateMoveType type = getPlayerStateMoveType(p);
	int isInAttackState = type == MUGEN_STATE_MOVE_TYPE_ATTACK;
	int isOtherPlayerHit = isPlayerHit(getPlayerOtherPlayer(p));
	int wasItCurrentAttack = !isHitDataActive(p); // TODO: properly

	return isInAttackState && isOtherPlayerHit && wasItCurrentAttack;
}

int isPlayerHit(Player* p)
{
	MugenStateMoveType moveType = getPlayerStateMoveType(p);
	return moveType == MUGEN_STATE_MOVE_TYPE_BEING_HIT;  // TODO: properly
}

int hasPlayerMoveBeenReversedByOtherPlayer(Player * p)
{
	return p->mHasMoveBeenReversed;
}

void setPlayerMoveHit(Player * p)
{
	p->mMoveHit = 1;
}

void setPlayerMoveHitReset(Player * p)
{
	p->mMoveContactCounter = 0;
	p->mMoveHit = 0;
	p->mMoveGuarded = 0;
	p->mHasMoveBeenReversed = 0;
}

void setPlayerMoveGuarded(Player * p)
{
	p->mMoveGuarded = 1;
}

int getPlayerFallAmountInCombo(Player* p)
{
	return p->mFallAmountInCombo;
}

void increasePlayerFallAmountInCombo(Player* p)
{
	p->mFallAmountInCombo++;
}

void resetPlayerFallAmountInCombo(Player* p)
{
	p->mFallAmountInCombo = 0;
}

int getPlayerHitCount(Player* p)
{
	return p->mHitCount;
}

void increasePlayerHitCount(Player* p)
{
	p->mHitCount++;
}

void resetPlayerHitCount(Player* p)
{
	p->mHitCount = 0;
	resetPlayerFallAmountInCombo(p);
}

void setPlayerAttackMultiplier(Player* p, double tValue)
{
	p->mAttackMultiplier = tValue;
}

double getPlayerFallDefenseMultiplier(Player* p)
{
	int f = p->mConstants.mHeader.mFallDefenseUp;
	return 100.0 / (f+100);
}

void setPlayerHuman(int i)
{
	Player* p = getRootPlayer(i);
	p->mAILevel = 0;
}

void setPlayerArtificial(int i)
{
	Player* p = getRootPlayer(i);
	p->mAILevel = 8; // TODO: properly
}

int getPlayerAILevel(Player* p)
{
	return p->mAILevel;
}

int getPlayerLife(Player* p)
{
	return p->mLife;
}

int getPlayerLifeMax(Player* p)
{
	return p->mConstants.mHeader.mLife;
}

int getPlayerPower(Player* p)
{
	return p->mPower;
}

int getPlayerPowerMax(Player* p)
{
	return p->mConstants.mHeader.mPower;
}

void addPlayerPower(Player* p, int tPower)
{
	p->mPower += tPower;

	p->mPower = max(0, min(getPlayerPowerMax(p), p->mPower));

	double perc = p->mPower / (double)p->mConstants.mHeader.mPower;
	setPowerBarPercentage(p, perc);

}

int isPlayerBeingAttacked(Player* p) {
	Player* otherPlayer = getPlayerOtherPlayer(p);

	return isHitDataActive(otherPlayer);
}

int isPlayerInGuardDistance(Player* p)
{
	Player* otherPlayer = getPlayerOtherPlayer(p);

	int isBeingAttacked = isPlayerBeingAttacked(p);

	double dist = fabs(getPlayerAxisDistanceX(p));
	int isInDistance = dist < getHitDataGuardDistance(otherPlayer); // TODO: check helpers

	return isBeingAttacked && isInDistance;
}

int getDefaultPlayerAttackDistance(Player * p)
{
	return p->mConstants.mSizeData.mAttackDistance;
}

Position getPlayerHeadPosition(Player * p)
{
	return makePosition(getPlayerHeadPositionX(p), getPlayerHeadPositionY(p), 0);
}

double getPlayerHeadPositionX(Player* p)
{
	return p->mConstants.mSizeData.mHeadPosition.x;
}

double getPlayerHeadPositionY(Player* p)
{
	return p->mConstants.mSizeData.mHeadPosition.y;
}

void setPlayerHeadPosition(Player * p, double tX, double tY)
{
	p->mConstants.mSizeData.mHeadPosition = makePosition(tX, tY, 0);
}

Position getPlayerMiddlePosition(Player * p)
{
	return makePosition(getPlayerMiddlePositionX(p), getPlayerMiddlePositionY(p), 0);
}

double getPlayerMiddlePositionX(Player * p)
{
	return p->mConstants.mSizeData.mMidPosition.x;
}

double getPlayerMiddlePositionY(Player * p)
{
	return p->mConstants.mSizeData.mMidPosition.y;
}

void setPlayerMiddlePosition(Player* p, double tX, double tY)
{
	p->mConstants.mSizeData.mMidPosition = makePosition(tX, tY, 0);
}

int getPlayerShadowOffset(Player * p)
{
	return p->mConstants.mSizeData.mShadowOffset;
}

void setPlayerShadowOffset(Player * p, int tOffset)
{
	p->mConstants.mSizeData.mShadowOffset = tOffset;
}

int isPlayerHelper(Player* p)
{
	return p->mIsHelper;
}

void setPlayerIsFacingRight(Player * p, int tIsFacingRight)
{
	setPlayerFaceDirection(p, tIsFacingRight ? FACE_DIRECTION_RIGHT : FACE_DIRECTION_LEFT);
}

int getPlayerIsFacingRight(Player* p)
{
	return p->mFaceDirection == FACE_DIRECTION_RIGHT;
}

void turnPlayerAround(Player * p)
{
	setPlayerIsFacingRight(p, !getPlayerIsFacingRight(p));
}

Player* getPlayerOtherPlayer(Player* p) {
	return p->mOtherPlayer;
}

double getPlayerScaleX(Player * p)
{
	return p->mConstants.mSizeData.mScale.x;
}

void setPlayerScaleX(Player * p, double tScaleX)
{
	p->mConstants.mSizeData.mScale.x = tScaleX;
}

double getPlayerScaleY(Player * p)
{
	return p->mConstants.mSizeData.mScale.y;
}

void setPlayerScaleY(Player * p, double tScaleY)
{
	p->mConstants.mSizeData.mScale.y = tScaleY;
}

Player * clonePlayerAsHelper(Player * p)
{
	Player* helper = allocMemory(sizeof(Player));
	*helper = *p;

	resetHelperState(helper);
	setPlayerExternalDependencies(helper);
	setRegisteredStateToHelperMode(helper->mStateMachineID);

	helper->mParent = p;
	helper->mIsHelper = 1;

	return helper;
}

typedef struct {
	Player* mParent;

} MovePlayerHelperCaller;

static int moveSinglePlayerHelper(void* tCaller, void* tData) {
	MovePlayerHelperCaller* caller = tCaller;
	Player* helper = tData;
	Player* parent = caller->mParent;
	Player* parentParent = parent->mParent;
	
	addHelperToPlayer(parentParent, helper);

	return 1;
}

static void movePlayerHelpers(Player* p) {
	MovePlayerHelperCaller caller;
	caller.mParent = p;
	list_remove_predicate(&p->mHelpers, moveSinglePlayerHelper, &caller);
	delete_list(&p->mHelpers);
}

static void removeHelperFromPlayer(Player* p, Player* tHelper) {
	list_remove(&p->mHelpers, tHelper->mHelperIDInParent);
}

static void removePlayerBindingCB(void* tCaller, void* tData) {
	(void)tCaller;
	Player* p = tData;
	removePlayerBinding(p);
}

static void removePlayerBoundHelpers(Player* p) {
	list_map(&p->mBoundHelpers, removePlayerBindingCB, NULL);
	delete_list(&p->mBoundHelpers);
}

static void destroyGeneralPlayer(Player* p) {
	removeRegisteredStateMachine(p->mStateMachineID);
	removeRegisteredAnimation(p->mAnimationID);
	removeFromPhysicsHandler(p->mPhysicsID);
	removePlayerHitData(p);
	freeMemory(p);
}

void destroyPlayer(Player * p) // TODO: rename
{
	assert(p->mIsHelper);
	assert(p->mParent);
	assert(p->mHelperIDInParent != -1);

	removePlayerBoundHelpers(p);
	movePlayerHelpers(p);
	removeHelperFromPlayer(p->mParent, p);
	destroyGeneralPlayer(p);
}

void addHelperToPlayer(Player * p, Player * tHelper)
{
	
	tHelper->mHelperIDInParent = list_push_back(&p->mHelpers, tHelper);
	tHelper->mParent = p;
}

int getPlayerID(Player * p)
{
	return p->mID;
}

void setPlayerID(Player * p, int tID)
{
	p->mID = tID;
}

void setPlayerHelperControl(Player * p, int tCanControl)
{
	if (!tCanControl) {
		setRegisteredStateDisableCommandState(p->mStateMachineID);
	}
}

static void addProjectileToRoot(Player* tPlayer, Player* tProjectile) {
	Player* root = tPlayer->mRoot;

	tProjectile->mProjectileID = int_map_push_back(&root->mProjectiles, tProjectile);
}

Player * createNewProjectileFromPlayer(Player * p)
{
	Player* helper = allocMemory(sizeof(Player));
	*helper = *p;

	resetHelperState(helper);
	setPlayerExternalDependencies(helper);
	disableRegisteredStateMachine(helper->mStateMachineID);
	addProjectileToRoot(p, helper);
	addAdditionalProjectileData(helper);
	setPlayerPhysics(helper, MUGEN_STATE_PHYSICS_NONE);
	setPlayerIsFacingRight(helper, getPlayerIsFacingRight(p));
	helper->mIsProjectile = 1;

	return helper;
}

void removeProjectile(Player* p) {
	assert(p->mIsProjectile);
	assert(p->mProjectileID != -1);
	removeAdditionalProjectileData(p);
	destroyGeneralPlayer(p);
}

int getPlayerControlTime(Player * p)
{
	if (getPlayerStateType(p) == MUGEN_STATE_TYPE_AIR) {
		return getActiveHitDataAirGuardControlTime(p);
	}
	else {
		return getActiveHitDataGuardControlTime(p);
	}
}

void setPlayerDrawScale(Player * p, Vector3D tScale)
{
	setRegisteredAnimationOneFrameDrawScale(p->mAnimationID, tScale);
}

void setPlayerDrawAngle(Player * p, double tAngle)
{
	setRegisteredAnimationOneFrameDrawAngle(p->mAnimationID, tAngle);
}

void addPlayerDrawAngle(Player * p, double tAngle)
{
	addRegisteredAnimationOneFrameDrawAngle(p->mAnimationID, tAngle);
}

void setPlayerFixedDrawAngle(Player * p, double tAngle)
{
	setRegisteredAnimationOneFrameFixedDrawAngle(p->mAnimationID, tAngle);
}

static void bindHelperToPlayer(Player* tHelper, Player* tBind, int tTime, int tFacing, Vector3D tOffset, PlayerBindPositionType tType) {
	tHelper->mIsBound = 1;
	tHelper->mBoundNow = 0;
	tHelper->mBoundDuration = tTime;
	tHelper->mBoundFaceSet = tFacing;
	tHelper->mBoundOffset = tOffset;
	tHelper->mBoundPositionType = tType;
	tHelper->mBoundTarget = tBind;

	tHelper->mBoundID = list_push_back(&tBind->mBoundHelpers, tHelper);
}

void bindPlayerToRoot(Player * p, int tTime, int tFacing, Vector3D tOffset)
{
	bindHelperToPlayer(p, p->mRoot, tTime, tFacing, tOffset, PLAYER_BIND_POSITION_TYPE_AXIS);
}

void bindPlayerToParent(Player * p, int tTime, int tFacing, Vector3D tOffset)
{
	bindHelperToPlayer(p, p->mParent, tTime, tFacing, tOffset, PLAYER_BIND_POSITION_TYPE_AXIS);
}

typedef struct {
	Player* mBindRoot;
	int mID;
	Position mOffset;
	int mTime;
} BindPlayerTargetsCaller;

static void bindSinglePlayerAsTarget(Player* tBindRoot, Player* tTarget, BindPlayerTargetsCaller* tCaller);

static void bindPlayerTargetCB(void* tCaller, void* tData) {
	BindPlayerTargetsCaller* caller = tCaller;
	Player* helper = tData;

	bindSinglePlayerAsTarget(caller->mBindRoot, helper, caller);
}

static void bindSinglePlayerAsTarget(Player* tBindRoot, Player* tTarget, BindPlayerTargetsCaller* tCaller) {

	if (tCaller->mID == -1 || tCaller->mID == getPlayerID(tTarget)) {
		bindHelperToPlayer(tTarget, tBindRoot, tCaller->mTime, 0, tCaller->mOffset, PLAYER_BIND_POSITION_TYPE_AXIS);
	}

	list_map(&tTarget->mHelpers, bindPlayerTargetCB, tCaller);
}

void bindPlayerTargets(Player * p, int tTime, Vector3D tOffset, int tID)
{
	BindPlayerTargetsCaller caller;
	caller.mBindRoot = p;
	caller.mID = tID;
	caller.mOffset = tOffset;
	caller.mTime = tTime + 1; // TODO: fix
	bindSinglePlayerAsTarget(p, getPlayerOtherPlayer(p)->mRoot, &caller);
}

int isPlayerBound(Player * p)
{
	return p->mIsBound;
}

void addPlayerTargetLife(Player * p, int tID, int tLife, int tCanKill, int tIsAbsolute)
{
	Player* otherPlayer = getPlayerOtherPlayer(p)->mRoot;
	if (tID != -1 && getPlayerID(otherPlayer) != tID) return;

	(void)tIsAbsolute; // TODO


	int playerLife = getPlayerLife(otherPlayer);
	if (!tCanKill && playerLife + tLife <= 0) {
		tLife = -(playerLife - 1);
	}

	addPlayerDamage(otherPlayer, -tLife);

}

void setPlayerTargetControl(Player * p, int tID, int tControl)
{
	Player* otherPlayer = getPlayerOtherPlayer(p)->mRoot;
	if (tID != -1 && getPlayerID(otherPlayer) != tID) return;

	setPlayerControl(otherPlayer, tControl);
}

void setPlayerTargetFacing(Player * p, int tID, int tFacing)
{
	Player* otherPlayer = getPlayerOtherPlayer(p)->mRoot;
	if (tID != -1 && getPlayerID(otherPlayer) != tID) return;

	if (tFacing == 1) {
		setPlayerIsFacingRight(otherPlayer, getPlayerIsFacingRight(p));
	}
	else {
		setPlayerIsFacingRight(otherPlayer, !getPlayerIsFacingRight(p));
	}
}

void changePlayerTargetState(Player * p, int tID, int tNewState)
{
	Player* otherPlayer = getPlayerOtherPlayer(p)->mRoot;
	if (tID != -1 && getPlayerID(otherPlayer) != tID) return;

	changePlayerState(otherPlayer, tNewState);
}

typedef struct {
	int mSearchID;
	Player* mPlayer;

} SearchPlayerIDCaller;

static Player* searchSinglePlayerForID(Player* p, int tID);

static void searchPlayerForIDCB(void* tCaller, void* tData) {
	SearchPlayerIDCaller* caller = tCaller;
	Player* p = tData;

	Player* ret = searchSinglePlayerForID(p, caller->mSearchID);
	if (ret != NULL) {
		caller->mPlayer = ret;
	}
}

static Player* searchSinglePlayerForID(Player* p, int tID) {
	if (getPlayerID(p) == tID) return p;

	SearchPlayerIDCaller caller;
	caller.mSearchID = tID;
	caller.mPlayer = NULL;
	list_map(&p->mHelpers, searchPlayerForIDCB, &caller);

	return caller.mPlayer;
}

int doesPlayerIDExist(Player * p, int tID)
{
	return searchSinglePlayerForID(p->mRoot, tID) != NULL;
}

Player * getPlayerByIDOrNullIfNonexistant(Player * p, int tID)
{
	return searchSinglePlayerForID(p->mRoot, tID);
}

int getPlayerRoundsExisted(Player * p)
{
	return p->mRoundsExisted;
}

int getPlayerPaletteNumber(Player * p)
{
	return p->mPreferredPalette;
}

void setPlayerScreenBound(Player * p, int tIsBoundToScreen, int tIsCameraFollowingX, int tIsCameraFollowingY)
{
	p->mIsBoundToScreen = tIsBoundToScreen;
	(void)tIsCameraFollowingX;
	(void)tIsCameraFollowingY;
	// TODO

}

static void resetPlayerHitBySlotGeneral(Player * p, int tSlot) {
	p->mNotHitBy[tSlot].mFlag2Amount = 0;
	p->mNotHitBy[tSlot].mNow = 0;
	p->mNotHitBy[tSlot].mIsActive = 1;
}

void resetPlayerHitBy(Player * p, int tSlot)
{
	resetPlayerHitBySlotGeneral(p, tSlot);
	p->mNotHitBy[tSlot].mIsHitBy = 1;
}

void resetPlayerNotHitBy(Player * p, int tSlot)
{
	resetPlayerHitBySlotGeneral(p, tSlot);
	p->mNotHitBy[tSlot].mIsHitBy = 0;
}

void setPlayerNotHitByFlag1(Player * p, int tSlot, char * tFlag)
{
	strcpy(p->mNotHitBy[tSlot].mFlag1, tFlag);
	turnStringLowercase(p->mNotHitBy[tSlot].mFlag1);
}

static void copyOverCleanFlag2(char* tDst, char* tSrc) {
	int n = strlen(tSrc);

	int o = 0;
	int i;
	for (i = 0; i < n; i++) {
		if (tSrc[i] == ' ') continue;
		tDst[o++] = tSrc[i];
	}
	tDst[o] = '\0';
}

void addPlayerNotHitByFlag2(Player * p, int tSlot, char * tFlag)
{

	char* nFlag = allocMemory(strlen(tFlag) + 5);
	copyOverCleanFlag2(nFlag, tFlag);
	assert(strlen(nFlag) == 2);
	turnStringLowercase(nFlag);

	assert(p->mNotHitBy[tSlot].mFlag2Amount < MAXIMUM_HITSLOT_FLAG_2_AMOUNT);

	strcpy(p->mNotHitBy[tSlot].mFlag2[p->mNotHitBy[tSlot].mFlag2Amount], nFlag);
	p->mNotHitBy[tSlot].mFlag2Amount++;
}

void setPlayerNotHitByTime(Player * p, int tSlot, int tTime)
{
	p->mNotHitBy[tSlot].mTime = tTime;
}

int getDefaultPlayerSparkNumberIsInPlayerFile(Player * p)
{
	return p->mConstants.mHeader.mIsSparkNoInPlayerFile;
}

int getDefaultPlayerSparkNumber(Player * p)
{
	return p->mConstants.mHeader.mSparkNo;
}

int getDefaultPlayerGuardSparkNumberIsInPlayerFile(Player * p)
{
	return p->mConstants.mHeader.mIsGuardSparkNoInPlayerFile;
}

int getDefaultPlayerGuardSparkNumber(Player * p)
{
	return p->mConstants.mHeader.mGuardSparkNo;
}

int isPlayerProjectile(Player * p)
{
	return p->mIsProjectile;
}
