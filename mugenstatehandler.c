#include "mugenstatehandler.h"

#include <assert.h>

#include <tari/datastructures.h>

#include "playerdefinition.h"
#include "pausecontrollers.h"
#include "mugenassignmentevaluator.h"
#include "mugenstatecontrollers.h"
#include "playerhitdata.h"

typedef struct {
	MugenStates* mStates;
	int mIsUsingTemporaryOtherStateMachine;
	MugenStates* mTemporaryStates;

	int mPreviousState;
	int mState;
	int mTimeInState;
	Player* mPlayer;

	int mIsPaused;
	int mIsInHelperMode;
	int mIsInputControlDisabled;
	int mIsDisabled;
} RegisteredState;

static struct {
	int mPauseHandler;
	IntMap mRegisteredStates;

} gData;

static void loadStateHandler(void* tData) {
	(void)tData;

	gData.mPauseHandler = instantiateActor(PauseHandler);
	
	gData.mRegisteredStates = new_int_map();
}

typedef struct {
	RegisteredState* mRegisteredState;
	MugenState* mState;

	int mHasChangedState;
} MugenStateControllerCaller;

static int evaluateTrigger(MugenStateControllerTrigger* tTrigger, Player* tPlayer) {
	return evaluateAssignment(tTrigger->mAssignment, tPlayer);
}

static void updateSingleController(void* tCaller, void* tData) {
	MugenStateControllerCaller* caller = tCaller;
	MugenStateController* controller = tData;
	
	if (caller->mHasChangedState) return;
	if (!evaluateTrigger(&controller->mTrigger, caller->mRegisteredState->mPlayer)) return;

	caller->mHasChangedState = handleMugenStateControllerAndReturnWhetherStateChanged(controller, caller->mRegisteredState->mPlayer);
}

static MugenStates* getCurrentStateMachineStates(RegisteredState* tRegisteredState) {
	if (tRegisteredState->mIsUsingTemporaryOtherStateMachine) {
		return tRegisteredState->mTemporaryStates;
	}
	else {
		return tRegisteredState->mStates;
	}
}

static void updateSingleState(RegisteredState* tRegisteredState, int tState, MugenStates* tStates) {

	int isEvaluating = 1;
	while (isEvaluating) {
		// printf("%d %d eval: %d\n", tRegisteredState->mPlayer->mRootID, tRegisteredState->mPlayer->mID, tState);
		if (!int_map_contains(&tStates->mStates, tState)) return;
		MugenState* state = int_map_get(&tStates->mStates, tState);
		MugenStateControllerCaller caller;
		caller.mRegisteredState = tRegisteredState;
		caller.mState = state;
		caller.mHasChangedState = 0;
		vector_map(&state->mControllers, updateSingleController, &caller);
		
		if (!caller.mHasChangedState) break;
		else {
			if (tState < 0 || tState == tRegisteredState->mState) break;
			tState = tRegisteredState->mState;
		}
	}
}

static void updateSingleStateMachineByReference(RegisteredState* tRegisteredState) {
	if (tRegisteredState->mIsPaused) return;

	MugenStates* ownStates = tRegisteredState->mStates;
	MugenStates* activeStates = getCurrentStateMachineStates(tRegisteredState);

	tRegisteredState->mTimeInState++;
	if (!tRegisteredState->mIsInHelperMode) {
		if (!tRegisteredState->mIsUsingTemporaryOtherStateMachine) {
			updateSingleState(tRegisteredState, -3, ownStates);
		}
		updateSingleState(tRegisteredState, -2, ownStates);
	}
	if (!tRegisteredState->mIsInputControlDisabled) {
		updateSingleState(tRegisteredState, -1, ownStates);
	}
	updateSingleState(tRegisteredState, tRegisteredState->mState, activeStates);
}

static void updateSingleStateMachine(void* tCaller, void* tData) {
	(void)tCaller;
	RegisteredState* registeredState = tData;
	updateSingleStateMachineByReference(registeredState);
}

static void updateStateHandler(void* tData) {
	(void)tData;
	int_map_map(&gData.mRegisteredStates, updateSingleStateMachine, NULL);
}

ActorBlueprint MugenStateHandler = {
	.mLoad = loadStateHandler,
	.mUpdate = updateStateHandler,
};

int registerMugenStateMachine(MugenStates * tStates, Player* tPlayer)
{
	RegisteredState* e = allocMemory(sizeof(RegisteredState));
	e->mStates = tStates;
	e->mIsUsingTemporaryOtherStateMachine = 0;
	e->mPreviousState = 0;
	e->mState = 0;
	e->mTimeInState = 0;
	e->mPlayer = tPlayer;
	e->mIsPaused = 0;
	e->mIsInHelperMode = 0;
	e->mIsInputControlDisabled = 0;
	e->mIsDisabled = 0;

	return int_map_push_back_owned(&gData.mRegisteredStates, e);
}

