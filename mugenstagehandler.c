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

static void updateSingleStaticStageElement(StaticStageHandlerElement* e) {
	double scale = e->mCoordinates.y / (double)gData.mCameraCoordinates.y;
	Vector3D deltaInCameraSpace = vecSub(vecScale(gData.mCameraPosition, -1), vecScale(e->mStart, 1 / scale));
	deltaInCameraSpace = vecScale3D(deltaInCameraSpace, e->mDelta);
	deltaInCameraSpace.z = 0;

	Vector3D deltaInElementSpace = vecScale(deltaInCameraSpace, 1 / scale); // TODO: fix
	e->mReferencePosition = vecAdd(e->mStart, deltaInElementSpace);
	e->mReferencePosition = vecSub(e->mReferencePosition, getStageCoordinateSystemOffset(e->mCoordinates.y)); // TODO: properly
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
	(void)tTime, tFrequency, tAmplitude, tPhase;
	// TODO;
}

void addMugenStageHandlerStaticBackgroundElement(Position tStart, int tSpriteGroup, int tSpriteItem, MugenSpriteFile* tSprites, Position tDelta)
{
	StaticStageHandlerElement* e = allocMemory(sizeof(StaticStageHandlerElement));
	e->mStart = tStart;
	e->mDelta = tDelta;
	e->mCoordinates = makeVector3DI(640, 480, 0);

	e->mAnimation = createOneFrameMugenAnimationForSprite(tSpriteGroup, tSpriteItem);
	e->mAnimationID = addMugenAnimation(NULL, e->mAnimation, tSprites, &e->mReferencePosition, e->mCoordinates.y, e->mCoordinates.y);
	setRegisteredAnimationToUseFixedZ(e->mAnimationID);

	updateSingleStaticStageElement(e);

	vector_push_back_owned(&gData.mStaticElements, e);
}

Position * getMugenStageHandlerCameraPositionReference()
{
	return &gData.mCameraPosition;
}
