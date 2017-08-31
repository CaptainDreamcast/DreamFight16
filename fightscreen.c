#include "fightscreen.h"

#include <stdio.h>

#include <tari/input.h>
#include <tari/stagehandler.h>
#include <tari/collisionhandler.h>
#include <tari/system.h>

#include "stage.h"
#include "mugenanimationreader.h"
#include "mugencommandreader.h"
#include "mugenstatereader.h"
#include "playerdefinition.h"
#include "mugenanimationhandler.h"
#include "mugencommandhandler.h"
#include "mugenstatehandler.h"
#include "collision.h"
#include "fightui.h"
#include "mugenexplod.h"
#include "gamelogic.h"
#include "config.h"
#include "playerhitdata.h"
#include "titlescreen.h"
#include "projectile.h"

static Screen* getNextFightScreenScreen() {
	if (hasPressedAbortFlank()) {
		abortScreenHandling(); // TODO
	}

	return NULL;
}

static void loadFightScreen() {
	setupGameCollisions();
	
	instantiateActor(MugenConfig);
	instantiateActor(HitDataHandler);
	instantiateActor(ProjectileHandler);
	instantiateActor(MugenAnimationHandler);
	instantiateActor(MugenCommandHandler);
	instantiateActor(MugenStateHandler);
	instantiateActor(ExplodHandler);

	setStageMugenDefinition("assets/stages/stage0.def");
	instantiateActor(StageBP);

	
	//setPlayerDefinitionPath(0, "assets/Mima_RP/Mima_RP.def");
	//setPlayerDefinitionPath(0, "assets/Ryu/Ryu.def");
	setPlayerDefinitionPath(0, "assets/Beat/Beat.def");
	setPlayerDefinitionPath(0, "assets/kfm/kfm.def");
	//setPlayerDefinitionPath(0, "assets/SonicV2/SonicV2.def");

	//setPlayerDefinitionPath(1, "assets/Ryu/Ryu.def");
	//setPlayerDefinitionPath(1, "assets/liukang/liukang.def");
	setPlayerDefinitionPath(1, "assets/Beat/Beat.def");
	//setPlayerDefinitionPath(1, "assets/kfm/kfm.def");
	//setPlayerDefinitionPath(1, "assets/SonicV2/SonicV2.def");

	loadPlayers();

	instantiateActor(FightUIBP);
	instantiateActor(GameLogic);
	
	// activateCollisionHandlerDebugMode();

}

static void updateFightScreen() {
	updatePlayers();
}

Screen FightScreen = {
	.mLoad = loadFightScreen,
	.mGetNextScreen = getNextFightScreenScreen,
	.mUpdate = updateFightScreen,
};
