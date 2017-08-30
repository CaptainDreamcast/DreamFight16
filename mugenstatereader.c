#include "mugenstatereader.h"

#include <assert.h>

#include <tari/log.h>
#include <tari/system.h>
#include <tari/memoryhandler.h>

#include "mugendefreader.h"
#include "mugenstatecontrollers.h"

static int isMugenStateDef(char* tName) {
	char firstW[100];
	sscanf(tName, "%s", firstW);
	turnStringLowercase(firstW);
	return !strcmp("statedef", firstW);
}

static MugenDefScriptGroup* getFirstStateDefGroup(MugenDefScript* tScript) {
	MugenDefScriptGroup* cur = tScript->mFirstGroup;

	while (cur != NULL) {
		if (isMugenStateDef(cur->mName)) return cur;
		cur = cur->mNext;
	}

	logError("Unable to find state definition.");
	abortSystem();

	return NULL;
}

static struct {
	int mCurrentGroup;

} gMugenStateDefParseState;

static void handleMugenStateDefType(MugenState* tState, MugenDefScriptGroupElement* tElement) {
	char* type = getAllocatedMugenDefStringVariableAsElement(tElement);
	turnStringLowercase(type);

	if (!strcmp("s", type)) {
		tState->mType = MUGEN_STATE_TYPE_STANDING;
	} else if (!strcmp("c", type)) {
		tState->mType = MUGEN_STATE_TYPE_CROUCHING;
	} else if (!strcmp("a", type)) {
		tState->mType = MUGEN_STATE_TYPE_AIR;
	} else if (!strcmp("l", type)) {
		tState->mType = MUGEN_STATE_TYPE_LYING;
	}
	else if (!strcmp("u", type)) {
		tState->mType = MUGEN_STATE_TYPE_UNCHANGED;
	}
	else {
		logError("Unable to determine Mugen state type.");
		logErrorString(type);
		abortSystem();
	}

	freeMemory(type);
}

static void handleMugenStateDefMoveType(MugenState* tState, MugenDefScriptGroupElement* tElement) {
	char* moveType = getAllocatedMugenDefStringVariableAsElement(tElement);
	turnStringLowercase(moveType);

	if (!strcmp("a", moveType)) {
		tState->mMoveType = MUGEN_STATE_MOVE_TYPE_ATTACK;
	} else if (!strcmp("i", moveType)) {
		tState->mMoveType = MUGEN_STATE_MOVE_TYPE_IDLE;
	}
	else if (!strcmp("h", moveType)) {
		tState->mMoveType = MUGEN_STATE_MOVE_TYPE_BEING_HIT;
	}
	else if (!strcmp("u", moveType)) {
		tState->mMoveType = MUGEN_STATE_MOVE_TYPE_UNCHANGED;
	}
	else {
		logError("Unable to determine Mugen state move type.");
		logErrorString(moveType);
		abortSystem();
	}

	freeMemory(moveType);
}

static void handleMugenStateDefPhysics(MugenState* tState, MugenDefScriptGroupElement* tElement) {
	char* physics = getAllocatedMugenDefStringVariableAsElement(tElement);
	turnStringLowercase(physics);

	if (!strcmp("u", physics)) {
		tState->mPhysics = MUGEN_STATE_PHYSICS_UNCHANGED;
	} else if (!strcmp("s", physics)) {
		tState->mPhysics = MUGEN_STATE_PHYSICS_STANDING;
	}
	else if (!strcmp("c", physics)) {
		tState->mPhysics = MUGEN_STATE_PHYSICS_CROUCHING;
	}
	else if (!strcmp("a", physics)) {
		tState->mPhysics = MUGEN_STATE_PHYSICS_AIR;
	}
	else if (!strcmp("n", physics)) {
		tState->mPhysics = MUGEN_STATE_PHYSICS_NONE;
	}
	else {
		logError("Unable to determine Mugen state physics.");
		logErrorString(physics);
		abortSystem();
	}

	freeMemory(physics);
}


