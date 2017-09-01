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
#include "warningscreen.h"


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

	int mSpeakerID;
	int mTextID;

	int mIsStoryOver;
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
		removeRegisteredAnimation(gData.mOldAnimationID);
		destroyMugenAnimation(gData.mOldAnimation);
	}

	if (gData.mAnimationID != -1) {
		setRegisteredAnimationBasePosition(gData.mAnimationID, &gData.mOldAnimationBasePosition);
	}

	gData.mOldAnimationID = gData.mAnimationID;
	gData.mOldAnimation = gData.mAnimation;
	

	int group = getMugenDefNumberVariableAsGroup(gData.mCurrentGroup, "group");
	int item =  getMugenDefNumberVariableAsGroup(gData.mCurrentGroup, "item");
	gData.mAnimation = createOneFrameMugenAnimationForSprite(group, item);

	gData.mAnimationID = addMugenAnimation(NULL, gData.mAnimation, &gData.mSprites, &gData.mAnimationBasePosition, 480, 480);
	setRegisteredAnimationToNotUseStage(gData.mAnimationID);
	setRegisteredAnimationToUseFixedZ(gData.mAnimationID);

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
		removeHandledText(gData.mSpeakerID);
	}

	char* speaker = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "speaker");
	char* text = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "text");

	gData.mSpeakerID = addHandledText(makePosition(40, 340, 3), speaker, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, 0, 0), makePosition(INF, INF, 1), INF);

	int dur = strlen(text);
	gData.mTextID = addHandledTextWithBuildup(makePosition(50, 360, 3), text, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, 0, 0), makePosition(540, 480, 1), INF, dur);
	
	freeMemory(speaker);
	freeMemory(text);

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
	char* player = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "player");
	char* enemy = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "enemy");

	setPlayerDefinitionPath(0, player);
	setPlayerDefinitionPath(1, enemy);
	setPlayerHuman(0);
	setPlayerArtificial(1);

	freeMemory(player);
	freeMemory(enemy);

	setScreenAfterFightScreen(&StoryScreen);
	setGameModeStory();

	gData.mIsStoryOver = 1;
	addFadeOut(30, goToFight, NULL);
}

static int isEndingGroup() {
	char* name = gData.mCurrentGroup->mName;
	char firstW[100];
	sscanf(name, "%s", firstW);

	return !strcmp("Ending", firstW);
}

static void goToStoryOver(void* tCaller) {
	(void)tCaller;
	setNewScreen(&WarningScreen);
}

static void loadEndingGroup() {
	gData.mIsStoryOver = 1;
	addFadeOut(30, goToStoryOver, NULL);
}

static void loadNextStoryGroup() {
	int isRunning = 1;
	while (isRunning) {
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
		else if (isEndingGroup()) {
			loadEndingGroup();
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
		gData.mCurrentGroup = gData.mCurrentGroup->mNext;
	}

	assert(gData.mCurrentGroup);
	gData.mCurrentGroup = gData.mCurrentGroup->mNext;
	assert(gData.mCurrentGroup);

	gData.mAnimationID = -1;
	gData.mOldAnimationID = -1;
	gData.mTextID = -1;

	gData.mOldAnimationBasePosition = makePosition(0, 0, 1);
	gData.mAnimationBasePosition = makePosition(0, 0, 2);

	loadNextStoryGroup();
}



static void loadStoryScreen() {
	startNextStoryPart();
	gData.mIsStoryOver = 0;
	
	instantiateActor(MugenAnimationHandler);

	char* defPath = getCurrentStoryDefinitionFile();
	gData.mScript = loadMugenDefScript(defPath);

	char* spritePath = getAllocatedMugenDefStringVariable(&gData.mScript, "Header", "sprites");
	gData.mSprites = loadMugenSpriteFileWithoutPalette(spritePath);
	freeMemory(spritePath);

	findStartOfStoryBoard();
}


static void updateText() {
	if (gData.mIsStoryOver) return;
	if (gData.mTextID == -1) return;

	if (hasPressedAFlank() || hasPressedStart()) {
		if (isHandledTextBuiltUp(gData.mTextID)) {
			loadNextStoryGroup();
		}
		else {
			setHandledTextBuiltUp(gData.mTextID);
		}
	}
}

static void updateStoryScreen() {

	updateText();

	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}

}

Screen StoryScreen = {
	.mLoad = loadStoryScreen,
	.mUpdate = updateStoryScreen,
};