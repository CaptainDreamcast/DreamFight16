#include "mugendefreader.h"

#include <assert.h>
#include <ctype.h>

#include <tari/file.h>
#include <tari/memoryhandler.h>
#include <tari/log.h>
#include <tari/system.h>
 
typedef struct MugenDefToken_t {
	char mValue[100];
	struct MugenDefToken_t* mNext;

} MugenDefToken;

static MugenDefToken* makeMugenDefToken(char* tValue) {
	MugenDefToken* e = allocMemory(sizeof(MugenDefToken));
	strcpy(e->mValue, tValue);
	e->mNext = NULL;

	return e;
}

static int isEmpty(BufferPointer p) {
	return *p == ' ' || *p < 0;
}

static int increaseAndCheckIfOver(Buffer* b, BufferPointer* p) {
	(*p)++;
	return ((uint32_t)*p == (uint32_t)b->mData + b->mLength);
}

static int decreaseAndCheckIfOver(BufferPointer* p) {
	(*p)--;
	return (int32_t)*p < 0;
}

static int isComment(BufferPointer p) {
	return *p == ';';
}

static int isLinebreak(BufferPointer p) {
	return *p == '\n' || *p == 0xD || *p == 0xA;
}

static MugenDefToken* parseRecursively(Buffer* b, BufferPointer p);

static int increasePointerToNextLine(Buffer* b, BufferPointer* p) {
	while (!isLinebreak(*p)) {
		if (increaseAndCheckIfOver(b, p)) return 1;
	}

	while (isLinebreak(*p)) {
		if (increaseAndCheckIfOver(b, p)) return 1;
	}

	return 0;
}

static MugenDefToken* parseComment(Buffer* b, BufferPointer p) {
	
	if (!increasePointerToNextLine(b, &p)) {
		return parseRecursively(b, p);
	}
	else return NULL;
}

static BufferPointer findEndOfToken(Buffer* b, BufferPointer p, char start, char end) {
	assert(*p == start);

	int depth = 1;

	while ((depth > 0 || *p != end) && (uint32_t)p < ((uint32_t)b->mData)+b->mLength) {
		assert(!increaseAndCheckIfOver(b, &p));
		depth += *p == start;
		depth -= *p == end;
	}

	assert(*p == end);
	return p;
}

static BufferPointer getNextDefCharPosition(Buffer* b, BufferPointer p, char tChar) {
	while (*p != tChar) {
		assert(!increaseAndCheckIfOver(b, &p));
	}

	return p;
}

static char* makeMugenDefString(char* tPos, int tLength) {
	char* e = allocMemory(tLength + 10);
	memcpy(e, tPos, tLength);
	e[tLength] = '\0';
	return e;
}

static char* makeMugenDefStringFromEndPoint(BufferPointer tStart, BufferPointer tEnd) {
	int length = tEnd - tStart + 1;
	assert(length > 0);
	return makeMugenDefString(tStart, length);
}

static void destroyMugenDefString(char* tVal) {
	freeMemory(tVal);
}


static void moveBufferPointerForward(Buffer* b, BufferPointer* p) {
	while (isEmpty(*p) && !isLinebreak(*p)) {
		assert(!increaseAndCheckIfOver(b, p));
	}
}

static void moveBufferPointerBack(BufferPointer* p) {
	while (isEmpty(*p)) {
		assert(!decreaseAndCheckIfOver(p));
	}
}

static BufferPointer removeCommentFromToken(BufferPointer s, BufferPointer e) {
	BufferPointer i;
	for(i = s; i <= e; i++) {
		if (*i == ';') return i - 1;
	}

	return e;
}