static void handleMugenStateDefAnimation(MugenState* tState, MugenDefScriptGroupElement* tElement, MugenDefScriptGroup* tGroup) {
	tState->mIsChangingAnimation = 1;
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tElement->mName, tGroup, &tState->mAnimation));
} 

static void handleMugenStateDefVelocitySetting(MugenState* tState, MugenDefScriptGroupElement* tElement, MugenDefScriptGroup* tGroup) {
	tState->mIsSettingVelocity = 1;
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tElement->mName, tGroup, &tState->mVelocity));
}

static void handleMugenStateDefControl(MugenState* tState, MugenDefScriptGroupElement* tElement, MugenDefScriptGroup* tGroup) {
	tState->mIsChangingControl = 1;
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tElement->mName, tGroup, &tState->mControl));
}

static void handleMugenStateSpritePriority(MugenState* tState, MugenDefScriptGroupElement* tElement, MugenDefScriptGroup* tGroup) {
	tState->mIsChangingSpritePriority = 1;
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tElement->mName, tGroup, &tState->mSpritePriority));
}

static void handleMugenStatePowerAdd(MugenState* tState, MugenDefScriptGroupElement* tElement, MugenDefScriptGroup* tGroup) {
	tState->mIsAddingPower = 1;
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tElement->mName, tGroup, &tState->mPowerAdd));
}

static void handleMugenStateJuggle(MugenState* tState, MugenDefScriptGroupElement* tElement, MugenDefScriptGroup* tGroup) {
	tState->mDoesRequireJuggle = 1;
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tElement->mName, tGroup, &tState->mJuggleRequired));
}

static void handleMugenStateHitDefPersistence(MugenState* tState, MugenDefScriptGroupElement* tElement) {
	tState->mDoHitDefinitionsPersist = getMugenDefNumberVariableAsElement(tElement);
}

static void handleMugenStateMoveHitPersistence(MugenState* tState, MugenDefScriptGroupElement* tElement) {
	tState->mDoMoveHitInfosPersist = getMugenDefNumberVariableAsElement(tElement);
}

static void handleMugenStateHitCountPersistence(MugenState* tState, MugenDefScriptGroupElement* tElement) {
	tState->mDoesHitCountPersist = getMugenDefNumberVariableAsElement(tElement);
}

static void handleMugenStateFacePlayer2(MugenState* tState, MugenDefScriptGroupElement* tElement, MugenDefScriptGroup* tGroup) {
	tState->mHasFacePlayer2Info = 1;
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tElement->mName, tGroup, &tState->mDoesFacePlayer2));
}

static void handleMugenStatePriority(MugenState* tState, MugenDefScriptGroupElement* tElement, MugenDefScriptGroup* tGroup) {
	tState->mHasPriority = 1;
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tElement->mName, tGroup, &tState->mPriority));
}

typedef struct {
	MugenState* mState;
	MugenDefScriptGroup* mGroup;
} MugenStateDefCaller;


