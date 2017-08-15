#pragma once

#include "mugenassignment.h"
#include "playerdefinition.h"

int evaluateAssignment(MugenAssignment* tAssignment, Player* tPlayer);
double evaluateAssignmentAndReturnAsFloat(MugenAssignment* tAssignment, Player* tPlayer);
int evaluateAssignmentAndReturnAsInteger(MugenAssignment* tAssignment, Player* tPlayer);
char* evaluateAssignmentAndReturnAsAllocatedString(MugenAssignment* tAssignment, Player* tPlayer);
Vector3D evaluateAssignmentAndReturnAsVector3D(MugenAssignment* tAssignment, Player* tPlayer);
