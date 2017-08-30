#pragma once

#include "playerdefinition.h"

#include <tari/actorhandler.h>

extern ActorBlueprint ProjectileHandler;

void addAdditionalProjectileData(Player* tProjectile);
void removeAdditionalProjectileData(Player* tProjectile);
void handleProjectileHit(Player* tProjectile);

void setProjectileID(Player* p, int tID);

void setProjectileAnimation(Player* p, int tAnimation);
int getProjectileHitAnimation(Player* p);
void setProjectileHitAnimation(Player* p, int tAnimation);
int getProjectileRemoveAnimation(Player* p);
void setProjectileRemoveAnimation(Player* p, int tAnimation);
void setProjectileCancelAnimation(Player* p, int tAnimation);
void setProjectileScale(Player* p, double tX, double tY);
void setProjectileRemoveAfterHit(Player* p, int tValue);
void setProjectileRemoveTime(Player* p, int tTime);
void setProjectileVelocity(Player* p, double tX, double tY);
void setProjectileRemoveVelocity(Player* p, double tX, double tY);
void setProjectileAcceleration(Player* p, double tX, double tY);
void setProjectileVelocityMultipliers(Player* p, double tX, double tY);


void setProjectileHitAmountBeforeVanishing(Player* p, int tHitAmount);
void setProjectilMisstime(Player* p, int tMissTime);
void setProjectilePriority(Player* p, int tPriority);
void setProjectileSpritePriority(Player* p, int tSpritePriority);

void setProjectileEdgeBound(Player* p, int tEdgeBound);
void setProjectileStageBound(Player* p, int tStageBound);
void setProjectileHeightBoundValues(Player* p, int tLowerBound, int tUpperBound);
void setProjectilePosition(Player* p, Position tPosition);

void setProjectileShadow(Player* p, int tShadow);
void setProjectileSuperMoveTime(Player* p, int tSuperMoveTime);
void setProjectilePauseMoveTime(Player* p, int tPauseMoveTime);

void setProjectileHasOwnPalette(Player* p, int tValue);
void setProjectileRemapPalette(Player* p, int tGroup, int tItem); // TODO: rename
void setProjectileAfterImageTime(Player* p, int tAfterImageTime);
void setProjectileAfterImageLength(Player* p, int tAfterImageLength);
void setProjectileAfterImage(Player* p, int tAfterImage);