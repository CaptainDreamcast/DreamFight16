#include "playerhitdata.h"

#include <assert.h>

#include <tari/datastructures.h>

#include "mugensound.h"


typedef struct {
	int mIsActive;
	Player* mPlayer;

	MugenStateType mType;
	MugenAttackClass mAttackClass;
	MugenAttackType mAttackType;

	char mHitFlag[10];
	char mGuardFlag[10];

	MugenAffectTeam mAffectTeam;
	MugenHitAnimationType mAnimationType;
	MugenHitAnimationType mAirAnimationType;
	MugenHitAnimationType mFallAnimationType;

	int mPriority;
	MugenHitPriorityType mPriorityType;

	int mDamage;
	int mGuardDamage;

	int mPlayer1PauseTime;
	int mPlayer2ShakeTime;
	int mGuardPlayer1PauseTime;
	int mGuardPlayer2ShakeTime;

	int mIsSparkInPlayerFile;
	int mSparkNumber;

	int mIsGuardSparkInPlayerFile;
	int mGuardSparkNumber;

	Position mSparkOffset;

	int mIsHitSoundInPlayerFile;
	MugenSound mHitSound;
	int mIsGuardSoundInPlayerFile;
	MugenSound mGuardSound;

	MugenAttackHeight mGroundType;
	MugenAttackHeight mAirType;

	int mGroundSlideTime;
	int mGuardSlideTime;
	int mGroundHitTime;
	int mGuardHitTime;
	int mAirHitTime;

	int mGuardControlTime;
	int mGuardDistance;

	double mVerticalAcceleration;
	Velocity mGroundVelocity;
	double mGuardVelocity;
	Velocity mAirVelocity;
	Velocity mAirGuardVelocity;

	double mGroundCornerPushVelocityOffset;
	double mAirCornerPushVelocityOffset;
	double mDownCornerPushVelocityOffset;
	double mGuardCornerPushVelocityOffset;
	double mAirGuardCornerPushVelocityOffset;

	int mAirGuardControlTime;
	int mAirJugglePoints;

	int mHasMinimumDistance;
	Vector3DI mMinimumDistance;

	int mHasMaximumDistance;
	Vector3DI mMaximumDistance;

	int mHasSnap;
	Vector3DI mSnap;

	int mPlayer1DrawingPriority;
	int mPlayer2DrawingPriority;

	int mIsPlayer1TurningAround;
	int mPlayer1ChangeFaceDirectionRelativeToPlayer2;
	int mPlayer2ChangeFaceDirectionRelativeToPlayer1;

	int mPlayer1StateNumber;
	int mPlayer2StateNumber;

	int mCanPlayer2GetPlayer1State;
	int mIsForcingPlayer2ToStandingPosition;

	int mFall;
	Velocity mFallVelocity;

	int mCanRecoverFall;
	int mFallRecoveryTime;
	int mFallDamage;

	int mAirFall;
	int mForcePlayer2OutOfFallState;

	Velocity mDownVelocity;
	int mDownHitTime;
	int mDownDoesBounce;

	int mHitID;
	int mChainID;
	Vector3DI mNoChainIDs;

	int mDoesOnlyHitOneEnemy;

	int mCanKill;
	int mCanGuardKill;
	int mCanFallKill;

	int mNumberOfHitsForComboCounter;

	int mGetPlayer1Power;
	int mGetPlayer1GuardPower;

	int mGivePlayer2Power;
	int mGivePlayer2GuardPower;

	int mPaletteEffectTime;
	Vector3DI mPaletteEffectMultiplication;
	Vector3DI mPaletteEffectAddition;

	int mEnvironmentShakeTime;
	double mEnvironmentShakeFrequency;
	int mEnvironmentShakeAmplitude;
	double mEnvironmentShakePhase;

	int mFallEnvironmentShakeTime;
	double mFallEnvironmentShakeFrequency;
	int mFallEnvironmentShakeAmplitude;
	double mFallEnvironmentShakePhase;

	Velocity mVelocity;

	int mIsFacingRight;

	HitDefAttributeSlot mReversalDef;
} PlayerHitData;

static struct {
	IntMap mPassiveHitDataMap;
	IntMap mActiveHitDataMap;
} gData;

static void loadHitDataHandler(void* tData) {
	(void)tData;
	gData.mPassiveHitDataMap = new_int_map();
	gData.mActiveHitDataMap = new_int_map();
}

ActorBlueprint HitDataHandler = {
	.mLoad = loadHitDataHandler,
};

