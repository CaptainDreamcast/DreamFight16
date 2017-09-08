#include "config.h"

#include <tari/mugendefreader.h>

static struct {
	double mDefaultAttackDamageDoneToPowerMultiplier;
	double mDefaultAttackDamageReceivedToPowerMultiplier;

} gData;

static void loadRules(MugenDefScript* tScript) {
	getMugenDefFloatOrDefault(&gData.mDefaultAttackDamageDoneToPowerMultiplier, tScript, "Rules", "Default.Attack.LifeToPowerMul", 0.7);
	getMugenDefFloatOrDefault(&gData.mDefaultAttackDamageReceivedToPowerMultiplier, tScript, "Rules", "Default.GetHit.LifeToPowerMul", 0.6);
}

static void loadMugenConfig(void* tData) {
	(void)tData;

	MugenDefScript script = loadMugenDefScript("assets/data/mugen.cfg"); // TODO: proper path
	loadRules(&script);
	unloadMugenDefScript(script);
}

ActorBlueprint MugenConfig = {
	.mLoad = loadMugenConfig,
};

double getDefaultAttackDamageDoneToPowerMultiplier()
{
	return gData.mDefaultAttackDamageDoneToPowerMultiplier;
}

double getDefaultAttackDamageReceivedToPowerMultiplier()
{
	return gData.mDefaultAttackDamageReceivedToPowerMultiplier;
}

