#include "mugenassignment.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <tari/memoryhandler.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/math.h>

#include "playerdefinition.h"
#include "stage.h"
#include "playerhitdata.h"
#include "gamelogic.h"

static MugenAssignment* makeMugenAssignment(MugenAssignmentType tType, void* tData) {
	MugenAssignment* e = allocMemory(sizeof(MugenAssignment));
	e->mType = tType;
	e->mData = tData;
	return e;
}

MugenAssignment * makeTrueMugenAssignment()
{
	MugenFixedBooleanAssignment* data = allocMemory(sizeof(MugenFixedBooleanAssignment));
	data->mValue = 1;
	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_FIXED_BOOLEAN, data);
}

void destroyFalseMugenAssignment(MugenAssignment* tAssignment) {
	freeMemory(tAssignment->mData);
	freeMemory(tAssignment);
}

void destroyMugenAssignment(MugenAssignment * tAssignment)
{
	// TODO
	freeMemory(tAssignment->mData);
	freeMemory(tAssignment);
}

MugenAssignment * makeFalseMugenAssignment()
{
	MugenFixedBooleanAssignment* data = allocMemory(sizeof(MugenFixedBooleanAssignment));
	data->mValue = 0;
	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_FIXED_BOOLEAN, data);
}

MugenAssignment * makeMugenOneElementAssignment(MugenAssignmentType tType, MugenAssignment * a) 
{

	MugenDependOnOneAssignment* data = allocMemory(sizeof(MugenDependOnOneAssignment));
	data->a = a;
	return makeMugenAssignment(tType, data);
}


MugenAssignment * makeMugenTwoElementAssignment(MugenAssignmentType tType, MugenAssignment * a, MugenAssignment * b)
{
	MugenDependOnTwoAssignment* data = allocMemory(sizeof(MugenDependOnTwoAssignment));
	data->a = a;
	data->b = b;
	return makeMugenAssignment(tType, data);
}



MugenAssignment * makeNumberMugenAssignment(int tVal)
{

	MugenNumberAssignment* number = allocMemory(sizeof(MugenNumberAssignment));
	number->mValue = tVal;

	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_NUMBER, number);
}

MugenAssignment * makeFloatMugenAssignment(double tVal)
{
	MugenFloatAssignment* f = allocMemory(sizeof(MugenFloatAssignment));
	f->mValue = tVal;

	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_FLOAT, f);
}

MugenAssignment * makeStringMugenAssignment(char * tVal)
{
	MugenStringAssignment* s = allocMemory(sizeof(MugenStringAssignment));
	s->mValue = allocMemory(strlen(tVal) + 10);
	strcpy(s->mValue, tVal);

	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_STRING, s);
}

MugenAssignment * make2DVectorMugenAssignment(Vector3D tVal)
{
	MugenDependOnTwoAssignment* data = allocMemory(sizeof(MugenDependOnTwoAssignment));
	data->a = makeFloatMugenAssignment(tVal.x);
	data->b = makeFloatMugenAssignment(tVal.y);
	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_VECTOR, data);
}

MugenAssignment * makeAndMugenAssignment(MugenAssignment * a, MugenAssignment * b)
{
	return makeMugenTwoElementAssignment(MUGEN_ASSIGNMENT_TYPE_AND, a, b);
}

MugenAssignment * makeOrMugenAssignment(MugenAssignment * a, MugenAssignment * b)
{
	return makeMugenTwoElementAssignment(MUGEN_ASSIGNMENT_TYPE_OR, a, b);
}

static int isOnHighestLevelWithStartPosition(char* tText, char* tPattern, int* tOptionalPosition, int tStart) {
	int n = strlen(tText);
	int m = strlen(tPattern);

	int depth1 = 0;
	int depth2 = 0;
	int depth3 = 0;
	int i;
	for (i = tStart; i < n - m + 1; i++) {
		if (tText[i] == '(') depth1++;
		if (tText[i] == ')') depth1--;
		if (tText[i] == '[') depth2++;
		if (tText[i] == ']') depth2--;
		if (tText[i] == '"') depth3 ^= 1;

		if (depth1 || depth2 || depth3) continue;

		int isSame = 1;
		int j;
		for (j = 0; j < m; j++) {
			if (tText[i + j] != tPattern[j]) {
				isSame = 0;
				break;
			}
		}

		if (isSame) {
			if (tOptionalPosition) *tOptionalPosition = i;
			return 1;
		}
	}

	return 0;
}

static int isOnHighestLevel(char* tText, char* tPattern, int* tOptionalPosition) {
	return isOnHighestLevelWithStartPosition(tText, tPattern, tOptionalPosition, 0);
}

static MugenAssignment* parseOneElementMugenAssignmentFromString(char* tText, MugenAssignmentType tType) {
	
	MugenAssignment* a = parseMugenAssignmentFromString(tText);

	return makeMugenOneElementAssignment(tType, a);
}

