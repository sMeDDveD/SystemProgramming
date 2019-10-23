#ifndef PROCESS_LIST_H 
#define PROCESS_LIST_H

#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <stdio.h>

BOOL GetProcessList(PCHAR aFileName);
DWORD GetPriority(DWORD processID);
BOOL GetModulesList(DWORD dwPID);
BOOL GetThreadsList(DWORD dwOwnerPID);
void GetError(PCHAR msg);
FILE** GetOutputFile(void);

/*PROCESS_LIST_H */
#endif
