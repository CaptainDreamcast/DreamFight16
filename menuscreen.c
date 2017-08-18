#include "menuscreen.h"

#include <tari/optionhandler.h>
#include <tari/screeneffect.h>
#include <tari/input.h>

#include "fightscreen.h"
#include "titlescreen.h"

static void fadeOutFinished(void* tData) {
	Screen* screen = tData;
	setNewScreen(screen);
}

static void fadeOut(Screen* tScreen) {
	addFadeOut(30, fadeOutFinished, tScreen);
}

static void selectFightOption(void* tData) {
	(void)tData;
	fadeOut(&FightScreen);
}

static void loadMenuScreen() {
	instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionButtonStart();
	setOptionTextSize(20);
	setOptionTextBreakSize(-5);
	
	addOption(makePosition(50, 100, 1), "Super Story Mode", selectFightOption, NULL);
	addOption(makePosition(50, 150, 1), "Arcade Mode", selectFightOption, NULL);
	addOption(makePosition(50, 200, 1), "2 Player Mode", selectFightOption, NULL);

	addFadeIn(30, NULL, NULL);
}

static void updateMenuScreen() {
	if (hasPressedBFlank()) {
		fadeOut(&TitleScreen);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}
}

Screen MenuScreen = {
	.mLoad = loadMenuScreen,
	.mUpdate = updateMenuScreen,
};