int initPlayerHitDataAndReturnID(Player* tPlayer)
{
	PlayerHitData* passive = allocMemory(sizeof(PlayerHitData));
	passive->mIsActive = 0;
	passive->mPlayer = tPlayer;

	int id = int_map_push_back_owned(&gData.mPassiveHitDataMap, passive);

	PlayerHitData* active = allocMemory(sizeof(PlayerHitData));
	active->mIsActive = 0;
	active->mPlayer = tPlayer;

	int_map_push_owned(&gData.mActiveHitDataMap, id, active);

	return id;
}

void removePlayerHitData(Player* tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));

	int_map_remove(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	int_map_remove(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
}

void copyHitDataToActive(Player* tPlayer, void * tHitData)
{
	PlayerHitData* passive = tHitData;
	assert(passive->mIsActive);
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* active = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);

	*active = *passive;
}

int isReceivedHitDataActive(void* tHitData)
{
	PlayerHitData* passive = tHitData;
	return passive->mIsActive;
}

int isHitDataActive(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mIsActive;
}

void setHitDataActive(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsActive = 1;

	e->mReversalDef.mIsActive = 0;
}

void setReceivedHitDataInactive(void * tHitData)
{
	PlayerHitData* passive = tHitData;
	passive->mIsActive = 0;
}

void setHitDataInactive(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsActive = 0;
}

void * getPlayerHitDataReference(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e;
}

Player * getReceivedHitDataPlayer(void * tHitData)
{
	PlayerHitData* passive = tHitData;

	return passive->mPlayer;
}

MugenStateType getHitDataType(Player * tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mType;
}

void setHitDataType(Player* tPlayer, MugenStateType tType)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mType = tType;
}

MugenAttackClass getHitDataAttackClass(Player * tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mAttackClass;
}

void setHitDataAttackClass(Player* tPlayer, MugenAttackClass tClass)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAttackClass = tClass;
}

MugenAttackType getHitDataAttackType(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mAttackType;
}

void setHitDataAttackType(Player* tPlayer, MugenAttackType tType)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAttackType = tType;
}

void setHitDataHitFlag(Player* tPlayer, char * tFlag)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	assert(strlen(tFlag) < sizeof e->mHitFlag);
	strcpy(e->mHitFlag, tFlag);
}

char* getActiveHitDataGuardFlag(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardFlag;
}

void setHitDataGuardFlag(Player* tPlayer, char * tFlag)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	assert(strlen(tFlag) < sizeof e->mGuardFlag);
	strcpy(e->mGuardFlag, tFlag);
}

void setHitDataAffectTeam(Player* tPlayer, MugenAffectTeam tAffectTeam)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAffectTeam = tAffectTeam;
}

MugenHitAnimationType getActiveHitDataAnimationType(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mAnimationType;
}

MugenHitAnimationType getHitDataAnimationType(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mAnimationType;
}

void setHitDataAnimationType(Player* tPlayer, MugenHitAnimationType tAnimationType)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAnimationType = tAnimationType;
}

MugenHitAnimationType getHitDataAirAnimationType(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirAnimationType;
}

void setHitDataAirAnimationType(Player* tPlayer, MugenHitAnimationType tAnimationType)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirAnimationType = tAnimationType;
}

void setHitDataFallAnimationType(Player* tPlayer, MugenHitAnimationType tAnimationType)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallAnimationType = tAnimationType;
}

void setHitDataPriority(Player* tPlayer, int tPriority, MugenHitPriorityType tPriorityType)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPriority = tPriority;
	e->mPriorityType = tPriorityType;
}

int getActiveHitDataDamage(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mDamage;
}

int getActiveHitDataGuardDamage(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardDamage;
}

int getHitDataDamage(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mDamage;
}

void setHitDataDamage(Player* tPlayer, int tDamage, int tGuardDamage)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mDamage = tDamage;
	e->mGuardDamage = tGuardDamage;
}

int getActiveHitDataPlayer1PauseTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mPlayer1PauseTime;
}

int getActiveHitDataPlayer1GuardPauseTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardPlayer1PauseTime;
}

int getHitDataPlayer1PauseTime(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mPlayer1PauseTime;
}

int getActiveHitDataPlayer2PauseTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mPlayer2ShakeTime;
}

int getActiveHitDataPlayer2GuardPauseTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardPlayer2ShakeTime;
}

int getHitDataPlayer2PauseTime(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mPlayer2ShakeTime;
}

