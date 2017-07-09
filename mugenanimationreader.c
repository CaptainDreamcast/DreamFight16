#include "mugenanimationreader.h"

#include <stdio.h>
#include <assert.h>

#include <tari/log.h>
#include <tari/collision.h>
#include <tari/system.h>
#include <tari/math.h>

#include "mugendefreader.h"


static struct {
	int mIsLoaded;

	List mDefaultHitboxes1;
	List mDefaultHitboxes2;

	int mHasOwnHitbox1;
	List mOwnHitbox1;

	int mHasOwnHitbox2;
	List mOwnHitbox2;

	int mIsLoopStart;
} gMugenAnimationState;

static void setGlobalAnimationState() {
	if (gMugenAnimationState.mIsLoaded) return;

	gMugenAnimationState.mDefaultHitboxes1 = new_list();
	gMugenAnimationState.mDefaultHitboxes2 = new_list();
	gMugenAnimationState.mIsLoaded = 1;
}

static void unsetGlobalAnimationState() {
	if (!gMugenAnimationState.mIsLoaded) return;

	gMugenAnimationState.mIsLoaded = 0;
}

static void resetGlobalAnimationState() {
	unsetGlobalAnimationState();
	setGlobalAnimationState();
}

static void setSingleAnimationState() {
	gMugenAnimationState.mIsLoopStart = 0;
	gMugenAnimationState.mHasOwnHitbox1 = 0;
	gMugenAnimationState.mHasOwnHitbox2 = 0;
}

static void unsetSingleAnimationState() {
	gMugenAnimationState.mHasOwnHitbox1 = 0;
	gMugenAnimationState.mHasOwnHitbox2 = 0;
}

static void resetSingleAnimationState() {
	unsetSingleAnimationState();
	setSingleAnimationState();
}


static MugenAnimation* makeEmptyMugenAnimation() {
	MugenAnimation* ret = allocMemory(sizeof(MugenAnimation));
	ret->mLoopStart = -1;
	ret->mSteps = new_vector();
	return ret;
}

static int getAnimationID(MugenDefScriptGroup* tGroup) {
	char text1[20], text2[20];
	int ret;
	sscanf(tGroup->mName, "%s %s %d", text1, text2, &ret);
	return ret;
}

typedef struct {
	MugenAnimations* mAnimations;
	int mGroupID;
} AnimationLoadCaller;

static int isAnimationVector(char* tVariableName) {
	char text[100];
	sscanf(tVariableName, "%s", text);
	return !strcmp(text, "vector_statement");
}

static void insertAnimationStepIntoAnimation(MugenAnimations* tAnimations, int tGroupID, MugenAnimationStep* tElement) {
	assert(int_map_contains(&tAnimations->mAnimations, tGroupID));
	
	MugenAnimation* e = int_map_get(&tAnimations->mAnimations, tGroupID);
	if (gMugenAnimationState.mIsLoopStart) e->mLoopStart = vector_size(&e->mSteps);
	vector_push_back_owned(&e->mSteps, tElement);
}

static void handleNewAnimationStep(MugenAnimations* tAnimations, int tGroupID, MugenDefScriptGroupElement* tElement) {
	assert(tElement->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT);
	MugenDefScriptVectorElement* vectorElement = tElement->mData;
	
	MugenAnimationStep* e = allocMemory(sizeof(MugenAnimationStep));
	if (gMugenAnimationState.mHasOwnHitbox1) {
		e->mAttackHitboxes = gMugenAnimationState.mOwnHitbox1;
	}
	else {
		e->mAttackHitboxes = gMugenAnimationState.mDefaultHitboxes1;
	}

	if (gMugenAnimationState.mHasOwnHitbox2) {
		e->mPassiveHitboxes = gMugenAnimationState.mOwnHitbox2;
	}
	else {
		e->mPassiveHitboxes = gMugenAnimationState.mDefaultHitboxes2;
	}

	e->mGroupNumber = atoi(vectorElement->mVector.mElement[0]);
	e->mSpriteNumber = atoi(vectorElement->mVector.mElement[1]);
	e->mDelta.x = atof(vectorElement->mVector.mElement[2]);
	e->mDelta.y = atof(vectorElement->mVector.mElement[3]);
	e->mDuration = atoi(vectorElement->mVector.mElement[4]);

	char* flipFlags = vectorElement->mVector.mElement[5];
	e->mIsFlippingHorizontally = strchr(flipFlags, 'H') != NULL;
	e->mIsFlippingVertically = strchr(flipFlags, 'V') != NULL;
	
	insertAnimationStepIntoAnimation(tAnimations, tGroupID, e);
	resetSingleAnimationState();
}

static void handleHitboxSizeAssignment() {

}

static int isHitboxSizeAssignment(char* tName) {
	return !strcmp("Clsn2Default", tName) || !strcmp("Clsn2", tName) || !strcmp("Clsn1Default", tName) || !strcmp("Clsn1", tName);
}

