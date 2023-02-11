#pragma once
#include "Declarations.h"
#include "TraceCall.h"
#include "Hook.h"
#include "Log.h"

ofstream trace;
ofstream mTrace;
ofstream dTrace;

string moduleToTrack;
BOOL isMainModuleToTrack = 1;


string line;
UINT32 argCount = 0;
string functionName;
stringstream strToInt;
fstream hookFunctionsFile;
fstream blacklistAPIsFile;
vector<string> apiBlacklist;
vector<HOOK_FUNCTION> hookFunctionsTracker;

KNOB<string> moduleNameToTrack(KNOB_MODE_WRITEONCE, "pintool", "m", "", "specify module name to track");
KNOB<string> hookFunctionsFilePath(KNOB_MODE_WRITEONCE, "pintool", "f", "", "specify file containing hook functions");
KNOB<string> blacklistAPIsFilePath(KNOB_MODE_WRITEONCE, "pintool", "b", "", "specify file containing  APIs to blacklist from tracing");
KNOB<string> traceOut(KNOB_MODE_WRITEONCE, "pintool", "o", "", "specify trace output file name");
KNOB<string> mTraceOut(KNOB_MODE_WRITEONCE, "pintool", "mo", "", "specify module trace output file name");
KNOB<string> dTraceOut(KNOB_MODE_WRITEONCE, "pintool", "do", "", "specify dump trace output file name");

EXCEPT_HANDLING_RESULT ExceptionHandler(
	THREADID tid, 
	EXCEPTION_INFO *pExceptInfo, 
	PHYSICAL_CONTEXT *pPhysCtxt, 
	VOID *v
){
	trace.flush();
	mTrace.flush();
	dTrace.flush();
	return EHR_UNHANDLED;
}

VOID ImgLoadInstrument(IMG img, VOID *v){

	if (IMG_IsMainExecutable(img) && isMainModuleToTrack) {
		moduleToTrack = ExtractImageName(IMG_Name(img));
	}
	else {
		LogModuleLoad(img);
		HookMemDumpAPI(img);
		HookAPI(img);
	}
}

VOID ImgUnloadInstrument(IMG img, VOID *v) {
	LogModuleUnload(img);
}

VOID InsInstrument(INS ins, VOID *v) {
	if (INS_IsControlFlow(ins) || INS_IsFarJump(ins)) {
		INS_InsertCall(
			ins,
			IPOINT_BEFORE, (AFUNPTR)TraceCall,
			IARG_INST_PTR,
			IARG_BRANCH_TARGET_ADDR,
			IARG_END
		);
	}
}

VOID Fini(INT32 code, VOID *v)
{
	trace << "#eof";
	trace.close();

	mTrace << "#eof";
	mTrace.close();

	dTrace << "#eof";
	dTrace.close();
}

INT32 Usage()
{
	PIN_ERROR("This Pintool application logs the API calls made by the specified module and dumps the runtime generated artefacts\n");
	return -1;
}

int main(int argc, char * argv[])
{

	PIN_InitSymbolsAlt(EXPORT_SYMBOLS);

	// Initialize pin

	if (PIN_Init(argc, argv)) {
		return Usage();
	}

	// Set API trace output file name

	if (traceOut.Value().empty()) {
		trace.open("APItrace.out");
	}
	else {
		trace.open(traceOut.Value().c_str());
	}

	// Set Module trace output file name

	if (mTraceOut.Value().empty()) {
		mTrace.open("ModuleTrace.out");
	}
	else {
		mTrace.open(mTraceOut.Value().c_str());
	}

	// Set Dump trace output file name

	if (dTraceOut.Value().empty()) {
		dTrace.open("DumpTrace.out");
	}
	else {
		dTrace.open(dTraceOut.Value().c_str());
	}

	// Read hook functions

	if (!hookFunctionsFilePath.Value().empty()) {

		hookFunctionsFile.open(hookFunctionsFilePath.Value().c_str(), ios::in);

		if (hookFunctionsFile.is_open()) {

			while (getline(hookFunctionsFile, line)) {

				size_t commaOffset = line.find_first_of(",");
				functionName = line.substr(0, commaOffset);

				strToInt << std::dec << line.substr(commaOffset + 1, strlen(line.c_str()));
				strToInt >> argCount;
				strToInt.clear();

				hookFunctionsTracker.push_back({ functionName, argCount});
			}

		}

		hookFunctionsFile.close();
	}

	// Read APIs to blacklist from tracing

	if (!blacklistAPIsFilePath.Value().empty()) {

		blacklistAPIsFile.open(blacklistAPIsFilePath.Value().c_str(), ios::in);

		if (blacklistAPIsFile.is_open()) {
			while (getline(blacklistAPIsFile, line)) {
				apiBlacklist.push_back(line);
				line.clear();
			}

		}

		blacklistAPIsFile.close();
	}

	// Write API trace header

	trace << "Section, Base.RVA, API\n";

	// Check if need to trace user specified module

	if (!moduleNameToTrack.Value().empty()) {
		moduleToTrack = moduleNameToTrack.Value();
		isMainModuleToTrack = 0;
	}

	// Instrument each instruction
	INS_AddInstrumentFunction(InsInstrument, NULL);

	// Register Image Load instrumentaion function
	IMG_AddInstrumentFunction(ImgLoadInstrument, NULL);

	// Register Image Unoad instrumentaion function
	IMG_AddUnloadFunction(ImgUnloadInstrument, 0);

	// Register Fini to be called when the application exits
	PIN_AddFiniFunction(Fini, 0);

	// Register PIN exception Handler
	PIN_AddInternalExceptionHandler(ExceptionHandler, NULL);

	// Start the program, never returns
	PIN_StartProgram();

	return 0;
}