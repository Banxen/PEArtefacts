#pragma once
#include "Declarations.h"

extern ofstream trace;
extern ofstream mTrace;
extern string moduleToTrack;
extern vector<string> apiBlacklist;

#define MAX_ARG_COUNT 5
#define MAX_BUFFER_SIZE 10

VOID LogCall(
	string callFromSection,
	const ADDRINT baseAddress,
	const ADDRINT callFrom,
	string callToSecImg,
	string apiName,
	UINT offset
);

VOID LogShellcodeEntry(
	const ADDRINT baseAddress, 
	const ADDRINT callFrom,
	const ADDRINT pageBase,
	UINT pageOffset
);

VOID LogModuleLoad(IMG img);

VOID LogModuleUnload(IMG img);

VOID LogArguments(
	ADDRINT returnAddr, 
	UINT32 argCount,
	VOID *arg1, 
	VOID *arg2, 
	VOID *arg3, 
	VOID *arg4, 
	VOID *arg5
);

VOID LogReturnValue(ADDRINT returnValue);