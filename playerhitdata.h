#pragma once

#include <tari/actorhandler.h>

#include "mugenstatereader.h"
#include "playerdefinition.h"

typedef enum {
	MUGEN_ATTACK_CLASS_NORMAL,
	MUGEN_ATTACK_CLASS_SPECIAL,
	MUGEN_ATTACK_CLASS_HYPER,
} MugenAttackClass;

typedef enum {
	MUGEN_ATTACK_TYPE_ATTACK,
	MUGEN_ATTACK_TYPE_THROW,
	MUGEN_ATTACK_TYPE_PROJECTILE,
} MugenAttackType;

typedef enum {
	MUGEN_ATTACK_HEIGHT_HIGH,
	MUGEN_ATTACK_HEIGHT_LOW,
	MUGEN_ATTACK_HEIGHT_TRIP,
	MUGEN_ATTACK_HEIGHT_NONE,
} MugenAttackHeight;

typedef enum {
	MUGEN_HIT_ANIMATION_TYPE_LIGHT,
	MUGEN_HIT_ANIMATION_TYPE_MEDIUM,
	MUGEN_HIT_ANIMATION_TYPE_HARD,
	MUGEN_HIT_ANIMATION_TYPE_BACK,
	MUGEN_HIT_ANIMATION_TYPE_UP,
	MUGEN_HIT_ANIMATION_TYPE_DIAGONAL_UP,
	MUGEN_HIT_ANIMATION_TYPE_HEAVY,

} MugenHitAnimationType;

typedef enum {
	MUGEN_AFFECT_TEAM_BOTH,
	MUGEN_AFFECT_TEAM_ENEMY,
	MUGEN_AFFECT_TEAM_FRIENDLY,

} MugenAffectTeam;

typedef enum {
	MUGEN_HIT_PRIORITY_DODGE,
	MUGEN_HIT_PRIORITY_HIT,
	MUGEN_HIT_PRIORITY_MISS,
} MugenHitPriorityType;

extern ActorBlueprint HitDataHandler;

int initPlayerHitDataAndReturnID(Player* tPlayer);
void removePlayerHitData(Player* tPlayer);

void copyHitDataToActive(Player* tPlayer, void* tHitData);

int isReceivedHitDataActive(void* tHitData);
int isHitDataActive(Player* tPlayer);
void setHitDataActive(Player* tPlayer);
void setReceivedHitDataInactive(void* tHitData);
void setHitDataInactive(Player* tPlayer);

void* getPlayerHitDataReference(Player* tPlayer);
Player* getReceivedHitDataPlayer(void* tHitData);

MugenStateType getHitDataType(Player* tPlayer);
void setHitDataType(Player* tPlayer, MugenStateType tType);
MugenAttackClass getHitDataAttackClass(Player* tPlayer);
void setHitDataAttackClass(Player* tPlayer, MugenAttackClass tClass);

MugenAttackType getHitDataAttackType(Player* tPlayer);
void setHitDataAttackType(Player* tPlayer, MugenAttackType tType);

void setHitDataHitFlag(Player* tPlayer, char* tFlag);
void setHitDataGuardFlag(Player* tPlayer, char* tFlag);
void setHitDataAffectTeam(Player* tPlayer, MugenAffectTeam tAffectTeam);

MugenHitAnimationType getActiveHitDataAnimationType(Player* tPlayer);
MugenHitAnimationType getHitDataAnimationType(Player* tPlayer);
void setHitDataAnimationType(Player* tPlayer, MugenHitAnimationType tAnimationType);
MugenHitAnimationType getHitDataAirAnimationType(Player* tPlayer);
void setHitDataAirAnimationType(Player* tPlayer, MugenHitAnimationType tAnimationType);
void setHitDataFallAnimationType(Player* tPlayer, MugenHitAnimationType tAnimationType);

void setHitDataPriority(Player* tPlayer, int tPriority, MugenHitPriorityType tPriorityType);
int getActiveHitDataDamage(Player* tPlayer);
int getActiveHitDataGuardDamage(Player* tPlayer);
int getHitDataDamage(Player* tPlayer);
void setHitDataDamage(Player* tPlayer, int tDamage, int tGuardDamage);

