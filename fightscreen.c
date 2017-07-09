#include "fightscreen.h"

#include <stdio.h>

#include <tari/input.h>
#include <tari/stagehandler.h>

#include "stage.h"
#include "mugenanimationreader.h"

static Screen* getNextFightScreenScreen() {
	
	if (hasPressedAbortFlank()) {
		abortScreenHandling();
	}

	if (hasPressedA()) {
		return &FightScreen;
	}

	if (hasPressedLeft()) {
		scrollBackgroundRight(-1);
	} else if (hasPressedRight()) {
		scrollBackgroundRight(1);
	}
	
	if (hasPressedUp()) {
		scrollBackgroundDown(-1);
	}
	else if (hasPressedDown()) {
		scrollBackgroundDown(1);
	}
	return NULL;
}

static void loadFightScreen() {
	loadStageFromMugenDefinition("assets/XX'PYRAMID'SUNSET'XX.def");
	loadMugenAnimationFile("assets/kfm.air");
}

Screen FightScreen = {
	.mLoad = loadFightScreen,
	.mGetNextScreen = getNextFightScreenScreen,
};