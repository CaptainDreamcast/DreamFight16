#pragma once

#include <tari/actorhandler.h>
#include <tari/animation.h>

#include "mugenspritefilereader.h"
#include "mugenanimationreader.h"

void setMugenStageHandlerCameraCoordinates(Vector3DI tCoordinates);
void setMugenStageHandlerCameraRange(GeoRectangle tRect);
void setMugenStageHandlerCameraPosition(Position p);
void addMugenStageHandlerCameraPositionX(double tX);
void setMugenStageHandlerCameraPositionX(double tX);

void setMugenStageHandlerScreenShake(int tTime, double tFrequency, int tAmplitude, double tPhase);

void addMugenStageHandlerAnimatedBackgroundElement(Position tStart, int tAnimationID, MugenAnimations* tAnimations, MugenSpriteFile* tSprites, Position tDelta, Vector3DI tTile, Vector3DI tTileSpacing, Vector3DI tCoordinates);
void addMugenStageHandlerStaticBackgroundElement(Position tStart, int tSpriteGroup, int tSpriteItem, MugenSpriteFile* tSprites, Position tDelta, Vector3DI tTile, Vector3DI tTileSpacing, Vector3DI tCoordinates);
Position* getMugenStageHandlerCameraPositionReference();

extern ActorBlueprint MugenStageHandler;