void setHitDataPauseTime(Player* tPlayer, int tPlayer1PauseTime, int tPlayer2PauseTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPlayer1PauseTime = tPlayer1PauseTime;
	e->mPlayer2ShakeTime = tPlayer2PauseTime;
}

void setHitDataGuardPauseTime(Player* tPlayer, int tPlayer1PauseTime, int tPlayer2PauseTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGuardPlayer1PauseTime = tPlayer1PauseTime;
	e->mGuardPlayer2ShakeTime = tPlayer2PauseTime;
}

int isActiveHitDataSparkInPlayerFile(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mIsSparkInPlayerFile;
}

int isActiveHitDataGuardSparkInPlayerFile(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mIsGuardSparkInPlayerFile;
}

int isHitDataSparkInPlayerFile(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mIsSparkInPlayerFile;
}

int getActiveHitDataSparkNumber(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mSparkNumber;
}

int getActiveHitDataGuardSparkNumber(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardSparkNumber;
}

int getHitDataGuardSparkNumber(Player * tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardSparkNumber;
}

int getHitDataSparkNumber(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mSparkNumber;
}

void setHitDataSparkNumber(Player* tPlayer, int tIsInPlayerFile, int tNumber)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsSparkInPlayerFile = tIsInPlayerFile;
	e->mSparkNumber = tNumber;
}

void setHitDataGuardSparkNumber(Player* tPlayer, int tIsInPlayerFile, int tNumber)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsGuardSparkInPlayerFile = tIsInPlayerFile;
	e->mGuardSparkNumber = tNumber;
}

Position getActiveHitDataSparkXY(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mSparkOffset;
}

Position getHitDataSparkXY(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mSparkOffset;
}

void setHitDataSparkXY(Player* tPlayer, int tX, int tY)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mSparkOffset = makePosition(tX, tY, 0);
}

void setHitDataHitSound(Player* tPlayer, int tIsInPlayerFile, int tGroup, int tItem)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsHitSoundInPlayerFile = tIsInPlayerFile;
	e->mHitSound = makeMugenSound(tGroup, tItem);
}

void setHitDataGuardSound(Player* tPlayer, int tIsInPlayerFile, int tGroup, int tItem)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsGuardSoundInPlayerFile = tIsInPlayerFile;
	e->mGuardSound = makeMugenSound(tGroup, tItem);
}

MugenAttackHeight getActiveHitDataGroundType(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundType;
}

MugenAttackHeight getHitDataGroundType(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundType;
}

void setHitDataGroundType(Player* tPlayer, MugenAttackHeight tType)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGroundType = tType;
}

MugenAttackHeight getActiveHitDataAirType(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirType;
}

MugenAttackHeight getHitDataAirType(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirType;
}

void setHitDataAirType(Player* tPlayer, MugenAttackHeight tType)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirType = tType;
}

int getActiveHitDataGroundHitTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundHitTime;
}

int getHitDataGroundHitTime(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundHitTime;
}

void setHitDataGroundHitTime(Player* tPlayer, int tHitTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGroundHitTime = tHitTime;
}

int getActiveHitDataGroundSlideTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundSlideTime;
}

int getHitDataGroundSlideTime(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundSlideTime;
}

void setHitDataGroundSlideTime(Player* tPlayer, int tSlideTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGroundSlideTime = tSlideTime;
}

int getActiveHitDataGuardHitTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardHitTime;
}

int getHitDataGuardHitTime(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardHitTime;
}

void setHitDataGuardHitTime(Player* tPlayer, int tHitTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGuardHitTime = tHitTime;
}

int getHitDataGuardSlideTime(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardSlideTime;
}

void setHitDataGuardSlideTime(Player* tPlayer, int tSlideTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGuardSlideTime = tSlideTime;
}

int getActiveHitDataAirHitTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirHitTime;
}

void setHitDataAirHitTime(Player* tPlayer, int tHitTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirHitTime = tHitTime;
}

int getActiveHitDataGuardControlTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardControlTime;
}

int getHitDataGuardControlTime(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardControlTime;
}

void setHitDataGuardControlTime(Player* tPlayer, int tControlTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGuardControlTime = tControlTime;
}

int getHitDataGuardDistance(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardDistance;
}

void setHitDataGuardDistance(Player* tPlayer, int tDistance)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGuardDistance = tDistance;
}

double getActiveHitDataYAccel(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mVerticalAcceleration;
}

double getHitDataYAccel(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mVerticalAcceleration;
}

void setHitDataYAccel(Player* tPlayer, double YAccel)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mVerticalAcceleration = YAccel;
}