int getActiveHitDataPlayer1PauseTime(Player* tPlayer);
int getActiveHitDataPlayer1GuardPauseTime(Player* tPlayer);
int getHitDataPlayer1PauseTime(Player* tPlayer);
int getActiveHitDataPlayer2PauseTime(Player* tPlayer);
int getActiveHitDataPlayer2GuardPauseTime(Player* tPlayer);
int getHitDataPlayer2PauseTime(Player* tPlayer);
void setHitDataPauseTime(Player* tPlayer, int tPlayer1PauseTime, int tPlayer2PauseTime);

void setHitDataGuardPauseTime(Player* tPlayer, int tPlayer1PauseTime, int tPlayer2PauseTime);

int isActiveHitDataSparkInPlayerFile(Player* tPlayer);
int isActiveHitDataGuardSparkInPlayerFile(Player* tPlayer);
int isHitDataSparkInPlayerFile(Player* tPlayer);
int getActiveHitDataSparkNumber(Player* tPlayer);
int getActiveHitDataGuardSparkNumber(Player* tPlayer);
int getHitDataGuardSparkNumber(Player * tPlayer);
int getHitDataSparkNumber(Player* tPlayer);
void setHitDataSparkNumber(Player* tPlayer, int tIsInPlayerFile, int tNumber);
void setHitDataGuardSparkNumber(Player* tPlayer, int tIsInPlayerFile, int tNumber);
Position getActiveHitDataSparkXY(Player* tPlayer);
Position getHitDataSparkXY(Player* tPlayer);
void setHitDataSparkXY(Player* tPlayer, int tX, int tY);

void setHitDataHitSound(Player* tPlayer, int tIsInPlayerFile, int tGroup, int tItem);
void setHitDataGuardSound(Player* tPlayer, int tIsInPlayerFile, int tGroup, int tItem);

MugenAttackHeight getActiveHitDataGroundType(Player* tPlayer);
MugenAttackHeight getHitDataGroundType(Player* tPlayer);
void setHitDataGroundType(Player* tPlayer, MugenAttackHeight tType);
MugenAttackHeight getActiveHitDataAirType(Player* tPlayer);
MugenAttackHeight getHitDataAirType(Player* tPlayer);
void setHitDataAirType(Player* tPlayer, MugenAttackHeight tType);

int getActiveHitDataGroundHitTime(Player* tPlayer);
int getHitDataGroundHitTime(Player* tPlayer);
void setHitDataGroundHitTime(Player* tPlayer, int tHitTime);
int getActiveHitDataGroundSlideTime(Player* tPlayer);
int getHitDataGroundSlideTime(Player* tPlayer);
void setHitDataGroundSlideTime(Player* tPlayer, int tSlideTime);

int getActiveHitDataGuardHitTime(Player* tPlayer);
int getHitDataGuardHitTime(Player* tPlayer);
void setHitDataGuardHitTime(Player* tPlayer, int tHitTime);
int getHitDataGuardSlideTime(Player* tPlayer);
void setHitDataGuardSlideTime(Player* tPlayer, int tSlideTime);

int getActiveHitDataAirHitTime(Player* tPlayer);
void setHitDataAirHitTime(Player* tPlayer, int tHitTime);
int getActiveHitDataGuardControlTime(Player* tPlayer);
int getHitDataGuardControlTime(Player* tPlayer);
void setHitDataGuardControlTime(Player* tPlayer, int tControlTime);
int getHitDataGuardDistance(Player* tPlayer);
void setHitDataGuardDistance(Player* tPlayer, int tDistance);

double getActiveHitDataYAccel(Player* tPlayer);
double getHitDataYAccel(Player* tPlayer);
void setHitDataYAccel(Player* tPlayer, double YAccel);

