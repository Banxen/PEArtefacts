#pragma once
#include "Hook.h"
#include "TrampolineRoutines.h"

VOID HookMemDumpAPI(IMG img) {

	if (IMG_Name(img).find("KERNEL32") != string::npos) {
		return;
	}

	for (SYM sym = IMG_RegsymHead(img); SYM_Valid(sym); sym = SYM_Next(sym)) {

		string routineName = PIN_UndecorateSymbolName(SYM_Name(sym), UNDECORATION_NAME_ONLY);

		if (routineName == "VirtualAlloc") {

			RTN foundRoutine = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));

			if (RTN_Valid(foundRoutine)) {

				RTN_Open(foundRoutine);

				RTN_InsertCall(foundRoutine, IPOINT_BEFORE, (AFUNPTR)AllocationPrologue,
					IARG_RETURN_IP,
					IARG_UINT32, 0xffffffff,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_END);
				RTN_InsertCall(foundRoutine, IPOINT_AFTER, (AFUNPTR)AllocationEpilogue,
					IARG_FUNCRET_EXITPOINT_VALUE,
					IARG_END);

				RTN_Close(foundRoutine);
			}
		}

		if (routineName == "VirtualAllocEx") {

			RTN foundRoutine = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));

			if (RTN_Valid(foundRoutine)) {

				RTN_Open(foundRoutine);

				RTN_InsertCall(foundRoutine, IPOINT_BEFORE, (AFUNPTR)AllocationPrologue,
					IARG_RETURN_IP,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 4,
					IARG_END);
				RTN_InsertCall(foundRoutine, IPOINT_AFTER, (AFUNPTR)AllocationEpilogue,
					IARG_FUNCRET_EXITPOINT_VALUE,
					IARG_END);

				RTN_Close(foundRoutine);
			}
		}

		if (routineName == "VirtualFree") {

			RTN foundRoutine = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));

			if (RTN_Valid(foundRoutine)) {

				RTN_Open(foundRoutine);

				RTN_InsertCall(foundRoutine, IPOINT_BEFORE, (AFUNPTR)AllocationFreePrologue,
					IARG_RETURN_IP,
					IARG_UINT32, 0xffffffff,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_END);

				RTN_Close(foundRoutine);
			}
		}

		if (routineName == "VirtualFreeEx") {

			RTN foundRoutine = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));

			if (RTN_Valid(foundRoutine)) {

				RTN_Open(foundRoutine);

				RTN_InsertCall(foundRoutine, IPOINT_BEFORE, (AFUNPTR)AllocationFreePrologue,
					IARG_RETURN_IP,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_END);

				RTN_Close(foundRoutine);
			}
		}

		if (routineName == "WriteProcessMemory") {

			RTN foundRoutine = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));

			if (RTN_Valid(foundRoutine)) {
				
				RTN_Open(foundRoutine);

				RTN_InsertCall(foundRoutine, IPOINT_BEFORE, (AFUNPTR)WriteMemoryPrologue,
					IARG_RETURN_IP,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
					IARG_FUNCARG_ENTRYPOINT_VALUE, 4,
					IARG_END);

				RTN_Close(foundRoutine);
			}
		}
	}
}

VOID HookAPI(IMG img) {

	for (SYM sym = IMG_RegsymHead(img); SYM_Valid(sym); sym = SYM_Next(sym)) {

		string routineName = PIN_UndecorateSymbolName(SYM_Name(sym), UNDECORATION_NAME_ONLY);

		for (UINT16 i = 0; i < hookFunctionsTracker.size(); i++) {

			if (routineName == hookFunctionsTracker[i].functionName) {

				RTN foundRoutine = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));

				if (RTN_Valid(foundRoutine)) {

					RTN_Open(foundRoutine);

					RTN_InsertCall(foundRoutine, IPOINT_BEFORE, (AFUNPTR)LogArguments,
						IARG_RETURN_IP,
						IARG_UINT32, hookFunctionsTracker[i].argCount,
						IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
						IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
						IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
						IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
						IARG_FUNCARG_ENTRYPOINT_VALUE, 4,
						IARG_END);

					RTN_InsertCall(foundRoutine, IPOINT_AFTER, (AFUNPTR)LogReturnValue,
						IARG_FUNCRET_EXITPOINT_VALUE,
						IARG_END
					);

					RTN_Close(foundRoutine);

				}
			}
		}
	}
}