static MugenDefToken* parseAssignment(Buffer* b, BufferPointer p, char tAssignmentToken) {
	debugLog("Parse assignment token.");

	BufferPointer equal = getNextDefCharPosition(b, p, tAssignmentToken);
	MugenDefToken* equalToken = makeMugenDefToken("=");

	BufferPointer start = p;
	BufferPointer end = equal - 1;
	moveBufferPointerForward(b, &start);
	moveBufferPointerBack(&end);
	char* text = makeMugenDefStringFromEndPoint(start, end);
	MugenDefToken* variableToken = makeMugenDefToken(text);
	destroyMugenDefString(text);

	start = equal + 1;
	end = equal;
	if (increasePointerToNextLine(b, &end)) end = ((char*)b->mData) + b->mLength - 1;
	else {
		decreaseAndCheckIfOver(&end);
	}
	while (isLinebreak(end)) decreaseAndCheckIfOver(&end);

	moveBufferPointerForward(b, &start);
	moveBufferPointerBack(&end);
	end = removeCommentFromToken(start, end);
	text = makeMugenDefStringFromEndPoint(start, end);
	MugenDefToken* valueToken = makeMugenDefToken(text);
	destroyMugenDefString(text);

	debugString(variableToken->mValue);
	debugString(valueToken->mValue);

	equalToken->mNext = variableToken;
	variableToken->mNext = valueToken;
	if (!increasePointerToNextLine(b, &p)) {
		valueToken->mNext = parseRecursively(b, p);
	}
	return equalToken;
}


char* getLineAsAllocatedString(Buffer* b, BufferPointer p) {
	BufferPointer start = p;
	BufferPointer end = p;
	if (increasePointerToNextLine(b, &end)) end = ((char*)b->mData) + b->mLength - 1;
	else {
		decreaseAndCheckIfOver(&end);
	}
	while (isLinebreak(end)) decreaseAndCheckIfOver(&end);
	moveBufferPointerBack(&end);
	end = removeCommentFromToken(start, end);
	char* text = makeMugenDefStringFromEndPoint(start, end);
	return text;
}

static MugenDefToken* parseVectorStatement(Buffer* b, BufferPointer p) {

	MugenDefToken* isVectorToken = makeMugenDefToken("vector_statement");

	char* text = getLineAsAllocatedString(b, p);
	MugenDefToken* valueToken = makeMugenDefToken(text);
	destroyMugenDefString(text);
	isVectorToken->mNext = valueToken;
	
	if (!increasePointerToNextLine(b, &p)) {
		valueToken->mNext = parseRecursively(b, p);
	}

	return isVectorToken;
}

static MugenDefToken* parseLoopStartStatement(Buffer* b, BufferPointer p) {

	MugenDefToken* loopStartToken = makeMugenDefToken("Loopstart");

	if (!increasePointerToNextLine(b, &p)) {
		loopStartToken->mNext = parseRecursively(b, p);
	}

	return loopStartToken;
}

static MugenDefToken* parseGroup(Buffer* b, BufferPointer p) {
	debugLog("Parse group.")
	
	BufferPointer end = findEndOfToken(b, p, '[', ']');

	char* val = makeMugenDefStringFromEndPoint(p, end);
	MugenDefToken* groupToken = makeMugenDefToken(val);
	destroyMugenDefString(val);

	debugString(groupToken->mValue);

	if (!increasePointerToNextLine(b, &p)) {
		groupToken->mNext = parseRecursively(b, p);
	}
	return groupToken;
}

static int isGroup(BufferPointer p) {
	return *p == '[';
}

static int searchForChar(Buffer* b, BufferPointer p, char tChar) {
	while (p < (char*)b->mData + b->mLength && !isLinebreak(p) && !isComment(p)) {
		if (*p == tChar) return 1;
		p++;
	}

	return 0;
}

static int isAssignment(Buffer* b, BufferPointer p, char tAssignmentChar) {
	return searchForChar(b, p, tAssignmentChar);
}

static int isVectorStatement(Buffer* b, BufferPointer p) {
	return searchForChar(b, p, ',');
}

static int isLoopStartStatement(Buffer* b, BufferPointer p) {
	char* text = getLineAsAllocatedString(b, p);
	int ret = !strcmp("Loopstart", text);
	destroyMugenDefString(text);
	return ret;
}

static MugenDefToken* parseRecursively(Buffer* b, BufferPointer p) {
	while (isEmpty(p)) {
		if (increaseAndCheckIfOver(b, &p)) return NULL;
	}

	if (isComment(p)) return parseComment(b, p);
	else if (isGroup(p)) return parseGroup(b, p);
	else if(isAssignment(b, p, '=')) return parseAssignment(b, p, '=');
	else if(isAssignment(b, p, ':')) return parseAssignment(b, p, ':');
	else if (isVectorStatement(b, p)) return parseVectorStatement(b, p);
	else if (isLoopStartStatement(b, p)) return parseLoopStartStatement(b, p);
	else {
		logError("Unable to parse token.");
		p[100] = '\0';
		printf("%s\n", p);
		
		abortSystem();
		return NULL;
	}
}

