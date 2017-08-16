#pragma once

#include <tari/datastructures.h>

#include "mugenstatereader.h"
#include "mugenanimationreader.h"
#include "mugenspritefilereader.h"
#include "mugencommandreader.h"

typedef enum {
	PLAYER_BIND_POSITION_TYPE_AXIS,
	PLAYER_BIND_POSITION_TYPE_HEAD,
	PLAYER_BIND_POSITION_TYPE_MID,
} PlayerBindPositionType;

#define MAXIMUM_HITSLOT_FLAG_2_AMOUNT 10

typedef struct {
	int mIsActive;

	char mFlag1[10];
	char mFlag2[MAXIMUM_HITSLOT_FLAG_2_AMOUNT][10];
	int mFlag2Amount;

	int mNow;
	int mTime;
	int mIsHitBy;
} HitDefAttributeSlot;

typedef struct {
	char mName[100];
	char mDisplayName[100];
	char mVersion[100];
	char mMugenVersion[100];
	char mAuthor[100];

	char mPaletteDefaults[100];
	Vector3DI mLocalCoordinates;
} PlayerHeader;

typedef struct Player_t{
	struct Player_t* mRoot;
	struct Player_t* mOtherPlayer;
	int mPreferredPalette;
	int mRootID;
	int mControllerID;
	int mID;
	int mIsHelper;

	List mHelpers; // contains Player
	struct Player_t* mParent;
	int mHelperIDInParent;

	char mDefinitionPath[1024];
	PlayerHeader mHeader;
	MugenCommands mCommands;
	MugenConstants mConstants;
	MugenAnimations mAnimations;
	MugenSpriteFile mSprites;

	int mVars[100];
	int mSystemVars[100];
	double mFloatVars[100];
	double mSystemFloatVars[100];

	int mCommandID;
	int mStateMachineID;
	int mAnimationID;
	int mPhysicsID;
	int mHitDataID;

	MugenStateType mStateType;
	MugenStateMoveType mMoveType;
	MugenStatePhysics mStatePhysics;

	int mIsInControl;
	int mMoveContactCounter;
	int mMoveHit;
	int mMoveGuarded;
	int mIsAlive;
	FaceDirection mFaceDirection;

	int mNoWalkFlag;
	int mNoAutoTurnFlag;
	int mNoLandFlag;
	int mPushDisabledFlag;
	int mNoJuggleCheckFlag;
	int mIntroFlag;

	int mIsHitShakeOver;
	int mIsHitOver;
	int mIsFalling;
	int mCanRecoverFromFall;

	double mDefenseMultiplier;

	int mIsFrozen;
	Position mFreezePosition;

	int mIsLyingDown;
	Duration mLyingDownTime;

	int mIsHitPaused;
	Duration mHitPauseNow;
	int mHitPauseDuration;

	int mIsSuperPaused;

	int mLife;
	int mPower;

	int mHitCount;
	int mFallAmountInCombo;

	double mAttackMultiplier;

	int mHasMoveBeenReversed;

	int mIsBound;
	int mBoundNow;
	int mBoundDuration;
	int mBoundFaceSet;
	Position mBoundOffset;
	PlayerBindPositionType mBoundPositionType;
	struct Player_t* mBoundTarget;
	int mBoundID;

	List mBoundHelpers;

	int mRoundsExisted;
	int mComboCounter;

	int mRoundsWon;

	HitDefAttributeSlot mNotHitBy[2];
} Player;

void loadPlayers();
void resetPlayers();
void updatePlayers();

void playerHitCB(Player* p, void* tHitData);

void setPlayerDefinitionPath(int i, char* tDefinitionPath);
Player* getRootPlayer(int i);
Player* getPlayerRoot(Player* p);
Player* getPlayerParent(Player* p);

int getPlayerState(Player* p); 
int getPlayerPreviousState(Player* p);


MugenStateType getPlayerStateType(Player* p);
void setPlayerStateType(Player* p, MugenStateType tType);
MugenStateMoveType getPlayerStateMoveType(Player* p);
void setPlayerStateMoveType(Player* p, MugenStateMoveType tType);

int getPlayerControl(Player* p);
void setPlayerControl(Player* p, int tNewControl);

MugenStatePhysics getPlayerPhysics(Player* p);
void setPlayerPhysics(Player* p, MugenStatePhysics tNewPhysics);

int getPlayerMoveContactCounter(Player* p);
void resetPlayerMoveContactCounter(Player* p);
void setPlayerMoveContactCounterActive(Player* p);
char* getPlayerHitDefinitionAttributes(Player* p);

