#include "projectile.h"

#include <assert.h>

#include <tari/datastructures.h>
#include <tari/math.h>

#include "stage.h"

typedef enum {
	PROJECTILE_STATE_IDLE,
	PROJECTILE_STATE_HIT,
	PROJECTILE_STATE_REMOVE,
} ProjectileState;

typedef struct {
	Player* mPlayer;

	int mID;
	int mHitAnimation;
	int mRemoveAnimation;
	int mCancelAnimation;

	Vector3D mScale;
	int mRemoveAfterHit;
	int mRemoveTime;

	Vector3D mRemoveVelocity;
	Vector3D mAcceleration;
	Vector3D mVelocityMultipliers;

	int mHitAmountBeforeVanishing;
	int mMissTime;
	int mPriority;
	int mSpritePriority;
	int mEdgeBound;
	int mStageBound;
	int mLowerBound;
	int mUpperBound;
	int mShadow;
	int mSuperMoveTime;
	int mPauseMoveTime;
	int mHasOwnPalette;
	
	int mRemapPaletteGroup;
	int mRemapPaletteItem;

	int mAfterImageTime;
	int mAfterImageLength;
	int mAfterImage;

	int mNow;
	ProjectileState mState;
} Projectile;

static struct {
	IntMap mProjectileList;
} gData;

static void loadProjectileHandler(void* tData) {
	(void)tData;
	gData.mProjectileList = new_int_map();
}

static int updateProjectileDurationAndReturnIfOver(Projectile* e) {
	if (e->mRemoveTime == -1) return 0;

	e->mNow++;
	if (e->mNow >= e->mRemoveTime) {
		removeProjectile(e->mPlayer);
		return 1;
	}

	return 0;
}

static int updateProjectileScreenBoundAndReturnIfOver(Projectile* e) {
	double left = getStageLeftOfScreenBasedOnPlayer(getPlayerCoordinateP(e->mPlayer));
	double right = getStageRightOfScreenBasedOnPlayer(getPlayerCoordinateP(e->mPlayer));

	double x = getPlayerPositionX(e->mPlayer, getPlayerCoordinateP(e->mPlayer));

	double overShootLeft = left - x;
	double overShootRight = x - right;
	double maxi = max(overShootLeft, overShootRight);
	
	if (maxi > e->mEdgeBound) {
		printf("ded\n");	
		removeProjectile(e->mPlayer);
		return 1;
	}

	return 0;
}

static void updateSingleProjectile(void* tCaller, void* tData) {
	(void)tCaller;
	Projectile* e = tData;
	
	if (updateProjectileDurationAndReturnIfOver(e)) return;
	if (updateProjectileScreenBoundAndReturnIfOver(e)) return;
}

static void updateProjectileHandler(void* tData) {
	(void)tData;
	int_map_map(&gData.mProjectileList, updateSingleProjectile, NULL);
}

ActorBlueprint ProjectileHandler = {
	.mLoad = loadProjectileHandler,
	.mUpdate = updateProjectileHandler,
};

void addAdditionalProjectileData(Player* tProjectile) {
	Projectile* e = allocMemory(sizeof(Projectile));
	e->mNow = 0;
	e->mState = PROJECTILE_STATE_IDLE;
	e->mPlayer = tProjectile;
	tProjectile->mProjectileDataID = int_map_push_back_owned(&gData.mProjectileList, e);
}

void removeAdditionalProjectileData(Player* tProjectile) {
	assert(int_map_contains(&gData.mProjectileList, tProjectile->mProjectileDataID));
	int_map_remove(&gData.mProjectileList, tProjectile->mProjectileDataID);
}