static MugenDefScript makeEmptyMugenDefScript() {
	MugenDefScript d;
	d.mGroups = new_string_map();
	d.mFirstGroup = NULL;
	return d;
}

static int isGroupToken(MugenDefToken* t) {
	return t->mValue[0] == '[';
}

static struct {
	char mGroup[100];

} gScriptMaker;

static void setGroup(MugenDefScript* tScript, MugenDefToken* t) {
	debugLog("Setting group.");

	MugenDefScriptGroup* prev;
	if (string_map_contains(&tScript->mGroups, gScriptMaker.mGroup)) {
		prev = string_map_get(&tScript->mGroups, gScriptMaker.mGroup);
	}
	else {
		prev = NULL;
	}
		
	strcpy(gScriptMaker.mGroup, t->mValue + 1);
	gScriptMaker.mGroup[strlen(gScriptMaker.mGroup) - 1] = '\0';

	MugenDefScriptGroup* e = allocMemory(sizeof(MugenDefScriptGroup));
	e->mElements = new_string_map();
	strcpy(e->mName, gScriptMaker.mGroup);
	e->mNext = NULL;
	e->mOrderedElementList = new_list();

	
	string_map_push_owned(&tScript->mGroups, gScriptMaker.mGroup, e);

	if (prev != NULL) {
		prev->mNext = e;
	}

	if (tScript->mFirstGroup == NULL) {
		tScript->mFirstGroup = e;
	}

	debugString(gScriptMaker.mGroup);
}

static int isStringToken(MugenDefToken* t) {
	return t->mValue[0] == '"';
}

static int isVectorToken(MugenDefToken* t) {
	int len = strlen(t->mValue);
	int i;
	int isVector = 0;
	for (i = 0; i < len; i++) {
		if (t->mValue[i] == ',') isVector = 1;
	}

	return isVector;
}


static int isNumberToken(MugenDefToken* t) {
	char* p;
	if (t->mValue[0] == '-') p = t->mValue + 1;
	else p = t->mValue;

	int len = strlen(p);
	int i;
	for (i = 0; i < len; i++) {
		if (*p < '0' || *p > '9') return 0;
		p++;
	}

	
	return 1;
}

static int isFloatToken(MugenDefToken* t) {
	char* p;
	if (t->mValue[0] == '-') p = t->mValue + 1;
	else p = t->mValue;

	int len = strlen(p);
	int i;
	int hasPoint = 0;
	for (i = 0; i < len; i++) {
		if (*p == '.') {
			if (hasPoint) return 0;
			else hasPoint = 1;
		}
		else if (*p < '0' || *p > '9') return 0;

		p++;
	}

	return 1;
}

static void setStringElement(MugenDefScriptGroupElement* element, MugenDefToken* t) {
	debugLog("Setting string element.");
	element->mType = MUGEN_DEF_SCRIPT_GROUP_STRING_ELEMENT;
	
	MugenDefScriptStringElement* e = allocMemory(sizeof(MugenDefScriptStringElement));
	strcpy(e->mString, t->mValue + 1);
	e->mString[strlen(e->mString) - 1] = '\0';
	element->mData = e;

	debugString(e->mString);
}

static void setNumberElement(MugenDefScriptGroupElement* element, MugenDefToken* t) {
	debugLog("Setting number element.");
	element->mType = MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT;

	MugenDefScriptNumberElement* e = allocMemory(sizeof(MugenDefScriptNumberElement));
	e->mValue = atoi(t->mValue);
	element->mData = e;

	debugInteger(e->mValue);
}

static void setFloatElement(MugenDefScriptGroupElement* element, MugenDefToken* t) {
	debugLog("Setting float element.");
	element->mType = MUGEN_DEF_SCRIPT_GROUP_FLOAT_ELEMENT;

	MugenDefScriptFloatElement* e = allocMemory(sizeof(MugenDefScriptFloatElement));
	e->mValue = atof(t->mValue);
	element->mData = e;

	debugDouble(e->mValue);
}



