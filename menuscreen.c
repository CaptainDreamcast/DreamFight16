#include "menuscreen.h"

#include <tari/optionhandler.h>
#include <tari/screeneffect.h>
#include <tari/input.h>

#include "fightscreen.h"
#include "titlescreen.h"
#include "twoplayerselectscreen.h"
#include "storyscreen.h"

static void fadeOutFinished(void* tData) {
	Screen* screen = tData;
	setNewScreen(screen);
}

static void fadeOut(Screen* tScreen) {
	addFadeOut(30, fadeOutFinished, tScreen);
}

static void selectStoryOption(void* tData) {
	(void)tData;
	fadeOut(&StoryScreen);
}

static void selectTwoPlayerOption(void* tData) {
	(void)tData;
	fadeOut(&TwoPlayerSelectScreen);
}

static void loadMenuScreen() {
	instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionButtonStart();
	setOptionTextSize(20);
	setOptionTextBreakSize(-5);
	
	addOption(makePosition(70, 100, 1), "Super Story Mode", selectStoryOption, NULL);
	addOption(makePosition(70, 150, 1), "2 Player Mode", selectTwoPlayerOption, NULL);

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
