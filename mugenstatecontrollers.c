#include "mugenstatecontrollers.h"

#include <assert.h>
#include <string.h>

#include <tari/memoryhandler.h>
#include <tari/log.h>
#include <tari/system.h>

#include "playerdefinition.h"
#include "playerhitdata.h"
#include "mugenexplod.h"
#include "stage.h"
#include "mugenstagehandler.h"
#include "config.h"
#include "pausecontrollers.h"
#include "mugenassignmentevaluator.h"
#include "fightui.h"

typedef struct {

	char mName[100];
	MugenAssignment** mTriggerRoot;
} TriggerParseCaller;

static void checkSingleElementForTrigger(void* tCaller, void* tData) {
	TriggerParseCaller* caller = tCaller;
	MugenDefScriptGroupElement* e = tData;

	if (strcmp(e->mName, caller->mName)) return;

	char* text = getAllocatedMugenDefStringVariableAsElement(e);

	MugenAssignment* trigger = parseMugenAssignmentFromString(text);
	freeMemory(text);

	*caller->mTriggerRoot = makeAndMugenAssignment(*caller->mTriggerRoot, trigger);
}

static int parseTriggerAndReturnIfFound(char* tName, MugenAssignment** tRoot, MugenDefScriptGroup* tGroup) {
	if (!string_map_contains(&tGroup->mElements, tName)) return 0;

	MugenAssignment* triggerRoot = makeTrueMugenAssignment();
	TriggerParseCaller caller;
	strcpy(caller.mName, tName);
	caller.mTriggerRoot = &triggerRoot;
	list_map(&tGroup->mOrderedElementList, checkSingleElementForTrigger, &caller);

	*tRoot = makeOrMugenAssignment(*tRoot, triggerRoot);

	return 1;
}

int parseNumberedTriggerAndReturnIfFound(int i, MugenAssignment** tRoot, MugenDefScriptGroup* tGroup) {
	char name[100];
	sprintf(name, "trigger%d", i);

	return parseTriggerAndReturnIfFound(name, tRoot, tGroup);
}

void parseStateControllerTriggers(MugenStateController* tController, MugenDefScriptGroup* tGroup) {


	MugenAssignment* allRoot = makeFalseMugenAssignment();
	int hasAllTrigger = parseTriggerAndReturnIfFound("triggerall", &allRoot, tGroup);
	if (!hasAllTrigger) {
		destroyFalseMugenAssignment(allRoot);
		allRoot = makeTrueMugenAssignment();
	}

	MugenAssignment* root = makeFalseMugenAssignment();
	int i = 1;
	while (parseNumberedTriggerAndReturnIfFound(i, &root, tGroup)) i++;

	root = makeAndMugenAssignment(allRoot, root);
	tController->mTrigger.mAssignment = root;
}

typedef struct {
	int mIsSettingX;
	MugenAssignment* x;
	int mIsSettingY;
	MugenAssignment* y;
} Set2DPhysicsController;


static void parse2DPhysicsController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	Set2DPhysicsController* e = allocMemory(sizeof(Set2DPhysicsController));
	e->mIsSettingX = fetchAssignmentFromGroupAndReturnWhetherItExists("x", tGroup, &e->x);
	e->mIsSettingY = fetchAssignmentFromGroupAndReturnWhetherItExists("y", tGroup, &e->y);
	tController->mData = e;
}

typedef struct {
	MugenAssignment* mState;

	int mIsChangingControl;
	MugenAssignment* mControl;
} ChangeStateController;


static void parseChangeStateController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	ChangeStateController* e = allocMemory(sizeof(ChangeStateController));
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mState));

	e->mIsChangingControl = fetchAssignmentFromGroupAndReturnWhetherItExists("ctrl", tGroup, &e->mControl);

	tController->mData = e;
}

static void fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString(char* tName, MugenDefScriptGroup* tGroup, MugenAssignment** tDst, char* tDefault) {
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists(tName, tGroup, tDst)) {
		*tDst = makeStringMugenAssignment(tDefault);
	}
}

typedef struct {
	MugenAssignment* mState;

	int mIsChangingControl;
	MugenAssignment* mControl;

	MugenAssignment* mID;
} TargetChangeStateController;

static void parseTargetChangeStateController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	TargetChangeStateController* e = allocMemory(sizeof(TargetChangeStateController));
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mState));
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("id", tGroup, &e->mID, "");

	e->mIsChangingControl = fetchAssignmentFromGroupAndReturnWhetherItExists("ctrl", tGroup, &e->mControl);

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mAttribute;
	MugenAssignment* mHitFlag;

	MugenAssignment* mGuardFlag;
	MugenAssignment* mAffectTeam;
	MugenAssignment* mAnimationType;
	MugenAssignment* mAirAnimationType;
	MugenAssignment* mFallAnimationType;

	MugenAssignment* mPriority;
	MugenAssignment* mDamage;
	MugenAssignment* mPauseTime;
	MugenAssignment* mGuardPauseTime;
	MugenAssignment* mSparkNumber;
	MugenAssignment* mGuardSparkNumber;

	MugenAssignment* mSparkXY;
	MugenAssignment* mHitSound;
	MugenAssignment* mGuardSound;
	MugenAssignment* mGroundType;
	MugenAssignment* mAirType;

	MugenAssignment* mGroundSlideTime;
	MugenAssignment* mGuardSlideTime;
	MugenAssignment* mGroundHitTime;
	MugenAssignment* mGuardHitTime;
	MugenAssignment* mAirHitTime;

	MugenAssignment* mGuardControlTime;
	MugenAssignment* mGuardDistance;
	MugenAssignment* mYAccel;
	MugenAssignment* mGroundVelocity;
	MugenAssignment* mGuardVelocity;
	MugenAssignment* mAirVelocity;
	MugenAssignment* mAirGuardVelocity;

	MugenAssignment* mGroundCornerPushVelocityOffset;
	MugenAssignment* mAirCornerPushVelocityOffset;
	MugenAssignment* mDownCornerPushVelocityOffset;
	MugenAssignment* mGuardCornerPushVelocityOffset;
	MugenAssignment* mAirGuardCornerPushVelocityOffset;

	MugenAssignment* mAirGuardControlTime;
	MugenAssignment* mAirJuggle;
	MugenAssignment* mMinimumDistance;
	MugenAssignment* mMaximumDistance;
	MugenAssignment* mSnap;

	MugenAssignment* mPlayerSpritePriority1;
	MugenAssignment* mPlayerSpritePriority2;

	MugenAssignment* mPlayer1ChangeFaceDirection;
	MugenAssignment* mPlayer1ChangeFaceDirectionRelativeToPlayer2;
	MugenAssignment* mPlayer2ChangeFaceDirectionRelativeToPlayer1;

	MugenAssignment* mPlayer1StateNumber;
	MugenAssignment* mPlayer2StateNumber;
	MugenAssignment* mPlayer2CapableOfGettingPlayer1State;
	MugenAssignment* mForceStanding;

	MugenAssignment* mFall;
	MugenAssignment* mFallXVelocity;
	MugenAssignment* mFallYVelocity;
	MugenAssignment* mFallCanBeRecovered;
	MugenAssignment* mFallRecoveryTime;
	MugenAssignment* mFallDamage;

	MugenAssignment* mAirFall;
	MugenAssignment* mForceNoFall;
	MugenAssignment* mDownVelocity;
	MugenAssignment* mDownHitTime;
	MugenAssignment* mDownBounce;

	MugenAssignment* mHitID;
	MugenAssignment* mChainID;
	MugenAssignment* mNoChainID;
	MugenAssignment* mHitOnce;

	MugenAssignment* mKill;
	MugenAssignment* mGuardKill;
	MugenAssignment* mFallKill;
	MugenAssignment* mNumberOfHits;

	MugenAssignment* mGetPower;
	MugenAssignment* mGivePower;

	MugenAssignment* mPaletteEffectTime;
	MugenAssignment* mPaletteEffectMultiplication;
	MugenAssignment* mPaletteEffectAddition;

	MugenAssignment* mEnvironmentShakeTime;
	MugenAssignment* mEnvironmentShakeFrequency;
	MugenAssignment* mEnvironmentShakeAmplitude;
	MugenAssignment* mEnvironmentShakePhase;

	MugenAssignment* mFallEnvironmentShakeTime;
	MugenAssignment* mFallEnvironmentShakeFrequency;
	MugenAssignment* mFallEnvironmentShakeAmplitude;
	MugenAssignment* mFallEnvironmentShakePhase;
} HitDefinitionController;


static void parseHitDefinitionController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	HitDefinitionController* e = allocMemory(sizeof(HitDefinitionController));
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("attr", tGroup, &e->mAttribute));

	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("hitflag", tGroup, &e->mHitFlag)) {
		e->mHitFlag = makeStringMugenAssignment("MAF");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("guardflag", tGroup, &e->mGuardFlag)) {
		e->mGuardFlag = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("affectteam", tGroup, &e->mAffectTeam)) {
		e->mAffectTeam = makeStringMugenAssignment("E");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("animtype", tGroup, &e->mAnimationType)) {
		e->mAnimationType = makeStringMugenAssignment("Light");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("air.animtype", tGroup, &e->mAirAnimationType)) {
		e->mAirAnimationType = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("fall.animtype", tGroup, &e->mFallAnimationType)) {
		e->mFallAnimationType = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("priority", tGroup, &e->mPriority)) {
		e->mPriority = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("damage", tGroup, &e->mDamage)) {
		e->mDamage = makeStringMugenAssignment("0 , 0");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("pausetime", tGroup, &e->mPauseTime)) {
		e->mPauseTime = makeStringMugenAssignment("0 , 0");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("guard.pausetime", tGroup, &e->mGuardPauseTime)) {
		e->mGuardPauseTime = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("sparkno", tGroup, &e->mSparkNumber)) {
		e->mSparkNumber = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("guard.sparkno", tGroup, &e->mGuardSparkNumber)) {
		e->mGuardSparkNumber = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("sparkxy", tGroup, &e->mSparkXY)) {
		e->mSparkXY = makeStringMugenAssignment("0 , 0");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("hitsound", tGroup, &e->mHitSound)) {
		e->mHitSound = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("guardsound", tGroup, &e->mGuardSound)) {
		e->mGuardSound = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("ground.type", tGroup, &e->mGroundType)) {
		e->mGroundType = makeStringMugenAssignment("High");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("air.type", tGroup, &e->mAirType)) {
		e->mAirType = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("ground.slidetime", tGroup, &e->mGroundSlideTime)) {
		e->mGroundSlideTime = makeStringMugenAssignment("0");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("guard.slidetime", tGroup, &e->mGuardSlideTime)) {
		e->mGuardSlideTime = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("ground.hittime", tGroup, &e->mGroundHitTime)) {
		e->mGroundHitTime = makeStringMugenAssignment("0");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("guard.hittime", tGroup, &e->mGuardHitTime)) {
		e->mGuardHitTime = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("air.hittime", tGroup, &e->mAirHitTime)) {
		e->mAirHitTime = makeStringMugenAssignment("20");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("guard.ctrltime", tGroup, &e->mGuardControlTime)) {
		e->mGuardControlTime = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("guard.dist", tGroup, &e->mGuardDistance)) {
		e->mGuardDistance = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("yaccel", tGroup, &e->mYAccel)) {
		e->mYAccel = makeStringMugenAssignment("");
	}
	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("ground.velocity", tGroup, &e->mGroundVelocity)) {
		e->mGroundVelocity = makeStringMugenAssignment("0 , 0");
	}
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("guard.velocity", tGroup, &e->mGuardVelocity, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("air.velocity", tGroup, &e->mAirVelocity, "0 , 0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("airguard.velocity", tGroup, &e->mAirGuardVelocity, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("airguard.velocity", tGroup, &e->mAirGuardVelocity, "");

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ground.cornerpush.veloff", tGroup, &e->mGroundCornerPushVelocityOffset, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("air.cornerpush.veloff", tGroup, &e->mAirCornerPushVelocityOffset, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("down.cornerpush.veloff", tGroup, &e->mDownCornerPushVelocityOffset, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("guard.cornerpush.veloff", tGroup, &e->mGuardCornerPushVelocityOffset, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("airguard.cornerpush.veloff", tGroup, &e->mAirGuardCornerPushVelocityOffset, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("airguard.ctrltime", tGroup, &e->mAirGuardControlTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("air.juggle", tGroup, &e->mAirJuggle, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("mindist", tGroup, &e->mMinimumDistance, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("maxdist", tGroup, &e->mMaximumDistance, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("snap", tGroup, &e->mSnap, "");

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p1sprpriority", tGroup, &e->mPlayerSpritePriority1, "1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p2sprpriority", tGroup, &e->mPlayerSpritePriority2, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p1facing", tGroup, &e->mPlayer1ChangeFaceDirection, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p1getp2facing", tGroup, &e->mPlayer1ChangeFaceDirectionRelativeToPlayer2, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p2facing", tGroup, &e->mPlayer2ChangeFaceDirectionRelativeToPlayer1, "0");

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p1stateno", tGroup, &e->mPlayer1StateNumber, "-1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p2stateno", tGroup, &e->mPlayer2StateNumber, "-1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p2getp1state", tGroup, &e->mPlayer2CapableOfGettingPlayer1State, "1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("forcestand", tGroup, &e->mForceStanding, "");

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall", tGroup, &e->mFall, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.xvelocity", tGroup, &e->mFallXVelocity, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.yvelocity", tGroup, &e->mFallYVelocity, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.recover", tGroup, &e->mFallCanBeRecovered, "1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.recovertime", tGroup, &e->mFallRecoveryTime, "4");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.damage", tGroup, &e->mFallDamage, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("air.fall", tGroup, &e->mAirFall, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("forcenofall", tGroup, &e->mForceNoFall, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("down.velocity", tGroup, &e->mDownVelocity, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("down.hittime", tGroup, &e->mDownHitTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("down.bounce", tGroup, &e->mDownBounce, "0");

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("id", tGroup, &e->mHitID, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("chainID", tGroup, &e->mChainID, "-1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("nochainID", tGroup, &e->mNoChainID, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("hitonce", tGroup, &e->mHitOnce, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("kill", tGroup, &e->mKill, "1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("guard.kill", tGroup, &e->mGuardKill, "1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.kill", tGroup, &e->mFallKill, "1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("numhits", tGroup, &e->mNumberOfHits, "1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("getpower", tGroup, &e->mGetPower, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("givepower", tGroup, &e->mGivePower, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("palfx.time", tGroup, &e->mPaletteEffectTime, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("palfx.mul", tGroup, &e->mPaletteEffectMultiplication, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("palfx.add", tGroup, &e->mPaletteEffectAddition, "");

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("envshake.time", tGroup, &e->mEnvironmentShakeTime, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("envshake.freq", tGroup, &e->mEnvironmentShakeFrequency, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("envshake.ampl", tGroup, &e->mEnvironmentShakeAmplitude, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("envshake.phase", tGroup, &e->mEnvironmentShakePhase, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.envshake.time", tGroup, &e->mFallEnvironmentShakeTime, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.envshake.freq", tGroup, &e->mFallEnvironmentShakeFrequency, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.envshake.ampl", tGroup, &e->mFallEnvironmentShakeAmplitude, "0");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("fall.envshake.phase", tGroup, &e->mFallEnvironmentShakePhase, "0");


	tController->mData = e;
}

typedef struct {
	int mDummy; // TODO
} PlaySoundController;


static void parsePlaySoundController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	PlaySoundController* e = allocMemory(sizeof(PlaySoundController));
	e->mDummy = 0; // TODO
	tController->mData = e;
	(void)tGroup;
}

typedef struct {
	int mDummy; // TODO
} WidthController;


static void parseWidthController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	WidthController* e = allocMemory(sizeof(WidthController));
	e->mDummy = 0; // TODO
	tController->mData = e;
	(void)tGroup;
}

typedef struct {
	MugenAssignment* tNewAnimation;

	MugenAssignment* tStep;
} ChangeAnimationController;


static void parseChangeAnimationController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	ChangeAnimationController* e = allocMemory(sizeof(ChangeAnimationController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->tNewAnimation));


	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("elem", tGroup, &e->tStep)) {
		e->tStep = makeNumberMugenAssignment(0);
	}

	tController->mData = e;
}

typedef struct {
	MugenAssignment* tValue;
} ControlSettingController;


static void parseControlSettingController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	ControlSettingController* e = allocMemory(sizeof(ControlSettingController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->tValue));

	tController->mData = e;
}

typedef struct {
	MugenAssignment* tValue;
} SpritePriorityController;


static void parseSpritePriorityController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	SpritePriorityController* e = allocMemory(sizeof(SpritePriorityController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->tValue));

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mFlag;

	int mHasFlag2;
	MugenAssignment* mFlag2;

	int mHasFlag3;
	MugenAssignment* mFlag3;
} SpecialAssertController;


static void parseSpecialAssertController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	SpecialAssertController* e = allocMemory(sizeof(SpecialAssertController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("flag", tGroup, &e->mFlag));
	e->mHasFlag2 = fetchAssignmentFromGroupAndReturnWhetherItExists("flag2", tGroup, &e->mFlag2);
	e->mHasFlag3 = fetchAssignmentFromGroupAndReturnWhetherItExists("flag3", tGroup, &e->mFlag3);

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mPositionOffset;

	int mHasSecondDustCloud;
	MugenAssignment* mPositionOffset2;

	MugenAssignment* mSpacing;
} MakeDustController;


static void parseMakeDustController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	MakeDustController* e = allocMemory(sizeof(MakeDustController));

	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("pos", tGroup, &e->mPositionOffset)) {
		e->mPositionOffset = make2DVectorMugenAssignment(makePosition(0, 0, 0));
	}

	e->mHasSecondDustCloud = fetchAssignmentFromGroupAndReturnWhetherItExists("pos2", tGroup, &e->mPositionOffset2);

	if (!fetchAssignmentFromGroupAndReturnWhetherItExists("spacing", tGroup, &e->mSpacing)) {
		e->mSpacing = makeNumberMugenAssignment(1);
	}

	tController->mData = e;
}

typedef enum {
	VAR_SET_TYPE_SYSTEM,
	VAR_SET_TYPE_SYSTEM_FLOAT,
	VAR_SET_TYPE_INTEGER,
	VAR_SET_TYPE_FLOAT
} VarSetType;

typedef struct {
	VarSetType mType;
	int mID;

	MugenAssignment* mAssignment;
} VarSetControllerEntry;

typedef struct {
	Vector mVarSets;
} VarSetController;

typedef struct {
	VarSetController* mController;
	MugenDefScriptGroup* mGroup;
} VarSetControllerCaller;

static void parseSingleVarSetControllerEntry(void* tCaller, char* tName, void* tData) {
	VarSetControllerCaller* caller = tCaller;
	(void)tData;
	VarSetControllerEntry* e = allocMemory(sizeof(VarSetControllerEntry));

	int isEntry = strchr(tName, '(') != NULL;
	if (!isEntry) return;

	char name[100];
	strcpy(name, tName);
	char* start = strchr(name, '(');
	assert(start != NULL);
	*start = '\0';

	char value[100];
	strcpy(value, start + 1);
	char* end = strchr(value, ')');
	assert(end != NULL);
	*end = '\0';


	if (!strcmp("var", name)) {
		e->mType = VAR_SET_TYPE_INTEGER;
	}
	else if (!strcmp("sysvar", name)) {
		e->mType = VAR_SET_TYPE_SYSTEM;
	}
	else if (!strcmp("sysfvar", name)) {
		e->mType = VAR_SET_TYPE_SYSTEM_FLOAT;
	}
	else if (!strcmp("fvar", name)) {
		e->mType = VAR_SET_TYPE_FLOAT;
	}
	else {
		logError("Unrecognized variable setting name.");
		logErrorString(name);
		abortSystem();
	}

	e->mID = atoi(value);

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists(tName, caller->mGroup, &e->mAssignment));

	vector_push_back_owned(&caller->mController->mVarSets, e);
}

static void loadSingleOriginalVarSetController(Vector* tDst, MugenDefScriptGroup* tGroup, MugenDefScriptGroupElement* tIDElement, VarSetType tType) {
	VarSetControllerEntry* e = allocMemory(sizeof(VarSetControllerEntry));
	e->mType = tType;
	e->mID = getMugenDefNumberVariableAsElement(tIDElement);
	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mAssignment));

	vector_push_back_owned(tDst, e);
}