static void setVectorElement(MugenDefScriptGroupElement* element, MugenDefToken* t) {
	debugLog("Setting vector element.");
	element->mType = MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT;

	MugenDefScriptVectorElement* e = allocMemory(sizeof(MugenDefScriptVectorElement));
	
	memset(e->mVector.mElement, 0, sizeof e->mVector.mElement);

	char* comma = t->mValue - 1;
	int i = 0;
	while (comma != NULL) {
		strcpy(e->mVector.mElement[i], comma+1);
		comma = strchr(e->mVector.mElement[i], ',');
		if (comma != NULL) *comma = '\0';
		i++;
	}

	element->mData = e;
}

static void setRawElement(MugenDefScriptGroupElement* element, MugenDefToken* t) {
	debugLog("Setting raw element.");
	element->mType = MUGEN_DEF_SCRIPT_GROUP_STRING_ELEMENT;

	MugenDefScriptStringElement* e = allocMemory(sizeof(MugenDefScriptStringElement));
	strcpy(e->mString, t->mValue);
	element->mData = e;

	debugString(e->mString);
}

static void addGroupElementToGroup(MugenDefScript* tScript, MugenDefScriptGroupElement* tElement, char* tVariableName) {
	debugString(gScriptMaker.mGroup);
	assert(string_map_contains(&tScript->mGroups, gScriptMaker.mGroup));

	strcpy(tElement->mName, tVariableName);
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, gScriptMaker.mGroup);
	string_map_push_owned(&e->mElements, tVariableName, tElement);
	list_push_back(&e->mOrderedElementList, tElement);
}

static void setAssignment(MugenDefScript* tScript, MugenDefToken** tToken) {
	debugLog("Setting assignment.");

	MugenDefToken* equalToken = *tToken;
	*tToken = (*tToken)->mNext;
	assert(equalToken != NULL);
	assert(!strcmp("=", equalToken->mValue));

	MugenDefToken* variableToken = *tToken;
	*tToken = (*tToken)->mNext;
	assert(variableToken != NULL);
	debugString(variableToken->mValue);

	MugenDefToken* valueToken = *tToken;
	assert(valueToken != NULL);
	debugString(valueToken->mValue);

	MugenDefScriptGroupElement* element = allocMemory(sizeof(MugenDefScriptGroupElement));
	if (isStringToken(valueToken)) {
		setStringElement(element, valueToken);
	}
	else if (isNumberToken(valueToken)) {
		setNumberElement(element, valueToken);
	}
	else if (isFloatToken(valueToken)) {
		setFloatElement(element, valueToken);
	}
	else if (isVectorToken(valueToken)) {
		setVectorElement(element, valueToken);
	}
	else {
		setRawElement(element, valueToken);
	}

	debugLog("Adding assignment to group.");
	addGroupElementToGroup(tScript, element, variableToken->mValue);
}

static int gVectorStatementCounter;

static void setVectorStatement(MugenDefScript* tScript, MugenDefToken** tToken) {
	debugLog("Setting vector statement.");

	MugenDefToken* isVectorStatementToken = *tToken;
	*tToken = (*tToken)->mNext;
	assert(isVectorStatementToken != NULL);
	assert(!strcmp("vector_statement", isVectorStatementToken->mValue));

	MugenDefToken* valueToken = *tToken;
	assert(valueToken != NULL);
	debugString(valueToken->mValue);

	MugenDefScriptGroupElement* element = allocMemory(sizeof(MugenDefScriptGroupElement));
	assert(isVectorToken(*tToken));
	setVectorElement(element, valueToken);
	
	debugLog("Adding vector statement to group.");
	char key[100];
	sprintf(key, "vector_statement %d", gVectorStatementCounter++);
	addGroupElementToGroup(tScript, element, key);
}

static int gLoopStartStatementCounter;

static void setLoopStartStatement(MugenDefScript* tScript, MugenDefToken** tToken) {
	debugLog("Setting loop start.");

	MugenDefToken* loopStartToken = *tToken;
	assert(loopStartToken != NULL);
	assert(!strcmp("Loopstart", loopStartToken->mValue));

	
	MugenDefScriptGroupElement* element = allocMemory(sizeof(MugenDefScriptGroupElement));
	setRawElement(element, *tToken);

	debugLog("Adding loop start to group.");
	char key[100];
	sprintf(key, "Loopstart %d", gLoopStartStatementCounter++);
	addGroupElementToGroup(tScript, element, key);
}

