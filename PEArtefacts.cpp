#pragma once
#include "Declarations.h"
#include "TraceCall.h"
#include "Log.h"

ofstream trace;
ofstream mTrace;
string moduleToTrack;
BOOL isMainModuleToTrack = 1;
KNOB<string> moduleNameToTrack(KNOB_MODE_WRITEONCE, "pintool", "m", "", "specify module name to track");
KNOB<string> traceOut(KNOB_MODE_WRITEONCE, "pintool", "o", "", "specify trace output file name");
KNOB<string> mTraceOut(KNOB_MODE_WRITEONCE, "pintool", "mo", "", "specify module trace output file name");

EXCEPT_HANDLING_RESULT ExceptionHandler(THREADID tid, EXCEPTION_INFO *pExceptInfo, PHYSICAL_CONTEXT *pPhysCtxt, VOID *v){
	trace.flush();
	return EHR_UNHANDLED;
}

VOID ImgLoadInstrument(IMG img, VOID *v){

	if (IMG_IsMainExecutable(img) && isMainModuleToTrack) {
		moduleToTrack = ExtractImageName(IMG_Name(img));
	}
	else {
		LogModuleLoad(img);
	}
}

VOID ImgUnloadInstrument(IMG img, VOID *v) {
	LogModuleUnload(img);
}

VOID InsInstrument(INS ins, VOID *v) {
	if (INS_IsControlFlow(ins)) {
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
}

INT32 Usage()
{
	PIN_ERROR("This Pintool application logs the API calls and loaded modules\n");
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

	//Write API trace header
	trace << "Section, Base.RVA, API\n";

	//Check if need to trace user specified module
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