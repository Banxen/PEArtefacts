#pragma once
#include "Declarations.h"

extern ofstream trace;
extern ofstream mTrace;

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