static void handleSingleMugenStateDefElement(void* tCaller, char* tKey, void* tData) {
	(void)tKey;
	MugenStateDefCaller* caller = tCaller;
	MugenState* state = caller->mState;
	MugenDefScriptGroup* group = caller->mGroup;

	MugenDefScriptGroupElement* e = tData;

	if (!strcmp("type", e->mName)) {
		handleMugenStateDefType(state, e);
	} else if (!strcmp("movetype", e->mName)) {
		handleMugenStateDefMoveType(state, e);
	}
	else if (!strcmp("physics", e->mName)) {
		handleMugenStateDefPhysics(state, e);
	}
	else if (!strcmp("anim", e->mName)) {
		handleMugenStateDefAnimation(state, e, group);
	}
	else if (!strcmp("velset", e->mName)) {
		handleMugenStateDefVelocitySetting(state, e, group);
	}
	else if (!strcmp("ctrl", e->mName)) {
		handleMugenStateDefControl(state, e, group);
	}
	else if (!strcmp("sprpriority", e->mName)) {
		handleMugenStateSpritePriority(state, e, group);
	}
	else if (!strcmp("poweradd", e->mName)) {
		handleMugenStatePowerAdd(state, e, group);
	}
	else if (!strcmp("juggle", e->mName)) {
		handleMugenStateJuggle(state, e, group);
	}
	else if (!strcmp("hitdefpersist", e->mName)) {
		handleMugenStateHitDefPersistence(state, e);
	}
	else if (!strcmp("movehitpersist", e->mName)) {
		handleMugenStateMoveHitPersistence(state, e);
	}
	else if (!strcmp("hitcountpersist", e->mName)) {
		handleMugenStateHitCountPersistence(state, e);
	}
	else if (!strcmp("facep2", e->mName)) {
		handleMugenStateFacePlayer2(state, e, group);
	}
	else if (!strcmp("priority", e->mName)) {
		handleMugenStatePriority(state, e, group);
	}
	else {
		logError("Unable to determine state def element.");
		logErrorString(e->mName);
		abortSystem();
	}
}

static void removeState(MugenStates* tStates, int tState) {
	// TODO: free rest
	int_map_remove(&tStates->mStates, tState);
}

static void handleMugenStateDef(MugenStates* tStates, MugenDefScriptGroup* tGroup) {

	MugenState* state = allocMemory(sizeof(MugenState));

	char dummy[100];
	sscanf(tGroup->mName, "%s %d", dummy, &state->mID);
	gMugenStateDefParseState.mCurrentGroup = state->mID;

	state->mType = MUGEN_STATE_TYPE_STANDING;
	state->mMoveType = MUGEN_STATE_MOVE_TYPE_IDLE;
	state->mPhysics = MUGEN_STATE_PHYSICS_NONE;
	state->mIsChangingAnimation = 0;
	state->mIsSettingVelocity = 0;
	state->mIsChangingControl = 0;
	state->mIsChangingSpritePriority = 0;
	state->mIsAddingPower = 0;
	state->mDoesRequireJuggle = 0;
	state->mDoHitDefinitionsPersist = 0;
	state->mDoMoveHitInfosPersist = 0;
	state->mDoesHitCountPersist = 0;
	state->mHasFacePlayer2Info = 0;
	state->mHasPriority = 0;

	state->mControllers = new_vector();

	MugenStateDefCaller caller;
	caller.mState = state;
	caller.mGroup = tGroup;
	string_map_map(&tGroup->mElements, handleSingleMugenStateDefElement, &caller);

	if (int_map_contains(&tStates->mStates, state->mID)) {
		removeState(tStates, state->mID);
	}
	int_map_push_owned(&tStates->mStates, state->mID, state);
}

static int isMugenStateController(char* tName) {
	char firstW[100];
	sscanf(tName, "%s", firstW);
	return !strcmp("State", firstW) || !strcmp("state", firstW);
}

static void handleMugenStateControllerInDefGroup(MugenStates* tStates, MugenDefScriptGroup* tGroup) {
	
	assert(int_map_contains(&tStates->mStates, gMugenStateDefParseState.mCurrentGroup));
	MugenState* state = int_map_get(&tStates->mStates, gMugenStateDefParseState.mCurrentGroup);

	MugenStateController* controller = parseMugenStateControllerFromGroup(tGroup);

	vector_push_back_owned(&state->mControllers, controller);
}

static void handleSingleMugenStateDefGroup(MugenStates* tStates, MugenDefScriptGroup* tGroup) {


	if (isMugenStateDef(tGroup->mName)) {
		handleMugenStateDef(tStates, tGroup);
	} else if (isMugenStateController(tGroup->mName)) {
		handleMugenStateControllerInDefGroup(tStates, tGroup);
	}
	else {
		logError("Unable to determine state def group type.");
		logErrorString(tGroup->mName);
		abortSystem();
	}

}

