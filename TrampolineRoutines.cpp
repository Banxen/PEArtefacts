#pragma once
#include "Declarations.h"
#include "TrampolineRoutines.h"
#include "Dump.h"
#include "Log.h"

vector<ALLOCATION_INFO> allocationTracker;

VOID AllocationPrologue(
	ADDRINT returnAddr, 
	UINT32 processHandle, 
	VOID* baseAddress, 
	ADDRINT allocationSize, 
	VOID* allocationType, 
	VOID* protection
) {
	ALLOCATION_INFO newAllocation = { 0 };

	newAllocation.allocationAddress = 0;
	newAllocation.allocationSize = allocationSize;
	newAllocation.processHandle = processHandle;

	allocationTracker.push_back(newAllocation);

	if (processHandle == CURR_PROCESS_HANDLE) {
		LogArguments(
			returnAddr, 
			4, 
			baseAddress, 
			(VOID*)allocationSize, 
			allocationType, 
			protection, 
			NULL
		);
	}
	else {
		LogArguments(
			returnAddr, 
			5, 
			(VOID*)processHandle, 
			baseAddress, 
			(VOID*)allocationSize, 
			allocationType, 
			protection
		);
	}
}

VOID AllocationEpilogue(ADDRINT allocationAddress) {
	PALLOCATION_INFO pLastAllocation = NULL;

	if (allocationTracker.size() > 0) {
		pLastAllocation = allocationTracker.end();
		pLastAllocation--;

		if (allocationAddress) {
			pLastAllocation->allocationAddress = allocationAddress;
		}
		else {
			allocationTracker.erase(pLastAllocation);
		}
	}

	LogReturnValue(allocationAddress);
}

VOID AllocationFreePrologue(
	ADDRINT returnAddr, 
	UINT32 processHandle, 
	ADDRINT address, 
	VOID* size, 
	VOID* freeType
) {
	PALLOCATION_INFO foundAllocation = NULL;
	CHAR* foundBaseAddress = NULL;
	
	if (processHandle == CURR_PROCESS_HANDLE) {
		foundAllocation = CheckAllocation(address, CURR_PROCESS_HANDLE);

		if (foundAllocation != NULL) {
			foundBaseAddress = (CHAR*)foundAllocation->allocationAddress;

			if (foundBaseAddress[0] == 'M' && foundBaseAddress[1] == 'Z') {
				GenerateDump(foundAllocation->allocationAddress, foundAllocation->allocationSize);
			}

			allocationTracker.erase(foundAllocation);
		}
	}

	if (processHandle == CURR_PROCESS_HANDLE) {
		LogArguments(
			returnAddr, 
			3, 
			(VOID*)address, 
			size, 
			freeType, 
			NULL, 
			NULL
		);
	}
	else {
		LogArguments(
			returnAddr, 
			4, 
			(VOID*)processHandle, 
			(VOID*)address, 
			size, 
			freeType, 
			NULL
		);
	}
}

VOID WriteMemoryPrologue(
	ADDRINT returnAddr, 
	UINT32 processHandle, 
	ADDRINT destAddress, 
	ADDRINT sourceAddress, 
	UINT32 size, 
	UINT32* bytesWritten
) {
	PALLOCATION_INFO foundAllocation = NULL;

	if (processHandle != CURR_PROCESS_HANDLE && size > WRITE_MEM_SIZE_THRESHOLD) {
		foundAllocation = CheckAllocation(sourceAddress, CURR_PROCESS_HANDLE);

		if (foundAllocation != NULL) {
			GenerateDump(foundAllocation->allocationAddress, foundAllocation->allocationSize);
		}
		else {
			GenerateDump(sourceAddress, size);
		}
	}

	LogArguments(
		returnAddr, 
		5, 
		(VOID*)processHandle, 
		(VOID*)destAddress, 
		(VOID*)sourceAddress, 
		(VOID*)size, 
		(VOID*)bytesWritten
	);
}