double getActiveHitDataGroundVelocityX(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundVelocity.x;
}

double getHitDataGroundVelocityX(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundVelocity.x;
}

double getActiveHitDataGroundVelocityY(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundVelocity.y;
}

double getHitDataGroundVelocityY(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundVelocity.y;
}

void setHitDataGroundVelocity(Player* tPlayer, double tX, double tY)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGroundVelocity = makePosition(tX, tY, 0);
}

double getActiveHitDataGuardVelocity(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardVelocity;
}

double getHitDataGuardVelocity(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardVelocity;
}

void setHitDataGuardVelocity(Player* tPlayer, double tX)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGuardVelocity = tX;
}

double getActiveHitDataAirVelocityX(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirVelocity.x;
}

double getHitDataAirVelocityX(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirVelocity.x;
}

double getActiveHitDataAirVelocityY(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirVelocity.y;
}

double getHitDataAirVelocityY(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirVelocity.y;
}

void setHitDataAirVelocity(Player* tPlayer, double tX, double tY)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirVelocity = makePosition(tX, tY, 0);
}

void setHitDataAirGuardVelocity(Player* tPlayer, double tX, double tY)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirGuardVelocity = makePosition(tX, tY, 0);
}

double getGroundCornerPushVelocityOffset(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGroundCornerPushVelocityOffset;
}

void setGroundCornerPushVelocityOffset(Player* tPlayer, double tX)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGroundCornerPushVelocityOffset = tX;
}

void setAirCornerPushVelocityOffset(Player* tPlayer, double tX)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirCornerPushVelocityOffset = tX;
}

void setDownCornerPushVelocityOffset(Player* tPlayer, double tX)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mDownCornerPushVelocityOffset = tX;
}

double getGuardCornerPushVelocityOffset(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGuardCornerPushVelocityOffset;
}

void setGuardCornerPushVelocityOffset(Player* tPlayer, double tX)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGuardCornerPushVelocityOffset = tX;
}

void setAirGuardCornerPushVelocityOffset(Player* tPlayer, double tX)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirGuardCornerPushVelocityOffset = tX;
}

int getActiveHitDataAirGuardControlTime(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirGuardControlTime;
}

void setHitDataAirGuardControlTime(Player* tPlayer, int tControlTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirGuardControlTime = tControlTime;
}

void setHitDataAirJuggle(Player* tPlayer, int tJuggle)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirJugglePoints = tJuggle;
}

void setHitDataMinimumDistanceInactive(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mHasMinimumDistance = 0;
}

void setHitDataMinimumDistance(Player* tPlayer, int x, int y)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mHasMinimumDistance = 1;
	e->mMinimumDistance = makeVector3DI(x, y, 0);
}

void setHitDataMaximumDistanceInactive(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mHasMaximumDistance = 0;
}

void setHitDataMaximumDistance(Player* tPlayer, int x, int y)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mHasMaximumDistance = 1;
	e->mMaximumDistance = makeVector3DI(x, y, 0);
}

void setHitDataSnapInactive(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mHasSnap = 0;
}

void setHitDataSnap(Player* tPlayer, int x, int y)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mHasSnap = 1;
	e->mSnap = makeVector3DI(x, y, 0);
}

void setHitDataPlayer1SpritePriority(Player* tPlayer, int tPriority)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPlayer1DrawingPriority = tPriority;
}

void setHitDataPlayer2SpritePriority(Player* tPlayer, int tPriority)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPlayer2DrawingPriority = tPriority;
}

void setHitDataPlayer1FaceDirection(Player* tPlayer, int tFaceDirection)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsPlayer1TurningAround = tFaceDirection == -1;
}

void setHitDataPlayer1ChangeFaceDirectionRelativeToPlayer2(Player* tPlayer, int tFaceDirection)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPlayer1ChangeFaceDirectionRelativeToPlayer2 = tFaceDirection;
}

void setHitDataPlayer2ChangeFaceDirectionRelativeToPlayer1(Player* tPlayer, int tFaceDirection)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPlayer2ChangeFaceDirectionRelativeToPlayer1 = tFaceDirection;
}

int getActiveHitDataPlayer1StateNumber(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mPlayer1StateNumber;
}

void setPlayer1StateNumber(Player* tPlayer, int tStateNumber)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPlayer1StateNumber = tStateNumber;
}

int getActiveHitDataPlayer2StateNumber(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mPlayer2StateNumber;
}

void setPlayer2StateNumber(Player* tPlayer, int tStateNumber)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPlayer2StateNumber = tStateNumber;
}