static MugenAssignment* parseTwoElementMugenAssignmentFromStringWithFixedPosition(char* tText, MugenAssignmentType tType, char* tPattern, int tPosition) {
	int n = strlen(tText);
	int m = strlen(tPattern);

	assert(tPosition != -1);
	assert(tPosition > 0);
	assert(tPosition < n - m);

	int start = tPosition;
	int end = tPosition + m;

	char text1[MUGEN_DEF_STRING_LENGTH];
	char text2[MUGEN_DEF_STRING_LENGTH];

	strcpy(text1, tText);
	text1[start] = '\0';

	strcpy(text2, &tText[end]);

	MugenAssignment* a = parseMugenAssignmentFromString(text1);
	MugenAssignment* b = parseMugenAssignmentFromString(text2);

	return makeMugenTwoElementAssignment(tType, a, b);
}

static MugenAssignment* parseTwoElementMugenAssignmentFromString(char* tText, MugenAssignmentType tType, char* tPattern) {
	int pos = -1;
	isOnHighestLevel(tText, tPattern, &pos);
	return parseTwoElementMugenAssignmentFromStringWithFixedPosition(tText, tType, tPattern, pos);
}

static int isEmpty(char* tChar) {
	return !strcmp("", tChar);
}

static int isEmptyCharacter(char tChar) {
	return tChar == ' ';
}

static MugenAssignment* parseMugenNullFromString() {
	MugenFixedBooleanAssignment* data = allocMemory(sizeof(MugenFixedBooleanAssignment));
	data->mValue = 0;
	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_NULL, data);
}



static int isInBraces(char* tText) {
	int n = strlen(tText);
	if (tText[0] != '(' || tText[n - 1] != ')') return 0;

	int depth = 0;
	int i;
	for (i = 0; i < n - 1; i++) {
		if (tText[i] == '(') depth++;
		if (tText[i] == ')') depth--;

		if (!depth) return 0;
	}

	return 1;
}

static MugenAssignment* parseMugenAssignmentStringInBraces(char* tText) {
	int n = strlen(tText);
	tText[n - 1] = '\0';
	tText++;

	return parseMugenAssignmentFromString(tText);
}

static int isNegation(char* tText) {
	return (tText[0] == '!');
}

static MugenAssignment* parseMugenNegationFromString(char* tText) {
	tText++;
	return parseOneElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_NEGATION);
}

static int isUnaryMinus(char* tText) {
	return (tText[0] == '-');
}

static MugenAssignment* parseMugenUnaryMinusFromString(char* tText) {
	tText++;
	return parseOneElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_UNARY_MINUS);
}

static int isRange(char* tText) {
	int n = strlen(tText);
	if ((tText[0] != '[' && tText[0] != '(') || (tText[n - 1] != ']' && tText[n - 1] != ')')) return 0;

	int depth = 0;
	int i;
	for (i = 0; i < n - 1; i++) {
		if (tText[i] == '(' || tText[i] == '[') depth++;
		if (tText[i] == ')' || tText[i] == ']') depth--;

		if (!depth) return 0;
	}

	return 1;
}

static MugenAssignment* parseMugenRangeFromString(char* tText) {
	MugenRangeAssignment* e = allocMemory(sizeof(MugenRangeAssignment));
	int n = strlen(tText);
	e->mExcludeLeft = tText[0] == '(';
	e->mExcludeRight = tText[n - 1] == ')';
	
	tText[n - 1] = '\0';
	tText++;

	e->a = parseMugenAssignmentFromString(tText);
	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_RANGE, e);
}


static int isInequality(char* tText) {
	return isOnHighestLevel(tText, "!=", NULL);
}

static MugenAssignment* parseMugenInequalityFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_INEQUALITY, "!=");
}

static int isVariableSet(char* tText) {
	return isOnHighestLevel(tText, ":=", NULL);
}

static MugenAssignment* parseMugenVariableSetFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_SET_VARIABLE, ":=");
}

static int isComparison(char* tText) {
	int position;

	// TODO: check for multiple
	if(!isOnHighestLevel(tText, "=", &position)) return 0;
	if (position == 0) return 0;
	if (tText[position - 1] == '!') return 0;
	if (tText[position - 1] == '<') return 0;
	if (tText[position - 1] == '>') return 0;
	if (tText[position - 1] == ':') return 0;

	return 1;
}

static MugenAssignment* parseMugenComparisonFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_COMPARISON, "=");
}

static int isAnd(char* tText) {
	return isOnHighestLevel(tText, "&&", NULL);
}

static MugenAssignment* parseMugenAndFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_AND, "&&");
}

static int isOr(char* tText) {
	return isOnHighestLevel(tText, "||", NULL);
}

static MugenAssignment* parseMugenOrFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_OR, "||");
}

static int isBitwiseAnd(char* tText) {
	return isOnHighestLevel(tText, "&", NULL);
}

static MugenAssignment* parseMugenBitwiseAndFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_BITWISE_AND, "&");
}

static int isBitwiseOr(char* tText) {
	return isOnHighestLevel(tText, "|", NULL);
}

