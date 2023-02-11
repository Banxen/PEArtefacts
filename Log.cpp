#pragma once
#include "Log.h"

BOOL entryLogged = FALSE;

VOID LogCall(
	string callFromSection, 
	const ADDRINT baseAddress, 
	const ADDRINT callFrom, 
	string callToSecImg, 
	string apiName, 
	UINT offset
) {
	for (UINT16 i = 0; i < apiBlacklist.size(); i++) {
		if (apiName == apiBlacklist[i]) {
			return;
		}
	}

	trace << callFromSection << ", ";
	trace << "0x" << std::hex << baseAddress << "." << std::hex << callFrom - baseAddress << ", ";
	trace << callToSecImg;

	if (apiName.empty()) {
		trace << "." << std::hex << offset;
	}
	else {
		trace << "." << apiName << "+" << std::hex << offset;

		if (offset) {
			trace << "::Hooked";
		}
	}
		
	trace << "\n";
	trace.flush();
	entryLogged = FALSE;
}

VOID LogShellcodeEntry(
	const ADDRINT baseAddress, 
	const ADDRINT callFrom, 
	const ADDRINT pageBase, 
	UINT pageOffset
) {
	trace << ".shellcode" << ", ";
	trace << "0x" << std::hex << baseAddress << "." << std::hex << callFrom - baseAddress << ", ";
	trace << "0x" << std::hex << pageBase << "." << std::hex << pageOffset << "\n";
	trace.flush();
}

VOID LogModuleLoad(IMG img){
	mTrace << "Load: " << IMG_Name(img) << "\n";
	mTrace.flush();
}

VOID LogModuleUnload(IMG img) {
	mTrace << "Unload: " << IMG_Name(img) << "\n";
	mTrace.flush();
}

VOID TypeIdentifyAndLogArg(VOID *arg) {

	if (!PIN_CheckReadAccess(arg)) {
		trace << std::hex << arg << "\n";
		return;
	}
	else {

		trace << std::hex << arg;

		UNICODE_STRING unicodeStr = *(PUNICODE_STRING)arg;

		if (PIN_CheckReadAccess(unicodeStr.Buffer)) {
			if (strlen((char*)unicodeStr.Buffer) == 1) {

				size_t lenUnicode = wcslen(unicodeStr.Buffer);

				if (lenUnicode * 2 == unicodeStr.Length) {
					wstring wideStr(unicodeStr.Buffer);
					string asciiStr(wideStr.begin(), wideStr.end());
					trace << " -> U\"" << asciiStr << "\"\n";
					return;
				}
			}
		}
	
		size_t asciiLen = strlen((char*)arg);

		if (asciiLen == 1) {
			
			size_t wideLen = wcslen((wchar_t*)arg);

			if (wideLen > 1) {
				wstring wideStr((wchar_t*)arg);
				string asciiStr(wideStr.begin(), wideStr.end());

				trace << " -> L\"" << asciiStr << "\"\n";
				return;
			}
		}
		else if(asciiLen > 1) {
			trace << " -> \"" << (char*)arg << "\"\n";
			return;
		}

		UINT8 *buffer = (UINT8 *)arg;

		trace << " -> {";

		for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
			if (isprint(buffer[i])) {
				trace << char(buffer[i]);
			}
			else {
				trace << "0x" << std::hex << (UINT8)buffer[i];
			}
		}

		trace << "}\n";
	}
}

VOID LogArguments(
	ADDRINT returnAddr, 
	UINT32 argCount, 
	VOID *arg1, 
	VOID *arg2, 
	VOID *arg3, 
	VOID *arg4, 
	VOID *arg5
) {
	
	if (entryLogged) {
		return;
	}

	VOID* argArray[MAX_ARG_COUNT] = { arg1, arg2, arg3, arg4, arg5 };

	PIN_LockClient();

	IMG returnToImg = IMG_FindByAddress(returnAddr);

	if (IMG_Valid(returnToImg)) {
		if (IMG_Name(returnToImg).find(moduleToTrack) == string::npos) {
			entryLogged = FALSE;
			return;
		}
	}

	if (argCount > MAX_ARG_COUNT) {
		argCount = MAX_ARG_COUNT;
	}
	
	for (UINT16 i = 0; i < argCount; i++) {

		trace << setw(32) << "Arg[" << i << "]: ";

		if (argArray[i] != NULL) {
			TypeIdentifyAndLogArg(argArray[i]);
		}
		else {
			trace << "NULL\n";
		}

		trace.flush();
	}

	entryLogged = TRUE;

	PIN_UnlockClient();
}

VOID LogReturnValue(ADDRINT returnValue) {

	if (entryLogged) {
		trace << setw(44) << "Return Value: 0x" << std::hex << returnValue << "\n";
		trace.flush();
		entryLogged = FALSE;
	}

}