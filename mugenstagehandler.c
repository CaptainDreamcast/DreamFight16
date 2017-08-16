#include "mugenstagehandler.h"

#include <assert.h>

#include <tari/datastructures.h>
#include <tari/math.h>

#include "mugenanimationhandler.h"
#include "stage.h"

typedef struct {
	Position mReferencePosition;

	Position mStart;
	Position mDelta;
	Vector3DI mCoordinates;

	MugenAnimation* mAnimation;
	int mAnimationID;
} StaticStageHandlerElement;

static struct {
	Vector mStaticElements;

	Position mCameraPosition;
	Position mCameraTargetPosition;

	GeoRectangle mCameraRange;
	Vector3DI mCameraCoordinates;

	Vector3D mCameraSpeed;
} gData;



static void loadMugenStageHandler(void* tData) {
	(void)tData;
	gData.mCameraPosition = makePosition(0,0,0);
	gData.mCameraTargetPosition = makePosition(0, 0, 0);
	gData.mCameraRange = makeGeoRectangle(-INF, -INF, 2*INF, 2*INF);
	gData.mCameraSpeed = makePosition(3, 3, 0);

	gData.mStaticElements = new_vector();

}

static int getCameraCoordP() {
	return gData.mCameraCoordinates.y;
}

static void updateSingleStaticStageElement(StaticStageHandlerElement* e) {
	double scale = e->mCoordinates.y / (double)gData.mCameraCoordinates.y;

	Vector3D deltaInCameraSpace = makePosition(getCameraPositionX(getCameraCoordP()), getCameraPositionY(getCameraCoordP()), 0);
	deltaInCameraSpace = vecScale3D(deltaInCameraSpace, e->mDelta);
	deltaInCameraSpace.z = 0;

	Vector3D deltaInElementSpace = vecScale(deltaInCameraSpace, scale); // TODO: fix
	e->mReferencePosition = vecAdd(e->mStart, deltaInElementSpace);
	e->mReferencePosition = vecSub(e->mReferencePosition, vecScale(getStageCoordinateSystemOffset(getCameraCoordP()), 1)); // TODO: properly
	e->mReferencePosition = vecSub(e->mReferencePosition, makePosition(-e->mCoordinates.x / 2, 0, 0));
}

static void updateSingleStaticStageElementCB(void* tCaller, void* tData) {
	(void)tCaller;
	StaticStageHandlerElement* e = tData;

	updateSingleStaticStageElement(e);
}

static void updateCamera() {
	Vector3D delta = vecSub(gData.mCameraTargetPosition, gData.mCameraPosition);
	delta.z = 0;
	if (vecLength(delta) < 1e-6) return;
	
	gData.mCameraPosition = vecAdd(gData.mCameraPosition, vecScale(delta, 0.95));
}

static void updateMugenStageHandler(void* tData) {
	(void)tData;

	updateCamera();

	vector_map(&gData.mStaticElements, updateSingleStaticStageElementCB, NULL);
}

ActorBlueprint MugenStageHandler = {
	.mLoad = loadMugenStageHandler,
	.mUpdate = updateMugenStageHandler,
};


void setMugenStageHandlerCameraCoordinates(Vector3DI tCoordinates)
{
	gData.mCameraCoordinates = tCoordinates;
}

void setMugenStageHandlerCameraRange(GeoRectangle tRect)
{
	gData.mCameraRange = tRect;
}

void setMugenStageHandlerCameraPosition(Position p)
{
	gData.mCameraTargetPosition = p;
}

void scrollMugenStageHandlerLeft()
{
	gData.mCameraTargetPosition = vecAdd(gData.mCameraTargetPosition, makePosition(-gData.mCameraSpeed.x, 0, 0));
	gData.mCameraTargetPosition = clampPositionToGeoRectangle(gData.mCameraTargetPosition, gData.mCameraRange);
}

void scrollMugenStageHandlerRight()
{
	gData.mCameraTargetPosition = vecAdd(gData.mCameraTargetPosition, makePosition(gData.mCameraSpeed.x, 0, 0));
	gData.mCameraTargetPosition = clampPositionToGeoRectangle(gData.mCameraTargetPosition, gData.mCameraRange);
}

void scrollMugenStageHandlerUp()
{
	gData.mCameraTargetPosition = vecAdd(gData.mCameraTargetPosition, makePosition(0, -gData.mCameraSpeed.y, 0));
	gData.mCameraTargetPosition = clampPositionToGeoRectangle(gData.mCameraTargetPosition, gData.mCameraRange);
}

void scrollMugenStageHandlerDown()
{
	gData.mCameraTargetPosition = vecAdd(gData.mCameraTargetPosition, makePosition(0, gData.mCameraSpeed.y, 0));
	gData.mCameraTargetPosition = clampPositionToGeoRectangle(gData.mCameraTargetPosition, gData.mCameraRange);
}

void setMugenStageHandlerScreenShake(int tTime, double tFrequency, int tAmplitude, double tPhase)
{
	(void)tTime;
	(void)tFrequency;
	(void)tAmplitude;
	(void)tPhase;
	// TODO;
}

static void addMugenStageHandlerBackgroundElement(Position tStart, MugenAnimation* tAnimation, MugenSpriteFile * tSprites, Position tDelta) {
	StaticStageHandlerElement* e = allocMemory(sizeof(StaticStageHandlerElement));
	e->mStart = tStart;
	e->mDelta = tDelta;
	e->mCoordinates = makeVector3DI(320, 240, 0);

	e->mAnimation = tAnimation;
	e->mAnimationID = addMugenAnimation(NULL, e->mAnimation, tSprites, &e->mReferencePosition, e->mCoordinates.y, e->mCoordinates.y);
	setRegisteredAnimationToUseFixedZ(e->mAnimationID);

	updateSingleStaticStageElement(e);

	vector_push_back_owned(&gData.mStaticElements, e);
}

void addMugenStageHandlerAnimatedBackgroundElement(Position tStart, int tAnimationID, MugenAnimations* tAnimations, MugenSpriteFile * tSprites, Position tDelta)
{
	addMugenStageHandlerBackgroundElement(tStart, getMugenAnimation(tAnimations, tAnimationID), tSprites, tDelta);
}

void addMugenStageHandlerStaticBackgroundElement(Position tStart, int tSpriteGroup, int tSpriteItem, MugenSpriteFile* tSprites, Position tDelta)
{
	addMugenStageHandlerBackgroundElement(tStart, createOneFrameMugenAnimationForSprite(tSpriteGroup, tSpriteItem), tSprites, tDelta);	
}

Position * getMugenStageHandlerCameraPositionReference()
{
	return &gData.mCameraPosition;
}