static MugenAssignment* parseMugenBitwiseOrFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_BITWISE_OR, "|");
}

static int isLessOrEqualThan(char* tText) {
	int position;
	if (!isOnHighestLevel(tText, "<=", &position)) return 0;
	if (position == 0) return 0;

	return 1;
}

static MugenAssignment* parseMugenLessOrEqualFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_LESS_OR_EQUAL, "<=");
}

static int isLessThan(char* tText) {
	int position;
	if (!isOnHighestLevel(tText, "<", &position)) return 0;
	if (position == 0) return 0;

	return 1;
}

static MugenAssignment* parseMugenLessFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_LESS, "<");
}

static int isGreaterOrEqualThan(char* tText) {
	int position;
	if (!isOnHighestLevel(tText, ">=", &position)) return 0;
	if (position == 0) return 0;

	return 1;
}

static MugenAssignment* parseMugenGreaterOrEqualFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_GREATER_OR_EQUAL, ">=");
}

static int isGreaterThan(char* tText) {
	int position;
	if(!isOnHighestLevel(tText, ">", &position)) return 0;
	if (position == 0) return 0;

	return 1;
}

static MugenAssignment* parseMugenGreaterFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_GREATER, ">");
}

static int isExponentiation(char* tText) {
	return isOnHighestLevel(tText, "**", NULL);
}

static MugenAssignment* parseMugenExponentiationFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_EXPONENTIATION, "**");
}

static int isAddition(char* tText) {
	return isOnHighestLevel(tText, "+", NULL);
}

static MugenAssignment* parseMugenAdditionFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_ADDITION, "+");
}

static int isOperatorCharacter(char tChar) {
	return tChar == '-' || tChar == '+' || tChar == '|' || tChar == '&' || tChar == '*' || tChar == '/';
}

static int isBinaryOperator(char* tText, int tPosition) {
	int p = tPosition - 1;
	int poss = 0;
	while (p >= 0) {
		if (isEmptyCharacter(tText[p])) p--;
		else if (isOperatorCharacter(tText[p])) return 0;
		else {
			poss = 1;
			break;
		}
	}

	if (!poss) return 0;

	int n = strlen(tText);
	p = tPosition + 1;
	poss = 0;
	while (p < n) {
		if (isEmptyCharacter(tText[p])) p++;
		else if (isOperatorCharacter(tText[p])) return 0;
		else {
			poss = 1;
			break;
		}
	}

	return poss;
}

static int isSubtraction(char* tText) {
	int position;
	if(!isOnHighestLevel(tText, "-", &position)) return 0;
	return isBinaryOperator(tText, position);
}

static MugenAssignment* parseMugenSubtractionFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_SUBTRACTION, "-");
}

static int isMultiplication(char* tText) {
	int position = 0;
	int n = strlen(tText);

	// TODO: check for multiple
	if (!isOnHighestLevel(tText, "*", &position)) return 0;
	if (position == 0) return 0;
	if (position < n-1 && tText[position + 1] == '*') return 0;

	return 1;
}

static MugenAssignment* parseMugenMultiplicationFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_MULTIPLICATION, "*");
}

static int isDivision(char* tText) {
	return isOnHighestLevel(tText, "/", NULL);
}

static MugenAssignment* parseMugenDivisionFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_DIVISION, "/");
}

static int isModulo(char* tText) {
	return isOnHighestLevel(tText, "%", NULL);
}

static MugenAssignment* parseMugenModuloFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_MODULO, "%");
}

static int isNumericalConstant(char* tText) {
	if (*tText == '-') tText++;

	int n = strlen(tText);
	if (n == 0) return 0;

	int mPointAmount = 0;
	int i;
	for (i = 0; i < n; i++) {
		if (tText[i] == '.') mPointAmount++;
		else if (tText[i] >= '0' && tText[i] <= '9') continue;
		else return 0;
	}

	return mPointAmount <= 0;
}

static MugenAssignment* parseNumericalConstantFromString(char* tText) {
	int val = atoi(tText);

	return makeNumberMugenAssignment(val);
}

static int isFloatConstant(char* tText) {
	if (*tText == '-') tText++;

	int n = strlen(tText);
	if (n == 0) return 0;

	int mPointAmount = 0;
	int i;
	for (i = 0; i < n; i++) {
		if (tText[i] == '.') mPointAmount++;
		else if (tText[i] >= '0' && tText[i] <= '9') continue;
		else return 0;
	}

	return mPointAmount <= 1;
}

static MugenAssignment* parseFloatConstantFromString(char* tText) {
	double f = atof(tText);

	return makeFloatMugenAssignment(f);
}

static int isStringConstant(char* tText) {
	int n = strlen(tText);
	if (n == 0) return 0;
	return tText[0] == '"' && tText[n - 1] == '"';
}

