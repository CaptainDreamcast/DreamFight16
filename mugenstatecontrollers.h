#pragma once

#include "mugenassignment.h"
#include "mugendefreader.h"
#include "playerdefinition.h"

MugenStateController* parseMugenStateControllerFromGroup(MugenDefScriptGroup* tGroup);
int handleMugenStateControllerAndReturnWhetherStateChanged(MugenStateController* tController, Player* tPlayer);