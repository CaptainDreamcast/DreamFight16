#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

#include "playerdefinition.h"

extern ActorBlueprint PauseHandler;

void setSuperPausePlayer(Player* tPlayer);
void setSuperPauseTime(int tTime);
void setSuperPauseBufferTimeForCommandsDuringPauseEnd(int tBufferTime);
void setSuperPauseMoveTime(int tMoveTime);
void setSuperPauseIsPausingBG(int tIsPausingBG);
void setSuperPauseAnimation(int tIsInPlayerFile, int tAnimationNumber);
void setSuperPauseSound(int tIsInPlayerFile, int tSoundGroup, int tSoundItem);
void setSuperPausePosition(Position tPosition);
void setSuperPauseDarkening(int tIsDarkening);
void setSuperPausePlayer2DefenseMultiplier(double tMultiplier);
void setSuperPausePowerToAdd(int tPowerToAdd);
void setSuperPausePlayerUnhittability(int tIsUnhittable);
void setSuperPauseActive();