void handleProjectileHit(Player * tProjectile)
{
	assert(int_map_contains(&gData.mProjectileList, tProjectile->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, tProjectile->mProjectileDataID);


	if (e->mHitAnimation != -1) {
		changePlayerAnimation(tProjectile, e->mHitAnimation);
	}

	if (e->mRemoveAfterHit) {
		removeProjectile(tProjectile);
	}
}

void setProjectileID(Player * tProjectile, int tID)
{
	assert(int_map_contains(&gData.mProjectileList, tProjectile->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, tProjectile->mProjectileDataID);
	e->mID = tID;
}

void setProjectileAnimation(Player * p, int tAnimation)
{
	changePlayerAnimation(p, tAnimation);
}

int getProjectileHitAnimation(Player * p)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	return e->mHitAnimation;
}

void setProjectileHitAnimation(Player * p, int tAnimation)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mHitAnimation = tAnimation;
}

int getProjectileRemoveAnimation(Player * p)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	return e->mRemoveAnimation;
}

void setProjectileRemoveAnimation(Player * p, int tAnimation)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mRemoveAnimation = tAnimation;
}

void setProjectileCancelAnimation(Player * p, int tAnimation)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mCancelAnimation = tAnimation;
}

void setProjectileScale(Player * p, double tX, double tY)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mScale = makePosition(tX, tY, 1);
}

void setProjectileRemoveAfterHit(Player * p, int tValue)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mRemoveAfterHit = tValue;
}

void setProjectileRemoveTime(Player * p, int tTime)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mRemoveTime = tTime;
}

void setProjectileVelocity(Player * p, double tX, double tY)
{
	setPlayerVelocityX(p, tX, getPlayerCoordinateP(p));
	setPlayerVelocityY(p, tY, getPlayerCoordinateP(p));
}

void setProjectileRemoveVelocity(Player * p, double tX, double tY)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mRemoveVelocity = makePosition(tX, tY, 0);
}

void setProjectileAcceleration(Player * p, double tX, double tY)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mAcceleration = makePosition(tX, tY, 0);
}

void setProjectileVelocityMultipliers(Player * p, double tX, double tY)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mVelocityMultipliers = makePosition(tX, tY, 1);
}

void setProjectileHitAmountBeforeVanishing(Player * p, int tHitAmount)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mHitAmountBeforeVanishing = tHitAmount;
}

void setProjectilMisstime(Player * p, int tMissTime)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mMissTime = tMissTime;
}

void setProjectilePriority(Player * p, int tPriority)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mPriority = tPriority;
}

void setProjectileSpritePriority(Player * p, int tSpritePriority)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mSpritePriority = tSpritePriority;
}

void setProjectileEdgeBound(Player * p, int tEdgeBound)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mEdgeBound = tEdgeBound;
}

void setProjectileStageBound(Player * p, int tStageBound)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mStageBound = tStageBound;
}

void setProjectileHeightBoundValues(Player * p, int tLowerBound, int tUpperBound)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mLowerBound = tLowerBound;
	e->mUpperBound = tUpperBound;
}

void setProjectilePosition(Player * p, Position tPosition)
{
	setPlayerPosition(p, tPosition, getPlayerCoordinateP(p));
}

void setProjectileShadow(Player * p, int tShadow)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mShadow = tShadow;
}

void setProjectileSuperMoveTime(Player * p, int tSuperMoveTime)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mSuperMoveTime = tSuperMoveTime;
}

void setProjectilePauseMoveTime(Player * p, int tPauseMoveTime)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mPauseMoveTime = tPauseMoveTime;
}

void setProjectileHasOwnPalette(Player * p, int tValue)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mHasOwnPalette = tValue;
}

void setProjectileRemapPalette(Player * p, int tGroup, int tItem)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mRemapPaletteGroup = tGroup;
	e->mRemapPaletteItem = tItem;
}

void setProjectileAfterImageTime(Player * p, int tAfterImageTime)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mAfterImageTime = tAfterImageTime;
}

void setProjectileAfterImageLength(Player * p, int tAfterImageLength)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mAfterImageLength = tAfterImageLength;
}

void setProjectileAfterImage(Player * p, int tAfterImage)
{
	assert(int_map_contains(&gData.mProjectileList, p->mProjectileDataID));
	Projectile* e = int_map_get(&gData.mProjectileList, p->mProjectileDataID);
	e->mAfterImage = tAfterImage;
}
