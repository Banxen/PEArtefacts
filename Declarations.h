#pragma once
#include "pin.H"
#include <fstream>
#include <iostream>

typedef struct _ALLOCATION_INFO {
	ADDRINT allocationAddress;
	ADDRINT allocationSize;
	UINT processHandle;
} ALLOCATION_INFO, *PALLOCATION_INFO;

typedef struct _UNICODE_STRING {
	unsigned short Length;
	unsigned short MaximumLength;
	wchar_t* Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

using namespace::std;

