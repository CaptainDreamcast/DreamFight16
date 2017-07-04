#include "fightscreen.h"

#include <stdio.h>

#include <tari/input.h>

static Screen* getNextFightScreenScreen() {
	
	if (hasPressedAbortFlank()) {
		abortScreenHandling();
	}

	return NULL;
}

Screen FightScreen = {
	.mGetNextScreen = getNextFightScreenScreen,
};