int getActiveHitDataPlayer2CapableOfGettingPlayer1State(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mCanPlayer2GetPlayer1State;
}

void setHitDataPlayer2CapableOfGettingPlayer1State(Player* tPlayer, int tVal)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mCanPlayer2GetPlayer1State = tVal;
}

void setHitDataForceStanding(Player* tPlayer, int tIsForcedToStand)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsForcingPlayer2ToStandingPosition = tIsForcedToStand;
}

int getActiveHitDataFall(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mFall;
}

int getHitDataFall(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mFall;
}

void setActiveHitDataFall(Player * tPlayer, int tIsCausingPlayer2ToFall)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	e->mFall = tIsCausingPlayer2ToFall;
}

void setHitDataFall(Player* tPlayer, int tIsCausingPlayer2ToFall)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFall = tIsCausingPlayer2ToFall;
}

double getActiveHitDataFallXVelocity(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mFallVelocity.x;
}

double getHitDataFallXVelocity(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mFallVelocity.x;
}

void setActiveHitDataFallXVelocity(Player * tPlayer, double tX)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	e->mFallVelocity.x = tX;
}

void setHitDataFallXVelocity(Player* tPlayer, double tX)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallVelocity.x = tX;
}

double getActiveHitDataFallYVelocity(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mFallVelocity.y;
}

double getHitDataFallYVelocity(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mFallVelocity.y;
}

void setActiveHitDataFallYVelocity(Player * tPlayer, double tY)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	e->mFallVelocity.y = tY;
}

void setHitDataFallYVelocity(Player* tPlayer, double tY)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallVelocity.y = tY;
}

int getActiveHitDataFallRecovery(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mCanRecoverFall;
}

int getHitDataFallRecovery(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mCanRecoverFall;
}

void setHitDataFallRecovery(Player* tPlayer, int tCanRecover)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mCanRecoverFall = tCanRecover;
}

void setHitDataFallRecoveryTime(Player* tPlayer, int tRecoverTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallRecoveryTime = tRecoverTime;
}

void setHitDataFallDamage(Player* tPlayer, int tDamage)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallDamage = tDamage;
}

int getActiveHitDataAirFall(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mAirFall;
}

void setHitDataAirFall(Player* tPlayer, int tIsCausingPlayer2ToFall)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mAirFall = tIsCausingPlayer2ToFall;
}

void setHitDataForceNoFall(Player* tPlayer, int tForcePlayer2NotToFall)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mForcePlayer2OutOfFallState = tForcePlayer2NotToFall;
}

void setHitDataDownVelocity(Player* tPlayer, double tX, double tY)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mDownVelocity = makePosition(tX, tY, 0);
}

void setHitDataDownHitTime(Player* tPlayer, int tHitTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mDownHitTime = tHitTime;
}

void setHitDataDownBounce(Player* tPlayer, int tDoesBounce)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mDownDoesBounce = tDoesBounce;
}

void setHitDataHitID(Player* tPlayer, int tID)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mHitID = tID;
}

void setHitDataChainID(Player* tPlayer, int tID)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mChainID = tID;
}

void setHitDataNoChainID(Player* tPlayer, int tID1, int tID2)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mNoChainIDs = makeVector3DI(tID1, tID2, -1);
}

void setHitDataHitOnce(Player* tPlayer, int tIsOnlyAffectingOneOpponent)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mDoesOnlyHitOneEnemy = tIsOnlyAffectingOneOpponent;
}

void setHitDataKill(Player* tPlayer, int tCanKill)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mCanKill = tCanKill;
}

void setHitDataGuardKill(Player* tPlayer, int tCanKill)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mCanGuardKill = tCanKill;
}

void setHitDataFallKill(Player* tPlayer, int tCanKill)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mCanFallKill = tCanKill;
}

void setHitDataNumberOfHits(Player* tPlayer, int tNumberOfHits)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mNumberOfHitsForComboCounter = tNumberOfHits;
}

int getActiveHitDataPlayer1PowerAdded(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGetPlayer1Power;
}

int getActiveHitDataPlayer1GuardPowerAdded(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGetPlayer1GuardPower;
}

int getHitDataPlayer1PowerAdded(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGetPlayer1Power;
}

void setHitDataGetPower(Player* tPlayer, int tPlayer1PowerAdded, int tPlayer1PowerAddedWhenGuarded)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGetPlayer1Power = tPlayer1PowerAdded;
	e->mGetPlayer1GuardPower = tPlayer1PowerAddedWhenGuarded;
}