double getActiveHitDataGroundVelocityX(Player* tPlayer);
double getHitDataGroundVelocityX(Player* tPlayer);
double getActiveHitDataGroundVelocityY(Player* tPlayer);
double getHitDataGroundVelocityY(Player* tPlayer);
void setHitDataGroundVelocity(Player* tPlayer, double tX, double tY);
double getActiveHitDataGuardVelocity(Player* tPlayer);
double getHitDataGuardVelocity(Player* tPlayer);
void setHitDataGuardVelocity(Player* tPlayer, double tX);
double getActiveHitDataAirVelocityX(Player* tPlayer);
double getHitDataAirVelocityX(Player* tPlayer);
double getActiveHitDataAirVelocityY(Player* tPlayer);
double getHitDataAirVelocityY(Player* tPlayer);
void setHitDataAirVelocity(Player* tPlayer, double tX, double tY);
void setHitDataAirGuardVelocity(Player* tPlayer, double tX, double tY);

double getGroundCornerPushVelocityOffset(Player* tPlayer);
void setGroundCornerPushVelocityOffset(Player* tPlayer, double tX);
void setAirCornerPushVelocityOffset(Player* tPlayer, double tX);
void setDownCornerPushVelocityOffset(Player* tPlayer, double tX);
double getGuardCornerPushVelocityOffset(Player* tPlayer);
void setGuardCornerPushVelocityOffset(Player* tPlayer, double tX);
void setAirGuardCornerPushVelocityOffset(Player* tPlayer, double tX);

int getActiveHitDataAirGuardControlTime(Player* tPlayer);
void setHitDataAirGuardControlTime(Player* tPlayer, int tControlTime);
void setHitDataAirJuggle(Player* tPlayer, int tJuggle);

void setHitDataMinimumDistanceInactive(Player* tPlayer);
void setHitDataMinimumDistance(Player* tPlayer, int x, int y);

void setHitDataMaximumDistanceInactive(Player* tPlayer);
void setHitDataMaximumDistance(Player* tPlayer, int x, int y);

void setHitDataSnapInactive(Player* tPlayer);
void setHitDataSnap(Player* tPlayer, int x, int y);

void setHitDataPlayer1SpritePriority(Player* tPlayer, int tPriority);
void setHitDataPlayer2SpritePriority(Player* tPlayer, int tPriority);
void setHitDataPlayer1FaceDirection(Player* tPlayer, int tFaceDirection);
void setHitDataPlayer1ChangeFaceDirectionRelativeToPlayer2(Player* tPlayer, int tFaceDirection);
void setHitDataPlayer2ChangeFaceDirectionRelativeToPlayer1(Player* tPlayer, int tFaceDirection);

int getActiveHitDataPlayer1StateNumber(Player* tPlayer);
void setPlayer1StateNumber(Player* tPlayer, int tStateNumber);
int getActiveHitDataPlayer2StateNumber(Player* tPlayer);
void setPlayer2StateNumber(Player* tPlayer, int tStateNumber);
int getActiveHitDataPlayer2CapableOfGettingPlayer1State(Player* tPlayer);
void setHitDataPlayer2CapableOfGettingPlayer1State(Player* tPlayer, int tVal);
void setHitDataForceStanding(Player* tPlayer, int tIsForcedToStand);

int getActiveHitDataFall(Player* tPlayer);
int getHitDataFall(Player* tPlayer);
void setActiveHitDataFall(Player* tPlayer, int tIsCausingPlayer2ToFall);
void setHitDataFall(Player* tPlayer, int tIsCausingPlayer2ToFall);
double getActiveHitDataFallXVelocity(Player* tPlayer);
double getHitDataFallXVelocity(Player* tPlayer);
void setActiveHitDataFallXVelocity(Player* tPlayer, double tX);
void setHitDataFallXVelocity(Player* tPlayer, double tX);
double getActiveHitDataFallYVelocity(Player* tPlayer);
double getHitDataFallYVelocity(Player* tPlayer);
void setActiveHitDataFallYVelocity(Player* tPlayer, double tY);
void setHitDataFallYVelocity(Player* tPlayer, double tY);
int getActiveHitDataFallRecovery(Player* tPlayer);
int getHitDataFallRecovery(Player* tPlayer);
void setHitDataFallRecovery(Player* tPlayer, int tCanRecover);
void setHitDataFallRecoveryTime(Player* tPlayer, int tRecoverTime);
void setHitDataFallDamage(Player* tPlayer, int tDamage);
int getActiveHitDataAirFall(Player* tPlayer);
void setHitDataAirFall(Player* tPlayer, int tIsCausingPlayer2ToFall);
void setHitDataForceNoFall(Player* tPlayer, int tForcePlayer2NotToFall);

