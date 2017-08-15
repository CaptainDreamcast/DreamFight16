#pragma once

#include <tari/datastructures.h>
#include <tari/animation.h>

typedef enum {
	MUGEN_COMMAND_INPUT_STEP_TYPE_PRESS,
	MUGEN_COMMAND_INPUT_STEP_TYPE_MULTIPLE,
	MUGEN_COMMAND_INPUT_STEP_TYPE_HOLDING,
	MUGEN_COMMAND_INPUT_STEP_TYPE_RELEASE,

} MugenCommandInputStepType;

typedef enum {
	MUGEN_COMMAND_INPUT_STEP_TARGET_MULTIPLE = 0,

	MUGEN_COMMAND_INPUT_STEP_TARGET_DOWN,
	MUGEN_COMMAND_INPUT_STEP_TARGET_UP,
	MUGEN_COMMAND_INPUT_STEP_TARGET_FORWARD,
	MUGEN_COMMAND_INPUT_STEP_TARGET_BACKWARD,

	MUGEN_COMMAND_INPUT_STEP_TARGET_A,
	MUGEN_COMMAND_INPUT_STEP_TARGET_B,
	MUGEN_COMMAND_INPUT_STEP_TARGET_C,
	MUGEN_COMMAND_INPUT_STEP_TARGET_X,
	MUGEN_COMMAND_INPUT_STEP_TARGET_Y,
	MUGEN_COMMAND_INPUT_STEP_TARGET_Z,

	MUGEN_COMMAND_INPUT_STEP_TARGET_START,

	MUGEN_COMMAND_INPUT_STEP_TARGET_DOWN_FORWARD,
	MUGEN_COMMAND_INPUT_STEP_TARGET_DOWN_BACKWARD,
	MUGEN_COMMAND_INPUT_STEP_TARGET_UP_FORWARD,
	MUGEN_COMMAND_INPUT_STEP_TARGET_UP_BACKWARD,

	MUGEN_COMMAND_INPUT_STEP_TARGET_MULTI_DOWN,
	MUGEN_COMMAND_INPUT_STEP_TARGET_MULTI_UP,
	MUGEN_COMMAND_INPUT_STEP_TARGET_MULTI_FORWARD,
	MUGEN_COMMAND_INPUT_STEP_TARGET_MULTI_BACKWARD,
} MugenCommandInputStepTarget;

typedef struct {
	Vector mSubSteps;
} MugenCommandInputStepMultipleTargetData;

typedef struct {
	Duration mDuration;
} MugenCommandInputStepReleaseData;

typedef struct{
	MugenCommandInputStepType mType;
	MugenCommandInputStepTarget mTarget;
	int mDoesNotAllowOtherInputBetween;
	void* mData;
} MugenCommandInputStep;

typedef struct {
	Vector mInputSteps;
	Duration mTime;
	Duration mBufferTime;
} MugenCommandInput;

typedef struct {
	Vector mInputs;
} MugenCommand;

typedef struct {
	StringMap mCommands;
} MugenCommands;

MugenCommands loadMugenCommandFile(char* tPath);
