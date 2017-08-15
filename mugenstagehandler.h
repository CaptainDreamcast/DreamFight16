#pragma once

#include <tari/actorhandler.h>
#include <tari/animation.h>

#include "mugenspritefilereader.h"

void setMugenStageHandlerCameraCoordinates(Vector3DI tCoordinates);
void setMugenStageHandlerCameraRange(GeoRectangle tRect);
void setMugenStageHandlerCameraPosition(Position p);
void scrollMugenStageHandlerLeft();
void scrollMugenStageHandlerRight();
void scrollMugenStageHandlerUp();
void scrollMugenStageHandlerDown();

void setMugenStageHandlerScreenShake(int tTime, double tFrequency, int tAmplitude, double tPhase);

void addMugenStageHandlerStaticBackgroundElement(Position tStart, int tSpriteGroup, int tSpriteItem, MugenSpriteFile* tSprites, Position tDelta);
Position* getMugenStageHandlerCameraPositionReference();

extern ActorBlueprint MugenStageHandler;