int getPlayerVariable(Player* p, int tIndex);
void setPlayerVariable(Player* p, int tIndex, int tValue);
void addPlayerVariable(Player* p, int tIndex, int tValue);
int getPlayerSystemVariable(Player* p, int tIndex);
void setPlayerSystemVariable(Player* p, int tIndex, int tValue);
void addPlayerSystemVariable(Player* p, int tIndex, int tValue);
double getPlayerFloatVariable(Player* p, int tIndex);
void setPlayerFloatVariable(Player* p, int tIndex, double tValue);
void addPlayerFloatVariable(Player* p, int tIndex, double tValue);
double getPlayerSystemFloatVariable(Player* p, int tIndex);
void setPlayerSystemFloatVariable(Player* p, int tIndex, double tValue);
void addPlayerSystemFloatVariable(Player* p, int tIndex, double tValue);

int getPlayerTimeInState(Player* p);
int getPlayerAnimationNumber(Player* p);
int getRemainingPlayerAnimationTime(Player* p);

Vector3D getPlayerPosition(Player* p, int tCoordinateP);
double getPlayerPositionBasedOnScreenCenterX(Player* p, int tCoordinateP);
double getPlayerPositionX(Player* p, int tCoordinateP);
double getPlayerPositionBasedOnStageFloorY(Player* p, int tCoordinateP);
double getPlayerPositionY(Player* p, int tCoordinateP);
double getPlayerVelocityX(Player* p, int tCoordinateP);
double getPlayerVelocityY(Player* p, int tCoordinateP);

double getPlayerStandFrictionThreshold(Player* p);
double getPlayerCrouchFrictionThreshold(Player* p);
double getPlayerAirGetHitGroundLevelY(Player* p);
double getPlayerAirGetHitGroundRecoveryGroundYTheshold(Player* p);
double getPlayerAirGetHitAirRecoveryVelocityYThreshold(Player* p);
double getPlayerAirGetHitTripGroundLevelY(Player* p);
double getPlayerDownBounceOffsetX(Player* p);
double getPlayerDownBounceOffsetY(Player* p);
double getPlayerDownVerticalBounceAcceleration(Player* p);
double getPlayerDownBounceGroundLevel(Player* p);
double getPlayerLyingDownFrictionThreshold(Player* p);
double getPlayerVerticalAcceleration(Player* p);

double getPlayerForwardWalkVelocityX(Player* p);
double getPlayerBackwardWalkVelocityX(Player* p);
double getPlayerForwardRunVelocityX(Player* p);
double getPlayerBackwardRunVelocityX(Player* p);
double getPlayerBackwardRunVelocityY(Player* p);
double getPlayerForwardRunJumpVelocityX(Player* p);
double getPlayerNeutralJumpVelocityX(Player* p);
double getPlayerForwardJumpVelocityX(Player* p);
double getPlayerBackwardJumpVelocityX(Player* p);
double getPlayerJumpVelocityY(Player* p);
double getPlayerNeutralAirJumpVelocityX(Player* p);
double getPlayerForwardAirJumpVelocityX(Player* p);
double getPlayerBackwardAirJumpVelocityX(Player* p);
double getPlayerAirJumpVelocityY(Player* p);


int isPlayerAlive(Player* p);

void setPlayerVelocityX(Player* p, double x, int tCoordinateP);
void setPlayerVelocityY(Player* p, double y, int tCoordinateP);
void multiplyPlayerVelocityX(Player* p, double x, int tCoordinateP);
void multiplyPlayerVelocityY(Player* p, double y, int tCoordinateP);
void addPlayerVelocityX(Player* p, double x, int tCoordinateP);
void addPlayerVelocityY(Player* p, double y, int tCoordinateP);

void setPlayerPosition(Player* p, Position tPosition, int tCoordinateP);
void setPlayerPositionX(Player* p, double x, int tCoordinateP);
void setPlayerPositionY(Player* p, double y, int tCoordinateP);
void addPlayerPositionX(Player* p, double x, int tCoordinateP);
void addPlayerPositionY(Player* p, double y, int tCoordinateP);

int isPlayerCommandActive(Player* p, char* tCommandName);

int hasPlayerStateSelf(Player* p, int mNewState);
void changePlayerState(Player* p, int mNewState);
void changePlayerStateToOtherPlayerStateMachine(Player* p, Player* tOtherPlayer, int mNewState);
void changePlayerStateBeforeImmediatelyEvaluatingIt(Player* p, int mNewState);
void changePlayerStateToSelfBeforeImmediatelyEvaluatingIt(Player* p, int tNewState);

