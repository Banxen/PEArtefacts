#pragma once
#include "Declarations.h"

extern vector<ALLOCATION_INFO> allocationTracker;

VOID GenerateDump(ADDRINT startAddress, ADDRINT size);
PALLOCATION_INFO CheckAllocation(ADDRINT instructionAddress, UINT processHandle);