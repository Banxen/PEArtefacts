#pragma once
#include "pin.H"

#define WRITE_MEM_SIZE_THRESHOLD 10
#define CURR_PROCESS_HANDLE 0xffffffff

VOID AllocationPrologue(
	ADDRINT returnAddr, 
	UINT32 processHandle, 
	VOID* baseAddress, 
	ADDRINT allocationSize, 
	VOID* allocationType, 
	VOID* protection
);

VOID AllocationEpilogue(
	ADDRINT allocationAddress
);

VOID AllocationFreePrologue(
	ADDRINT returnAddr, 
	UINT32 processHandle, 
	ADDRINT address, 
	VOID* size, 
	VOID* freeType
);

VOID WriteMemoryPrologue(
	ADDRINT returnAddr,
	UINT32 processHandle, 
	ADDRINT destAddress, 
	ADDRINT sourceAddress, 
	UINT32 size,
	UINT32* bytesWritten
);