static void parseVarSetController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	VarSetController* e = allocMemory(sizeof(VarSetController));
	e->mVarSets = new_vector();


	int isIntegerVersion = string_map_contains(&tGroup->mElements, "v");
	int isFloatVersion = string_map_contains(&tGroup->mElements, "fv");

	if (isIntegerVersion) {
		loadSingleOriginalVarSetController(&e->mVarSets, tGroup, string_map_get(&tGroup->mElements, "v"), VAR_SET_TYPE_INTEGER);
	}
	else if (isFloatVersion) {
		loadSingleOriginalVarSetController(&e->mVarSets, tGroup, string_map_get(&tGroup->mElements, "fv"), VAR_SET_TYPE_FLOAT);
	}
	else {
		VarSetControllerCaller caller;
		caller.mController = e;
		caller.mGroup = tGroup;

		string_map_map(&tGroup->mElements, parseSingleVarSetControllerEntry, &caller);
	}

	assert(vector_size(&e->mVarSets) == 1);

	tController->mData = e;
}

typedef struct {
	VarSetType mType;
	MugenAssignment* mValue;
	MugenAssignment* mFirst;
	MugenAssignment* mLast;
} VarRangeSetController;

static void parseVarRangeSetController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	VarRangeSetController* e = allocMemory(sizeof(VarRangeSetController));

	int isIntegerVersion = string_map_contains(&tGroup->mElements, "value");

	if (isIntegerVersion) {
		e->mType = VAR_SET_TYPE_INTEGER;
		assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mValue));
	}
	else {
		e->mType = VAR_SET_TYPE_FLOAT;
		assert(fetchAssignmentFromGroupAndReturnWhetherItExists("fvalue", tGroup, &e->mValue));
	}

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("first", tGroup, &e->mFirst, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("last", tGroup, &e->mLast, "");
 
	tController->mData = e;
}

typedef struct {
	int mDummy;
} NullController;


static void parseNullController(MugenStateController* tController) {
	NullController* e = allocMemory(sizeof(NullController));
	e->mDummy = 0;

	tController->mData = e;
}

typedef struct {
	int mHasStateType;
	MugenAssignment* mStateType;
	int mHasMoveType;
	MugenAssignment* mMoveType;
	int mHasPhysics;
	MugenAssignment* mPhysics;
} StateTypeSetController;


static void parseStateTypeSetController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	StateTypeSetController* e = allocMemory(sizeof(StateTypeSetController));

	e->mHasStateType = fetchAssignmentFromGroupAndReturnWhetherItExists("statetype", tGroup, &e->mStateType);
	e->mHasMoveType = fetchAssignmentFromGroupAndReturnWhetherItExists("movetype", tGroup, &e->mMoveType);
	e->mHasPhysics = fetchAssignmentFromGroupAndReturnWhetherItExists("physics", tGroup, &e->mPhysics);

	tController->mData = e;
}

static void parseForceFeedbackController(MugenStateController* tController) {
	parseNullController(tController); // TODO
}

typedef struct {
	MugenAssignment* mValue;
} DefenseMultiplierController;

static void parseDefenseMultiplierController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	DefenseMultiplierController* e = allocMemory(sizeof(DefenseMultiplierController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mValue));

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mAnim;

	MugenAssignment* mID;
	MugenAssignment* mPosition;
	MugenAssignment* mPositionType;
	MugenAssignment* mHorizontalFacing;
	MugenAssignment* mVerticalFacing;
	MugenAssignment* mBindTime;
	MugenAssignment* mVelocity;
	MugenAssignment* mAcceleration;
	MugenAssignment* mRandomOffset;
	MugenAssignment* mRemoveTime;
	MugenAssignment* mSuperMove;
	MugenAssignment* mSuperMoveTime;
	MugenAssignment* mPauseMoveTime;
	MugenAssignment* mScale;
	MugenAssignment* mSpritePriority;
	MugenAssignment* mOnTop;
	MugenAssignment* mShadow;
	MugenAssignment* mOwnPalette;
	MugenAssignment* mIsRemovedOnGetHit;
	MugenAssignment* mIgnoreHitPause;
	int mHasTransparencyType;
	MugenAssignment* mTransparencyType;

} ExplodController;