static int isAssignmentToken(MugenDefToken* tToken) {
	return !strcmp(tToken->mValue, "=");
}

static int isVectorStatementToken(MugenDefToken* tToken) {
	return !strcmp(tToken->mValue, "vector_statement");
}

static int isLoopStartStatementToken(MugenDefToken* tToken) {
	return !strcmp(tToken->mValue, "Loopstart");
}


static void tokensToDefScript(MugenDefScript* tScript, MugenDefToken* tToken) {
	if (tToken == NULL) return;

	if (isGroupToken(tToken)) setGroup(tScript, tToken);
	else if(isAssignmentToken(tToken)){
		setAssignment(tScript, &tToken);
	}
	else if (isVectorStatementToken(tToken)) {
		setVectorStatement(tScript, &tToken);
	}
	else if (isLoopStartStatementToken(tToken)) {
		setLoopStartStatement(tScript, &tToken);
	}
	else {
		logError("Unable to read token.");
		logErrorString(tToken->mValue);
		abortSystem();
	}

	tokensToDefScript(tScript, tToken->mNext);
}

static void deleteTokens(MugenDefToken* t) {
	if (t == NULL) return;
	deleteTokens(t->mNext);
	freeMemory(t);
}

MugenDefScript loadMugenDefScript(char * tPath)
{
	debugLog("Start loading script.");
	debugString(tPath);
	

	Buffer b = fileToBuffer(tPath);
	BufferPointer p = getBufferPointer(b);
	
	debugLog("Parse file to tokens.");
	MugenDefToken* root = parseRecursively(&b, p);

	debugLog("Parse tokens to script.");
	MugenDefScript d = makeEmptyMugenDefScript();
	tokensToDefScript(&d, root);
	deleteTokens(root);

	return d;
}

int isMugenDefStringVariable(MugenDefScript* tScript, char * tGroupName, char * tVariableName)
{
	if (!string_map_contains(&tScript->mGroups, tGroupName)) {
		return 0;
	}
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	if (!string_map_contains(&e->mElements, tVariableName)) {
		return 0;
	}

	return 1;
}

char* getAllocatedMugenDefStringVariable(MugenDefScript* tScript, char * tGroupName, char * tVariableName)
{
	assert(string_map_contains(&tScript->mGroups, tGroupName));
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	assert(string_map_contains(&e->mElements, tVariableName));
	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	char* ret = allocMemory(1024);
	if (element->mType == MUGEN_DEF_SCRIPT_GROUP_STRING_ELEMENT) {
		MugenDefScriptStringElement* stringElement = element->mData;
		strcpy(ret, stringElement->mString);
	} else if (element->mType == MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT) {
		MugenDefScriptNumberElement* numberElement = element->mData;
		sprintf(ret, "%d", numberElement->mValue);
	} else if (element->mType == MUGEN_DEF_SCRIPT_GROUP_FLOAT_ELEMENT) {
		MugenDefScriptFloatElement* floatElement = element->mData;
		sprintf(ret, "%f", floatElement->mValue);
	}
	else if (element->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT) {
		MugenDefScriptVectorElement* vectorElement = element->mData;
		sprintf(ret, "%s,%s,%s,%s,%s,%s", vectorElement->mVector.mElement[0], vectorElement->mVector.mElement[1], vectorElement->mVector.mElement[2], vectorElement->mVector.mElement[3], vectorElement->mVector.mElement[4], vectorElement->mVector.mElement[5]);
	}
	else {
		logError("Unknown type.");
		logErrorInteger(element->mType);
		abortSystem();
	}
	return ret;
}

int isMugenDefFloatVariable(MugenDefScript * tScript, char * tGroupName, char * tVariableName)
{
	if (!string_map_contains(&tScript->mGroups, tGroupName)) {
		return 0;
	}
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	if (!string_map_contains(&e->mElements, tVariableName)) {
		return 0;
	}

	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	return element->mType == MUGEN_DEF_SCRIPT_GROUP_FLOAT_ELEMENT || element->mType == MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT;
}

