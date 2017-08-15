#pragma once

#include <tari/actorhandler.h>

extern ActorBlueprint GameLogic;

int getGameTime();

int getRoundNumber();
int getRoundStateNumber();
int getMatchNumber();

int isMatchOver();