void changePlayerAnimation(Player* p, int tNewAnimation);
void changePlayerAnimationWithStartStep(Player* p, int tNewAnimation, int tStartStep);
void changePlayerAnimationToPlayer2AnimationWithStartStep(Player* p, int tNewAnimation, int tStartStep);

int isPlayerStartingAnimationElementWithID(Player* p, int tStepID);
int getPlayerTimeFromAnimationElement(Player* p, int tStep);
int getPlayerAnimationElementFromTimeOffset(Player* p, int tTime);

void setPlayerSpritePriority(Player* p, int tPriority);

void setPlayerNoWalkFlag(Player* p);
void setPlayerNoAutoTurnFlag(Player* p);
void setPlayerInvisibleFlag(Player* p);
void setPlayerNoLandFlag(Player* p);
void setPlayerNoShadow(Player* p);
void setPlayerPushDisabledFlag(Player* p, int tIsDisabled);
void setPlayerNoJuggleCheckFlag(Player* p);
void setPlayerIntroFlag(Player* p);
void setPlayerNoAirGuardFlag(Player* p);

int isPlayerInIntro(Player* p);

int doesPlayerHaveAnimationHimself(Player* p, int tAnimation);

int isPlayerFalling(Player* p);
int canPlayerRecoverFromFalling(Player* p);
int isPlayerHitShakeOver(Player* p);
int isPlayerHitOver(Player* p);

int getPlayerSlideTime(Player* p);

void setPlayerDefenseMultiplier(Player* p, double tValue);
void setPlayerPositionFrozen(Player* p);

MugenSpriteFile* getPlayerSprites(Player* p);
MugenAnimations* getPlayerAnimations(Player* p);
MugenAnimation* getPlayerAnimation(Player* p, int tNumber);

int getPlayerCoordinateP(Player* p);
char* getPlayerDisplayName(Player* p);
char* getPlayerName(Player* p);
char* getPlayerAuthorName(Player* p);

int isPlayerPaused(Player* p);
void setPlayerHitPaused(Player* p, int tDuration);
void setPlayerUnHitPaused(Player* p);
void setPlayerSuperPaused(Player* p);
void setPlayerUnSuperPaused(Player* p);

void addPlayerDamage(Player* p, int tDamage);

int getPlayerTargetAmount(Player* p);
int getPlayerTargetAmountWithID(Player* p, int tID);

int getPlayerHelperAmount(Player* p);
int getPlayerHelperAmountWithID(Player* p, int tID);
Player* getPlayerHelperOrNullIfNonexistant(Player* p, int tID);

int getPlayerProjectileAmount(Player* p);

int getPlayerTimeLeftInHitPause(Player* p);

double getPlayerFrontAxisDistanceToScreen(Player* p);
double getPlayerBackAxisDistanceToScreen(Player* p);

double getPlayerFrontDistanceToScreen(Player* p);
double getPlayerBackDistanceToScreen(Player* p);

double getPlayerFrontX(Player* p);
double getPlayerBackX(Player* p);

double getPlayerScreenEdgeInFrontX(Player* p);
double getPlayerScreenEdgeInBackX(Player* p);

double getPlayerDistanceToFrontOfOtherPlayerX(Player* p);
double getPlayerAxisDistanceX(Player* p);
double getPlayerAxisDistanceY(Player* p);
double getPlayerDistanceToRootX(Player* p);
double getPlayerDistanceToRootY(Player* p);

int getPlayerGroundSizeFront(Player* p);
void setPlayerGroundSizeFront(Player* p, int tGroundSizeFront);
int getPlayerGroundSizeBack(Player* p);
void setPlayerGroundSizeBack(Player* p, int tGroundSizeBack);
int getPlayerAirSizeFront(Player* p);
void setPlayerAirSizeFront(Player* p, int tAirSizeFront);
int getPlayerAirSizeBack(Player* p);
void setPlayerAirSizeBack(Player* p, int tAirSizeBack);
int getPlayerHeight(Player* p);
void setPlayerHeight(Player* p, int tHeight);

void increasePlayerRoundsExisted();
void increasePlayerRoundsWon(Player* p);
int hasPlayerWonByKO(Player* p);
int hasPlayerWonPerfectly(Player* p);
int hasPlayerWon(Player* p);
int hasPlayerLost(Player* p);

