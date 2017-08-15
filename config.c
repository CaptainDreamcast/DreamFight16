#include "config.h"

#include "mugendefreader.h"

static struct {
	double mDefaultAttackDamageDoneToPowerMultiplier;
	double mDefaultAttackDamageReceivedToPowerMultiplier;

} gData;

static void loadRules(MugenDefScript* tScript) {
	loadFloatOrDefault(&gData.mDefaultAttackDamageDoneToPowerMultiplier, tScript, "Rules", "Default.Attack.LifeToPowerMul", 0.7);
	loadFloatOrDefault(&gData.mDefaultAttackDamageReceivedToPowerMultiplier, tScript, "Rules", "Default.GetHit.LifeToPowerMul", 0.6);
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

