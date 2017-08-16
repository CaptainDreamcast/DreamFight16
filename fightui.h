#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

#include "mugenanimationreader.h"
#include "mugenspritefilereader.h"
#include "playerdefinition.h"

void playHitSpark(Position tPosition, Player* tPlayer, int tIsInPlayerFile, int tNumber, int tIsFacingRight, int tPositionCoordinateP, int tScaleCoordinateP);
void setLifeBarPercentage(Player* tPlayer, double tPercentage);
void setPowerBarPercentage(Player* tPlayer, double tPercentage);

MugenAnimation* getFightEffectAnimation(int tNumber);
MugenSpriteFile* getFightEffectSprites();

void playRoundAnimation(int tRound, void(*tFunc)());
void playFightAnimation(void(*tFunc)());
void playKOAnimation(void(*tFunc)());
void playWinAnimation(char* tName, void(*tFunc)());

void setBarInvisibleForOneFrame();

extern ActorBlueprint FightUIBP;