int hasPlayerMoveHitOtherPlayer(Player* p);
int isPlayerHit(Player* p);
int hasPlayerMoveBeenReversedByOtherPlayer(Player* p);
void setPlayerMoveHit(Player* p);
void setPlayerMoveHitReset(Player* p);
void setPlayerMoveGuarded(Player* p);


int getPlayerFallAmountInCombo(Player* p);
void increasePlayerFallAmountInCombo(Player* p);
void resetPlayerFallAmountInCombo(Player* p);

int getPlayerHitCount(Player* p);
void increasePlayerHitCount(Player* p);
void resetPlayerHitCount(Player* p);

void setPlayerAttackMultiplier(Player* p, double tValue);

double getPlayerFallDefenseMultiplier(Player* p);

int getPlayerAILevel(Player* p);

int getPlayerLife(Player* p);
int getPlayerLifeMax(Player* p);
int getPlayerPower(Player* p);
int getPlayerPowerMax(Player* p);
void addPlayerPower(Player* p, int tPower);

int isPlayerBeingAttacked(Player* p);
int isPlayerInGuardDistance(Player* p);
int getDefaultPlayerAttackDistance(Player* p);

Position getPlayerHeadPosition(Player* p);
double getPlayerHeadPositionX(Player* p);
double getPlayerHeadPositionY(Player* p);
void setPlayerHeadPosition(Player* p, double tX, double tY);

Position getPlayerMiddlePosition(Player* p);
double getPlayerMiddlePositionX(Player* p);
double getPlayerMiddlePositionY(Player* p);
void setPlayerMiddlePosition(Player* p, double tX, double tY);

int getPlayerShadowOffset(Player* p);
void setPlayerShadowOffset(Player* p, int tOffset);

int isPlayerHelper(Player* p);

void setPlayerIsFacingRight(Player* p, int tIsFacingRight);
int getPlayerIsFacingRight(Player* p);
void turnPlayerAround(Player* p);

Player* getPlayerOtherPlayer(Player* p);

double getPlayerScaleX(Player* p);
void setPlayerScaleX(Player* p, double tScaleX);
double getPlayerScaleY(Player* p);
void setPlayerScaleY(Player* p, double tScaleY);

Player* clonePlayerAsHelper(Player* p);
void destroyPlayer(Player* tPlayer);
void addHelperToPlayer(Player* p, Player* tHelper);
int getPlayerID(Player* p);
void setPlayerID(Player* p, int tID);
void setPlayerHelperControl(Player* p, int tCanControl);

int getPlayerControlTime(Player* p);

void setPlayerDrawScale(Player* p, Vector3D tScale);
void setPlayerDrawAngle(Player* p, double tAngle);
void addPlayerDrawAngle(Player* p, double tAngle);
void setPlayerFixedDrawAngle(Player* p, double tAngle);

void bindPlayerToRoot(Player* p, int tTime, int tFacing, Vector3D tOffset);
void bindPlayerToParent(Player* p, int tTime, int tFacing, Vector3D tOffset);
void bindPlayerTargets(Player* p, int tTime, Vector3D tOffset, int tID);
int isPlayerBound(Player* p);

void addPlayerTargetLife(Player* p, int tID, int tLife, int tCanKill, int tIsAbsolute);
void setPlayerTargetControl(Player* p, int tID, int tControl);
void setPlayerTargetFacing(Player* p, int tID, int tFacing);
void changePlayerTargetState(Player* p, int tID, int tNewState);

int doesPlayerIDExist(Player* p, int tID);
Player* getPlayerByIDOrNullIfNonexistant(Player* p, int tID);

int getPlayerRoundsExisted(Player* p);

int getPlayerPaletteNumber(Player* p);

void setPlayerScreenBound(Player* p, int tIsBoundToScreen, int tIsCameraFollowingX, int tIsCameraFollowingY);

void resetPlayerHitBy(Player* p, int tSlot);
void resetPlayerNotHitBy(Player* p, int tSlot);
void setPlayerNotHitByFlag1(Player* p, int tSlot, char* tFlag);
void addPlayerNotHitByFlag2(Player* p, int tSlot, char* tFlag);
void setPlayerNotHitByTime(Player* p, int tSlot, int tTime);

int getDefaultPlayerSparkNumberIsInPlayerFile(Player* p);
int getDefaultPlayerSparkNumber(Player* p);
int getDefaultPlayerGuardSparkNumberIsInPlayerFile(Player* p);
int getDefaultPlayerGuardSparkNumber(Player* p);