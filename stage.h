#pragma once

#include <tari/geometry.h>
#include <tari/datastructures.h>
#include <tari/actorhandler.h>

#include "mugenspritefilereader.h"

// documentation at http://www.elecbyte.com/mugendocs-11b1/bgs.html


void setStageMugenDefinition(char* tPath);
extern ActorBlueprint StageBP;

double parseCoordinatesToLocalCoordinateSystem(double tCoordinate, int tOtherCoordinateSystemAsP);

Position getPlayerStartingPosition(int i, int tCoordinateP);
Position getStageCoordinateSystemOffset(int tCoordinateP);
int doesPlayerStartFacingLeft(int i);

double getCameraPositionX(int tCoordinateP);
double getCameraPositionY(int tCoordinateP);
double getScreenFactorFromCoordinateP(int tCoordinateP);
int getStageCoordinateP();

double transformCoordinates(double tVal, int tSrcP, int tDstP);
Vector3D transformCoordinatesVector(Vector3D tVal, int tSrcP, int tDstP);

double getStageTopOfScreenBasedOnPlayer(int tCoordinateP);
double getStageLeftOfScreenBasedOnPlayer(int tCoordinateP);
double getStageRightOfScreenBasedOnPlayer(int tCoordinateP);
Position getStageCenterOfScreenBasedOnPlayer(int tCoordinateP);

int getGameWidth(int tCoordinateP);