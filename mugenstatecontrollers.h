#pragma once

#include <tari/mugendefreader.h>

#include "mugenassignment.h"
#include "playerdefinition.h"

MugenStateController* parseMugenStateControllerFromGroup(MugenDefScriptGroup* tGroup);
int handleMugenStateControllerAndReturnWhetherStateChanged(MugenStateController* tController, Player* tPlayer);