static void loadMugenStateDefinitionsFromScript(MugenStates* tStates, MugenDefScript* tScript) {
	MugenDefScriptGroup* current = getFirstStateDefGroup(tScript);

	while (current != NULL) {
		handleSingleMugenStateDefGroup(tStates, current);
		
		current = current->mNext;
	}
}

void loadMugenStateDefinitionsFromFile(MugenStates* tStates, char* tPath) {
	MugenDefScript script = loadMugenDefScript(tPath);
	loadMugenStateDefinitionsFromScript(tStates, &script);
	unloadMugenDefScript(script);
}

static MugenConstants makeEmptyMugenConstants() {
	MugenConstants ret;
	ret.mStates.mStates = new_int_map();
	return ret;
}



static void loadMugenConstantsSizeData(MugenConstantsSizeData* tSizeData, MugenDefScript* tScript) {
	tSizeData->mScale = makePosition(1, 1, 1);
	loadFloatOrDefault(&tSizeData->mScale.x, tScript, "Size", "xscale", 1);
	loadFloatOrDefault(&tSizeData->mScale.y, tScript, "Size", "yscale", 1);
	loadIntegerOrDefault(&tSizeData->mGroundBackWidth, tScript, "Size", "ground.back", 10);
	loadIntegerOrDefault(&tSizeData->mGroundFrontWidth, tScript, "Size", "ground.front", 10);
	loadIntegerOrDefault(&tSizeData->mAirBackWidth, tScript, "Size", "air.back", 10);
	loadIntegerOrDefault(&tSizeData->mAirFrontWidth, tScript, "Size", "air.front", 10);
	loadIntegerOrDefault(&tSizeData->mHeight, tScript, "Size", "height", 50);
	loadIntegerOrDefault(&tSizeData->mAttackDistance, tScript, "Size", "attack.dist", 100);
	loadIntegerOrDefault(&tSizeData->mProjectileAttackDistance, tScript, "Size", "proj.attack.dist", 100);
	loadIntegerOrDefault(&tSizeData->mDoesScaleProjectiles, tScript, "Size", "proj.doscale", 0);
	loadVectorOrDefault(&tSizeData->mHeadPosition, tScript, "Size", "head.pos", makePosition(0, 0, 0));
	loadVectorOrDefault(&tSizeData->mMidPosition, tScript, "Size", "mid.pos", makePosition(0, 0, 0));
	loadIntegerOrDefault(&tSizeData->mShadowOffset, tScript, "Size", "shadowoffset", 0);
	loadVectorIOrDefault(&tSizeData->mDrawOffset, tScript, "Size", "draw.offset", makeVector3DI(0, 0, 0));
}

static void loadSingleSparkNumber(int* oIsSparkInPlayerFile, int* oSparkNumber, MugenDefScript* tScript, char* tGroup, char* tName, int tDefaultNumber) {
	if (!isMugenDefStringVariable(tScript, tGroup, tName)) {
		*oIsSparkInPlayerFile = 0;
		*oSparkNumber = tDefaultNumber;
		return;
	}

	char* item = getAllocatedMugenDefStringVariable(tScript, tGroup, tName);
	turnStringLowercase(item);

	if (item[0] == 's') {
		*oIsSparkInPlayerFile = 1;
		*oSparkNumber = atoi(item + 1);
	}
	else {
		*oIsSparkInPlayerFile = 0;
		*oSparkNumber = atoi(item);
	}
}

