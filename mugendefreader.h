#pragma once

#include <tari/datastructures.h>
#include <tari/geometry.h>

#define MUGEN_DEF_STRING_LENGTH 500

typedef enum {
	MUGEN_DEF_SCRIPT_GROUP_STRING_ELEMENT,
	MUGEN_DEF_SCRIPT_GROUP_NUMBER_ELEMENT,
	MUGEN_DEF_SCRIPT_GROUP_FLOAT_ELEMENT,
	MUGEN_DEF_SCRIPT_GROUP_VECTOR_ELEMENT,
} MugenDefScriptGroupElementType;

typedef struct {
	int mSize;
	char** mElement;

} MugenStringVector;

typedef struct {
	MugenStringVector mVector;
} MugenDefScriptVectorElement;

typedef struct {
	char* mString;

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

int isMugenDefVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);

int isMugenDefFloatVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
double getMugenDefFloatVariable(MugenDefScript* tScript, char* tGroupName, char* tVariableName);
int isMugenDefFloatVariableAsElement(MugenDefScriptGroupElement* tElement);
double getMugenDefFloatVariableAsElement(MugenDefScriptGroupElement* tElement);


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
MugenStringVector copyMugenDefStringVectorVariableAsElement(MugenDefScriptGroupElement * tElement);

void loadStringOrDefault(char* tDst, MugenDefScript* s, char* tGroup, char* tVariable, char* tDefault);

void loadFloatOrDefault(double* tDst, MugenDefScript* s, char* tGroup, char* tVariable, double tDefault);
void loadIntegerOrDefault(int* tDst, MugenDefScript* s, char* tGroup, char* tVariable, int tDefault);
void loadVectorOrDefault(Vector3D* tDst, MugenDefScript* s, char* tGroup, char* tVariable, Vector3D tDefault);
void loadVectorIOrDefault(Vector3DI* tDst, MugenDefScript* s, char* tGroup, char* tVariable, Vector3DI tDefault);