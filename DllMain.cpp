//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you

#pragma hdrstop
#pragma argsused

#include <Windows.h>
#include "logger.h"

#pragma link "lua5.1.lib"

extern tXTRACE_Driver gXTRACE_Driver;

char MyModuleFileName[MAX_PATH];        // Full path and name
char MyModulePath[MAX_PATH];            // Path only
char* MyModuleName;            			// Filename only
HINSTANCE g_hInst;
bool gDllUnloadInProgress = false;

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD ul_reason_being_called, LPVOID lpReserved)
{
	//return _libmain(ul_reason_being_called);
	if (DLL_PROCESS_ATTACH == ul_reason_being_called) {
		g_hInst = hInst;
		MyModuleFileName[0] = '\0';
		MyModulePath[0] = '\0';
		MyModuleName = NULL;
		DWORD dwRet = GetModuleFileName(hInst, MyModuleFileName, sizeof(MyModuleFileName));
		if (ERROR_INSUFFICIENT_BUFFER != dwRet && dwRet > 0) {
			CopyMemory(MyModulePath, MyModuleFileName, MAX_PATH);
			char *end = strrchr(MyModulePath, '\\');
			if (end) {
				*end++ = '\0';
				MyModuleName = end;
			}
		}
		XTRACE_Init(&gXTRACE_Driver, GetModuleHandle(NULL));
		XTRACE_Setup(NULL);
		XTRACE_SetMask(0xFFFFFFFF);
		XTRACE(XPDIAG2, "%s: Process attach...", MyModuleName);
		DWORD DebugLevel = GetHEDebugLevel(MyModuleName);
		XTRACE(XPDIAG2, "%s: Trace mask = %04Xh", MyModuleName, DebugLevel);
		XTRACE_SetMask(DebugLevel);
	}
	if (ul_reason_being_called == DLL_PROCESS_DETACH) {
		// at least shutdown all threads and prevent callbacks to fire!
        gDllUnloadInProgress = true;
		XTRACE_SetMask(0xFFFFFFFF);
		XTRACE(XPDIAG2, "%s: Process detach...", MyModuleName);
		//heDriverShutdown();
        //heConfigShutdown();
		XTRACE(XPDIAG2, "%s: Process detached.",MyModuleName);
    }
	return 1;
}
/*
extern "C" int _libmain(unsigned long reason)
{
	return 1;
}
*/