static MugenAssignment* parseStringConstantFromString(char* tText) {
	MugenStringAssignment* s = allocMemory(sizeof(MugenStringAssignment));
	s->mValue = allocMemory(strlen(tText + 1) + 10);
	strcpy(s->mValue, tText+1);
	s->mValue[strlen(s->mValue) - 1] = '\0';

	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_STRING, s);
}

int doAssignmentStringsBeginsWithPattern(char* tPattern, char* tText) {
	int n = strlen(tPattern);
	int m = strlen(tText);
	if (m < n) return 0;

	int i;
	for (i = 0; i < n; i++) {
		if (tPattern[i] != tText[i]) return 0;
	}

	return 1;
}

static int isVariable(char* tText) {
	char testText[100];
	strcpy(testText, tText);
	turnStringLowercase(testText);

	if (!strcmp("movecontact", testText)) return 1;
	if (!strcmp("movehit", testText)) return 1;
	if (!strcmp("time", testText)) return 1;
	if (!strcmp("roundstate", testText)) return 1;
	if (!strcmp("animtime", testText)) return 1;
	if (!strcmp("animelem", testText)) return 1;
	if (!strcmp("animelemno", testText)) return 1;
	if (!strcmp("command", testText)) return 1;
	if (!strcmp("pos x", testText)) return 1;
	if (!strcmp("pos y", testText)) return 1;
	if (!strcmp("p2bodydist x", testText)) return 1;
	if (!strcmp("p2bodydist y", testText)) return 1;
	if (!strcmp("p2dist y", testText)) return 1;
	if (!strcmp("hitshakeover", testText)) return 1;
	if (!strcmp("backedgebodydist", testText)) return 1;
	if (!strcmp("frontedgebodydist", testText)) return 1;
	if (!strcmp("anim", testText)) return 1;
	if (!strcmp("vel x", testText)) return 1;
	if (!strcmp("vel y", testText)) return 1;
	if (!strcmp("stateno", testText)) return 1;
	if (!strcmp("p2stateno", testText)) return 1;
	if (!strcmp("ctrl", testText)) return 1;
	if (!strcmp("hitdefattr", testText)) return 1;
	if (!strcmp("statetype", testText)) return 1;
	if (!strcmp("movetype", testText)) return 1;
	if (!strcmp("p2movetype", testText)) return 1;
	if (!strcmp("p2statetype", testText)) return 1;
	if (!strcmp("statetime", testText)) return 1;
	if (!strcmp("alive", testText)) return 1;
	if (!strcmp("prevstateno", testText)) return 1;
	if (!strcmp("inguarddist", testText)) return 1;
	if (!strcmp("hitover", testText)) return 1;
	if (!strcmp("matchover", testText)) return 1;
	if (!strcmp("roundsexisted", testText)) return 1;
	if (!strcmp("roundno", testText)) return 1;
	if (!strcmp("matchno", testText)) return 1;
	if (!strcmp("turns", testText)) return 1;
	if (!strcmp("animelemtime", testText)) return 1;
	if (!strcmp("var", testText)) return 1;
	if (!strcmp("gethitvar", testText)) return 1;
	if (!strcmp("selfanimexist", testText)) return 1;
	if (!strcmp("sysvar", testText)) return 1;
	if (!strcmp("sysfvar", testText)) return 1;
	if (!strcmp("const", testText)) return 1;
	if (!strcmp("ifelse", testText)) return 1;
	if (!strcmp("cond", testText)) return 1;
	if (!strcmp("backedgedist", testText)) return 1;
	if (!strcmp("pi", testText)) return 1;
	if (!strcmp("sin", testText)) return 1;
	if (!strcmp("cos", testText)) return 1;
	if (!strcmp("atan", testText)) return 1;
	if (!strcmp("exp", testText)) return 1;
	if (!strcmp("ln", testText)) return 1;
	if (!strcmp("abs", testText)) return 1;
	if (!strcmp("floor", testText)) return 1; 
	if (!strcmp("ceil", testText)) return 1;
	if (!strcmp("const720p", testText)) return 1;
	if (!strcmp("const240p", testText)) return 1;
	if (!strcmp("p2dist x", testText)) return 1;
	if (!strcmp("hitfall", testText)) return 1;
	if (!strcmp("canrecover", testText)) return 1;
	if (!strcmp("camerapos x", testText)) return 1;
	if (!strcmp("enemynear", testText)) return 1;
	if (!strcmp("numenemy", testText)) return 1;
	if (!strcmp("numhelper", testText)) return 1;
	if (!strcmp("numexplod", testText)) return 1;
	if (!strcmp("numproj", testText)) return 1;
	if (!strcmp("ailevel", testText)) return 1;
	if (!strcmp("random", testText)) return 1;
	if (!strcmp("power", testText)) return 1;
	if (!strcmp("powermax", testText)) return 1;
	if (!strcmp("ishelper", testText)) return 1;
	if (!strcmp("teammode", testText)) return 1;
	if (!strcmp("p1name", testText)) return 1;
	if (!strcmp("p2name", testText)) return 1;
	if (!strcmp("p4name", testText)) return 1;
	if (!strcmp("helper", testText)) return 1;
	if (!strcmp("rootdist x", testText)) return 1;
	if (!strcmp("rootdist y", testText)) return 1;
	if (!strcmp("target", testText)) return 1;
	if (!strcmp("numtarget", testText)) return 1;
	if (!strcmp("gametime", testText)) return 1;
	if (!strcmp("hitpausetime", testText)) return 1;
	if (!strcmp("win", testText)) return 1;
	if (!strcmp("lose", testText)) return 1;
	if (!strcmp("winko", testText)) return 1;
	if (!strcmp("winperfect", testText)) return 1;
	if (!strcmp("palno", testText)) return 1;
	if (!strcmp("life", testText)) return 1;
	if (!strcmp("p2life", testText)) return 1;
	if (!strcmp("lifemax", testText)) return 1;
	if (!strcmp("root", testText)) return 1;
	if (!strcmp("parent", testText)) return 1;
	if (!strcmp("playerid", testText)) return 1;
	if (!strcmp("playeridexist", testText)) return 1;
	if (!strcmp("moveguarded", testText)) return 1;
	if (!strcmp("gameheight", testText)) return 1;
	if (!strcmp("gamewidth", testText)) return 1;
	if (!strcmp("teamside", testText)) return 1;

	if (!strcmp("data.power", testText)) return 1;
	if (!strcmp("data.fall.defence_mul", testText)) return 1;

	if (!strcmp("movement.stand.friction.threshold", testText)) return 1;
	if (!strcmp("movement.crouch.friction.threshold", testText)) return 1;
	if (!strcmp("movement.air.gethit.groundlevel", testText)) return 1;
	if (!strcmp("movement.air.gethit.groundrecover.ground.threshold", testText)) return 1;
	if (!strcmp("movement.air.gethit.groundrecover.groundlevel", testText)) return 1;
	if (!strcmp("movement.air.gethit.airrecover.threshold", testText)) return 1;
	if (!strcmp("movement.air.gethit.airrecover.yaccel", testText)) return 1;
	if (!strcmp("movement.air.gethit.trip.groundlevel", testText)) return 1;
	if (!strcmp("movement.down.bounce.offset.x", testText)) return 1;
	if (!strcmp("movement.down.bounce.offset.y", testText)) return 1;
	if (!strcmp("movement.down.bounce.groundlevel", testText)) return 1;
	if (!strcmp("movement.down.bounce.yaccel", testText)) return 1;
	if (!strcmp("movement.down.friction.threshold", testText)) return 1;
	if (!strcmp("movement.yaccel", testText)) return 1;

	if (!strcmp("velocity.walk.fwd.x", testText)) return 1;
	if (!strcmp("velocity.walk.back.x", testText)) return 1;
	if (!strcmp("velocity.run.fwd.x", testText)) return 1;
	if (!strcmp("velocity.run.back.x", testText)) return 1;
	if (!strcmp("velocity.run.back.y", testText)) return 1;
	if (!strcmp("velocity.runjump.fwd.x", testText)) return 1;
	if (!strcmp("velocity.jump.neu.x", testText)) return 1;
	if (!strcmp("velocity.jump.fwd.x", testText)) return 1;
	if (!strcmp("velocity.jump.back.x", testText)) return 1;
	if (!strcmp("velocity.jump.y", testText)) return 1;
	if (!strcmp("velocity.airjump.neu.x", testText)) return 1;
	if (!strcmp("velocity.airjump.fwd.x", testText)) return 1;
	if (!strcmp("velocity.airjump.back.x", testText)) return 1;
	if (!strcmp("velocity.airjump.y", testText)) return 1;
	if (!strcmp("velocity.air.gethit.groundrecover.x", testText)) return 1;
	if (!strcmp("velocity.air.gethit.groundrecover.y", testText)) return 1;
	if (!strcmp("velocity.air.gethit.airrecover.mul.x", testText)) return 1;
	if (!strcmp("velocity.air.gethit.airrecover.mul.y", testText)) return 1;
	if (!strcmp("velocity.air.gethit.airrecover.add.x", testText)) return 1;
	if (!strcmp("velocity.air.gethit.airrecover.add.y", testText)) return 1;
	if (!strcmp("velocity.air.gethit.airrecover.up", testText)) return 1;
	if (!strcmp("velocity.air.gethit.airrecover.down", testText)) return 1;
	if (!strcmp("velocity.air.gethit.airrecover.fwd", testText)) return 1;
	if (!strcmp("velocity.air.gethit.airrecover.back", testText)) return 1;
	

	// TODO: move hit vars seperate
	if (!strcmp("slidetime", testText)) return 1;
	if (!strcmp("ctrltime", testText)) return 1;
	if (!strcmp("groundtype", testText)) return 1;
	if (!strcmp("animtype", testText)) return 1;
	if (!strcmp("xvel", testText)) return 1;
	if (!strcmp("yvel", testText)) return 1;
	if (!strcmp("fall", testText)) return 1;
	if (!strcmp("airtype", testText)) return 1;
	if (!strcmp("fall.yvel", testText)) return 1;
	if (!strcmp("yaccel", testText)) return 1;
	if (!strcmp("hitcount", testText)) return 1;
	if (!strcmp("damage", testText)) return 1;
	if (!strcmp("hitshaketime", testText)) return 1;
	if (!strcmp("movereversed", testText)) return 1;
	if (!strcmp("isbound", testText)) return 1;

	// TODO: think if there is a better way for this
	if (!strcmp("nowalk", testText)) return 1;
	if (!strcmp("noautoturn", testText)) return 1;
	if (!strcmp("noairguard", testText)) return 1;
	if (!strcmp("intro", testText)) return 1;
	if (!strcmp("nobardisplay", testText)) return 1;
	if (!strcmp("roundnotover", testText)) return 1;
	if (!strcmp("nojugglecheck", testText)) return 1;
	if (!strcmp("nostandguard", testText)) return 1;
	if (!strcmp("nocrouchguard", testText)) return 1;
	if (!strcmp("nofg", testText)) return 1;
	if (!strcmp("nobg", testText)) return 1;
	if (!strcmp("invisible", testText)) return 1;
	if (!strcmp("noshadow", testText)) return 1;
	if (!strcmp("nomusic", testText)) return 1;
	if (!strcmp("globalnoshadow", testText)) return 1;
	if (!strcmp("nokoslow", testText)) return 1;
	if (!strcmp("unguardable", testText)) return 1;
	if (!strcmp("timerfreeze", testText)) return 1;

	// TODO: change to better
	if (!strcmp("enemy", testText)) return 1;
	if (!strcmp("name", testText)) return 1;
	if (!strcmp("authorname", testText)) return 1;
	if (!strcmp("id", testText)) return 1;

	// TODO: refactor into own type
	if (!strcmp("aa", testText)) return 1;
	if (!strcmp("ap", testText)) return 1;
	if (!strcmp("sc", testText)) return 1;
	if (!strcmp("na", testText)) return 1;
	if (!strcmp("sa", testText)) return 1;
	if (!strcmp("ha", testText)) return 1;
	if (!strcmp("s", testText)) return 1;
	if (!strcmp("a", testText)) return 1;
	if (!strcmp("c", testText)) return 1;
	if (!strcmp("h", testText)) return 1;
	if (!strcmp("l", testText)) return 1;
	if (!strcmp("i", testText)) return 1;
	if (!strcmp("f", testText)) return 1;
	if (!strcmp("n", testText)) return 1;
	if (!strcmp("at", testText)) return 1;
	if (!strcmp("nt", testText)) return 1;
	if (!strcmp("m-", testText)) return 1;
	if (!strcmp("a-", testText)) return 1;
	if (!strcmp("ht", testText)) return 1;
	if (!strcmp("np", testText)) return 1;
	if (!strcmp("hp", testText)) return 1;

	if (!strcmp("haf", testText)) return 1;
	if (!strcmp("mafd", testText)) return 1;
	if (!strcmp("mafp", testText)) return 1;
	if (!strcmp("maf", testText)) return 1;
	if (!strcmp("ma", testText)) return 1;
	if (!strcmp("m", testText)) return 1;

	if (!strcmp("heavy", testText)) return 1;
	if (!strcmp("light", testText)) return 1;
	if (!strcmp("medium", testText)) return 1;
	if (!strcmp("trip", testText)) return 1;
	if (!strcmp("hard", testText)) return 1;
	if (!strcmp("low", testText)) return 1;
	if (!strcmp("high", testText)) return 1;
	if (!strcmp("med", testText)) return 1;
	if (!strcmp("hit", testText)) return 1;
	if (!strcmp("miss", testText)) return 1;
	if (!strcmp("left", testText)) return 1;

	if (!strcmp("normal", testText)) return 1;
	if (!strcmp("player", testText)) return 1;

	if (!strcmp("none", testText)) return 1;
	if (!strcmp("facing", testText)) return 1;
	if (!strcmp("back", testText)) return 1;
	if (!strcmp("up", testText)) return 1;
	if (!strcmp("diagup", testText)) return 1;

	if (!strcmp("addalpha", testText)) return 1;

	if (!strcmp("p1", testText)) return 1;
	if (!strcmp("p2", testText)) return 1;
	if (!strcmp("p3", testText)) return 1;
	if (!strcmp("p4", testText)) return 1;

	// TODO: move elsewhere
	if (!strcmp("=", testText)) return 1;
	if (!strcmp(">=", testText)) return 1;
	if (!strcmp("<=", testText)) return 1;


	// TODO: kinda problematic
	if (testText[0] == 's') return 1;
	if (testText[0] == 'f') return 1;
	if (doAssignmentStringsBeginsWithPattern("projhit", testText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("projcontact", testText)) return 1;

	return 0;

}

static MugenAssignment* parseMugenVariableFromString(char* tText) {
	MugenVariableAssignment* data = allocMemory(sizeof(MugenVariableAssignment));
	strcpy(data->mName, tText);

	return makeMugenAssignment(MUGEN_ASSIGNMENT_TYPE_VARIABLE, data);
}

static int isArray(char* tText) {

	int n = strlen(tText);
	char* open = strchr(tText, '(');
	char* close = strrchr(tText, ')');

	return open != NULL && close != NULL && close > open && open != tText && close == (tText + n - 1);
}

static MugenAssignment* parseArrayFromString(char* tText) {
	int posOpen = -1, posClose = -1;
	posOpen = strchr(tText, '(') - tText;
	posClose = strrchr(tText, ')') - tText;
	assert(posOpen >= 0);
	assert(posClose >= 0);

	char text1[MUGEN_DEF_STRING_LENGTH];
	char text2[MUGEN_DEF_STRING_LENGTH];

	strcpy(text1, tText);
	text1[posOpen] = '\0';

	strcpy(text2, &tText[posOpen+1]);
	char* text2End = strrchr(text2, ')');
	*text2End = '\0';

	MugenAssignment* a = parseMugenAssignmentFromString(text1);
	MugenAssignment* b = parseMugenAssignmentFromString(text2);

	return makeMugenTwoElementAssignment(MUGEN_ASSIGNMENT_TYPE_ARRAY, a, b);
}

static int isVectorAssignment(char* tText) {
	if (!doAssignmentStringsBeginsWithPattern("AnimElem", tText)) return 0;

	// TODO: properly
	return 1;
}

static int isVectorTarget(char* tText) {
	if (doAssignmentStringsBeginsWithPattern("target", tText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("p1", tText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("p2", tText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("helper", tText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("enemy", tText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("enemynear", tText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("root", tText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("playerid", tText)) return 1;
	if (doAssignmentStringsBeginsWithPattern("parent", tText)) return 1;

	// TODO: properly
	return 0;
}

static int isCommaContextFree(char* tText, int tPosition) {
	assert(tText[tPosition] == ',');
	tPosition--;
	while (tPosition >= 0 && isEmptyCharacter(tText[tPosition])) tPosition--;
	assert(tPosition >= 0);
	int end = tPosition+1;

	int depth1 = 0;
	int depth2 = 0;
	while (tPosition >= 0) {
		if (tText[tPosition] == ')') depth1++;
		if (tText[tPosition] == '(') {
			if (!depth1) break;
			else depth1--;
		}
		if (tText[tPosition] == ']') depth2++;
		if (tText[tPosition] == '[') {
			if (!depth2) break;
			else depth2--;
		}

		if (!depth1 && !depth2 && (tText[tPosition] == ',' || isEmptyCharacter(tText[tPosition]) || isOperatorCharacter(tText[tPosition]))) break;
		tPosition--;
	}
	assert(tPosition >= -1);
	int start = tPosition + 1;

	char prevWord[200];
	strcpy(prevWord, &tText[start]);
	int length = end - start;
	prevWord[length] = '\0';

	return !isVectorTarget(prevWord);
}

static int hasContextFreeComma(char* tText, int* tPosition) {
	int isRunning = 1;
	int position = 0;
	while (isRunning) {
		if (!isOnHighestLevelWithStartPosition(tText, ",", &position, position)) return 0;
		if (isCommaContextFree(tText, position)) {
			if (tPosition) *tPosition = position;
			return 1;
		}

		position++;
	}

	return 0;
}

static int isVector(char* tText) {
	return hasContextFreeComma(tText, NULL) && !isVectorAssignment(tText);
}

static MugenAssignment* parseMugenContextFreeVectorFromString(char* tText) {
	int position;
	assert(hasContextFreeComma(tText, &position));

	return parseTwoElementMugenAssignmentFromStringWithFixedPosition(tText, MUGEN_ASSIGNMENT_TYPE_VECTOR, ",", position);
}

static int isVectorInsideAssignmentOrTargetAccess(char* tText) {
	return isOnHighestLevel(tText, ",", NULL);
}

static MugenAssignment* parseMugenVectorFromString(char* tText) {
	return parseTwoElementMugenAssignmentFromString(tText, MUGEN_ASSIGNMENT_TYPE_VECTOR, ",");
}

static int isOperatorAndReturnType(char* tText, char* tDst) {
	int position;
	int isThere;
	int isDifferent;

	isThere = isOnHighestLevel(tText, ">=", &position);
	isDifferent = strcmp(">=", tText);
	if (isThere && isDifferent && position == 0) {
		strcpy(tDst, ">=");
		return 1;
	}

	isThere = isOnHighestLevel(tText, "<=", &position);
	isDifferent = strcmp("<=", tText);
	if (isThere && isDifferent && position == 0) {
		strcpy(tDst, "<=");
		return 1;
	}

	isThere = isOnHighestLevel(tText, "=", &position);
	isDifferent = strcmp("=", tText);
	if (isThere && isDifferent && position == 0) {
		strcpy(tDst, "=");
		return 1;
	}

	return 0;
}

static int isOperatorArgument(char* tText) {
	char dst[10];

	int ret = isOperatorAndReturnType(tText, dst);
	return ret;
}

static MugenAssignment* parseMugenOperatorArgumentFromString(char* tText) {
	char dst[10];
	char text[200];
	assert(isOperatorAndReturnType(tText, dst));

	sprintf(text, "%s $$ %s", dst, tText + strlen(dst));
	return parseTwoElementMugenAssignmentFromString(text, MUGEN_ASSIGNMENT_TYPE_OPERATOR_ARGUMENT, "$$");
}

static void sanitizeTextFront(char** tText) {
	int n = strlen(*tText);
	int i;
	for (i = 0; i < n; i++) {
		if (**tText != ' ') {
			return;
		}

		(*tText)++;
	}
}

static void sanitizeTextBack(char* tText) {
	int n = strlen(tText);

	int i;
	for (i = n - 1; i >= 0; i--) {
		if (tText[i] == ' ') tText[i] = '\0';
		else return;
	}
}

static void sanitizeText(char** tText) {
	sanitizeTextFront(tText);
	sanitizeTextBack(*tText);
}

MugenAssignment * parseMugenAssignmentFromString(char * tText)
{
	sanitizeText(&tText);

	if (isEmpty(tText)) {
		return parseMugenNullFromString();
	}
	else if (isVector(tText)) {
		return parseMugenContextFreeVectorFromString(tText);
	}
	else if (isOperatorArgument(tText)) {
		return parseMugenOperatorArgumentFromString(tText);
	}
	else if (isOr(tText)) {
		return parseMugenOrFromString(tText);
	}
	else if (isAnd(tText)) {
		return parseMugenAndFromString(tText);
	}
	else if (isBitwiseOr(tText)) {
		return parseMugenBitwiseOrFromString(tText);
	}
	else if (isBitwiseAnd(tText)) {
		return parseMugenBitwiseAndFromString(tText);
	}
	else if (isVariableSet(tText)) {
		return parseMugenVariableSetFromString(tText);
	}
	else if (isInequality(tText)) {
		return parseMugenInequalityFromString(tText);
	}
	else if (isComparison(tText)) {
		return parseMugenComparisonFromString(tText);
	}
	else if (isGreaterOrEqualThan(tText)) {
		return parseMugenGreaterOrEqualFromString(tText);
	}
	else if (isLessOrEqualThan(tText)) {
		return parseMugenLessOrEqualFromString(tText);
	}
	else if (isLessThan(tText)) {
		return parseMugenLessFromString(tText);
	}
	else if (isGreaterThan(tText)) {
		return parseMugenGreaterFromString(tText);
	}
	else if (isAddition(tText)) {
		return parseMugenAdditionFromString(tText);
	}
	else if (isSubtraction(tText)) {
		return parseMugenSubtractionFromString(tText);
	}
	else if (isModulo(tText)) {
		return parseMugenModuloFromString(tText);
	}
	else if (isMultiplication(tText)) {
		return parseMugenMultiplicationFromString(tText);
	}
	else if (isDivision(tText)) {
		return parseMugenDivisionFromString(tText);
	}
	else if (isExponentiation(tText)) {
		return parseMugenExponentiationFromString(tText);
	}
	else if (isNegation(tText)) {
		return parseMugenNegationFromString(tText);
	}
	else if (isUnaryMinus(tText)) {
		return parseMugenUnaryMinusFromString(tText);
	}
	else if (isInBraces(tText)) {
		return parseMugenAssignmentStringInBraces(tText);
	}
	else if (isRange(tText)) {
		return parseMugenRangeFromString(tText);
	}
	else if (isNumericalConstant(tText)) {
		return parseNumericalConstantFromString(tText);
	}
	else if (isFloatConstant(tText)) {
		return parseFloatConstantFromString(tText);
	}
	else if (isStringConstant(tText)) {
		return parseStringConstantFromString(tText);
	}
	else if (isVectorInsideAssignmentOrTargetAccess(tText)) {
		return parseMugenVectorFromString(tText);
	}
	else if (isArray(tText)) {
		return parseArrayFromString(tText);
	}
	else if (isVariable(tText)) {
		return parseMugenVariableFromString(tText);
	}
	else {
		logError("Unable to determine Mugen assignment.");
		logErrorString(tText);
		abortSystem();
	}
	
	return NULL;
}


int fetchAssignmentFromGroupAndReturnWhetherItExists(char* tName, MugenDefScriptGroup* tGroup, MugenAssignment** tOutput) {
	if (!string_map_contains(&tGroup->mElements, tName)) return 0;

	MugenDefScriptGroupElement* e = string_map_get(&tGroup->mElements, tName);
	char* text = getAllocatedMugenDefStringVariableAsElement(e);
	*tOutput = parseMugenAssignmentFromString(text);
	freeMemory(text);

	return 1;
}
