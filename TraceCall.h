#pragma once
#include "pin.H"

#define PAGE_ALLIGNMENT 4096

extern ofstream trace;
extern BOOL isMainModuleToTrack;
extern string moduleToTrack;

VOID TraceCall(
	const ADDRINT callFrom, 
	const ADDRINT callTo
);

string ExtractImageName(string imageNamePath);