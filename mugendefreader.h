#pragma once

#include <tari/datastructures.h>
#include <tari/geometry.h>

typedef enum {
	MUGEN_DEF_SCRIPT_GROUP_STRING_ELEMENT,
	MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT,
	MUGEN_DEF_SCRIPT_GROUP_FLOAT_ELEMENT,
	MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT,
} MugenDefScriptGroupElementType;

typedef struct {
	char mElement[20][100];

} MugenStringVector;

typedef struct {
	MugenStringVector mVector;
} MugenDefScriptVectorElement;

typedef struct {
	char mString[100];

} MugenDefScriptStringElement;

typedef struct {
	int mValue;
} MugenDefScriptNumberElement;

typedef struct {
	double mValue;
} MugenDefScriptFloatElement;

typedef struct {
	char mName[100];
	MugenDefScriptGroupElementType mType;
	void* mData;

} MugenDefScriptGroupElement;

typedef struct MugenDefScriptGroup_t{
	char mName[100];
	StringMap mElements;
	List mOrderedElementList;
	struct MugenDefScriptGroup_t* mNext;
} MugenDefScriptGroup;

typedef struct {
	MugenDefScriptGroup* mFirstGroup;
	StringMap mGroups;
} MugenDefScript;

MugenDefScript loadMugenDefScript(char* tPath);
void unloadMugenDefScript(MugenDefScript tScript);
int isMugenDefStringVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
char* getAllocatedMugenDefStringVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
int isMugenDefStringVariableAsElement(MugenDefScriptGroupElement* tElement);
char* getAllocatedMugenDefStringVariableAsElement(MugenDefScriptGroupElement* tElement);


int isMugenDefFloatVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
double getMugenDefFloatVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);

int isMugenDefNumberVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
int getMugenDefNumberVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
int isMugenDefNumberVariableAsElement(MugenDefScriptGroupElement* tElement);
int getMugenDefNumberVariableAsElement(MugenDefScriptGroupElement* tElement);

int isMugenDefVectorVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
Vector3D getMugenDefVectorVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
int isMugenDefVectorVariableAsElement(MugenDefScriptGroupElement* tElement);
Vector3D getMugenDefVectorVariableAsElement(MugenDefScriptGroupElement* tElement);

int isMugenDefVectorIVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
Vector3DI getMugenDefVectorIVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);

int isMugenDefStringVectorVariableAsElement(MugenDefScriptGroupElement* tElement);
MugenStringVector getMugenDefStringVectorVariableAsElement(MugenDefScriptGroupElement* tElement);