static void handleCollisionHitboxAssignment(List* tHitboxes, MugenDefScriptGroupElement* tElement) {
	assert(tElement->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT);

	MugenDefScriptVectorElement* vectorElement = tElement->mData;
	
	double x1 = atof(vectorElement->mVector.mElement[0]);
	double y1 = atof(vectorElement->mVector.mElement[1]);
	double x2 = atof(vectorElement->mVector.mElement[2]);
	double y2 = atof(vectorElement->mVector.mElement[3]);

	Position topLeft = makePosition(min(x1, x2), min(y1, y2), 0);
	Position bottomRight = makePosition(max(x1, x2), max(y1, y2), 0);

	Collider* e = allocMemory(sizeof(Collider));
	*e = makeColliderFromRect(makeCollisionRect(topLeft, bottomRight));
	
	list_push_back_owned(tHitboxes, e);
}

static void handleOwnHitboxExistence(List* tHitboxes, int* tHasHitbox) {
	if (!(*tHasHitbox)) {
		*tHitboxes = new_list();
		*tHasHitbox = 1;
	}
}

static void handleHitboxAssignment(MugenDefScriptGroupElement* tElement) {
	char name[100];
	strcpy(name, tElement->mName);
	char* opening = strchr(name, '[');
	assert(opening);
	*opening = '\0';

	if (!strcmp("Clsn1Default", name)) {
		handleCollisionHitboxAssignment(&gMugenAnimationState.mDefaultHitboxes1, tElement);	
	} else if (!strcmp("Clsn2Default", name)) {
		handleCollisionHitboxAssignment(&gMugenAnimationState.mDefaultHitboxes2, tElement);
	}
	else if (!strcmp("Clsn1", name)) {
		handleOwnHitboxExistence(&gMugenAnimationState.mOwnHitbox1, &gMugenAnimationState.mHasOwnHitbox1);
		handleCollisionHitboxAssignment(&gMugenAnimationState.mOwnHitbox1, tElement);
	}
	else if (!strcmp("Clsn2", name)) {
		handleOwnHitboxExistence(&gMugenAnimationState.mOwnHitbox2, &gMugenAnimationState.mHasOwnHitbox2);
		handleCollisionHitboxAssignment(&gMugenAnimationState.mOwnHitbox2, tElement);
	}
	else {
		logError("Unable to decode assignment type.");
		logErrorString(name);
		abortSystem();
	}

}

static int isHitboxAssignment(char* tName) {
	char name[100];
	strcpy(name, tName);
	char* opening = strchr(name, '[');
	if (!opening) return 0;

	*opening = '\0';

	return !strcmp("Clsn2Default", name) || !strcmp("Clsn2", name) || !strcmp("Clsn1Default", name) || !strcmp("Clsn1", name);
}


static int isLoopStart(char* tVariableName) {
	char text[100];
	sscanf(tVariableName, "%s", text);
	return !strcmp(text, "Loopstart");
}

static void handleLoopStart() {
	gMugenAnimationState.mIsLoopStart = 1;
}

static void loadSingleAnimationElementStatement(void* tCaller, void* tData) {
	MugenDefScriptGroupElement* element = tData;
	AnimationLoadCaller* caller = tCaller;

	if (isAnimationVector(element->mName)) {
		handleNewAnimationStep(caller->mAnimations, caller->mGroupID, element);
	} else if (isHitboxSizeAssignment(element->mName)) {
		handleHitboxSizeAssignment();
	}
	else if (isHitboxAssignment(element->mName)) {
		handleHitboxAssignment(element);
	}
	else if (isLoopStart(element->mName)) {
		handleLoopStart();
	}
	else {
		logError("Unrecognized type.");
		logErrorString(element->mName);
		abortSystem();
	}
}

static void loadSingleAnimationGroup(MugenAnimations* tAnimations, MugenDefScriptGroup* tGroup) {
	int id = getAnimationID(tGroup);
	MugenAnimation* anim = makeEmptyMugenAnimation();
	int_map_push_owned(&tAnimations->mAnimations, id, anim);
	resetGlobalAnimationState();
	resetSingleAnimationState();

	AnimationLoadCaller caller;
	caller.mAnimations = tAnimations;
	caller.mGroupID = id;
	list_map(&tGroup->mOrderedElementList, loadSingleAnimationElementStatement, &caller);

	unsetSingleAnimationState();
	unsetGlobalAnimationState();

	
}

static void loadAnimationFileFromDefScript(MugenAnimations* tAnimations, MugenDefScript* tScript) {
	MugenDefScriptGroup* current = tScript->mFirstGroup;

	while (current != NULL) {
		loadSingleAnimationGroup(tAnimations, current);

		current = current->mNext;
	}

}

MugenAnimations loadMugenAnimationFile(char * tPath)
{
	MugenAnimations ret;
	ret.mAnimations = new_int_map();

	MugenDefScript defScript = loadMugenDefScript(tPath);

	loadAnimationFileFromDefScript(&ret, &defScript);

	return ret;
}
