#pragma once

#include <tari/actorhandler.h>
#include <tari/wrapper.h>

extern ActorBlueprint GameLogic;

int getGameTime();

int getRoundNumber();
int getRoundStateNumber();
int getMatchNumber();

int isMatchOver();
void setRoundNotOverFlag();

void setScreenAfterFightScreen(Screen* tScreen);
void setGameModeStory();
void setGameModeTwoPlayer();