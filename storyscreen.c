#include "storyscreen.h"

#include <assert.h>

#include <tari/log.h>
#include <tari/system.h>
#include <tari/animation.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/input.h>
#include <tari/screeneffect.h>

#include "mugendefreader.h"
#include "story.h"
#include "mugenspritefilereader.h"
#include "mugenanimationreader.h"
#include "mugenanimationhandler.h"
#include "titlescreen.h"
#include "playerdefinition.h"
#include "fightscreen.h"
#include "gamelogic.h"


static struct {
	MugenDefScript mScript;
	MugenDefScriptGroup* mCurrentGroup;
	MugenSpriteFile mSprites;

	MugenAnimation* mOldAnimation;
	MugenAnimation* mAnimation;
	int mAnimationID;
	int mOldAnimationID;

	Position mOldAnimationBasePosition;
	Position mAnimationBasePosition;

	int mTextID;

	int mNow;
} gData;

static int isImageGroup() {
	char* name = gData.mCurrentGroup->mName;
	char firstW[100];
	sscanf(name, "%s", firstW);

	return !strcmp("Image", firstW);
}

static void increaseGroup() {
	gData.mCurrentGroup = gData.mCurrentGroup->mNext;
}

static void loadImageGroup() {
	if (gData.mOldAnimationID != -1) {
		removeHandledAnimation(gData.mOldAnimationID);
		destroyMugenAnimation(gData.mOldAnimation);
	}

	gData.mOldAnimationID = gData.mAnimationID;
	gData.mOldAnimation = gData.mAnimation;

	printf("acc\n");
	int group = getMugenDefNumberVariableAsGroup(gData.mCurrentGroup, "group");
	int item =  getMugenDefNumberVariableAsGroup(gData.mCurrentGroup, "item");
	printf("make anim");
	gData.mAnimation = createOneFrameMugenAnimationForSprite(group, item);

	printf("addd\n");
	gData.mAnimationID = addMugenAnimation(NULL, gData.mAnimation, &gData.mSprites, &gData.mAnimationBasePosition, 480, 480);
	setRegisteredAnimationToNotUseStage(gData.mAnimationID);

	increaseGroup();
}

static int isTextGroup() {
	char* name = gData.mCurrentGroup->mName;
	char firstW[100];
	sscanf(name, "%s", firstW);

	return !strcmp("Text", firstW);
}

static void loadTextGroup() {
	if (gData.mTextID != -1) {
		removeHandledText(gData.mTextID);
	}

	gData.mTextID = addHandledTextWithBuildup(makePosition(50, 300, 3), "sad", 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(540, 480, 1), INF, 120);

	increaseGroup();
}

static int isFightGroup() {
	char* name = gData.mCurrentGroup->mName;
	char firstW[100];
	sscanf(name, "%s", firstW);

	return !strcmp("Fight", firstW);
}

static void goToFight(void* tCaller) {
	(void)tCaller;
	setNewScreen(&FightScreen);
}

static void loadFightGroup() {
	setPlayerDefinitionPath(0, "assets/kfm/kfm.def");
	setPlayerDefinitionPath(1, "assets/kfm/kfm.def");

	startNextStoryPart();
	setScreenAfterFightScreen(&StoryScreen);
	setGameModeStory();

	addFadeOut(30, goToFight, NULL);
}

static void loadNextStoryGroup() {
	int isRunning = 1;
	while (isRunning) {
		printf("load group %s\n", gData.mCurrentGroup->mName);
		if (isImageGroup()) {
			loadImageGroup();
		}
		else if (isTextGroup()) {
			loadTextGroup();
			break;
		}
		else if (isFightGroup()) {
			loadFightGroup();
			break;
		}
		else {
			logError("Unidentified group type.");
			logErrorString(gData.mCurrentGroup->mName);
			abortSystem();
		}
	}
}

static void findStartOfStoryBoard() {
	gData.mCurrentGroup = gData.mScript.mFirstGroup;

	while (gData.mCurrentGroup && strcmp("STORYSTART", gData.mCurrentGroup->mName)) {
		printf("search group %s\n", gData.mCurrentGroup->mName);
		gData.mCurrentGroup = gData.mCurrentGroup->mNext;
	}

	assert(gData.mCurrentGroup);
	gData.mCurrentGroup = gData.mCurrentGroup->mNext;
	assert(gData.mCurrentGroup);

	gData.mAnimationID = -1;
	gData.mOldAnimationID = -1;
	gData.mTextID = -1;
	gData.mNow = 0;
	
	gData.mOldAnimationBasePosition = makePosition(0, 0, 1);
	gData.mAnimationBasePosition = makePosition(0, 0, 2);

	loadNextStoryGroup();
}



static void loadStoryScreen() {
	instantiateActor(MugenAnimationHandler);

	char* defPath = getCurrentStoryDefinitionFile();
	gData.mScript = loadMugenDefScript(defPath);

	char* spritePath = getAllocatedMugenDefStringVariable(&gData.mScript, "Header", "sprites");
	gData.mSprites = loadMugenSpriteFileWithoutPalette(spritePath);
	freeMemory(spritePath);

	findStartOfStoryBoard();
}



static void updateStoryScreen() {

	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}

}

Screen StoryScreen = {
	.mLoad = loadStoryScreen,
	.mUpdate = updateStoryScreen,
};