static void loadMugenConstantsHeader(MugenConstantsHeader* tHeader, MugenDefScript* tScript) {
	loadIntegerOrDefault(&tHeader->mLife, tScript, "Data", "life", 1000);
	loadIntegerOrDefault(&tHeader->mPower, tScript, "Data", "power", 3000);
	loadIntegerOrDefault(&tHeader->mAttack, tScript, "Data", "attack", 100);
	loadIntegerOrDefault(&tHeader->mDefense, tScript, "Data", "defence", 100);
	loadIntegerOrDefault(&tHeader->mFallDefenseUp, tScript, "Data", "fall.defence_up", 50);
	loadFloatOrDefault(&tHeader->mLiedownTime, tScript, "Data", "liedown.time", 60);
	loadIntegerOrDefault(&tHeader->mAirJugglePoints, tScript, "Data", "airjuggle", 15);

	loadSingleSparkNumber(&tHeader->mIsSparkNoInPlayerFile, &tHeader->mSparkNo, tScript, "Data", "sparkno", 2);
	loadSingleSparkNumber(&tHeader->mIsGuardSparkNoInPlayerFile, &tHeader->mGuardSparkNo, tScript, "Data", "guard.sparkno", 40);
	loadIntegerOrDefault(&tHeader->mKOEcho, tScript, "Data", "KO.echo", 0);
	loadIntegerOrDefault(&tHeader->mVolume, tScript, "Data", "volume", 0);

	loadIntegerOrDefault(&tHeader->mIntPersistIndex, tScript, "Data", "IntPersistIndex", 60);
	loadIntegerOrDefault(&tHeader->mFloatPersistIndex, tScript, "Data", "FloatPersistIndex", 40);
}

static void loadMugenConstantsVelocityData(MugenConstantsVelocityData* tVelocityData, MugenDefScript* tScript) {
	loadVectorOrDefault(&tVelocityData->mWalkForward, tScript, "Velocity", "walk.fwd", makePosition(1, 0, 0));
	loadVectorOrDefault(&tVelocityData->mWalkBackward, tScript, "Velocity", "walk.back", makePosition(-1, 0, 0));

	loadVectorOrDefault(&tVelocityData->mRunForward, tScript, "Velocity", "run.fwd", makePosition(1, 0, 0));
	loadVectorOrDefault(&tVelocityData->mRunBackward, tScript, "Velocity", "run.back", makePosition(-1, 0, 0));

	loadVectorOrDefault(&tVelocityData->mJumpNeutral, tScript, "Velocity", "jump.neu", makePosition(0, 1, 0));
	loadVectorOrDefault(&tVelocityData->mJumpBackward, tScript, "Velocity", "jump.back", makePosition(-1, 1, 0));
	loadVectorOrDefault(&tVelocityData->mJumpForward, tScript, "Velocity", "jump.fwd", makePosition(1, 1, 0));

	loadVectorOrDefault(&tVelocityData->mRunJumpBackward, tScript, "Velocity", "runjump.back", makePosition(-1, 1, 0));
	loadVectorOrDefault(&tVelocityData->mRunJumpForward, tScript, "Velocity", "runjump.fwd", makePosition(1, 1, 0));

	loadVectorOrDefault(&tVelocityData->mAirJumpNeutral, tScript, "Velocity", "airjump.neu", makePosition(0, 1, 0));
	loadVectorOrDefault(&tVelocityData->mAirJumpBackward, tScript, "Velocity", "airjump.back", makePosition(-1, 1, 0));
	loadVectorOrDefault(&tVelocityData->mAirJumpForward, tScript, "Velocity", "airjump.fwd", makePosition(1, 1, 0));

	loadVectorOrDefault(&tVelocityData->mAirGetHitGroundRecovery, tScript, "Velocity", "air.gethit.groundrecover", makePosition(0, 0, 0));
	loadVectorOrDefault(&tVelocityData->mAirGetHitAirRecoveryMultiplier, tScript, "Velocity", "air.gethit.airrecover.mul", makePosition(1, 1, 1));
	loadVectorOrDefault(&tVelocityData->mAirGetHitAirRecoveryOffset, tScript, "Velocity", "air.gethit.airrecover.add", makePosition(0, 0, 0));

	loadFloatOrDefault(&tVelocityData->mAirGetHitExtraXWhenHoldingBackward, tScript, "Velocity", "air.gethit.airrecover.back", 0);
	loadFloatOrDefault(&tVelocityData->mAirGetHitExtraXWhenHoldingForward, tScript, "Velocity", "air.gethit.airrecover.fwd", 0);
	loadFloatOrDefault(&tVelocityData->mAirGetHitExtraYWhenHoldingUp, tScript, "Velocity", "air.gethit.airrecover.up", 0);
	loadFloatOrDefault(&tVelocityData->mAirGetHitExtraYWhenHoldingDown, tScript, "Velocity", "air.gethit.airrecover.down", 0);
}