double getMugenDefFloatVariable(MugenDefScript * tScript, char * tGroupName, char * tVariableName)
{
	assert(string_map_contains(&tScript->mGroups, tGroupName));
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	assert(string_map_contains(&e->mElements, tVariableName));
	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	assert(element->mType == MUGEN_DEF_SCRIPT_GROUP_FLOAT_ELEMENT || element->mType == MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT);

	double ret;
	if (element->mType == MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT) {
		MugenDefScriptNumberElement* numberElement = element->mData;
		ret = numberElement->mValue;
	}
	else if (element->mType == MUGEN_DEF_SCRIPT_GROUP_FLOAT_ELEMENT) {
		MugenDefScriptFloatElement* floatElement = element->mData;
		ret = floatElement->mValue;
	}
	else {
		ret = 0;
		logError("Unknown type.");
		logErrorInteger(element->mType);
		abortSystem();
	}

	return ret;
}


int isMugenDefNumberVariable(MugenDefScript * tScript, char * tGroupName, char * tVariableName)
{
	if (!string_map_contains(&tScript->mGroups, tGroupName)) {
		return 0;
	}
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	if (!string_map_contains(&e->mElements, tVariableName)) {
		return 0;
	}

	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	return element->mType == MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT;
}


int getMugenDefNumberVariable(MugenDefScript * tScript, char * tGroupName, char * tVariableName)
{
	assert(string_map_contains(&tScript->mGroups, tGroupName));
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	assert(string_map_contains(&e->mElements, tVariableName));
	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	assert(element->mType == MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT);

	int ret;
	if (element->mType == MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT) {
		MugenDefScriptNumberElement* numberElement = element->mData;
		ret = numberElement->mValue;
	}
	else {
		ret = 0;
		logError("Unknown type.");
		logErrorInteger(element->mType);
		abortSystem();
	}

	return ret;
}

int isMugenDefVectorVariable(MugenDefScript * tScript, char * tGroupName, char * tVariableName)
{
	if (!string_map_contains(&tScript->mGroups, tGroupName)) {
		return 0;
	}
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	if (!string_map_contains(&e->mElements, tVariableName)) {
		return 0;
	}

	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	return element->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT;
}

Vector3D getMugenDefVectorVariable(MugenDefScript * tScript, char * tGroupName, char * tVariableName)
{
	assert(string_map_contains(&tScript->mGroups, tGroupName));
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	assert(string_map_contains(&e->mElements, tVariableName));
	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	assert(element->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT);

	Vector3D ret;
	if (element->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT) {
		MugenDefScriptVectorElement* vectorElement = element->mData;
		double x = atof(vectorElement->mVector.mElement[0]);
		double y = atof(vectorElement->mVector.mElement[1]);
		double z = atof(vectorElement->mVector.mElement[2]);
		ret = makePosition(x, y, z);
	}
	else {
		ret = makePosition(0, 0, 0);
		logError("Unknown type.");
		logErrorInteger(element->mType);
		abortSystem();
	}

	return ret;
}

int isMugenDefVectorIVariable(MugenDefScript * tScript, char * tGroupName, char * tVariableName)
{

	if (!string_map_contains(&tScript->mGroups, tGroupName)) {
		return 0;
	}
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	if (!string_map_contains(&e->mElements, tVariableName)) {
		return 0;
	}

	
	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	return element->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT;
}

Vector3DI getMugenDefVectorIVariable(MugenDefScript * tScript, char * tGroupName, char * tVariableName)
{
	assert(string_map_contains(&tScript->mGroups, tGroupName));
	MugenDefScriptGroup* e = string_map_get(&tScript->mGroups, tGroupName);

	assert(string_map_contains(&e->mElements, tVariableName));
	MugenDefScriptGroupElement* element = string_map_get(&e->mElements, tVariableName);

	assert(element->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT);

	Vector3DI ret;
	if (element->mType == MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT) {
		MugenDefScriptVectorElement* vectorElement = element->mData;
		int x = atoi(vectorElement->mVector.mElement[0]);
		int y = atoi(vectorElement->mVector.mElement[1]);
		int z = atoi(vectorElement->mVector.mElement[2]);
		ret = makeVector3DI(x, y, z);
	}
	else {
		ret = makeVector3DI(0, 0, 0);
		logError("Unknown type.");
		logErrorInteger(element->mType);
		abortSystem();
	}

	return ret;
}