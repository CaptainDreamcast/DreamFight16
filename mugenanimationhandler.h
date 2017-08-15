#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

#include "mugenanimationreader.h"
#include "mugenspritefilereader.h"
#include "playerdefinition.h"

int addMugenAnimation(Player* p, MugenAnimation * tStartAnimation, MugenSpriteFile* tSprites, Position* tBasePosition, int tPositionCoordinateSystemInP, int tScaleCoordinateSystemInP);
void removeRegisteredAnimation(int tID);

int getRegisteredAnimationAnimationNumber(int tID);
int getRegisteredAnimationRemainingAnimationTime(int tID);
void setRegisteredAnimationFaceDirection(int tID, FaceDirection tDirection);
void setRegisteredAnimationRectangleWidth(int tID, int tWidth);
void setRegisteredAnimationCameraPositionReference(int tID, Position* tCameraPosition);
void setRegisteredAnimationInvisibleFlag(int tID);
void setRegisteredAnimationOneFrameDrawScale(int tID, Vector3D tScale);
void setRegisteredAnimationOneFrameDrawAngle(int tID, double tAngle);
void addRegisteredAnimationOneFrameDrawAngle(int tID, double tAngle);
void setRegisteredAnimationOneFrameFixedDrawAngle(int tID, double tAngle);

void changeMugenAnimationWithStartStep(int tID, MugenAnimation* tNewAnimation, int tStartStep);

int isStartingHandledAnimationElementWithID(int tID, int tStepID);
int getTimeFromHandledAnimationElement(int tID, int tStep);
int getHandledAnimationElementFromTimeOffset(int tID, int tTime);

void setRegisteredAnimationSpritePriority(int tID, int tPriority);
void setRegisteredAnimationToUseFixedZ(int tID);
void pauseRegisteredAnimation(int tID);
void unpauseRegisteredAnimation(int tID);

void advanceRegisteredAnimationOneTick(int tID);

extern ActorBlueprint MugenAnimationHandler;