#include "gamelogic.h"

#include <tari/screeneffect.h>

#include "playerdefinition.h"
#include "fightui.h"

static struct {
	int mGameTime;
	int mRoundNumber;
	int mRoundStateNumber;

	int mIsDisplayingIntro;
	int mIsDisplayingWinPose;

	int mRoundNotOverFlag;

	Player* mRoundWinner;
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

static void startRound() {
	gData.mRoundStateNumber = 0;
	gData.mIsDisplayingIntro = 0;
	gData.mIsDisplayingWinPose = 0;
	changePlayerState(getRootPlayer(0), 0);
	changePlayerState(getRootPlayer(1), 0);
	setPlayerControl(getRootPlayer(0), 0);
	setPlayerControl(getRootPlayer(1), 0);
	addFadeIn(30, fadeInFinished, NULL);
}

static void loadGameLogic(void* tData) {
	(void)tData;
	gData.mGameTime = 0;
	gData.mRoundNumber = 1;
	startRound();
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

static void winAnimationFinishedCB() {
	printf("over\n");
}

static void startWinPose() {
	if (hasPlayerStateSelf(gData.mRoundWinner, 180)) {
		changePlayerState(gData.mRoundWinner, 180);

	}

	gData.mRoundStateNumber = 4;
	gData.mIsDisplayingWinPose = 1;
}

static void koAnimationFinishedCB() {
	startWinPose();
}

static void startKO(int i) {
	gData.mRoundWinner = getRootPlayer(i ^ 1);
	gData.mRoundStateNumber = 3;
	setPlayerControl(getRootPlayer(0), 0);
	setPlayerControl(getRootPlayer(1), 0);
	playKOAnimation(koAnimationFinishedCB);
}

static void updateWinCondition() {
	if (gData.mRoundStateNumber != 2) return;

	int i;
	for (i = 0; i < 2; i++) {

		if (!getPlayerLife(getRootPlayer(i))) {
			startKO(i);
			break;
		}
	}
}

static void updateNoControl() {
	if (gData.mRoundStateNumber < 3) return;
	setPlayerControl(getRootPlayer(0), 0);
	setPlayerControl(getRootPlayer(1), 0);
}

static void resetRound(void* tCaller) {
	(void)tCaller;
	enableDrawing();
	increasePlayerRoundsExisted();
	gData.mRoundNumber++;
	resetPlayers();
	startRound();
}

static void updateWinPose() {
	if (!gData.mIsDisplayingWinPose) return;

	if (!getRemainingPlayerAnimationTime(gData.mRoundWinner)) {
		increasePlayerRoundsWon(gData.mRoundWinner);
		if (hasPlayerWon(gData.mRoundWinner)) {
			playWinAnimation(getPlayerDisplayName(gData.mRoundWinner), winAnimationFinishedCB);
		}
		else {
			addFadeOut(30, resetRound, NULL);
		}
		gData.mIsDisplayingWinPose = 0;
	}

}

static void updateGameLogic(void* tData) {
	(void)tData;
	gData.mGameTime++;

	updateIntro();
	updateWinCondition();
	updateNoControl();
	updateWinPose();
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

void setRoundNotOverFlag()
{
	gData.mRoundNotOverFlag = 1; // TODO: use
}