int getActiveHitDataPlayer2PowerAdded(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGivePlayer2Power;
}

int getActiveHitDataPlayer2GuardPowerAdded(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mGivePlayer2GuardPower;
}

int getHitDataPlayer2PowerAdded(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mGivePlayer2Power;
}

void setHitDataGivePower(Player* tPlayer, int tPlayer2PowerAdded, int tPlayer2PowerAddedWhenGuarded)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mGivePlayer2Power = tPlayer2PowerAdded;
	e->mGivePlayer2GuardPower = tPlayer2PowerAddedWhenGuarded;
}

void setHitDataPaletteEffectTime(Player* tPlayer, int tEffectTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPaletteEffectTime = tEffectTime;
}

void setHitDataPaletteEffectMultiplication(Player* tPlayer, int tR, int tG, int tB)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPaletteEffectMultiplication = makeVector3DI(tR, tG, tB);
}

void setHitDataPaletteEffectAddition(Player* tPlayer, int tR, int tG, int tB)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mPaletteEffectAddition = makeVector3DI(tR, tG, tB);
}

void setHitDataEnvironmentShakeTime(Player* tPlayer, int tTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mEnvironmentShakeTime = tTime;
}

void setHitDataEnvironmentShakeFrequency(Player* tPlayer, double tFrequency)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mEnvironmentShakeFrequency = tFrequency;
}

void setHitDataEnvironmentShakeAmplitude(Player* tPlayer, int tAmplitude)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mEnvironmentShakeAmplitude = tAmplitude;
}

void setHitDataEnvironmentShakePhase(Player* tPlayer, double tPhase)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mEnvironmentShakePhase = tPhase;
}

void setHitDataFallEnvironmentShakeTime(Player* tPlayer, int tTime)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallEnvironmentShakeTime = tTime;
}

void setHitDataFallEnvironmentShakeFrequency(Player* tPlayer, double tFrequency)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallEnvironmentShakeFrequency = tFrequency;
}

void setHitDataFallEnvironmentShakeAmplitude(Player* tPlayer, int tAmplitude)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallEnvironmentShakeAmplitude = tAmplitude;
}

void setHitDataFallEnvironmentShakePhase(Player* tPlayer, double tPhase)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mFallEnvironmentShakePhase = tPhase;
}

double getActiveHitDataVelocityX(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mVelocity.x;
}

double getHitDataVelocityX(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mVelocity.x;
}

void setActiveHitDataVelocityX(Player * tPlayer, double x)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	e->mVelocity.x = x;
}

void setHitDataVelocityX(Player* tPlayer, double x)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mVelocity.x = x;
}

double getActiveHitDataVelocityY(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mVelocity.y;
}

double getHitDataVelocityY(Player* tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	return e->mVelocity.y;
}

void setActiveHitDataVelocityY(Player * tPlayer, double y)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	e->mVelocity.y = y;
}

void setHitDataVelocityY(Player* tPlayer, double y)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mVelocity.y = y;
}

int getActiveHitDataIsFacingRight(Player * tPlayer)
{
	assert(int_map_contains(&gData.mActiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mActiveHitDataMap, tPlayer->mHitDataID);
	return e->mIsFacingRight;
}

void setHitDataIsFacingRight(Player * tPlayer, int tIsFacingRight)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mIsFacingRight = tIsFacingRight;
}

void resetHitDataReversalDef(Player * tPlayer)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	e->mReversalDef.mIsActive = 1;
	e->mReversalDef.mFlag2Amount = 0;
}

void setHitDataReversalDefFlag1(Player * tPlayer, char * tFlag)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);
	strcpy(e->mReversalDef.mFlag1, tFlag);
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

void addHitDataReversalDefFlag2(Player * tPlayer, char * tFlag)
{
	assert(int_map_contains(&gData.mPassiveHitDataMap, tPlayer->mHitDataID));
	PlayerHitData* e = int_map_get(&gData.mPassiveHitDataMap, tPlayer->mHitDataID);

	char* nFlag = allocMemory(strlen(tFlag) + 5);
	copyOverCleanFlag2(nFlag, tFlag);
	assert(strlen(nFlag) == 2);
	turnStringLowercase(nFlag);

	assert(e->mReversalDef.mFlag2Amount < MAXIMUM_HITSLOT_FLAG_2_AMOUNT);

	strcpy(e->mReversalDef.mFlag2[e->mReversalDef.mFlag2Amount], nFlag);
	e->mReversalDef.mFlag2Amount++;
}


