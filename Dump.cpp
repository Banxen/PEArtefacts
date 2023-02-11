#pragma once
#include "Dump.h"
#include "Declarations.h"

extern ofstream dTrace;
ostringstream ossDumpName;
string dumpNameConst = "dump_";
string dFileName;
fstream memoryDump;

VOID GenerateDump(ADDRINT startAddress, ADDRINT size) {
	ossDumpName << std::hex << startAddress << "_" << std::hex << size;

	dFileName = dumpNameConst + ossDumpName.str();
	memoryDump.open(dFileName.c_str(), ios::in | ios::binary);

	if (!memoryDump) {
		memoryDump.open(dFileName.c_str(), ios::out | ios::binary);
		memoryDump.write((char*)startAddress, size);
		dTrace << dFileName << "\n";
	}

	dTrace.flush();
	memoryDump.close();
	ossDumpName.clear();
	ossDumpName.str("");
}

PALLOCATION_INFO CheckAllocation(ADDRINT instructionAddress, UINT processHandle) {
	vector<ALLOCATION_INFO>::iterator allocationIterator;
	PALLOCATION_INFO allocationStart = NULL;
	PALLOCATION_INFO allocationEnd = NULL;
	ADDRINT baseAddress = 0;
	ADDRINT size = 0;

	allocationStart = allocationTracker.begin();
	allocationEnd = allocationTracker.end();

	for (allocationIterator = allocationStart; allocationIterator < allocationEnd; allocationIterator++) {
		if (allocationIterator->allocationAddress && allocationIterator->allocationSize) {

			if (allocationIterator->processHandle == processHandle) {
				baseAddress = allocationIterator->allocationAddress;
				size = allocationIterator->allocationSize;

				if (instructionAddress >= baseAddress && instructionAddress <= baseAddress + size) {
					return allocationIterator;
				}
			}
		}
	}

	return NULL;
}