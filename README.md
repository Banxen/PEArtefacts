# PEArtefacts
PEArtefacts is a pintool which does following operations during a PE execution:

1. Log Windows API calls from the specified module. Default module is the main executable file.
2. Hook specified Windows APIs and log specified number of API arguments. The API name and number of arguments to be logged is configured in a text file which is passed using command-line parameter. 
3. Log calls to runtime generated code such as Shellcode. Default output file name is "APITrace.out"
4. Log full path of modules loaded and unloaded during execution. Default output file name is "ModuleTrace.out"
5. Dump and log dump information for interesting runtime allocated memory sections with a possiblity that these sections contains some form of unpacked code. Default output file name is DumpTrace.out"

**Note:** 

1. You can specify your own file name for the log files within the command-line. Additionally, a blacklist of Windows API names can be passed using a text file to exclude specific APIs from logging (Helps to remove some noisy APIs).
2. Sample HookAPIs.txt and APIs-Blacklist.txt are provided along with the project.

### Usage:

pin.exe -t PEArtefacts.dll [-m "ModuleName"]  [-f HookAPIs.txt] [-b APIsBlacklist.txt] [-o "APITrace.out"] [-mo "ModuleTrace.out"] [-do DumpTrace.out] -- executable [arguments]

**Example command-line:**

pin.exe -t PEArtefacts.dll -o "TraceLog.out" -- Test.exe

**Example command-line for logging information about specific module:**

pin.exe -t PEArtefacts.dll -m "somedll.dll" -o "TraceLog.out" -- regsvr32.exe SomeDll.dll

### Sample Output [ For API calls ]:
```
Section, Base.RVA, API
code, 0x400000.1492, user32.GetSystemMetrics+0
code, 0x400000.14a8, user32.SystemParametersInfoA+0
code, 0x400000.14b9, user32.SystemParametersInfoA+0
code, 0x400000.14c2, user32.GetSysColor+0
code, 0x400000.14cf, user32.GetSysColor+0
code, 0x400000.14dc, user32.GetSysColor+0
code, 0x400000.14e9, user32.GetSysColor+0
code, 0x400000.14f6, user32.GetSysColor+0
code, 0x400000.10ee, kernel32.GetModuleHandleA+0
code, 0x400000.1116, COMCTL32.InitCommonControlsEx+0
code, 0x400000.1143, user32.LoadImageA+0
code, 0x400000.115e, user32.LoadImageA+0
code, 0x400000.118c, user32.LoadImageA+0
code, 0x400000.119a, user32.RegisterClassExA+0
```

### Sample Output [ For module loading and unloading ]:
```
Load: C:\Windows\SysWOW64\KernelBase.dll
Load: C:\Windows\SysWOW64\kernel32.dll
Load: C:\Windows\SysWOW64\ntdll.dll
Load: C:\Windows\SysWOW64\advapi32.dll
Load: C:\Windows\SysWOW64\msvcrt.dll
Load: C:\Windows\SysWOW64\sechost.dll
```

### Build steps:
1. Download [Intel PIN](https://software.intel.com/content/www/us/en/develop/articles/pin-a-binary-instrumentation-tool-downloads.html) [Intel Pin 3.24 was used while creating this tool]
2. Create a folder with the name PEArtefacts inside **%pin_root_dir%\source\tools**
3. Put the files from the project inside the folder created in Step 2
4. Open the project with Visual Studio **(PEArtefacts.vcxproj)** [Visual Studio 2017 was used while creating this tool]
5. Compile the project by selecting Release|x32 for 32-bit version of PEArtefacts and Release|x64 for 64-bit version of PEArtefacts