static void loadMugenConstantsMovementData(MugenConstantsMovementData* tMovementData, MugenDefScript* tScript) {
	loadIntegerOrDefault(&tMovementData->mAllowedAirJumpAmount, tScript, "Movement", "airjump.num", 0);
	loadIntegerOrDefault(&tMovementData->mAirJumpMinimumHeight, tScript, "Movement", "airjump.height", 10);

	loadFloatOrDefault(&tMovementData->mVerticalAcceleration, tScript, "Movement", "yaccel", 1);
	loadFloatOrDefault(&tMovementData->mStandFiction, tScript, "Movement", "stand.friction", 0.2);
	loadFloatOrDefault(&tMovementData->mCrouchFriction, tScript, "Movement", "crouch.friction", 0.2);
	loadFloatOrDefault(&tMovementData->mStandFrictionThreshold, tScript, "Movement", "stand.friction.threshold", 0.1);
	loadFloatOrDefault(&tMovementData->mCrouchFrictionThreshold, tScript, "Movement", "crouch.friction.threshold", 0.1);

	loadIntegerOrDefault(&tMovementData->mAirGetHitGroundLevelY, tScript, "Movement", "air.gethit.groundlevel", 10);
	loadIntegerOrDefault(&tMovementData->mAirGetHitGroundRecoveryGroundYTheshold, tScript, "Movement", "air.gethit.groundrecover.ground.threshold", 10);
	loadIntegerOrDefault(&tMovementData->mAirGetHitGroundRecoveryGroundGoundLevelY, tScript, "Movement", "air.gethit.groundrecover.groundlevel", 10);
	loadFloatOrDefault(&tMovementData->mAirGetHitAirRecoveryVelocityYThreshold, tScript, "Movement", "air.gethit.airrecover.threshold", -1);
	loadFloatOrDefault(&tMovementData->mAirGetHitAirRecoveryVerticalAcceleration, tScript, "Movement", "air.gethit.airrecover.yaccel", 0.2);
	loadIntegerOrDefault(&tMovementData->mAirGetHitTripGroundLevelY, tScript, "Movement", "air.gethit.trip.groundlevel", 10);

	loadVectorOrDefault(&tMovementData->mBounceOffset, tScript, "Movement", "down.bounce.offset", makePosition(0, 0, 0));
	loadFloatOrDefault(&tMovementData->mVerticalBounceAcceleration, tScript, "Movement", "down.bounce.yaccel", 0);
	loadIntegerOrDefault(&tMovementData->mBounceGroundLevel, tScript, "Movement", "down.bounce.groundlevel", 10);
	loadFloatOrDefault(&tMovementData->mLyingDownFrictionThreshold, tScript, "Movement", "down.friction.threshold", 0.05);	
}

static void loadMugenConstantsFromScript(MugenConstants* tConstants, MugenDefScript* tScript) {
	loadMugenConstantsHeader(&tConstants->mHeader, tScript);
	loadMugenConstantsSizeData(&tConstants->mSizeData, tScript);
	loadMugenConstantsVelocityData(&tConstants->mVelocityData, tScript);
	loadMugenConstantsMovementData(&tConstants->mMovementData, tScript);
}

MugenConstants loadMugenConstantsFile(char * tPath)
{
	MugenDefScript script = loadMugenDefScript(tPath);
	MugenConstants ret = makeEmptyMugenConstants();
	loadMugenConstantsFromScript(&ret, &script);
	unloadMugenDefScript(script);
	return ret;
}
