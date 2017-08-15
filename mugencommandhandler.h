#pragma once

#include <tari/actorhandler.h>

#include "mugencommandreader.h"
#include "mugenanimationhandler.h"
#include "playerdefinition.h"

int registerMugenCommands(Player* tPlayer, MugenCommands* tCommands);

int isCommandActive(int tID, char* tCommandName);
void setPlayerCommandActiveForAI(int tID, char* tCommandName, Duration tBufferTime);

void setMugenCommandFaceDirection(int tID, FaceDirection tDirection);

extern ActorBlueprint MugenCommandHandler;