void removeRegisteredStateMachine(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	int_map_remove(&gData.mRegisteredStates, tID);
}

int getRegisteredStateState(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);

	return e->mState;
}

int getRegisteredStatePreviousState(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);

	return e->mPreviousState;
}

void pauseRegisteredStateMachine(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	e->mIsPaused = 1;
}

void unpauseRegisteredStateMachine(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	if (e->mIsDisabled) return;
	e->mIsPaused = 0;
}

void disableRegisteredStateMachine(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	e->mIsDisabled = 1;
	pauseRegisteredStateMachine(tID);
}

int getRegisteredStateTimeInState(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);

	// printf("%d %d time in state %d\n", e->mPlayer->mRootID, e->mPlayer->mID, e->mTimeInState);
	return e->mTimeInState;
}

void setRegisteredStateTimeInState(int tID, int tTime)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);

	e->mTimeInState = tTime;
}

void setRegisteredStateToHelperMode(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	e->mIsInHelperMode = 1;
}

void setRegisteredStateDisableCommandState(int tID)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	e->mIsInputControlDisabled = 1;
}

int hasHandledStateMachineState(int tID, int tNewState)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	
	MugenStates* states = getCurrentStateMachineStates(e);
	return int_map_contains(&states->mStates, tNewState);
}

int hasHandledStateMachineStateSelf(int tID, int tNewState)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	return int_map_contains(&e->mStates->mStates, tNewState);
}

void changeHandledStateMachineState(int tID, int tNewState)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	resetPlayerMoveContactCounter(e->mPlayer);
	e->mTimeInState = 0;

	e->mPreviousState = e->mState;
	e->mState = tNewState;

	printf("Changing %d %d from state %d to %d\n", e->mPlayer->mRootID, e->mPlayer->mID, e->mPreviousState, e->mState);

	MugenStates* states = getCurrentStateMachineStates(e);
	assert(int_map_contains(&states->mStates, e->mState));
	MugenState* newState = int_map_get(&states->mStates, e->mState);

	setPlayerStateType(e->mPlayer, newState->mType);
	setPlayerStateMoveType(e->mPlayer, newState->mMoveType);
	setPlayerPhysics(e->mPlayer, newState->mPhysics);

	if (!newState->mDoMoveHitInfosPersist) {
		setPlayerMoveHitReset(e->mPlayer);
	}

	if (!newState->mDoesHitCountPersist) {
		resetPlayerHitCount(e->mPlayer);
	}

	if (!newState->mDoHitDefinitionsPersist) {
		setHitDataInactive(e->mPlayer);
	}

	if (newState->mIsChangingAnimation) {
		int anim = evaluateAssignmentAndReturnAsInteger(newState->mAnimation, e->mPlayer);
		changePlayerAnimation(e->mPlayer, anim);
	}

	if (newState->mIsChangingControl) {
		int control = evaluateAssignmentAndReturnAsInteger(newState->mControl, e->mPlayer);
		setPlayerControl(e->mPlayer, control);
	}

	if (newState->mIsSettingVelocity) {
		Vector3D vel = evaluateAssignmentAndReturnAsVector3D(newState->mVelocity, e->mPlayer);
		setPlayerVelocityX(e->mPlayer, vel.x, getPlayerCoordinateP(e->mPlayer));
		setPlayerVelocityY(e->mPlayer, vel.y, getPlayerCoordinateP(e->mPlayer));
	}

	if (newState->mIsAddingPower) {
		int power = evaluateAssignmentAndReturnAsInteger(newState->mPowerAdd, e->mPlayer);
		addPlayerPower(e->mPlayer, power);
	}

}

void changeHandledStateMachineStateToOtherPlayerStateMachine(int tID, int tTemporaryID, int tNewState)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);

	assert(int_map_contains(&gData.mRegisteredStates, tTemporaryID));
	RegisteredState* borrowFromState = int_map_get(&gData.mRegisteredStates, tTemporaryID);

	e->mIsUsingTemporaryOtherStateMachine = 1;
	e->mTemporaryStates = borrowFromState->mStates;
	changeHandledStateMachineState(tID, tNewState);
}

void changeHandledStateMachineStateToOwnStateMachine(int tID, int tNewState)
{
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	e->mIsUsingTemporaryOtherStateMachine = 0;

	changeHandledStateMachineState(tID, tNewState);
}

void updateSingleStateMachineByID(int tID) {
	assert(int_map_contains(&gData.mRegisteredStates, tID));
	RegisteredState* e = int_map_get(&gData.mRegisteredStates, tID);
	updateSingleStateMachineByReference(e);
}
