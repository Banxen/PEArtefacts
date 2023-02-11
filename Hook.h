#pragma once
#include "Declarations.h"
#include "Log.h"

typedef struct _HOOK_FUNCTION {
	string functionName;
	UINT32 argCount;
} HOOK_FUNCTION, *PHOOK_FUNCTION;

extern vector<HOOK_FUNCTION> hookFunctionsTracker;

VOID HookMemDumpAPI(IMG img);

VOID HookAPI(IMG img);