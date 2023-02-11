# PEArtefacts
PEArtefacts is a pintool which does following operations during a PE execution:

1. Log Windows API calls from the specified module. Default module is the main executable file.
2. Hook specified Windows APIs and log specified number of API arguments. The API name and number of arguments to be logged is configured in a text file which is passed using command-line parameter. 
3. Log calls to runtime generated code such as Shellcode. Default output file name is "APITrace.out"
4. Log full path of modules loaded and unloaded during execution. Default output file name is "ModuleTrace.out"
5. Dump and log dump information for interesting runtime allocated memory sections with a possibility that these sections contain some form of unpacked code. Default output file name is "DumpTrace.out"

**Note:** 

1. You can specify your own file name for the log files within the command-line. Additionally, a blacklist of Windows API names can be passed using a text file to exclude specific APIs from logging (Helps to remove some noisy APIs).
2. Sample HookAPIs.txt and BlacklistAPIs.txt are provided along with the project.
3. All the log files and section dumps will be saved in the CWD of PEArtefacts.

### Usage:

pin.exe -t PEArtefacts.dll [-m "ModuleName"]  [-f HookAPIs.txt] [-b BlacklistAPIs.txt] [-o "APITrace.out"] [-mo "ModuleTrace.out"] [-do DumpTrace.out] -- executable [arguments]

**Example command-line:**

pin.exe -t PEArtefacts.dll -o "TraceLog.out" -- Test.exe

**Example command-line for logging information about specific module:**

pin.exe -t PEArtefacts.dll -m "somedll.dll" -o "TraceLog.out" -- regsvr32.exe SomeDll.dll

### Sample Output [ For API calls ]:
```
Section, Base.RVA, API
.text, 0xfd0000.1045, kernel32.CreateFileA+0
                            Arg[0]: 0x00600240 -> "popup32.exe"
                            Arg[1]: 0x00000001
                            Arg[2]: 0x00000001
                            Arg[3]: NULL
                            Arg[4]: 0x00000003
                            Return Value: 0x1
.text, 0xfd0000.1058, kernel32.GetFileSize+0
                            Arg[0]: 0x00000128
                            Arg[1]: NULL
                            Return Value: 0x800
.text, 0xfd0000.106e, kernel32.ReadFile+0
                            Arg[0]: 0x00000128
                            Arg[1]: 0x00fe5360 -> {0x 0x 0x 0x 0x 0x 0x 0x 0x 0x }
                            Arg[2]: 0x00000800
                            Arg[3]: 0x00fe52b8 -> {0x 0x 0x 0x 0x 0x 0x 0x 0x 0x }
                            Arg[4]: NULL
                            Return Value: 0x1
.text, 0xfd0000.1081, kernel32.VirtualAlloc+0
                            Arg[0]: NULL
                            Arg[1]: 0x00000801
                            Arg[2]: 0x00001000
                            Arg[3]: 0x00000040
                            Return Value: 0xca00000
.text, 0xfd0000.10a0, kernel32.IsDebuggerPresent+0
.shellcode, 0xfd0000.10b7, 0xca00000.0
.shellcode, 0xca00000.c2, kernel32.LoadLibraryA+0
                            Arg[0]: 0x0ca000b6 -> "user32.dll"
                            Return Value: 0x1
.shellcode, 0xca00000.dd, kernel32.GetProcAddress+0
                            Arg[0]: 0x76ef0000 -> "MZ"
                            Arg[1]: 0x0ca000cd -> "MessageBoxW"
                            Return Value: 0x76f71820
```

### Sample Output [ For module loading and unloading ]:
```
Load: C:\Windows\SysWOW64\ws2_32.dll
Load: C:\Windows\SysWOW64\SHCore.dll
Load: C:\Windows\SysWOW64\ntmarta.dll
Load: C:\Windows\SysWOW64\WinTypes.dll
Load: C:\Windows\SysWOW64\advapi32.dll
Load: C:\Windows\SysWOW64\ole32.dll
Unload: C:\Windows\SysWOW64\KernelBase.dll
Unload: C:\Windows\SysWOW64\kernel32.dll
Unload: C:\Windows\SysWOW64\ntdll.dll
Unload: C:\Windows\SysWOW64\apphelp.dll
Unload: C:\Windows\SysWOW64\user32.dll
Unload: C:\Windows\SysWOW64\win32u.dll
Unload: C:\Windows\SysWOW64\gdi32.dll
```

### Build steps:
1. Download [Intel PIN](https://software.intel.com/content/www/us/en/develop/articles/pin-a-binary-instrumentation-tool-downloads.html) [Intel Pin 3.24 was used while creating this tool]
2. Create a folder with the name PEArtefacts inside **%pin_root_dir%\source\tools**
3. Put the files from the project inside the folder created in Step 2
4. Open the project with Visual Studio **(PEArtefacts.vcxproj)** [Visual Studio 2017 was used while creating this tool]
5. Compile the project by selecting Release|x32 for 32-bit version of PEArtefacts and Release|x64 for 64-bit version of PEArtefacts
