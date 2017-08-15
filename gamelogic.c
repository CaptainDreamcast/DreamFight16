#include "gamelogic.h"

#include <tari/screeneffect.h>

#include "playerdefinition.h"
#include "fightui.h"

static struct {
	int mGameTime;
	int mRoundNumber;
	int mRoundStateNumber;

	int mIsDisplayingIntro;
} gData;

static void fightAnimationFinishedCB() {
	gData.mRoundStateNumber = 2;
	
}

static void roundAnimationFinishedCB() {
	playFightAnimation(fightAnimationFinishedCB);
}

static void introFinished() {
	gData.mIsDisplayingIntro = 0;
	playRoundAnimation(gData.mRoundNumber, roundAnimationFinishedCB);
}

static void fadeInFinished(void* tData) {
	(void)tData;

	
	setPlayerControl(getRootPlayer(0), 1);
	setPlayerControl(getRootPlayer(1), 1);
	fightAnimationFinishedCB();
	return;
	

	gData.mRoundStateNumber = 1;
	gData.mIsDisplayingIntro = 2;
	changePlayerState(getRootPlayer(0), 5900);
	changePlayerState(getRootPlayer(1), 5900);
}

static void loadGameLogic(void* tData) {
	(void)tData;
	gData.mGameTime = 0;
	gData.mRoundNumber = 1;
	gData.mRoundStateNumber = 0;
	gData.mIsDisplayingIntro = 0;

	changePlayerState(getRootPlayer(0), 0);
	changePlayerState(getRootPlayer(1), 0);
	setPlayerControl(getRootPlayer(0), 0);
	setPlayerControl(getRootPlayer(1), 0);
	addFadeIn(30, fadeInFinished, NULL);
}

static void updateIntro() {
	if (!gData.mIsDisplayingIntro) return;

	if (gData.mIsDisplayingIntro == 2) { // TODO: fix call stuff
		gData.mIsDisplayingIntro--;
		return;
	}
	
	int isAFinished = !isPlayerInIntro(getRootPlayer(0));
	int isBFinished = !isPlayerInIntro(getRootPlayer(1));

	if (isAFinished && isBFinished) {

		introFinished();
	}
}

static void updateGameLogic(void* tData) {
	(void)tData;
	gData.mGameTime++;

	updateIntro();
}

ActorBlueprint GameLogic = {
	.mLoad = loadGameLogic,
	.mUpdate = updateGameLogic,
};


int getGameTime()
{
	return gData.mGameTime;
}

int getRoundNumber()
{
	return gData.mRoundNumber; 
}

int getRoundStateNumber()
{
	return gData.mRoundStateNumber;
}

int getMatchNumber()
{
	return 1; // TODO
}

int isMatchOver()
{
	return 0; // TODO
}