void setHitDataDownVelocity(Player* tPlayer, double tX, double tY);
void setHitDataDownHitTime(Player* tPlayer, int tHitTime);
void setHitDataDownBounce(Player* tPlayer, int tDoesBounce);

void setHitDataHitID(Player* tPlayer, int tID);
void setHitDataChainID(Player* tPlayer, int tID);
void setHitDataNoChainID(Player* tPlayer, int tID1, int tID2);

void setHitDataHitOnce(Player* tPlayer, int tIsOnlyAffectingOneOpponent);
void setHitDataKill(Player* tPlayer, int tCanKill);
void setHitDataGuardKill(Player* tPlayer, int tCanKill);
void setHitDataFallKill(Player* tPlayer, int tCanKill);
void setHitDataNumberOfHits(Player* tPlayer, int tNumberOfHits);
int getActiveHitDataPlayer1PowerAdded(Player* tPlayer);
int getActiveHitDataPlayer1GuardPowerAdded(Player* tPlayer);
int getHitDataPlayer1PowerAdded(Player* tPlayer);
void setHitDataGetPower(Player* tPlayer, int tPlayer1PowerAdded, int tPlayer1PowerAddedWhenGuarded);
int getActiveHitDataPlayer2PowerAdded(Player* tPlayer);
int getActiveHitDataPlayer2GuardPowerAdded(Player* tPlayer);
int getHitDataPlayer2PowerAdded(Player* tPlayer);
void setHitDataGivePower(Player* tPlayer, int tPlayer2PowerAdded, int tPlayer2PowerAddedWhenGuarded);

void setHitDataPaletteEffectTime(Player* tPlayer, int tEffectTime);
void setHitDataPaletteEffectMultiplication(Player* tPlayer, int tR, int tG, int tB);
void setHitDataPaletteEffectAddition(Player* tPlayer, int tR, int tG, int tB);

void setHitDataEnvironmentShakeTime(Player* tPlayer, int tTime);
void setHitDataEnvironmentShakeFrequency(Player* tPlayer, double tFrequency);
void setHitDataEnvironmentShakeAmplitude(Player* tPlayer, int tAmplitude);
void setHitDataEnvironmentShakePhase(Player* tPlayer, double tPhase);

void setHitDataFallEnvironmentShakeTime(Player* tPlayer, int tTime);
void setHitDataFallEnvironmentShakeFrequency(Player* tPlayer, double tFrequency);
void setHitDataFallEnvironmentShakeAmplitude(Player* tPlayer, int tAmplitude);
void setHitDataFallEnvironmentShakePhase(Player* tPlayer, double tPhase);

double getActiveHitDataVelocityX(Player* tPlayer);
double getHitDataVelocityX(Player* tPlayer);
void setActiveHitDataVelocityX(Player* tPlayer, double x);
void setHitDataVelocityX(Player* tPlayer, double x);
double getActiveHitDataVelocityY(Player* tPlayer);
double getHitDataVelocityY(Player* tPlayer);
void setActiveHitDataVelocityY(Player* tPlayer, double y);
void setHitDataVelocityY(Player* tPlayer, double y);

int getActiveHitDataIsFacingRight(Player* tPlayer);
void setHitDataIsFacingRight(Player* tPlayer, int tIsFacingRight);

void resetHitDataReversalDef(Player* tPlayer);
void setHitDataReversalDefFlag1(Player* tPlayer, char* tFlag);
void addHitDataReversalDefFlag2(Player* tPlayer, char* tFlag);