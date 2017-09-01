#pragma once

#include <tari/actorhandler.h>

#include "mugenstatereader.h"
#include "playerdefinition.h"

extern ActorBlueprint MugenStateHandler;

int registerMugenStateMachine(MugenStates* tStates, Player* tPlayer);
void removeRegisteredStateMachine(int tID);
int getRegisteredStateState(int tID);
int getRegisteredStatePreviousState(int tID);
void pauseRegisteredStateMachine(int tID);
void unpauseRegisteredStateMachine(int tID);
void disableRegisteredStateMachine(int tID);

int getRegisteredStateTimeInState(int tID);
void setRegisteredStateTimeInState(int tID, int tTime);
void setRegisteredStateToHelperMode(int tID);
void setRegisteredStateDisableCommandState(int tID);

int hasHandledStateMachineState(int tID, int tNewState);
int hasHandledStateMachineStateSelf(int tID, int tNewState);
void changeHandledStateMachineState(int tID, int tNewState);
void changeHandledStateMachineStateToOtherPlayerStateMachine(int tID, int tTemporaryID, int tNewState);
void changeHandledStateMachineStateToOwnStateMachine(int tID, int tNewState);

void updateSingleStateMachineByID(int tID);