static void parseExplodController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	ExplodController* e = allocMemory(sizeof(ExplodController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("anim", tGroup, &e->mAnim));
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ID", tGroup, &e->mID, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pos", tGroup, &e->mPosition, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("postype", tGroup, &e->mPositionType, "p1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("facing", tGroup, &e->mHorizontalFacing, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("vfacing", tGroup, &e->mVerticalFacing, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("bindtime", tGroup, &e->mBindTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("vel", tGroup, &e->mVelocity, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("accel", tGroup, &e->mAcceleration, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("random", tGroup, &e->mRandomOffset, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("removetime", tGroup, &e->mRemoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("supermove", tGroup, &e->mSuperMove, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("supermovetime", tGroup, &e->mSuperMoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pausemovetime", tGroup, &e->mPauseMoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("scale", tGroup, &e->mScale, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("sprpriority", tGroup, &e->mSpritePriority, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ontop", tGroup, &e->mOnTop, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("shadow", tGroup, &e->mShadow, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ownpal", tGroup, &e->mOwnPalette, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("removeongethit", tGroup, &e->mIsRemovedOnGetHit, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ignorehitpause", tGroup, &e->mIgnoreHitPause, "");
	e->mHasTransparencyType = fetchAssignmentFromGroupAndReturnWhetherItExists("trans", tGroup, &e->mTransparencyType);

	tController->mData = e;
}

static void parseModifyExplodController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	ExplodController* e = allocMemory(sizeof(ExplodController));

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("anim", tGroup, &e->mAnim, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ID", tGroup, &e->mID, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pos", tGroup, &e->mPosition, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("postype", tGroup, &e->mPositionType, "p1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("facing", tGroup, &e->mHorizontalFacing, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("vfacing", tGroup, &e->mVerticalFacing, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("bindtime", tGroup, &e->mBindTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("vel", tGroup, &e->mVelocity, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("accel", tGroup, &e->mAcceleration, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("random", tGroup, &e->mRandomOffset, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("removetime", tGroup, &e->mRemoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("supermove", tGroup, &e->mSuperMove, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("supermovetime", tGroup, &e->mSuperMoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pausemovetime", tGroup, &e->mPauseMoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("scale", tGroup, &e->mScale, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("sprpriority", tGroup, &e->mSpritePriority, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ontop", tGroup, &e->mOnTop, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("shadow", tGroup, &e->mShadow, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ownpal", tGroup, &e->mOwnPalette, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("removeongethit", tGroup, &e->mIsRemovedOnGetHit, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ignorehitpause", tGroup, &e->mIgnoreHitPause, "");
	e->mHasTransparencyType = fetchAssignmentFromGroupAndReturnWhetherItExists("trans", tGroup, &e->mTransparencyType);

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mValue;
} PositionFreezeController;

static void parsePositionFreezeController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	PositionFreezeController* e = allocMemory(sizeof(PositionFreezeController));

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("value", tGroup, &e->mValue, "1");

	tController->mData = e;
}

typedef struct {
	int mHasValue;
	MugenStringVector mValue;

	int mHasValue2;
	MugenStringVector mValue2;

	MugenAssignment* mTime;
} NotHitByController;

static void readMugenDefStringVector(MugenStringVector* tDst, MugenDefScriptGroup* tGroup, char* tName, int* oHasValue) {
	if (string_map_contains(&tGroup->mElements, tName)) {
		MugenDefScriptGroupElement* elem = string_map_get(&tGroup->mElements, tName);
		*oHasValue = 1;
		if (isMugenDefStringVectorVariableAsElement(elem)) {
			*tDst = getMugenDefStringVectorVariableAsElement(elem);
		}
		else {
			char* text = getAllocatedMugenDefStringVariableAsElement(elem);
			strcpy(tDst->mElement[0], text);
			strcpy(tDst->mElement[1], "");
			freeMemory(text);
		}
	}
	else {
		*oHasValue = 0;
	}

}

static void parseNotHitByController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	NotHitByController* e = allocMemory(sizeof(NotHitByController));

	readMugenDefStringVector(&e->mValue, tGroup, "value", &e->mHasValue);
	readMugenDefStringVector(&e->mValue2, tGroup, "value2", &e->mHasValue2);
	assert(e->mHasValue || e->mHasValue2);

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("time", tGroup, &e->mTime, "1");

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mValue;
	int mHasXVelocity;
	MugenAssignment* mXVelocity;
	int mHasYVelocity;
	MugenAssignment* mYVelocity;
} HitFallSetController;

static void parseHitFallSetController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	HitFallSetController* e = allocMemory(sizeof(HitFallSetController));

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("value", tGroup, &e->mValue, "-1");
	e->mHasXVelocity = fetchAssignmentFromGroupAndReturnWhetherItExists("xvel", tGroup, &e->mXVelocity);
	e->mHasYVelocity = fetchAssignmentFromGroupAndReturnWhetherItExists("yvel", tGroup, &e->mYVelocity);

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mValue;
} SingleRequiredValueController;

static void parseSingleRequiredValueController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	SingleRequiredValueController* e = allocMemory(sizeof(SingleRequiredValueController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mValue));

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mTime;

	MugenAssignment* mFrequency;
	MugenAssignment* mAmplitude;
	MugenAssignment* mPhaseOffset;
} EnvironmentShakeController;

static void parseEnvironmentShakeController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	EnvironmentShakeController* e = allocMemory(sizeof(EnvironmentShakeController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("time", tGroup, &e->mTime));
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("freq", tGroup, &e->mFrequency, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ampl", tGroup, &e->mAmplitude, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("phase", tGroup, &e->mPhaseOffset, "");

	tController->mData = e;

}

typedef struct {
	MugenAssignment* mTime;
	MugenAssignment* mBufferTimeForCommandsDuringPauseEnd;
	MugenAssignment* mMoveTime;
	MugenAssignment* mDoesPauseBackground;

	MugenAssignment* mAnim;
	MugenAssignment* mSound;
	MugenAssignment* mPosition;
	MugenAssignment* mIsDarkening;
	MugenAssignment* mPlayer2DefenseMultiplier;
	MugenAssignment* mPowerToAdd;
	MugenAssignment* mSetPlayerUnhittable;

} SuperPauseController;

static void parseSuperPauseController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	SuperPauseController* e = allocMemory(sizeof(SuperPauseController));

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("time", tGroup, &e->mTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("endcmdbuftime", tGroup, &e->mBufferTimeForCommandsDuringPauseEnd, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("movetime", tGroup, &e->mMoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pausebg", tGroup, &e->mDoesPauseBackground, "");

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("anim", tGroup, &e->mAnim, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("sound", tGroup, &e->mSound, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pos", tGroup, &e->mPosition, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("darken", tGroup, &e->mIsDarkening, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("p2defmul", tGroup, &e->mPlayer2DefenseMultiplier, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("poweradd", tGroup, &e->mPowerToAdd, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("unhittable", tGroup, &e->mSetPlayerUnhittable, "");

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mType;
	char mName[200];
	MugenAssignment* mID;
	MugenAssignment* mPosition;
	MugenAssignment* mPositionType;

	MugenAssignment* mFacing;
	MugenAssignment* mStateNumber;
	MugenAssignment* mCanControl;

	MugenAssignment* mHasOwnPalette;
	MugenAssignment* mSuperMoveTime;
	MugenAssignment* mPauseMoveTime;

	MugenAssignment* mSizeScaleX;
	MugenAssignment* mSizeScaleY;
	MugenAssignment* mSizeGroundBack;
	MugenAssignment* mSizeGroundFront;
	MugenAssignment* mSizeAirBack;
	MugenAssignment* mSizeAirFront;

	MugenAssignment* mSizeHeight;
	MugenAssignment* mSizeProjectilesDoScale;
	MugenAssignment* mSizeHeadPosition;
	MugenAssignment* mSizeMiddlePosition;
	MugenAssignment* mSizeShadowOffset;

} HelperController;

static void parseHelpeControllerName(HelperController* e, MugenDefScriptGroup* tGroup) {
	strcpy(e->mName, "");
	if (!string_map_contains(&tGroup->mElements, "name")) return;
	MugenDefScriptGroupElement* elem = string_map_get(&tGroup->mElements, "name");
	if (!isMugenDefStringVariableAsElement(elem)) return;

	char* text = getAllocatedMugenDefStringVariableAsElement(elem);
	strcpy(e->mName, text);
	freeMemory(text);
}

static void parseHelperController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	HelperController* e = allocMemory(sizeof(HelperController));

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("helpertype", tGroup, &e->mType, "normal");
	parseHelpeControllerName(e, tGroup);
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("id", tGroup, &e->mID, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pos", tGroup, &e->mPosition, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("postype", tGroup, &e->mPositionType, "p1");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("facing", tGroup, &e->mFacing, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("stateno", tGroup, &e->mStateNumber, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("keyctrl", tGroup, &e->mCanControl, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("ownpal", tGroup, &e->mHasOwnPalette, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("supermovetime", tGroup, &e->mSuperMoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pausemovetime", tGroup, &e->mPauseMoveTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.xscale", tGroup, &e->mSizeScaleX, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.yscale", tGroup, &e->mSizeScaleY, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.ground.back", tGroup, &e->mSizeGroundBack, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.ground.front", tGroup, &e->mSizeGroundFront, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.air.back", tGroup, &e->mSizeAirBack, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.air.front", tGroup, &e->mSizeAirFront, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.height", tGroup, &e->mSizeHeight, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.proj.doscale", tGroup, &e->mSizeProjectilesDoScale, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.head.pos", tGroup, &e->mSizeHeadPosition, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.mid.pos", tGroup, &e->mSizeMiddlePosition, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("size.shadowoffset", tGroup, &e->mSizeShadowOffset, "");

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mValue;
	MugenAssignment* mCanKill;
	MugenAssignment* mIsAbsolute;

} LifeAddController;

static void parseLifeAddController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	LifeAddController* e = allocMemory(sizeof(LifeAddController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mValue));
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("kill", tGroup, &e->mCanKill, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("absolute", tGroup, &e->mIsAbsolute, "");

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mValue;
	MugenAssignment* mID;
	MugenAssignment* mCanKill;
	MugenAssignment* mIsAbsolute;

} TargetLifeAddController;

static void parseTargetLifeAddController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	TargetLifeAddController* e = allocMemory(sizeof(TargetLifeAddController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mValue));
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("id", tGroup, &e->mID, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("kill", tGroup, &e->mCanKill, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("absolute", tGroup, &e->mIsAbsolute, "");

	tController->mData = e;
}

typedef struct {
	int mHasID;
	MugenAssignment* mID;

} RemoveExplodController;

static void parseRemoveExplodController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	RemoveExplodController* e = allocMemory(sizeof(RemoveExplodController));

	e->mHasID = fetchAssignmentFromGroupAndReturnWhetherItExists("id", tGroup, &e->mID);

	tController->mData = e;
}

typedef struct {
	int mHasValue;
	MugenAssignment* mValue;

	int mHasScale;
	MugenAssignment* mScale;

} AngleDrawController;

static void parseAngleDrawController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	AngleDrawController* e = allocMemory(sizeof(AngleDrawController));

	e->mHasValue = fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mValue);
	e->mHasScale = fetchAssignmentFromGroupAndReturnWhetherItExists("scale", tGroup, &e->mScale);

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mTime;
	MugenAssignment* mFacing;
	MugenAssignment* mID;
	MugenAssignment* mPosition;

} BindController;

static void parseBindController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	BindController* e = allocMemory(sizeof(BindController));

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("time", tGroup, &e->mTime, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("facing", tGroup, &e->mFacing, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("pos", tGroup, &e->mPosition, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("id", tGroup, &e->mID, "");

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mValue;
	MugenAssignment* mMoveCameraFlags;

} ScreenBoundController;

static void parseScreenBoundController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	ScreenBoundController* e = allocMemory(sizeof(ScreenBoundController));

	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("value", tGroup, &e->mValue, "");
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("movecamera", tGroup, &e->mMoveCameraFlags, "");

	tController->mData = e;
}

typedef struct {
	MugenAssignment* mValue;
	MugenAssignment* mID;

} SetTargetFacingController;

static void parseSetTargetFacingController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	SetTargetFacingController* e = allocMemory(sizeof(SetTargetFacingController));

	assert(fetchAssignmentFromGroupAndReturnWhetherItExists("value", tGroup, &e->mValue));
	fetchAssignmentFromGroupAndReturnWhetherItExistsDefaultString("id", tGroup, &e->mID, "");

	tController->mData = e;
}

typedef struct {
	MugenStringVector mAttributes;
} ReversalDefinitionController;

static void parseReversalDefinitionController(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	ReversalDefinitionController* e = allocMemory(sizeof(ReversalDefinitionController));

	int hasString;
	readMugenDefStringVector(&e->mAttributes, tGroup, "reversal.attr", &hasString);
	assert(hasString);

	tController->mData = e;
}


void parseStateControllerType(MugenStateController* tController, MugenDefScriptGroup* tGroup) {
	assert(string_map_contains(&tGroup->mElements, "type"));
	MugenDefScriptGroupElement* e = string_map_get(&tGroup->mElements, "type");
	tController->mData = NULL;

	char* type = getAllocatedMugenDefStringVariableAsElement(e);
	turnStringLowercase(type);

	if (!strcmp("nothitby", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_NOT_HIT_BY;
		parseNotHitByController(tController, tGroup);
	}
	else if (!strcmp("hitby", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_HIT_BY;
		parseNotHitByController(tController, tGroup);
	}
	else if (!strcmp("changestate", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_CHANGE_STATE;
		parseChangeStateController(tController, tGroup);
	}
	else if (!strcmp("changeanim", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_CHANGE_ANIMATION;
		parseChangeAnimationController(tController, tGroup);
	}
	else if (!strcmp("assertspecial", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ASSERT_SPECIAL;
		parseSpecialAssertController(tController, tGroup);
	}
	else if (!strcmp("explod", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_EXPLOD;
		parseExplodController(tController, tGroup);
	}
	else if (!strcmp("playsnd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_PLAY_SOUND;
		parsePlaySoundController(tController, tGroup);
	}
	else if (!strcmp("ctrlset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_CONTROL;
		parseControlSettingController(tController, tGroup);
	}
	else if (!strcmp("hitdef", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_HIT_DEFINITION;
		parseHitDefinitionController(tController, tGroup);
	}
	else if (!strcmp("width", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_WIDTH;
		parseWidthController(tController, tGroup);
	}
	else if (!strcmp("sprpriority", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SPRITE_PRIORITY;
		parseSpritePriorityController(tController, tGroup);
	}
	else if (!strcmp("posadd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_POSITION;
		parse2DPhysicsController(tController, tGroup);
	}
	else if (!strcmp("varset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_VARIABLE;
		parseVarSetController(tController, tGroup);
	}
	else if (!strcmp("targetbind", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_BIND_TARGET;
		parseBindController(tController, tGroup);
	}
	else if (!strcmp("turn", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_TURN;
		parseNullController(tController);
	}
	else if (!strcmp("targetfacing", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_TARGET_FACING;
		parseSetTargetFacingController(tController, tGroup);
	}
	else if (!strcmp("targetlifeadd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_TARGET_LIFE;
		parseTargetLifeAddController(tController, tGroup);
	}
	else if (!strcmp("targetstate", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_TARGET_STATE;
		parseTargetChangeStateController(tController, tGroup);
	}
	else if (!strcmp("changeanim2", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_CHANGE_ANIMATION_2;
		parseChangeAnimationController(tController, tGroup);
	}
	else if (!strcmp("selfstate", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_SELF_STATE;
		parseChangeStateController(tController, tGroup);
	}
	else if (!strcmp("veladd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_VELOCITY;
		parse2DPhysicsController(tController, tGroup);
	}
	else if (!strcmp("velset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_VELOCITY;
		parse2DPhysicsController(tController, tGroup);
	}
	else if (!strcmp("velmul", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_MULTIPLY_VELOCITY;
		parse2DPhysicsController(tController, tGroup);
	}
	else if (!strcmp("afterimage", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_AFTER_IMAGE;
		parseNullController(tController); // TODO
	}
	else if (!strcmp("afterimagetime", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_AFTER_IMAGE_TIME;
		parseNullController(tController); // TODO
	}
	else if (!strcmp("palfx", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_PALETTE_EFFECT;
		parseNullController(tController); // TODO
	}
	else if (!strcmp("hitvelset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_HIT_VELOCITY;
		parse2DPhysicsController(tController, tGroup);
	}
	else if (!strcmp("screenbound", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SCREEN_BOUND;
		parseScreenBoundController(tController, tGroup);
	}
	else if (!strcmp("posfreeze", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_FREEZE_POSITION;
		parsePositionFreezeController(tController, tGroup);
	}
	else if (!strcmp("null", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_NULL;
		parseNullController(tController);
	}
	else if (!strcmp("posset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_POSITION;
		parse2DPhysicsController(tController, tGroup);
	}
	else if (!strcmp("envshake", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ENVIRONMENT_SHAKE;
		parseEnvironmentShakeController(tController, tGroup);
	}
	else if (!strcmp("reversaldef", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_REVERSAL_DEFINITION;
		parseReversalDefinitionController(tController, tGroup);
	}
	else if (!strcmp("hitoverride", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_HIT_OVERRIDE;
		parseNullController(tController); // TODO
	}
	else if (!strcmp("pause", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_PAUSE;
	}
	else if (!strcmp("superpause", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SUPER_PAUSE;
		parseSuperPauseController(tController, tGroup);
	}
	else if (!strcmp("makedust", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_MAKE_DUST;
		parseMakeDustController(tController, tGroup);
	}
	else if (!strcmp("statetypeset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_STATE_TYPE;
		parseStateTypeSetController(tController, tGroup);
	}
	else if (!strcmp("forcefeedback", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_FORCE_FEEDBACK;
		parseForceFeedbackController(tController);
	}
	else if (!strcmp("defencemulset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_DEFENSE_MULTIPLIER;
		parseDefenseMultiplierController(tController, tGroup);
	}
	else if (!strcmp("varadd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_VARIABLE;
		parseVarSetController(tController, tGroup);
	}
	else if (!strcmp("fallenvshake", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_FALL_ENVIRONMENT_SHAKE;
		parseNullController(tController);
	}
	else if (!strcmp("hitfalldamage", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_HIT_FALL_DAMAGE;
		parseNullController(tController);
	}
	else if (!strcmp("hitfallvel", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_HIT_FALL_VELOCITY;
		parseNullController(tController);
	}
	else if (!strcmp("hitfallset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_HIT_FALL;
		parseHitFallSetController(tController, tGroup);
	}
	else if (!strcmp("varrangeset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_VARIABLE_RANGE;
		parseVarRangeSetController(tController, tGroup);
	}
	else if (!strcmp("remappal", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_REMAP_PALETTE;
		parseNullController(tController); // TODO
	}
	else if (!strcmp("playerpush", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_PLAYER_PUSH;
		parseSingleRequiredValueController(tController, tGroup);
	}
	else if (!strcmp("poweradd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_POWER;
		parseSingleRequiredValueController(tController, tGroup);
	}
	else if (!strcmp("helper", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_HELPER;
		parseHelperController(tController, tGroup);
	}
	else if (!strcmp("stopsnd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_STOP_SOUND;
		parseNullController(tController); // TODO: Sound
	}
	else if (!strcmp("removeexplod", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_REMOVE_EXPLOD;
		parseRemoveExplodController(tController, tGroup);
	}
	else if (!strcmp("destroyself", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_DESTROY_SELF;
		parseNullController(tController);
	}
	else if (!strcmp("allpalfx", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_PALETTE_EFFECT_ALL;
	}
	else if (!strcmp("envcolor", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ENVIRONMENT_COLOR;
	}
	else if (!strcmp("victoryquote", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_VICTORY_QUOTE;
	}
	else if (!strcmp("attackmulset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_ATTACK_MULTIPLIER;
		parseSingleRequiredValueController(tController, tGroup);
	}
	else if (!strcmp("lifeadd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_LIFE;
		parseLifeAddController(tController, tGroup);
	}
	else if (!strcmp("sndpan", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_PAN_SOUND;
		parseNullController(tController); // TODO
	}
	else if (!strcmp("displaytoclipboard", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_DISPLAY_TO_CLIPBOARD;
		parseNullController(tController); // TODO
	}
	else if (!strcmp("appendtoclipboard", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_APPEND_TO_CLIPBOARD;
		parseNullController(tController); // TODO
	}
	else if (!strcmp("gravity", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_GRAVITY;
		parseNullController(tController);
	}
	else if (!strcmp("attackdist", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_ATTACK_DISTANCE;
		parseSingleRequiredValueController(tController, tGroup);
	}
	else if (!strcmp("targetpoweradd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_TARGET_POWER;
	}
	else if (!strcmp("movehitreset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_RESET_MOVE_HIT;
		parseNullController(tController);
	}
	else if (!strcmp("modifyexplod", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_MODIFY_EXPLOD;
		parseModifyExplodController(tController, tGroup);
	}
	else if (!strcmp("bgpalfx", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_PALETTE_EFFECT_BACKGROUND;
		parseNullController(tController); // TODO: maybe
	}
	else if (!strcmp("hitadd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_HIT;
	}
	else if (!strcmp("bindtoroot", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_BIND_TO_ROOT;
		parseBindController(tController, tGroup);
	}
	else if (!strcmp("trans", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_TRANSPARENCY;
		parseNullController(tController); // NULL
	}
	else if (!strcmp("bindtoparent", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_BIND_TO_PARENT;
		parseBindController(tController, tGroup);
	}
	else if (!strcmp("angleset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_ANGLE;
		parseSingleRequiredValueController(tController, tGroup);
	}
	else if (!strcmp("angleadd", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_ADD_ANGLE;
		parseSingleRequiredValueController(tController, tGroup);
	}
	else if (!strcmp("angledraw", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_DRAW_ANGLE;
		parseAngleDrawController(tController, tGroup);
	}
	else if (!strcmp("gamemakeanim", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_MAKE_GAME_ANIMATION;
	}
	else if (!strcmp("varrandom", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_VARIABLE_RANDOM;
	}
	else if (!strcmp("parentvarset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_PARENT_VARIABLE;
		parseVarSetController(tController, tGroup);
	}
	else if (!strcmp("projectile", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_PROJECTILE;
	}
	else if (!strcmp("powerset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_POWER;
	}
	else if (!strcmp("offset", type)) {
		tController->mType = MUGEN_STATE_CONTROLLER_TYPE_SET_OFFSET;
	}
	else {
		logError("Unable to determine state controller type.");
		logErrorString(type);
		abortSystem();
	}

	freeMemory(type);
}

MugenStateController * parseMugenStateControllerFromGroup(MugenDefScriptGroup* tGroup)
{
	MugenStateController* ret = allocMemory(sizeof(MugenStateController));
	parseStateControllerType(ret, tGroup);
	parseStateControllerTriggers(ret, tGroup);

	return ret;
}

static void handleVelocitySetting(MugenStateController* tController, Player* tPlayer) {
	Set2DPhysicsController* e = tController->mData;

	if (e->mIsSettingX) {
		double x = evaluateAssignmentAndReturnAsFloat(e->x, tPlayer);
		setPlayerVelocityX(tPlayer, x, getPlayerCoordinateP(tPlayer));
	}

	if (e->mIsSettingY) {
		double y = evaluateAssignmentAndReturnAsFloat(e->y, tPlayer);
		setPlayerVelocityY(tPlayer, y, getPlayerCoordinateP(tPlayer));
	}
}

static void handleVelocityMultiplication(MugenStateController* tController, Player* tPlayer) {
	Set2DPhysicsController* e = tController->mData;

	if (e->mIsSettingX) {
		double x = evaluateAssignmentAndReturnAsFloat(e->x, tPlayer);
		multiplyPlayerVelocityX(tPlayer, x, getPlayerCoordinateP(tPlayer));
	}

	if (e->mIsSettingY) {
		double y = evaluateAssignmentAndReturnAsFloat(e->y, tPlayer);
		multiplyPlayerVelocityY(tPlayer, y, getPlayerCoordinateP(tPlayer));
	}
}

static void handleVelocityAddition(MugenStateController* tController, Player* tPlayer) {
	Set2DPhysicsController* e = tController->mData;

	if (e->mIsSettingX) {
		double x = evaluateAssignmentAndReturnAsFloat(e->x, tPlayer);
		addPlayerVelocityX(tPlayer, x, getPlayerCoordinateP(tPlayer));
	}

	if (e->mIsSettingY) {
		double y = evaluateAssignmentAndReturnAsFloat(e->y, tPlayer);
		addPlayerVelocityY(tPlayer, y, getPlayerCoordinateP(tPlayer));
	}
}

static void handlePositionSetting(MugenStateController* tController, Player* tPlayer) {
	Set2DPhysicsController* e = tController->mData;

	if (e->mIsSettingX) {
		double x = evaluateAssignmentAndReturnAsFloat(e->x, tPlayer);
		setPlayerPositionX(tPlayer, x, getPlayerCoordinateP(tPlayer));
	}

	if (e->mIsSettingY) {
		double y = evaluateAssignmentAndReturnAsFloat(e->y, tPlayer);
		setPlayerPositionY(tPlayer, y, getPlayerCoordinateP(tPlayer));
	}
}

static void handlePositionAdding(MugenStateController* tController, Player* tPlayer) {
	Set2DPhysicsController* e = tController->mData;

	if (e->mIsSettingX) {
		double x = evaluateAssignmentAndReturnAsFloat(e->x, tPlayer);
		addPlayerPositionX(tPlayer, x, getPlayerCoordinateP(tPlayer));
	}

	if (e->mIsSettingY) {
		double y = evaluateAssignmentAndReturnAsFloat(e->y, tPlayer);
		addPlayerPositionY(tPlayer, y, getPlayerCoordinateP(tPlayer));
	}
}


static void handleStateChange(MugenStateController* tController, Player* tPlayer) {
	ChangeStateController* e = tController->mData;

	int state = evaluateAssignmentAndReturnAsInteger(e->mState, tPlayer);
	changePlayerStateBeforeImmediatelyEvaluatingIt(tPlayer, state);

	if (e->mIsChangingControl) {
		int control = evaluateAssignmentAndReturnAsInteger(e->mControl, tPlayer);
		setPlayerControl(tPlayer, control);
	}
}

static void handleSelfStateChange(MugenStateController* tController, Player* tPlayer) {
	ChangeStateController* e = tController->mData;

	int state = evaluateAssignmentAndReturnAsInteger(e->mState, tPlayer);

	if (e->mIsChangingControl) {
		int control = evaluateAssignmentAndReturnAsInteger(e->mControl, tPlayer);
		setPlayerControl(tPlayer, control);
	}

	changePlayerStateToSelfBeforeImmediatelyEvaluatingIt(tPlayer, state);
}

static void getSingleIntegerValueOrDefault(MugenAssignment* tAssignment, Player* tPlayer, int* tDst, int tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	if (!strcmp("", flag)) *tDst = tDefault;
	else *tDst = atoi(flag);

	freeMemory(flag);
}

static void getSingleFloatValueOrDefault(MugenAssignment* tAssignment, Player* tPlayer, double* tDst, double tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	if (!strcmp("", flag)) *tDst = tDefault;
	else *tDst = atof(flag);

	freeMemory(flag);
}

static void handleTargetStateChange(MugenStateController* tController, Player* tPlayer) {
	TargetChangeStateController* e = tController->mData;

	int id;
	getSingleIntegerValueOrDefault(e->mID, tPlayer, &id, -1);

	if (e->mIsChangingControl) {
		int control = evaluateAssignmentAndReturnAsInteger(e->mControl, tPlayer);
		setPlayerTargetControl(tPlayer, id, control);
	}

	int state = evaluateAssignmentAndReturnAsInteger(e->mState, tPlayer);
	changePlayerTargetState(tPlayer, id, state);
}

static void handlePlaySound(MugenStateController* tController, Player* tPlayer) {
	PlaySoundController* e = tController->mData;

	// TODO
	(void)e;
	(void)tPlayer;
}

static void handleHitDefinitionAttribute(HitDefinitionController* e, Player* tPlayer) {
	char* attr = evaluateAssignmentAndReturnAsAllocatedString(e->mAttribute, tPlayer);

	char arg1[20], comma[10], arg2[20];
	sscanf(attr, "%s %s %s", arg1, comma, arg2);
	assert(strcmp("", arg1));
	assert(strlen(arg2) == 2);
	assert(!strcmp(",", comma));

	if (strchr(arg1, 'S') != NULL) setHitDataType(tPlayer, MUGEN_STATE_TYPE_STANDING);
	else if (strchr(arg1, 'C') != NULL) setHitDataType(tPlayer, MUGEN_STATE_TYPE_CROUCHING);
	else if (strchr(arg1, 'A') != NULL) setHitDataType(tPlayer, MUGEN_STATE_TYPE_AIR);
	else {
		logError("Unable to parse hitdef attr.");
		logErrorString(arg1);
		abortSystem();
	}

	if (arg2[0] == 'N') setHitDataAttackClass(tPlayer, MUGEN_ATTACK_CLASS_NORMAL);
	else if (arg2[0] == 'S') setHitDataAttackClass(tPlayer, MUGEN_ATTACK_CLASS_SPECIAL);
	else if (arg2[0] == 'H') setHitDataAttackClass(tPlayer, MUGEN_ATTACK_CLASS_HYPER);
	else {
		logError("Unable to parse hitdef attr 2.");
		logErrorString(arg2);
		abortSystem();
	}

	if (arg2[1] == 'A') setHitDataAttackType(tPlayer, MUGEN_ATTACK_TYPE_ATTACK);
	else if (arg2[1] == 'T') setHitDataAttackType(tPlayer, MUGEN_ATTACK_TYPE_THROW);
	else if (arg2[1] == 'P')  setHitDataAttackType(tPlayer, MUGEN_ATTACK_TYPE_PROJECTILE);
	else {
		logError("Unable to parse hitdef attr 2.");
		logErrorString(arg2);
		abortSystem();
	}

	freeMemory(attr);
}

static void handleHitDefinitionSingleHitFlag(MugenAssignment* tFlagAssignment, Player* tPlayer, void(tSetFunc)(Player* tPlayer, char*)) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tFlagAssignment, tPlayer);
	tSetFunc(tPlayer, flag);
	freeMemory(flag);
}

static void handleHitDefinitionAffectTeam(MugenAssignment* tAffectAssignment, Player* tPlayer) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAffectAssignment, tPlayer);
	assert(strlen(flag) == 1);

	if (*flag == 'B') setHitDataAffectTeam(tPlayer, MUGEN_AFFECT_TEAM_BOTH);
	else if (*flag == 'E') setHitDataAffectTeam(tPlayer, MUGEN_AFFECT_TEAM_ENEMY);
	else if (*flag == 'F') setHitDataAffectTeam(tPlayer, MUGEN_AFFECT_TEAM_FRIENDLY);
	else {
		logError("Unable to parse hitdef affectteam.");
		logErrorString(flag);
		abortSystem();
	}

	freeMemory(flag);
}

static void handleHitDefinitionSingleAnimationType(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, MugenHitAnimationType), MugenHitAnimationType tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(flag);

	if (!strcmp("", flag)) tFunc(tPlayer, tDefault);
	else if (!strcmp("light", flag)) tFunc(tPlayer, MUGEN_HIT_ANIMATION_TYPE_LIGHT);
	else if (!strcmp("medium", flag) || !strcmp("med", flag)) tFunc(tPlayer, MUGEN_HIT_ANIMATION_TYPE_MEDIUM);
	else if (!strcmp("hard", flag)) tFunc(tPlayer, MUGEN_HIT_ANIMATION_TYPE_HARD);
	else if (!strcmp("heavy", flag)) tFunc(tPlayer, MUGEN_HIT_ANIMATION_TYPE_HEAVY);
	else if (!strcmp("back", flag)) tFunc(tPlayer, MUGEN_HIT_ANIMATION_TYPE_BACK);
	else if (!strcmp("up", flag)) tFunc(tPlayer, MUGEN_HIT_ANIMATION_TYPE_UP);
	else if (!strcmp("diagup", flag)) tFunc(tPlayer, MUGEN_HIT_ANIMATION_TYPE_DIAGONAL_UP);
	else {
		logError("Unable to parse hitdef animation type.");
		logErrorString(flag);
		abortSystem();
	}

	freeMemory(flag);
}

static void handleHitDefinitionPriority(MugenAssignment* tAssignment, Player* tPlayer) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(flag);

	char prioString[20], comma[10], typeString[20];
	int items = sscanf(flag, "%s %s %s", prioString, comma, typeString);

	int prio;
	if (items < 1 || !strcmp("", prioString)) prio = 4;
	else prio = atoi(prioString);

	MugenHitPriorityType type = MUGEN_HIT_PRIORITY_HIT;
	if (items < 3 || !strcmp("", typeString)) type = MUGEN_HIT_PRIORITY_HIT;
	else if (!strcmp("hit", typeString)) type = MUGEN_HIT_PRIORITY_HIT;
	else if (!strcmp("miss", typeString)) type = MUGEN_HIT_PRIORITY_MISS;
	else if (!strcmp("dodge", typeString)) type = MUGEN_HIT_PRIORITY_DODGE;
	else {
		logError("Unable to parse hitdef priority type.");
		logErrorString(flag);
		logErrorString(typeString);
		abortSystem();
	}

	setHitDataPriority(tPlayer, prio, type);

	freeMemory(flag);
}

static void getTwoIntegerValuesWithDefaultValues(MugenAssignment* tAssignment, Player* tPlayer, int* v1, int* v2, int tDefault1, int tDefault2) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string1[20], comma[10], string2[20];
	int items = sscanf(flag, "%s %s %s", string1, comma, string2);

	if (items < 1 || !strcmp("", string1)) *v1 = tDefault1;
	else *v1 = atoi(string1);
	if (items < 3 || !strcmp("", string2)) *v2 = tDefault2;
	else *v2 = atoi(string2);

	freeMemory(flag);
}

static void getTwoFloatValuesWithDefaultValues(MugenAssignment* tAssignment, Player* tPlayer, double* v1, double* v2, double tDefault1, double tDefault2) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string1[20], comma[10], string2[20];
	int items = sscanf(flag, "%s %s %s", string1, comma, string2);

	if (items < 1 || !strcmp("", string1)) *v1 = tDefault1;
	else *v1 = atof(string1);
	if (items < 3 || !strcmp("", string2)) *v2 = tDefault2;
	else *v2 = atof(string2);

	freeMemory(flag);
}

static void handleHitDefinitionDamage(MugenAssignment* tAssignment, Player* tPlayer) {
	int damage, guardDamage;

	getTwoIntegerValuesWithDefaultValues(tAssignment, tPlayer, &damage, &guardDamage, 0, 0);
	setHitDataDamage(tPlayer, damage, guardDamage);
}

static void handleHitDefinitionSinglePauseTime(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, int, int), int tDefault1, int tDefault2) {
	int p1PauseTime, p2PauseTime;

	getTwoIntegerValuesWithDefaultValues(tAssignment, tPlayer, &p1PauseTime, &p2PauseTime, tDefault1, tDefault2);
	tFunc(tPlayer, p1PauseTime, p2PauseTime);
}

static void handleHitDefinitionSparkNumberSingle(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, int, int), int tDefaultIsInFile, int tDefaultNumber) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(flag);

	char firstW[200];
	int which = -1;

	int items = sscanf(flag, "%s %d", firstW, &which);

	int isInPlayerFile;
	int number;
	if (items < 1) {
		isInPlayerFile = tDefaultIsInFile;
		number = tDefaultNumber;
	}
	else if (!strcmp("isinotherfile", firstW)) {
		assert(items == 2);
		isInPlayerFile = 1;
		number = which;
	}
	else {
		assert(items == 1);
		isInPlayerFile = 0;
		number = atoi(flag);
	}

	tFunc(tPlayer, isInPlayerFile, number);

	freeMemory(flag);
}

static void handleHitDefinitionSparkXY(MugenAssignment* tAssignment, Player* tPlayer) {
	int x, y;

	getTwoIntegerValuesWithDefaultValues(tAssignment, tPlayer, &x, &y, 0, 0);
	setHitDataSparkXY(tPlayer, x, y);
}

static void handleHitDefinitionSingleSound(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, int, int, int), int tDefaultGroup, int tDefaultItem) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(flag);

	char firstW[200], comma[10];
	int items = sscanf(flag, "%s", firstW);
	
	int group;
	int item;
	int isInPlayerFile;
	if (items < 1) {
		isInPlayerFile = 0;
		group = tDefaultGroup;
		item = tDefaultItem;
	}
	else if (!strcmp("isinotherfile", firstW)) {
		isInPlayerFile = 1;
		int fullItems = sscanf(flag, "%s %d %s %d", firstW, &group, comma, &item);
		assert(fullItems >= 2);

		if (fullItems < 3) {
			item = tDefaultItem;
		}
	}
	else {
		isInPlayerFile = 0;
		int fullItems = sscanf(flag, "%d %s %d", &group, comma, &item);
		assert(fullItems >= 1);

		if (fullItems < 2) {
			item = tDefaultItem;
		}
	}

	tFunc(tPlayer, isInPlayerFile, group, item);

	freeMemory(flag);
}

static void handleHitDefinitionSingleAttackHeight(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, MugenAttackHeight), MugenAttackHeight tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(flag);

	if (!strcmp("", flag)) tFunc(tPlayer, tDefault);
	else if (!strcmp("high", flag)) tFunc(tPlayer, MUGEN_ATTACK_HEIGHT_HIGH);
	else if (!strcmp("low", flag)) tFunc(tPlayer, MUGEN_ATTACK_HEIGHT_LOW);
	else if (!strcmp("trip", flag)) tFunc(tPlayer, MUGEN_ATTACK_HEIGHT_TRIP);
	else if (!strcmp("none", flag)) tFunc(tPlayer, MUGEN_ATTACK_HEIGHT_NONE);
	else {
		logError("Unable to parse hitdef attack height type.");
		logErrorString(flag);
		abortSystem();
	}

	freeMemory(flag);
}

static void handleExplodOneIntegerElement(MugenAssignment* tAssignment, Player* tPlayer, int tID, void(tFunc)(int, int), int tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	if (!strcmp("", flag)) tFunc(tID, tDefault);
	else tFunc(tID, atoi(flag));

	freeMemory(flag);
}

static void handleExplodTwoIntegerElements(MugenAssignment* tAssignment, Player* tPlayer, int tID, void(tFunc)(int, int, int), int tDefault1, int tDefault2) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string1[20], comma[10], string2[20];
	int items = sscanf(flag, "%s %s %s", string1, comma, string2);

	int val1;
	if (items < 1 || !strcmp("", string1)) val1 = tDefault1;
	else val1 = atoi(string1);

	int val2;
	if (items < 3 || !strcmp("", string2)) val2 = tDefault2;
	else val2 = atoi(string2);

	tFunc(tID, val1, val2);

	freeMemory(flag);
}

static void handleExplodThreeIntegerElements(MugenAssignment* tAssignment, Player* tPlayer, int tID, void(tFunc)(int, int, int, int), int tDefault1, int tDefault2, int tDefault3) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string[3][20], comma[2][10];
	int items = sscanf(flag, "%s %s %s %s %s", string[0], comma[0], string[1], comma[1], string[2]);

	int defaults[3];
	defaults[0] = tDefault1;
	defaults[1] = tDefault2;
	defaults[2] = tDefault3;

	int vals[3];
	int j;
	for (j = 0; j < 3; j++) {
		if (items < (1 + j * 2) || !strcmp("", string[j])) vals[j] = defaults[j];
		else vals[j] = atoi(string[j]);
	}

	tFunc(tID, vals[0], vals[1], vals[2]);

	freeMemory(flag);
}

static void handleExplodTwoFloatElements(MugenAssignment* tAssignment, Player* tPlayer, int tID, void(tFunc)(int, double, double), double tDefault1, double tDefault2) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string1[20], comma[10], string2[20];
	int items = sscanf(flag, "%s %s %s", string1, comma, string2);

	double val1;
	if (items < 1 || !strcmp("", string1)) val1 = tDefault1;
	else val1 = atof(string1);

	double val2;
	if (items < 3 || !strcmp("", string2)) val2 = tDefault2;
	else val2 = atof(string2);

	tFunc(tID, val1, val2);

	freeMemory(flag);
}

static void handleHitDefinitionOneIntegerElement(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, int), int tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	if (!strcmp("", flag)) tFunc(tPlayer, tDefault);
	else tFunc(tPlayer, atoi(flag));

	freeMemory(flag);
}

static void handleHitDefinitionTwoIntegerElements(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, int, int), int tDefault1, int tDefault2) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string1[20], comma[10], string2[20];
	int items = sscanf(flag, "%s %s %s", string1, comma, string2);

	int val1;
	if (items < 1 || !strcmp("", string1)) val1 = tDefault1;
	else val1 = atoi(string1);

	int val2;
	if (items < 3 || !strcmp("", string2)) val2 = tDefault2;
	else val2 = atoi(string2);

	tFunc(tPlayer, val1, val2);

	freeMemory(flag);
}

static void handleHitDefinitionThreeIntegerElements(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, int, int, int), int tDefault1, int tDefault2, int tDefault3) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string[3][20], comma[2][10];
	int items = sscanf(flag, "%s %s %s %s %s", string[0], comma[0], string[1], comma[1], string[2]);

	int defaults[3];
	defaults[0] = tDefault1;
	defaults[1] = tDefault2;
	defaults[2] = tDefault3;

	int vals[3];
	int j;
	for (j = 0; j < 3; j++) {
		if (items < (1 + j * 2) || !strcmp("", string[j])) vals[j] = defaults[j];
		else vals[j] = atoi(string[j]);
	}

	tFunc(tPlayer, vals[0], vals[1], vals[2]);

	freeMemory(flag);
}

static void handleHitDefinitionOneFloatElement(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, double), double tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	if (!strcmp("", flag)) tFunc(tPlayer, tDefault);
	else tFunc(tPlayer, atof(flag));

	freeMemory(flag);
}

static void handleHitDefinitionTwoFloatElements(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, double, double), double tDefault1, double tDefault2) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string1[20], comma[10], string2[20];
	int items = sscanf(flag, "%s %s %s", string1, comma, string2);

	double val1;
	if (items < 1 || !strcmp("", string1)) val1 = tDefault1;
	else val1 = atof(string1);

	double val2;
	if (items < 3 || !strcmp("", string2)) val2 = tDefault2;
	else val2 = atof(string2);

	tFunc(tPlayer, val1, val2);

	freeMemory(flag);
}

static void handleHitDefinitionTwoOptionalIntegerElements(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, int, int), void(tFuncDisable)(Player*)) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	if (!strcmp("", flag)) {
		tFuncDisable(tPlayer);
	}

	freeMemory(flag);

	int x, y;
	getTwoIntegerValuesWithDefaultValues(tAssignment, tPlayer, &x, &y, 0, 0);

	tFunc(tPlayer, x, y);
}

static void handleHitDefinitionSinglePowerAddition(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Player*, int, int), double tDefaultFactor, int tDamage) {

	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	char string1[20], comma[10], string2[20];
	int items = sscanf(flag, "%s %s %s", string1, comma, string2);

	int val1;
	if (items < 1 || !strcmp("", string1)) val1 = (int)(tDamage*tDefaultFactor);
	else val1 = atoi(string1);

	int val2;
	if (items < 3 || !strcmp("", string2)) val2 = val1 / 2;
	else val2 = atoi(string2);

	tFunc(tPlayer, val1, val2);

	freeMemory(flag);
}

static void handleHitDefinition(MugenStateController* tController, Player* tPlayer) {
	HitDefinitionController* e = tController->mData;

	setHitDataVelocityX(tPlayer, 0);
	setHitDataVelocityY(tPlayer, 0);

	handleHitDefinitionAttribute(e, tPlayer);
	handleHitDefinitionSingleHitFlag(e->mHitFlag, tPlayer, setHitDataHitFlag);
	handleHitDefinitionSingleHitFlag(e->mGuardFlag, tPlayer, setHitDataGuardFlag);
	handleHitDefinitionAffectTeam(e->mAffectTeam, tPlayer);
	handleHitDefinitionSingleAnimationType(e->mAnimationType, tPlayer, setHitDataAnimationType, MUGEN_HIT_ANIMATION_TYPE_LIGHT);
	handleHitDefinitionSingleAnimationType(e->mAirAnimationType, tPlayer, setHitDataAirAnimationType, getHitDataAnimationType(tPlayer));
	handleHitDefinitionSingleAnimationType(e->mFallAnimationType, tPlayer, setHitDataFallAnimationType, getHitDataAirAnimationType(tPlayer) == MUGEN_HIT_ANIMATION_TYPE_UP ? MUGEN_HIT_ANIMATION_TYPE_UP : MUGEN_HIT_ANIMATION_TYPE_BACK);
	handleHitDefinitionPriority(e->mPriority, tPlayer);
	handleHitDefinitionDamage(e->mDamage, tPlayer);
	handleHitDefinitionSinglePauseTime(e->mPauseTime, tPlayer, setHitDataPauseTime, 0, 0);
	handleHitDefinitionSinglePauseTime(e->mGuardPauseTime, tPlayer, setHitDataGuardPauseTime, getHitDataPlayer1PauseTime(tPlayer), getHitDataPlayer2PauseTime(tPlayer));
	
	handleHitDefinitionSparkNumberSingle(e->mSparkNumber, tPlayer, setHitDataSparkNumber, getDefaultPlayerSparkNumberIsInPlayerFile(tPlayer), getDefaultPlayerSparkNumber(tPlayer));
	handleHitDefinitionSparkNumberSingle(e->mGuardSparkNumber, tPlayer, setHitDataGuardSparkNumber, getDefaultPlayerGuardSparkNumberIsInPlayerFile(tPlayer), getDefaultPlayerGuardSparkNumber(tPlayer)); 
	handleHitDefinitionSparkXY(e->mSparkXY, tPlayer);
	handleHitDefinitionSingleSound(e->mHitSound, tPlayer, setHitDataHitSound, 0, 0); // TODO: proper default
	handleHitDefinitionSingleSound(e->mGuardSound, tPlayer, setHitDataGuardSound, 0, 0); // TODO: proper default

	handleHitDefinitionSingleAttackHeight(e->mGroundType, tPlayer, setHitDataGroundType, MUGEN_ATTACK_HEIGHT_HIGH);
	handleHitDefinitionSingleAttackHeight(e->mAirType, tPlayer, setHitDataAirType, getHitDataGroundType(tPlayer));

	handleHitDefinitionOneIntegerElement(e->mGroundHitTime, tPlayer, setHitDataGroundHitTime, 0);
	handleHitDefinitionOneIntegerElement(e->mGroundSlideTime, tPlayer, setHitDataGroundSlideTime, 0);
	handleHitDefinitionOneIntegerElement(e->mGuardHitTime, tPlayer, setHitDataGuardHitTime, getHitDataGroundHitTime(tPlayer));
	handleHitDefinitionOneIntegerElement(e->mGuardSlideTime, tPlayer, setHitDataGuardSlideTime, getHitDataGuardHitTime(tPlayer));
	handleHitDefinitionOneIntegerElement(e->mAirHitTime, tPlayer, setHitDataAirHitTime, 20);
	handleHitDefinitionOneIntegerElement(e->mGuardControlTime, tPlayer, setHitDataGuardControlTime, getHitDataGuardSlideTime(tPlayer));
	handleHitDefinitionOneIntegerElement(e->mGuardDistance, tPlayer, setHitDataGuardDistance, getDefaultPlayerAttackDistance(tPlayer)); 
	handleHitDefinitionOneFloatElement(e->mYAccel, tPlayer, setHitDataYAccel, transformCoordinates(0.7, 480, getPlayerCoordinateP(tPlayer)));
	handleHitDefinitionTwoFloatElements(e->mGroundVelocity, tPlayer, setHitDataGroundVelocity, 0, 0);
	handleHitDefinitionOneFloatElement(e->mGuardVelocity, tPlayer, setHitDataGuardVelocity, getHitDataGroundVelocityX(tPlayer));
	handleHitDefinitionTwoFloatElements(e->mAirVelocity, tPlayer, setHitDataAirVelocity, 0, 0);
	handleHitDefinitionTwoFloatElements(e->mAirGuardVelocity, tPlayer, setHitDataAirGuardVelocity, getHitDataAirVelocityX(tPlayer) * 1.5, getHitDataAirVelocityY(tPlayer) / 2);

	handleHitDefinitionOneFloatElement(e->mGroundCornerPushVelocityOffset, tPlayer, setGroundCornerPushVelocityOffset, getHitDataAttackType(tPlayer) == MUGEN_ATTACK_TYPE_ATTACK ? 0 : 1.3*getHitDataGuardVelocity(tPlayer));
	handleHitDefinitionOneFloatElement(e->mAirCornerPushVelocityOffset, tPlayer, setAirCornerPushVelocityOffset, getGroundCornerPushVelocityOffset(tPlayer));
	handleHitDefinitionOneFloatElement(e->mDownCornerPushVelocityOffset, tPlayer, setDownCornerPushVelocityOffset, getGroundCornerPushVelocityOffset(tPlayer));
	handleHitDefinitionOneFloatElement(e->mGuardCornerPushVelocityOffset, tPlayer, setGuardCornerPushVelocityOffset, getGroundCornerPushVelocityOffset(tPlayer));
	handleHitDefinitionOneFloatElement(e->mAirGuardCornerPushVelocityOffset, tPlayer, setAirGuardCornerPushVelocityOffset, getGuardCornerPushVelocityOffset(tPlayer));

	handleHitDefinitionOneIntegerElement(e->mAirGuardControlTime, tPlayer, setHitDataAirGuardControlTime, getHitDataGuardControlTime(tPlayer));
	handleHitDefinitionOneIntegerElement(e->mAirJuggle, tPlayer, setHitDataAirJuggle, 0);

	handleHitDefinitionTwoOptionalIntegerElements(e->mMinimumDistance, tPlayer, setHitDataMinimumDistance, setHitDataMinimumDistanceInactive);
	handleHitDefinitionTwoOptionalIntegerElements(e->mMaximumDistance, tPlayer, setHitDataMaximumDistance, setHitDataMaximumDistanceInactive);
	handleHitDefinitionTwoOptionalIntegerElements(e->mSnap, tPlayer, setHitDataSnap, setHitDataSnapInactive);

	handleHitDefinitionOneIntegerElement(e->mPlayerSpritePriority1, tPlayer, setHitDataPlayer1SpritePriority, 1);
	handleHitDefinitionOneIntegerElement(e->mPlayerSpritePriority2, tPlayer, setHitDataPlayer2SpritePriority, 0);

	handleHitDefinitionOneIntegerElement(e->mPlayer1ChangeFaceDirection, tPlayer, setHitDataPlayer1FaceDirection, 0);
	handleHitDefinitionOneIntegerElement(e->mPlayer1ChangeFaceDirectionRelativeToPlayer2, tPlayer, setHitDataPlayer1ChangeFaceDirectionRelativeToPlayer2, 0);
	handleHitDefinitionOneIntegerElement(e->mPlayer2ChangeFaceDirectionRelativeToPlayer1, tPlayer, setHitDataPlayer2ChangeFaceDirectionRelativeToPlayer1, 0);

	handleHitDefinitionOneIntegerElement(e->mPlayer1StateNumber, tPlayer, setPlayer1StateNumber, -1);
	handleHitDefinitionOneIntegerElement(e->mPlayer2StateNumber, tPlayer, setPlayer2StateNumber, -1);
	handleHitDefinitionOneIntegerElement(e->mPlayer2CapableOfGettingPlayer1State, tPlayer, setHitDataPlayer2CapableOfGettingPlayer1State, 1);
	handleHitDefinitionOneIntegerElement(e->mForceStanding, tPlayer, setHitDataForceStanding, getHitDataGroundVelocityY(tPlayer) != 0 ? 1 : 0);

	handleHitDefinitionOneIntegerElement(e->mFall, tPlayer, setHitDataFall, 0);
	handleHitDefinitionOneFloatElement(e->mFallXVelocity, tPlayer, setHitDataFallXVelocity, 0);
	handleHitDefinitionOneFloatElement(e->mFallYVelocity, tPlayer, setHitDataFallYVelocity, transformCoordinates(-9, 480, getPlayerCoordinateP(tPlayer)));
	handleHitDefinitionOneIntegerElement(e->mFallCanBeRecovered, tPlayer, setHitDataFallRecovery, 1);
	handleHitDefinitionOneIntegerElement(e->mFallRecoveryTime, tPlayer, setHitDataFallRecoveryTime, 4);
	handleHitDefinitionOneIntegerElement(e->mFallDamage, tPlayer, setHitDataFallDamage, 0);
	handleHitDefinitionOneIntegerElement(e->mAirFall, tPlayer, setHitDataAirFall, getHitDataFall(tPlayer));
	handleHitDefinitionOneIntegerElement(e->mForceNoFall, tPlayer, setHitDataForceNoFall, 0);

	handleHitDefinitionTwoFloatElements(e->mDownVelocity, tPlayer, setHitDataDownVelocity, getHitDataAirVelocityX(tPlayer), getHitDataAirVelocityY(tPlayer));
	handleHitDefinitionOneIntegerElement(e->mDownHitTime, tPlayer, setHitDataDownHitTime, 0);
	handleHitDefinitionOneIntegerElement(e->mDownBounce, tPlayer, setHitDataDownBounce, 0);

	handleHitDefinitionOneIntegerElement(e->mHitID, tPlayer, setHitDataHitID, 0);
	handleHitDefinitionOneIntegerElement(e->mChainID, tPlayer, setHitDataChainID, -1);
	handleHitDefinitionTwoIntegerElements(e->mNoChainID, tPlayer, setHitDataNoChainID, -1, -1);
	handleHitDefinitionOneIntegerElement(e->mHitOnce, tPlayer, setHitDataHitOnce, 1);

	handleHitDefinitionOneIntegerElement(e->mKill, tPlayer, setHitDataKill, 1);
	handleHitDefinitionOneIntegerElement(e->mGuardKill, tPlayer, setHitDataGuardKill, 1);
	handleHitDefinitionOneIntegerElement(e->mFallKill, tPlayer, setHitDataFallKill, 1);
	handleHitDefinitionOneIntegerElement(e->mNumberOfHits, tPlayer, setHitDataNumberOfHits, 1);
	handleHitDefinitionSinglePowerAddition(e->mGetPower, tPlayer, setHitDataGetPower, getDefaultAttackDamageDoneToPowerMultiplier(), getHitDataDamage(tPlayer));
	handleHitDefinitionSinglePowerAddition(e->mGivePower, tPlayer, setHitDataGivePower, getDefaultAttackDamageReceivedToPowerMultiplier(), getHitDataDamage(tPlayer));

	handleHitDefinitionOneIntegerElement(e->mPaletteEffectTime, tPlayer, setHitDataPaletteEffectTime, 0);
	handleHitDefinitionThreeIntegerElements(e->mPaletteEffectMultiplication, tPlayer, setHitDataPaletteEffectMultiplication, 1, 1, 1);
	handleHitDefinitionThreeIntegerElements(e->mPaletteEffectAddition, tPlayer, setHitDataPaletteEffectAddition, 0, 0, 0);

	handleHitDefinitionOneIntegerElement(e->mEnvironmentShakeTime, tPlayer, setHitDataEnvironmentShakeTime, 0); // TODO: proper defaults for the whole screen shake block
	handleHitDefinitionOneFloatElement(e->mEnvironmentShakeFrequency, tPlayer, setHitDataEnvironmentShakeFrequency, 0);
	handleHitDefinitionOneIntegerElement(e->mEnvironmentShakeAmplitude, tPlayer, setHitDataEnvironmentShakeAmplitude, 0);
	handleHitDefinitionOneFloatElement(e->mEnvironmentShakePhase, tPlayer, setHitDataEnvironmentShakePhase, 0);

	handleHitDefinitionOneIntegerElement(e->mFallEnvironmentShakeTime, tPlayer, setHitDataFallEnvironmentShakeTime, 0); // TODO: proper defaults for the whole screen shake block
	handleHitDefinitionOneFloatElement(e->mFallEnvironmentShakeFrequency, tPlayer, setHitDataFallEnvironmentShakeFrequency, 0);
	handleHitDefinitionOneIntegerElement(e->mFallEnvironmentShakeAmplitude, tPlayer, setHitDataFallEnvironmentShakeAmplitude, 0);
	handleHitDefinitionOneFloatElement(e->mFallEnvironmentShakePhase, tPlayer, setHitDataFallEnvironmentShakePhase, 0);

	setHitDataIsFacingRight(tPlayer, getPlayerIsFacingRight(tPlayer));

	setHitDataActive(tPlayer);
}

static void handleAnimationChange(MugenStateController* tController, Player* tPlayer) {
	ChangeAnimationController* e = tController->mData;

	int animation = evaluateAssignmentAndReturnAsInteger(e->tNewAnimation, tPlayer);
	int step = evaluateAssignmentAndReturnAsInteger(e->tStep, tPlayer);
	changePlayerAnimationWithStartStep(tPlayer, animation, step);
}

static void handleAnimationChange2(MugenStateController* tController, Player* tPlayer) {
	ChangeAnimationController* e = tController->mData;

	int animation = evaluateAssignmentAndReturnAsInteger(e->tNewAnimation, tPlayer);
	int step = evaluateAssignmentAndReturnAsInteger(e->tStep, tPlayer);
	changePlayerAnimationToPlayer2AnimationWithStartStep(tPlayer, animation, step);
}

static void handleControlSetting(MugenStateController* tController, Player* tPlayer) {
	ControlSettingController* e = tController->mData;

	int control = evaluateAssignmentAndReturnAsInteger(e->tValue, tPlayer);
	setPlayerControl(tPlayer, control);
}

static void handleWidth(MugenStateController* tController, Player* tPlayer) {
	WidthController* e = tController->mData;

	// TODO
	(void)e;
	(void)tPlayer;
}

static void handleSpritePriority(MugenStateController* tController, Player* tPlayer) {
	SpritePriorityController* e = tController->mData;

	int value = evaluateAssignmentAndReturnAsInteger(e->tValue, tPlayer);
	setPlayerSpritePriority(tPlayer, value);
}

static void handleSingleSpecialAssert(MugenAssignment* tAssignment, Player* tPlayer) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(flag);

	if (!strcmp("nowalk", flag)) {
		setPlayerNoWalkFlag(tPlayer);
	}
	else if (!strcmp("noautoturn", flag)) {
		setPlayerNoAutoTurnFlag(tPlayer);
	}
	else if (!strcmp("invisible", flag)) {
		setPlayerInvisibleFlag(tPlayer);
	}
	else if (!strcmp("noshadow", flag)) {
		setPlayerNoShadow(tPlayer);
	}
	else if (!strcmp("nojugglecheck", flag)) {
		setPlayerNoJuggleCheckFlag(tPlayer);
	}
	else if (!strcmp("intro", flag)) {
		setPlayerIntroFlag(tPlayer);
	}
	else if (!strcmp("noairguard", flag)) {
		setPlayerNoAirGuardFlag(tPlayer);
	}
	else if (!strcmp("nobardisplay", flag)) {
		setBarInvisibleForOneFrame();
	}
	else {
		logError("Unrecognized special assert flag.");
		logErrorString(flag);
		abortSystem();
	}
	freeMemory(flag);
}

static void handleSpecialAssert(MugenStateController* tController, Player* tPlayer) {
	SpecialAssertController* e = tController->mData;

	handleSingleSpecialAssert(e->mFlag, tPlayer);

	if (e->mHasFlag2) {
		handleSingleSpecialAssert(e->mFlag2, tPlayer);
	}
	if (e->mHasFlag3) {
		handleSingleSpecialAssert(e->mFlag3, tPlayer);
	}

}

static void handleMakeDust(MugenStateController* tController, Player* tPlayer) {
	MakeDustController* e = tController->mData;

	// TODO
	(void)e;
	(void)tPlayer;
}

typedef struct {
	Player* mPlayer;

} VarSetHandlingCaller;

static void handleSettingSingleVariable(void* tCaller, void* tData) {
	VarSetHandlingCaller* caller = tCaller;
	VarSetControllerEntry* e = tData;

	if (e->mType == VAR_SET_TYPE_SYSTEM) {
		int val = evaluateAssignmentAndReturnAsInteger(e->mAssignment, caller->mPlayer);
		setPlayerSystemVariable(caller->mPlayer, e->mID, val);
	}
	else if (e->mType == VAR_SET_TYPE_INTEGER) {
		int val = evaluateAssignmentAndReturnAsInteger(e->mAssignment, caller->mPlayer);
		setPlayerVariable(caller->mPlayer, e->mID, val);
	}
	else if (e->mType == VAR_SET_TYPE_SYSTEM_FLOAT) {
		double val = evaluateAssignmentAndReturnAsFloat(e->mAssignment, caller->mPlayer);
		setPlayerSystemFloatVariable(caller->mPlayer, e->mID, val);
	}
	else if (e->mType == VAR_SET_TYPE_FLOAT) {
		double val = evaluateAssignmentAndReturnAsFloat(e->mAssignment, caller->mPlayer);
		setPlayerFloatVariable(caller->mPlayer, e->mID, val);
	}
	else {
		logError("Unrecognized variable type.");
		logErrorInteger(e->mType);
		abortSystem();
	}
}

static void handleSettingVariable(MugenStateController* tController, Player* tPlayer) {
	VarSetController* e = tController->mData;
	VarSetHandlingCaller caller;
	caller.mPlayer = tPlayer;

	vector_map(&e->mVarSets, handleSettingSingleVariable, &caller);
}

static void handleAddingSingleVariable(void* tCaller, void* tData) {
	VarSetHandlingCaller* caller = tCaller;
	VarSetControllerEntry* e = tData;

	if (e->mType == VAR_SET_TYPE_SYSTEM) {
		int val = evaluateAssignmentAndReturnAsInteger(e->mAssignment, caller->mPlayer);
		addPlayerSystemVariable(caller->mPlayer, e->mID, val);
	}
	else if (e->mType == VAR_SET_TYPE_INTEGER) {
		int val = evaluateAssignmentAndReturnAsInteger(e->mAssignment, caller->mPlayer);
		addPlayerVariable(caller->mPlayer, e->mID, val);
	}
	else if (e->mType == VAR_SET_TYPE_SYSTEM_FLOAT) {
		double val = evaluateAssignmentAndReturnAsFloat(e->mAssignment, caller->mPlayer);
		addPlayerSystemFloatVariable(caller->mPlayer, e->mID, val);
	}
	else if (e->mType == VAR_SET_TYPE_FLOAT) {
		double val = evaluateAssignmentAndReturnAsFloat(e->mAssignment, caller->mPlayer);
		addPlayerFloatVariable(caller->mPlayer, e->mID, val);
	}
	else {
		logError("Unrecognized variable type.");
		logErrorInteger(e->mType);
		abortSystem();
	}
}

static void handleAddingVariable(MugenStateController* tController, Player* tPlayer) {
	VarSetController* e = tController->mData;
	VarSetHandlingCaller caller;
	caller.mPlayer = tPlayer;

	vector_map(&e->mVarSets, handleAddingSingleVariable, &caller);
}

static void handleNull() {}

static MugenStateType handleStateTypeAssignment(MugenAssignment* tAssignment, Player* tPlayer) {
	MugenStateType ret;

	char* text = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(text);
	
	if (!strcmp("a", text)) ret = MUGEN_STATE_TYPE_AIR;
	else if (!strcmp("s", text)) ret = MUGEN_STATE_TYPE_STANDING;
	else if (!strcmp("c", text)) ret = MUGEN_STATE_TYPE_CROUCHING;
	else {
		logError("Unrecognized state type");
		logErrorString(text);
		abortSystem();
		ret = MUGEN_STATE_TYPE_UNCHANGED;
	}
	freeMemory(text);

	return ret;
}

static MugenStateType handleStatePhysicsAssignment(MugenAssignment* tAssignment, Player* tPlayer) {
	MugenStatePhysics ret;

	char* text = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(text);
	if (!strcmp("s", text)) ret = MUGEN_STATE_PHYSICS_STANDING;
	else if (!strcmp("a", text)) ret = MUGEN_STATE_PHYSICS_AIR;
	else if (!strcmp("c", text)) ret = MUGEN_STATE_PHYSICS_CROUCHING;
	else if (!strcmp("n", text)) ret = MUGEN_STATE_PHYSICS_NONE;
	else {
		logError("Unrecognized state physics type");
		logErrorString(text);
		abortSystem();
		ret = MUGEN_STATE_PHYSICS_UNCHANGED;
	}
	freeMemory(text);

	return ret;
}

static MugenStateMoveType handleStateMoveTypeAssignment(MugenAssignment* tAssignment, Player* tPlayer) {
	MugenStateMoveType ret;

	char* text = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	if (!strcmp("A", text)) ret = MUGEN_STATE_MOVE_TYPE_ATTACK;
	else if (!strcmp("I", text)) ret = MUGEN_STATE_MOVE_TYPE_IDLE;
	else {
		logError("Unrecognized state move type");
		logErrorString(text);
		abortSystem();
		ret = MUGEN_STATE_MOVE_TYPE_UNCHANGED;
	}
	freeMemory(text);

	return ret;
}

static void handleStateTypeSet(MugenStateController* tController, Player* tPlayer) {
	StateTypeSetController* e = tController->mData;

	if (e->mHasStateType) {
		MugenStateType type = handleStateTypeAssignment(e->mStateType, tPlayer);
		setPlayerStateType(tPlayer, type);
	}

	if (e->mHasPhysics) {
		MugenStatePhysics physics = handleStatePhysicsAssignment(e->mPhysics, tPlayer);
		setPlayerPhysics(tPlayer, physics);
	}

	if (e->mHasMoveType) {
		MugenStateMoveType moveType = handleStateMoveTypeAssignment(e->mMoveType, tPlayer);
		setPlayerStateMoveType(tPlayer, moveType);
	}
}

static void handleHitVelocitySetting(MugenStateController* tController, Player* tPlayer) {
	Set2DPhysicsController* e = tController->mData;

	if (e->mIsSettingX) {
		double x = evaluateAssignmentAndReturnAsFloat(e->x, tPlayer);
		setActiveHitDataVelocityX(tPlayer, x); // TODO: check
	}

	if (e->mIsSettingY) {
		double y = evaluateAssignmentAndReturnAsFloat(e->y, tPlayer);
		setActiveHitDataVelocityY(tPlayer, y); // TODO: check
	}
}

static void handleDefenseMultiplier(MugenStateController* tController, Player* tPlayer) {
	DefenseMultiplierController* e = tController->mData;

	double val = evaluateAssignmentAndReturnAsFloat(e->mValue, tPlayer);
	setPlayerDefenseMultiplier(tPlayer, val);
}

static void handleFallEnvironmentShake() {
	// TODO: environment shake

}

static void handleExplodAnimation(MugenAssignment* tAssignment, Player* tPlayer, int tID) {
	char* text = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(text);

	char* numberPos;
	int isInFightDefFile;
	if (text[0] == 'f') {
		isInFightDefFile = 1;
		numberPos = text + 1;
	}
	else {
		isInFightDefFile = 0;
		numberPos = text;
	}
	int number = atoi(numberPos);

	setExplodAnimation(tID, isInFightDefFile, number);

	freeMemory(text);
}

ExplodPositionType getPositionTypeFromAssignment(MugenAssignment* tAssignment, Player* tPlayer) {
	char* text = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	ExplodPositionType type;
	if (!strcmp("p1", text)) {
		type = EXPLOD_POSITION_TYPE_RELATIVE_TO_P1;
	}
	else if (!strcmp("p2", text)) {
		type = EXPLOD_POSITION_TYPE_RELATIVE_TO_P2;
	}
	else if (!strcmp("front", text)) {
		type = EXPLOD_POSITION_TYPE_RELATIVE_TO_FRONT;
	}
	else if (!strcmp("back", text)) {
		type = EXPLOD_POSITION_TYPE_RELATIVE_TO_BACK;
	}
	else if (!strcmp("left", text)) {
		type = EXPLOD_POSITION_TYPE_RELATIVE_TO_LEFT;
	}
	else if (!strcmp("right", text)) {
		type = EXPLOD_POSITION_TYPE_RELATIVE_TO_RIGHT;
	}
	else if (!strcmp("none", text)) {
		type = EXPLOD_POSITION_TYPE_NONE;
	}
	else {
		logError("Unable to determine position type");
		logErrorString(text);
		abortSystem();
		type = EXPLOD_POSITION_TYPE_RELATIVE_TO_P1;
	}

	freeMemory(text);

	return type;
}

static void handleExplodPositionType(MugenAssignment* tAssignment, Player* tPlayer, int tID) {

	ExplodPositionType type = getPositionTypeFromAssignment(tAssignment, tPlayer);
	setExplodPositionType(tID, type);
}

static void handleExplodTransparencyType(MugenAssignment* tAssignment, int isUsed, Player* tPlayer, int tID) {
	if (!isUsed) {
		setExplodTransparencyType(tID, 0, EXPLOD_TRANSPARENCY_TYPE_ALPHA);
		return;
	}

	char* text = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	ExplodTransparencyType type;
	if (!strcmp("alpha", text)) {
		type = EXPLOD_TRANSPARENCY_TYPE_ALPHA;
	}
	else if (!strcmp("addalpha", text)) {
		type = EXPLOD_TRANSPARENCY_TYPE_ADD_ALPHA;
	}
	else {
		logError("Unable to determine explod transparency type");
		logErrorString(text);
		abortSystem();
		type = EXPLOD_TRANSPARENCY_TYPE_ALPHA;
	}

	setExplodTransparencyType(tID, 1, type);

	freeMemory(text);
}

static void handleExplod(MugenStateController* tController, Player* tPlayer) {
	ExplodController* e = tController->mData;

	int id = addExplod(tPlayer);
	handleExplodAnimation(e->mAnim, tPlayer, id);
	handleExplodOneIntegerElement(e->mID, tPlayer, id, setExplodID, -1);
	handleExplodTwoIntegerElements(e->mPosition, tPlayer, id, setExplodPosition, 0, 0);
	handleExplodPositionType(e->mPositionType, tPlayer, id);
	handleExplodOneIntegerElement(e->mHorizontalFacing, tPlayer, id, setExplodHorizontalFacing, 1);
	handleExplodOneIntegerElement(e->mVerticalFacing, tPlayer, id, setExplodVerticalFacing, 1);
	handleExplodOneIntegerElement(e->mBindTime, tPlayer, id, setExplodBindTime, -1);
	handleExplodTwoFloatElements(e->mVelocity, tPlayer, id, setExplodVelocity, 0, 0);
	handleExplodTwoFloatElements(e->mAcceleration, tPlayer, id, setExplodAcceleration, 0, 0);
	handleExplodTwoIntegerElements(e->mRandomOffset, tPlayer, id, setExplodRandomOffset, 0, 0);

	handleExplodOneIntegerElement(e->mRemoveTime, tPlayer, id, setExplodRemoveTime, -2);
	handleExplodOneIntegerElement(e->mSuperMove, tPlayer, id, setExplodSuperMove, 0);
	handleExplodOneIntegerElement(e->mSuperMoveTime, tPlayer, id, setExplodSuperMoveTime, 0);
	handleExplodOneIntegerElement(e->mPauseMoveTime, tPlayer, id, setExplodPauseMoveTime, 0);
	handleExplodTwoFloatElements(e->mScale, tPlayer, id, setExplodScale, 1, 1);
	handleExplodOneIntegerElement(e->mSpritePriority, tPlayer, id, setExplodSpritePriority, 0);
	handleExplodOneIntegerElement(e->mOnTop, tPlayer, id, setExplodOnTop, 0);
	handleExplodThreeIntegerElements(e->mShadow, tPlayer, id, setExplodShadow, 0, 0, 0);
	handleExplodOneIntegerElement(e->mOwnPalette, tPlayer, id, setExplodOwnPalette, 0);
	handleExplodOneIntegerElement(e->mIsRemovedOnGetHit, tPlayer, id, setExplodRemoveOnGetHit, 0);
	handleExplodOneIntegerElement(e->mIgnoreHitPause, tPlayer, id, setExplodIgnoreHitPause, 1);
	handleExplodTransparencyType(e->mTransparencyType, e->mHasTransparencyType, tPlayer, id);

}

static void handleHitFallDamage() {
	// TODO: read out fall damage
}

static void handlePositionFreeze(MugenStateController* tController, Player* tPlayer) {
	PositionFreezeController* e = tController->mData;

	int val = evaluateAssignment(e->mValue, tPlayer);

	if (val) setPlayerPositionFrozen(tPlayer);
}

static void handleHitFallVelocity(Player* tPlayer) {

	addPlayerVelocityX(tPlayer, getActiveHitDataFallXVelocity(tPlayer), getPlayerCoordinateP(tPlayer));
	addPlayerVelocityY(tPlayer, getActiveHitDataFallYVelocity(tPlayer), getPlayerCoordinateP(tPlayer));
}


static void handleSingleNotHitBy(int tSlot, int tHasValue, MugenStringVector tValue, int tTime, Player* tPlayer, void(*tResetFunc)(Player*, int)) {
	if (!tHasValue) return;
	tResetFunc(tPlayer, tSlot);

	int position = 0;
	char* flag1 = tValue.mElement[position++];
	setPlayerNotHitByFlag1(tPlayer, tSlot, flag1);
	int isRunning = 1;
	while (isRunning) {
		if (!strcmp("", tValue.mElement[position])) break;

		char* flag2 = tValue.mElement[position++];
		addPlayerNotHitByFlag2(tPlayer, tSlot, flag2);
	}

	setPlayerNotHitByTime(tPlayer, tSlot, tTime);
}

static void handleReversalDefinitionEntry(MugenStringVector tValue, Player* tPlayer) {
	resetHitDataReversalDef(tPlayer);
	
	int position = 0;
	char* flag1 = tValue.mElement[position++];
	setHitDataReversalDefFlag1(tPlayer, flag1);
	int isRunning = 1;
	while (isRunning) {
		if (!strcmp("", tValue.mElement[position])) break;

		char* flag2 = tValue.mElement[position++];
		addHitDataReversalDefFlag2(tPlayer, flag2);
	}
}



static void handleNotHitBy(MugenStateController* tController, Player* tPlayer) {
	NotHitByController* e = tController->mData;

	int time = evaluateAssignmentAndReturnAsInteger(e->mTime, tPlayer);

	handleSingleNotHitBy(0, e->mHasValue, e->mValue, time, tPlayer, resetPlayerNotHitBy);
	handleSingleNotHitBy(1, e->mHasValue2, e->mValue2, time, tPlayer, resetPlayerNotHitBy);
}

static void handleHitBy(MugenStateController* tController, Player* tPlayer) {
	NotHitByController* e = tController->mData;

	int time = evaluateAssignmentAndReturnAsInteger(e->mTime, tPlayer);

	handleSingleNotHitBy(0, e->mHasValue, e->mValue, time, tPlayer, resetPlayerHitBy);
	handleSingleNotHitBy(1, e->mHasValue2, e->mValue2, time, tPlayer, resetPlayerHitBy);
}

static void handleHitFallSet(MugenStateController* tController, Player* tPlayer) {
	HitFallSetController* e = tController->mData;
	Player* otherPlayer = getPlayerOtherPlayer(tPlayer);

	int val = evaluateAssignmentAndReturnAsInteger(e->mValue, tPlayer);
	if (val != -1) {
		setActiveHitDataFall(otherPlayer, val);
	}

	if (e->mHasXVelocity) {
		double x = evaluateAssignmentAndReturnAsFloat(e->mXVelocity, tPlayer);
		setActiveHitDataFallXVelocity(otherPlayer, x);
	}

	if (e->mHasYVelocity) {
		double y = evaluateAssignmentAndReturnAsFloat(e->mYVelocity, tPlayer);
		setActiveHitDataFallYVelocity(otherPlayer, y);
	}
}


static void handleAttackMultiplierSetting(MugenStateController* tController, Player* tPlayer) {
	SingleRequiredValueController* e = tController->mData;

	double value = evaluateAssignmentAndReturnAsFloat(e->mValue, tPlayer);

	setPlayerAttackMultiplier(tPlayer, value);
}

static void handlePowerAddition(MugenStateController* tController, Player* tPlayer) {
	SingleRequiredValueController* e = tController->mData;

	int value = evaluateAssignmentAndReturnAsInteger(e->mValue, tPlayer);

	addPlayerPower(tPlayer, value);
}

static void handleEnvironmentShake(MugenStateController* tController, Player* tPlayer) {
	EnvironmentShakeController* e = tController->mData;

	int time = evaluateAssignmentAndReturnAsInteger(e->mTime, tPlayer);

	double frequency;
	int amplitude;
	double phase;
	getSingleFloatValueOrDefault(e->mFrequency, tPlayer, &frequency, 60);
	getSingleIntegerValueOrDefault(e->mAmplitude, tPlayer, &amplitude, (int)transformCoordinates(-4, 240, getStageCoordinateP()));
	getSingleFloatValueOrDefault(e->mPhaseOffset, tPlayer, &phase, 90);

	setMugenStageHandlerScreenShake(time, frequency, amplitude, phase);
}

static void getSingleIntegerValueOrDefaultFunctionCall(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(int), int tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	int val;
	if (!strcmp("", flag)) val = tDefault;
	else val = atoi(flag);
	tFunc(val);

	freeMemory(flag);
}

static void getSingleFloatValueOrDefaultFunctionCall(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(double), double tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	double val;
	if (!strcmp("", flag)) val = tDefault;
	else val = atof(flag);
	tFunc(val);

	freeMemory(flag);
}

static void getSingleVector3DValueOrDefaultFunctionCall(MugenAssignment* tAssignment, Player* tPlayer, void(tFunc)(Vector3D), Vector3D tDefault) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);

	double x, y, z;
	char tx[20], comma1[10], ty[20], comma2[10], tz[20];
	int items = sscanf(flag, "%s %s %s %s %s", tx, comma1, ty, comma2, tz);

	if (items < 1) x = tDefault.x;
	else x = atof(tx);

	if (items < 3) y = tDefault.y;
	else y = atof(ty);

	if (items < 5) z = tDefault.z;
	else z = atof(tz);

	tFunc(makePosition(x, y, z));

	freeMemory(flag);
}

static void handleSuperPauseAnimation(MugenAssignment* tAssignment, Player* tPlayer) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(flag);

	char firstW[200];
	int which = -1;

	int items = sscanf(flag, "%s %d", firstW, &which);

	int isInPlayerFile;
	int id;
	if (items < 1) {
		isInPlayerFile = 0;
		id = 30;
	}
	else if (!strcmp("isinotherfile", firstW)) {
		assert(items == 2);
		isInPlayerFile = 1;
		id = which;
	}
	else {
		assert(items == 1);
		isInPlayerFile = 0;
		id = atoi(flag);
	}

	setSuperPauseAnimation(isInPlayerFile, id);

	freeMemory(flag);
}

static void handleSuperPauseSound(MugenAssignment* tAssignment, Player* tPlayer) {
	char* flag = evaluateAssignmentAndReturnAsAllocatedString(tAssignment, tPlayer);
	turnStringLowercase(flag);

	int isInPlayerFile;
	int group;
	int item;
	if (!strcmp("", flag)) {
		isInPlayerFile = 0;
		group = -1;
		item = -1;
	}
	else {
		char firstW[20], comma[10];
		int items = sscanf(flag, "%s", firstW);
		assert(items == 1);

		if (!strcmp("isinotherfile", firstW)) {
			isInPlayerFile = 1;
			int items = sscanf(flag, "%s %d %s %d", firstW, &group, comma, &item);
			assert(items == 4);
		}
		else {
			isInPlayerFile = 0;
			int items = sscanf(flag, "%d %s %d", &group, comma, &item);
			assert(items == 3);
		}
	}

	setSuperPauseSound(isInPlayerFile, group, item);

	freeMemory(flag);
}

static void handleSuperPause(MugenStateController* tController, Player* tPlayer) {
	SuperPauseController* e = tController->mData;

	setSuperPausePlayer(tPlayer);
	getSingleIntegerValueOrDefaultFunctionCall(e->mTime, tPlayer, setSuperPauseTime, 30);
	getSingleIntegerValueOrDefaultFunctionCall(e->mBufferTimeForCommandsDuringPauseEnd, tPlayer, setSuperPauseBufferTimeForCommandsDuringPauseEnd, 0);
	getSingleIntegerValueOrDefaultFunctionCall(e->mMoveTime, tPlayer, setSuperPauseMoveTime, 0);
	getSingleIntegerValueOrDefaultFunctionCall(e->mDoesPauseBackground, tPlayer, setSuperPauseIsPausingBG, 1);

	handleSuperPauseAnimation(e->mAnim, tPlayer);
	handleSuperPauseSound(e->mSound, tPlayer);
	getSingleVector3DValueOrDefaultFunctionCall(e->mPosition, tPlayer, setSuperPausePosition, makePosition(0, 0, 0));
	getSingleIntegerValueOrDefaultFunctionCall(e->mIsDarkening, tPlayer, setSuperPauseDarkening, 1);
	getSingleFloatValueOrDefaultFunctionCall(e->mPlayer2DefenseMultiplier, tPlayer, setSuperPausePlayer2DefenseMultiplier, 0);
	getSingleIntegerValueOrDefaultFunctionCall(e->mPowerToAdd, tPlayer, setSuperPausePowerToAdd, 0);
	getSingleIntegerValueOrDefaultFunctionCall(e->mSetPlayerUnhittable, tPlayer, setSuperPausePlayerUnhittability, 1);

	setSuperPauseActive();
}

// TOOD: move somewhere better
static Position getFinalPositionFromPositionType(ExplodPositionType tPositionType, Position mOffset, Player* tPlayer) {
	if (tPositionType == EXPLOD_POSITION_TYPE_RELATIVE_TO_P1) {
		Player* target = getRootPlayer(0);
		Position p = getPlayerPosition(target, getPlayerCoordinateP(tPlayer));
		int isReversed = !getPlayerIsFacingRight(target);
		if (isReversed) mOffset.x *= -1;
		return vecAdd(p, mOffset);
	} else if (tPositionType == EXPLOD_POSITION_TYPE_RELATIVE_TO_P2) {
		Player* target = getRootPlayer(1);
		Position p = getPlayerPosition(target, getPlayerCoordinateP(tPlayer));
		int isReversed = !getPlayerIsFacingRight(target);
		if (isReversed) mOffset.x *= -1;
		return vecAdd(p, mOffset);
	}
	else if (tPositionType == EXPLOD_POSITION_TYPE_RELATIVE_TO_FRONT) {
		Player* target = tPlayer;
		Position p = makePosition(getPlayerScreenEdgeInFrontX(target), getStageTopOfScreenBasedOnPlayer(getPlayerCoordinateP(target)), 0);
		int isReversed = !getPlayerIsFacingRight(target);
		if (isReversed) mOffset.x *= -1;
		return vecAdd(p, mOffset);
	}
	else if (tPositionType == EXPLOD_POSITION_TYPE_RELATIVE_TO_BACK) {
		Player* target = tPlayer;
		Position p = makePosition(getPlayerScreenEdgeInBackX(target), getStageTopOfScreenBasedOnPlayer(getPlayerCoordinateP(target)), 0);
		int isReversed = getPlayerIsFacingRight(target);
		if (isReversed) mOffset.x *= -1;
		return vecAdd(p, mOffset);
	}
	else if (tPositionType == EXPLOD_POSITION_TYPE_RELATIVE_TO_LEFT) {
		Position p = makePosition(getStageLeftOfScreenBasedOnPlayer(getPlayerCoordinateP(tPlayer)), getStageTopOfScreenBasedOnPlayer(getPlayerCoordinateP(tPlayer)), 0);
		return vecAdd(p, mOffset);
	}
	else if (tPositionType == EXPLOD_POSITION_TYPE_RELATIVE_TO_LEFT) {
		Position p = makePosition(getStageRightOfScreenBasedOnPlayer(getPlayerCoordinateP(tPlayer)), getStageTopOfScreenBasedOnPlayer(getPlayerCoordinateP(tPlayer)), 0);
		return vecAdd(p, mOffset);
	}
	else if (tPositionType == EXPLOD_POSITION_TYPE_NONE) {
		return mOffset;
	}
	else {
		logError("Unrecognized position type.");
		logErrorInteger(tPositionType);
		abortSystem();
		return makePosition(0, 0, 0);
	}

}

static void handleHelperOneFloatElement(MugenAssignment* tAssignment, Player* tPlayer, Player* tHelper, void(tFunc)(Player*, double), double tDefault) {
	double val;
	getSingleFloatValueOrDefault(tAssignment, tPlayer, &val, tDefault);
	tFunc(tHelper, val);
}

static void handleHelperOneIntegerElement(MugenAssignment* tAssignment, Player* tPlayer, Player* tHelper, void(tFunc)(Player*, int), int tDefault) {
	int val;
	getSingleIntegerValueOrDefault(tAssignment, tPlayer, &val, tDefault);
	tFunc(tHelper, val);
}

static void handleHelperTwoFloatElements(MugenAssignment* tAssignment, Player* tPlayer, Player* tHelper, void(tFunc)(Player*, double, double), double tDefault1, double tDefault2) {
	double val1, val2;
	getTwoFloatValuesWithDefaultValues(tAssignment, tPlayer, &val1, &val2, tDefault1, tDefault2);
	tFunc(tHelper, val1, val2);
}

static void handleHelperFacing(MugenAssignment* tAssignment, Player* tPlayer, Player* tHelper) {
	int facing;
	getSingleIntegerValueOrDefault(tAssignment, tPlayer, &facing, 1);

	if (facing == 1) {
		setPlayerIsFacingRight(tHelper, getPlayerIsFacingRight(tPlayer));
	}
	else {
		setPlayerIsFacingRight(tHelper, !getPlayerIsFacingRight(tPlayer));
	}

}

static void handleHelper(MugenStateController* tController, Player* tPlayer) {
	HelperController* e = tController->mData;
	Player* helper = clonePlayerAsHelper(tPlayer);

	handleHelperOneIntegerElement(e->mID, tPlayer, helper, setPlayerID, 0);
	
	Vector3DI mOffset = makeVector3DI(0, 0, 0);
	getTwoIntegerValuesWithDefaultValues(e->mPosition, tPlayer, &mOffset.x, &mOffset.y, 0, 0);
	ExplodPositionType positionType = getPositionTypeFromAssignment(e->mPositionType, tPlayer);
	Position position = getFinalPositionFromPositionType(positionType, makePosition(mOffset.x, mOffset.y, mOffset.z), tPlayer);
	
	handleHelperFacing(e->mFacing, tPlayer, helper);
	handleHelperOneIntegerElement(e->mStateNumber, tPlayer, helper, changePlayerState, 0);	
	handleHelperOneIntegerElement(e->mCanControl, tPlayer, helper, setPlayerHelperControl, 0);
	// TODO: own palette
	// TODO: supermovetime
	// TODO: pausemovetime

	handleHelperOneFloatElement(e->mSizeScaleX, tPlayer, helper, setPlayerScaleX, getPlayerScaleX(tPlayer));
	handleHelperOneFloatElement(e->mSizeScaleY, tPlayer, helper, setPlayerScaleY, getPlayerScaleY(tPlayer));
	handleHelperOneIntegerElement(e->mSizeGroundBack, tPlayer, helper, setPlayerGroundSizeBack, getPlayerGroundSizeBack(tPlayer));
	handleHelperOneIntegerElement(e->mSizeGroundFront, tPlayer, helper, setPlayerGroundSizeFront, getPlayerGroundSizeFront(tPlayer));
	handleHelperOneIntegerElement(e->mSizeAirBack, tPlayer, helper, setPlayerAirSizeBack, getPlayerAirSizeBack(tPlayer));
	handleHelperOneIntegerElement(e->mSizeAirFront, tPlayer, helper, setPlayerAirSizeFront, getPlayerAirSizeFront(tPlayer));
	handleHelperOneIntegerElement(e->mSizeHeight, tPlayer, helper, setPlayerHeight, getPlayerHeight(tPlayer));

	// TODO: scale projectiles

	handleHelperTwoFloatElements(e->mSizeHeadPosition, tPlayer, helper, setPlayerHeadPosition, getPlayerHeadPositionX(tPlayer), getPlayerHeadPositionY(tPlayer));
	handleHelperTwoFloatElements(e->mSizeMiddlePosition, tPlayer, helper, setPlayerMiddlePosition, getPlayerMiddlePositionX(tPlayer), getPlayerMiddlePositionY(tPlayer));
	handleHelperOneIntegerElement(e->mSizeShadowOffset, tPlayer, helper, setPlayerShadowOffset, getPlayerShadowOffset(tPlayer));

	setPlayerPosition(helper, position, getPlayerCoordinateP(helper));
	addHelperToPlayer(tPlayer, helper);
}

static void handleDestroySelf(Player* tPlayer) {
	destroyPlayer(tPlayer);
}

static void handleAddingLife(MugenStateController* tController, Player* tPlayer) {
	LifeAddController* e = tController->mData;

	int val, canKill, isAbsolute;
	getSingleIntegerValueOrDefault(e->mValue, tPlayer, &val, 0);
	getSingleIntegerValueOrDefault(e->mCanKill, tPlayer, &canKill, 1);
	getSingleIntegerValueOrDefault(e->mIsAbsolute, tPlayer, &isAbsolute, 0);

	// TODO absolute
	(void)isAbsolute;

	int playerLife = getPlayerLife(tPlayer);
	if (!canKill && playerLife + val <= 0) {
		val = -(playerLife - 1);
	}

	addPlayerDamage(tPlayer, -val);
}

static void handleAddingTargetLife(MugenStateController* tController, Player* tPlayer) {
	TargetLifeAddController* e = tController->mData;

	int val, canKill, isAbsolute, id;
	getSingleIntegerValueOrDefault(e->mValue, tPlayer, &val, 0);
	getSingleIntegerValueOrDefault(e->mID, tPlayer, &id, -1);
	getSingleIntegerValueOrDefault(e->mCanKill, tPlayer, &canKill, 1);
	getSingleIntegerValueOrDefault(e->mIsAbsolute, tPlayer, &isAbsolute, 0);

	addPlayerTargetLife(tPlayer, id, val, canKill, isAbsolute);
}

static void handleAngleDrawController(MugenStateController* tController, Player* tPlayer) {
	AngleDrawController* e = tController->mData;

	if (e->mHasScale) {
		Vector3D scale = evaluateAssignmentAndReturnAsVector3D(e->mScale, tPlayer);
		setPlayerDrawScale(tPlayer, scale);
	}

	if (e->mHasValue) {
		double val = evaluateAssignmentAndReturnAsFloat(e->mValue, tPlayer);
		setPlayerDrawAngle(tPlayer, val);
	}
}

static void handleAngleAddController(MugenStateController* tController, Player* tPlayer) {
	SingleRequiredValueController* e = tController->mData;

	double angle = evaluateAssignmentAndReturnAsFloat(e->mValue, tPlayer);

	addPlayerDrawAngle(tPlayer, angle);
}

static void handleAngleSetController(MugenStateController* tController, Player* tPlayer) {
	SingleRequiredValueController* e = tController->mData;

	double angle = evaluateAssignmentAndReturnAsFloat(e->mValue, tPlayer);

	setPlayerFixedDrawAngle(tPlayer, angle);
}

static void handleRemovingExplod(MugenStateController* tController, Player* tPlayer) {
	RemoveExplodController* e = tController->mData;
	(void)e;
	(void)tPlayer;
	// TODO: implement with rest of explod

}

static void handleBindToRootController(MugenStateController* tController, Player* tPlayer) {
	BindController* e = tController->mData;

	int time, facing;
	Vector3D offset = makePosition(0, 0, 0);
	getSingleIntegerValueOrDefault(e->mTime, tPlayer, &time, 1);
	getSingleIntegerValueOrDefault(e->mFacing, tPlayer, &facing, 0);
	getTwoFloatValuesWithDefaultValues(e->mPosition, tPlayer, &offset.x, &offset.y, 0, 0);

	bindPlayerToRoot(tPlayer, time, facing, offset);
}

static void handleBindToParentController(MugenStateController* tController, Player* tPlayer) {
	BindController* e = tController->mData;

	int time, facing;
	Vector3D offset = makePosition(0, 0, 0);
	getSingleIntegerValueOrDefault(e->mTime, tPlayer, &time, 1);
	getSingleIntegerValueOrDefault(e->mFacing, tPlayer, &facing, 0);
	getTwoFloatValuesWithDefaultValues(e->mPosition, tPlayer, &offset.x, &offset.y, 0, 0);

	bindPlayerToParent(tPlayer, time, facing, offset);
}

static void handleTargetBindController(MugenStateController* tController, Player* tPlayer) {
	BindController* e = tController->mData;

	int time, id;
	Vector3D offset = makePosition(0, 0, 0);
	getSingleIntegerValueOrDefault(e->mTime, tPlayer, &time, 1);
	getSingleIntegerValueOrDefault(e->mID, tPlayer, &id, -1);
	getTwoFloatValuesWithDefaultValues(e->mPosition, tPlayer, &offset.x, &offset.y, 0, 0);

	bindPlayerTargets(tPlayer, time, offset, id);
}

static void handleTurnController(Player* tPlayer) {
	turnPlayerAround(tPlayer);
}

static void handlePushPlayerController(MugenStateController* tController, Player* tPlayer) {
	SingleRequiredValueController* e = tController->mData;

	int isPushDisabled = evaluateAssignment(e->mValue, tPlayer);

	setPlayerPushDisabledFlag(tPlayer, isPushDisabled);
}

static void handleSettingVariableRange(MugenStateController* tController, Player* tPlayer) {
	VarRangeSetController* e = tController->mData;

	if (e->mType == VAR_SET_TYPE_INTEGER) {
		int value, first, last;
		getSingleIntegerValueOrDefault(e->mValue, tPlayer, &value, 0);
		getSingleIntegerValueOrDefault(e->mFirst, tPlayer, &first, 0);
		getSingleIntegerValueOrDefault(e->mLast, tPlayer, &last, 59);
		int i;
		for (i = first; i <= last; i++) {
			setPlayerVariable(tPlayer, i, value);
		}
	}
	else if (e->mType == VAR_SET_TYPE_FLOAT) {
		double value;
		int first, last;
		getSingleFloatValueOrDefault(e->mValue, tPlayer, &value, 0);
		getSingleIntegerValueOrDefault(e->mFirst, tPlayer, &first, 0);
		getSingleIntegerValueOrDefault(e->mLast, tPlayer, &last, 39);
		int i;
		for (i = first; i <= last; i++) {
			setPlayerFloatVariable(tPlayer, i, value);
		}
	}
	else {
		logError("Unrecognized var type.");
		logErrorInteger(e->mType);
		abortSystem();
	}
}

static void handleScreenBound(MugenStateController* tController, Player* tPlayer) {
	ScreenBoundController* e = tController->mData;

	int val;
	int moveCameraX, moveCameraY;

	getSingleIntegerValueOrDefault(e->mValue, tPlayer, &val, 0);
	getTwoIntegerValuesWithDefaultValues(e->mMoveCameraFlags, tPlayer, &moveCameraX, &moveCameraY, 0, 0);

	setPlayerScreenBound(tPlayer, val, moveCameraX, moveCameraY);
}

static void handleMoveHitReset(Player* tPlayer) {
	setPlayerMoveHitReset(tPlayer);
}

static void handleGravity(Player* tPlayer) {
	double accel = getPlayerVerticalAcceleration(tPlayer);
	addPlayerVelocityY(tPlayer, accel, getPlayerCoordinateP(tPlayer));
}

static void handleSettingAttackDistance(MugenStateController* tController, Player* tPlayer) {
	SingleRequiredValueController* e = tController->mData;

	int value = evaluateAssignmentAndReturnAsInteger(e->mValue, tPlayer);
	setHitDataGuardDistance(tPlayer, value);
}

static void handleSetTargetFacing(MugenStateController* tController, Player* tPlayer) {
	SetTargetFacingController* e = tController->mData;

	int value = evaluateAssignmentAndReturnAsInteger(e->mValue, tPlayer);
	int id;
	getSingleIntegerValueOrDefault(e->mID, tPlayer, &id, -1);

	setPlayerTargetFacing(tPlayer, id, value);
}

static void handleReversalDefinition(MugenStateController* tController, Player* tPlayer) {
	ReversalDefinitionController* e = tController->mData;

	handleReversalDefinitionEntry(e->mAttributes, tPlayer);
}


int handleMugenStateControllerAndReturnWhetherStateChanged(MugenStateController * tController, Player* tPlayer)
{

	if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_VELOCITY) {
		handleVelocitySetting(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_MULTIPLY_VELOCITY) {
		handleVelocityMultiplication(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ADD_VELOCITY) {
		handleVelocityAddition(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_CHANGE_STATE) {
		handleStateChange(tController, tPlayer);
		return 1;
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_SELF_STATE) {
		handleSelfStateChange(tController, tPlayer);
		return 1;
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_TARGET_STATE) {
		handleTargetStateChange(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_PLAY_SOUND) {
		handlePlaySound(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_HIT_DEFINITION) {
		handleHitDefinition(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_CHANGE_ANIMATION) {
		handleAnimationChange(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_CHANGE_ANIMATION_2) {
		handleAnimationChange2(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_CONTROL) {
		handleControlSetting(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_POSITION) {
		handlePositionSetting(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ADD_POSITION) {
		handlePositionAdding(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_WIDTH) {
		handleWidth(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SPRITE_PRIORITY) {
		handleSpritePriority(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ASSERT_SPECIAL) {
		handleSpecialAssert(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_MAKE_DUST) {
		handleMakeDust(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_VARIABLE) {
		handleSettingVariable(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_PARENT_VARIABLE) {
		handleSettingVariable(tController, getPlayerParent(tPlayer));
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ADD_VARIABLE) {
		handleAddingVariable(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_VARIABLE_RANGE) {
		handleSettingVariableRange(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_NULL) {
		handleNull();
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_STATE_TYPE) {
		handleStateTypeSet(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_FORCE_FEEDBACK) {
		handleNull(tController, tPlayer); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_HIT_VELOCITY) {
		handleHitVelocitySetting(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_DEFENSE_MULTIPLIER) {
		handleDefenseMultiplier(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_FALL_ENVIRONMENT_SHAKE) {
		handleFallEnvironmentShake();
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_EXPLOD) {
		handleExplod(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_MODIFY_EXPLOD) {
		handleNull(); // TODO: modify explod
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_HIT_FALL_DAMAGE) {
		handleHitFallDamage();
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_FREEZE_POSITION) {
		handlePositionFreeze(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_HIT_FALL_VELOCITY) {
		handleHitFallVelocity(tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_NOT_HIT_BY) {
		handleNotHitBy(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_HIT_BY) {
		handleHitBy(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_HIT_FALL) {
		handleHitFallSet(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_ATTACK_MULTIPLIER) {
		handleAttackMultiplierSetting(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_PAN_SOUND) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_STOP_SOUND) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_DISPLAY_TO_CLIPBOARD) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_APPEND_TO_CLIPBOARD) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_HIT_OVERRIDE) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ADD_POWER) {
		handlePowerAddition(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ENVIRONMENT_SHAKE) {
		handleEnvironmentShake(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SUPER_PAUSE) {
		handleSuperPause(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_AFTER_IMAGE) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_AFTER_IMAGE_TIME) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_PALETTE_EFFECT) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_HELPER) {
		handleHelper(tController, tPlayer); 
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_TRANSPARENCY) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_DESTROY_SELF) {
		handleDestroySelf(tPlayer); 
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ADD_LIFE) {
		handleAddingLife(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_REMOVE_EXPLOD) {
		handleRemovingExplod(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_DRAW_ANGLE) {
		handleAngleDrawController(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ADD_ANGLE) {
		handleAngleAddController(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_ANGLE) {
		handleAngleSetController(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_PALETTE_EFFECT_BACKGROUND) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_BIND_TO_ROOT) {
		handleBindToRootController(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_BIND_TO_PARENT) {
		handleBindToParentController(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_BIND_TARGET) {
		handleTargetBindController(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_TURN) {
		handleTurnController(tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_PLAYER_PUSH) {
		handlePushPlayerController(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_REMAP_PALETTE) {
		handleNull(); // TODO
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SCREEN_BOUND) {
		handleScreenBound(tController, tPlayer); 
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_RESET_MOVE_HIT) {
		handleMoveHitReset(tPlayer); 
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_GRAVITY) {
		handleGravity(tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_ATTACK_DISTANCE) {
		handleSettingAttackDistance(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_ADD_TARGET_LIFE) {
		handleAddingTargetLife(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_SET_TARGET_FACING) {
		handleSetTargetFacing(tController, tPlayer);
	}
	else if (tController->mType == MUGEN_STATE_CONTROLLER_TYPE_REVERSAL_DEFINITION) {
		handleReversalDefinition(tController, tPlayer);
	}
	else {
		logError("Unrecognized state controller.");
		logErrorInteger(tController->mType);
		abortSystem();
	}
	 
	return 0;
}
