#pragma once
#include "Declarations.h"
#include "TraceCall.h"
#include "Log.h"

ADDRINT pageStartAddress = 0;

string ExtractImageName(string imageNamePath) {
	unsigned int imageNameOffset = imageNamePath.find_last_of("\\") + 1;
	unsigned int dotOffset = imageNamePath.find_last_of(".");

	return imageNamePath.substr(imageNameOffset, dotOffset - imageNameOffset);
}

VOID TraceCall(const ADDRINT callFrom, const ADDRINT callTo) {

	PIN_LockClient();

	IMG callFromImg = IMG_FindByAddress(callFrom);
	IMG callToImg = IMG_FindByAddress(callTo);

	RTN callFromRoutine = RTN_FindByAddress(callFrom);
	RTN callToRoutine = RTN_FindByAddress(callTo);

	string callFromSection;
	string callToSection;

	if (IMG_Valid(callFromImg)) {
		if (IMG_Valid(callToImg)) {
			if ((IMG_Name(callFromImg).find(moduleToTrack) != string::npos)) {
				if (IMG_Name(callToImg).find(moduleToTrack) == string::npos) { // Call is not within the same module
					if (callToRoutine != RTN_Invalid()) {
						if (callFromRoutine != RTN_Invalid()) { // Call is from a valid routine to a valid routine
							callFromSection = SEC_Name(RTN_Sec(callFromRoutine));
						}
						else {
							callFromSection = ".unknown";
						}

						LogCall(
							callFromSection,
							IMG_StartAddress(callFromImg),
							callFrom,
							ExtractImageName(IMG_Name(callToImg)),
							RTN_FindNameByAddress(callTo),
							callTo - RTN_Address(callToRoutine)
						);
					}
				}
				else { // Call is within the same module
					if (callFromRoutine != RTN_Invalid()) {
						callFromSection = SEC_Name(RTN_Sec(callFromRoutine));
					}
					else {
						callFromSection = ".unknown";
					}

					if (callToRoutine != RTN_Invalid()) {
						callToSection = SEC_Name(RTN_Sec(callToRoutine));
					}
					else {
						callToSection = ".unknown";
					}

					if (callFromSection.compare(callToSection) != 0) { // Call is between different sections
						LogCall(
							callFromSection,
							IMG_StartAddress(callFromImg),
							callFrom,
							callToSection,
							"",
							callTo - IMG_StartAddress(callFromImg)
						);
					}
				}
			}
		}
		else { // Call to some runtime code
			if (IMG_Name(callFromImg).find(moduleToTrack) != string::npos) { // Call is from specified module code to some runtime code
				pageStartAddress = (callTo / PAGE_ALLIGNMENT)*PAGE_ALLIGNMENT;

				LogShellcodeEntry( 
					IMG_StartAddress(callFromImg),
					callFrom,
					pageStartAddress,
					callTo - pageStartAddress
				);
			}
		}
	}
	else { // Call came from some runtime code
		if (IMG_Valid(callToImg)) { // Call is from runtime code to some valid module
			if (callToRoutine != RTN_Invalid()) { // Call is to some valid routine
				if (IMG_Name(callToImg).find(moduleToTrack) == string::npos) { // Valid module is not the specified module [Just not logging call from runtime code back to specified module]				
					if (!(callFrom > pageStartAddress && callFrom < pageStartAddress + PAGE_ALLIGNMENT)) {
						pageStartAddress = (callFrom / PAGE_ALLIGNMENT)*PAGE_ALLIGNMENT;
					}

					LogCall(
						".shellcode",
						pageStartAddress,
						callFrom,
						ExtractImageName(IMG_Name(callToImg)),
						RTN_FindNameByAddress(callTo),
						callTo - RTN_Address(RTN_FindByAddress(callTo))
					);
				}
			}
		}
	}

	trace.flush();
	PIN_UnlockClient();
}