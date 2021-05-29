#pragma once
#include "Log.h"

VOID LogCall(string callFromSection, const ADDRINT baseAddress, const ADDRINT callFrom, string callToSecImg, string apiName, UINT offset) {
	trace << callFromSection << ", ";
	trace << "0x" << std::hex << baseAddress << "." << std::hex << callFrom - baseAddress << ", ";
	trace << callToSecImg;

	if (apiName.empty()) {
		trace << "." << std::hex << offset;
	}
	else {
		trace << "." << apiName << "+" << std::hex << offset;
	}

	if (offset) {
		trace << "::Hooked";
	}
		
	trace << "\n";
}

VOID LogShellcodeEntry(const ADDRINT baseAddress, const ADDRINT callFrom, const ADDRINT pageBase, UINT pageOffset) {
	trace << ".shellcode" << ", ";
	trace << "0x" << std::hex << baseAddress << "." << std::hex << callFrom - baseAddress << ", ";
	trace << "0x" << std::hex << pageBase << "." << std::hex << pageOffset << "\n";
}

VOID LogModuleLoad(IMG img){
	mTrace << "Load: " << IMG_Name(img) << "\n";
	mTrace.flush();
}

VOID LogModuleUnload(IMG img) {
	mTrace << "Unload: " << IMG_Name(img) << "\n";
	mTrace.flush();
}