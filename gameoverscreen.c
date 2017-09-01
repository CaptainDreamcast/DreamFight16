#include "gameoverscreen.h"

#include <tari/screeneffect.h>
#include <tari/animation.h>
#include <tari/input.h>

#include "titlescreen.h"

static struct {
	TextureData mTexture;
	int mAnimationID;

} gData;

static void loadGameOverScreen() {
	gData.mTexture = loadTexture("assets/gameover/BG.pkg");
	gData.mAnimationID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mTexture);
	addFadeIn(30, NULL, NULL);
}

static void goToTitleScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&TitleScreen);
}

static void updateGameOverScreen() {
	if (hasPressedStartFlank()) {
		addFadeOut(30, goToTitleScreen, NULL);
	}

	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}
}

Screen GameOverScreen = {
	.mLoad = loadGameOverScreen,
	.mUpdate = updateGameOverScreen,
};
