#include "processList.h"

static FILE* output = NULL;

BOOL GetProcessList(PCHAR aFileName)
{
	if (aFileName) 
	{
		fopen_s(&output, aFileName, "w");
	}
	else 
	{
		output = stdout;
	}

	HANDLE hProcessSnap;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		GetError("CreateToolhelp32Snapshot (для процессов)");
		return FALSE;
	}

	PROCESSENTRY32 processE32;
	processE32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &processE32))
	{
		GetError("Process32First");
		CloseHandle(hProcessSnap);
		return FALSE;
	}


	for(;Process32Next(hProcessSnap, &processE32);)
	{
		fprintf(output, "\n===================================================");
		fprintf(output, "\nИмя процесса:  %s", processE32.szExeFile);
		fprintf(output, "\n===================================================");

		DWORD dwPriorityClass = GetPriority(processE32.th32ProcessID);

		fprintf(output, "\n  PID           = 0x%08X", processE32.th32ProcessID);
		fprintf(output, "\n  Parent PID    = 0x%08X", processE32.th32ParentProcessID);
		fprintf(output, "\n  Число потоков = %d", processE32.cntThreads);
		fprintf(output, "\n  Приоритет     = %d", processE32.pcPriClassBase);
		if (dwPriorityClass)
			fprintf(output, "\n  Класс приоритета = %d", dwPriorityClass);

		GetModulesList(processE32.th32ProcessID);
		GetThreadsList(processE32.th32ProcessID);

		if (!aFileName) {
			system("pause");
		}

	}

	CloseHandle(hProcessSnap);
	return TRUE;
}


BOOL GetModulesList(DWORD dwPID)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		GetError("CreateToolhelp32Snapshot (для модулей)");
		return FALSE;
	}

	MODULEENTRY32 moduleE32;
	moduleE32.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hModuleSnap, &moduleE32))
	{
		GetError("Module32First"); 
		CloseHandle(hModuleSnap);          
		return FALSE;
	}

	DWORD cntModules = 0;
	fprintf(output, "\n---------------------------------------------------");
	for (; Module32Next(hModuleSnap, &moduleE32); cntModules++) 
	{
		fprintf(output, "\n     Имя модуля:     %s", moduleE32.szModule);
		fprintf(output, "\n     Путь к модулю = %s", moduleE32.szExePath);
		fprintf(output, "\n     PID           = 0x%08X", moduleE32.th32ProcessID);
		fprintf(output, "\n     Размер модуля = %d", moduleE32.modBaseSize);
	}
	fprintf(output, "\n---------------------------------------------------\n");

	fprintf(output, "\n     Число модулей = %d\n", cntModules);

	CloseHandle(hModuleSnap);
	return TRUE;
}

BOOL GetThreadsList(DWORD dwOwnerPID)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;

	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return FALSE;

	THREADENTRY32 threadE32;
	threadE32.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(hThreadSnap, &threadE32))
	{
		GetError("Thread32First");
		CloseHandle(hThreadSnap);          
		return FALSE;
	}

	fprintf(output, "\n---------------------------------------------------");
	for (; Thread32Next(hThreadSnap, &threadE32);)
	{
		if (threadE32.th32OwnerProcessID == dwOwnerPID)
		{
			fprintf(output, "\n     ID потока     = 0x%08X", threadE32.th32ThreadID);
			fprintf(output, "\n     Приоритет     = %d", threadE32.tpBasePri);
		}
	}
	fprintf(output, "\n---------------------------------------------------\n");

	CloseHandle(hThreadSnap);
	return TRUE;
}

void GetError(PCHAR msg)
{
	DWORD e;
	CHAR message[256];

	e = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, e,
		LANG_USER_DEFAULT,
		message, 256, NULL);

	message[lstrlen(message) - 2] = '\0';

	fprintf(output, "\n  WARNING: %s завершилась с ошибкой %d (%s)", msg, e, message);
}

DWORD GetPriority(DWORD processID) 
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (!hProcess) 
		return 0;
	else
	{
		int retVal = GetPriorityClass(hProcess);
		CloseHandle(hProcess);
		return retVal;
	}
	
}

FILE** GetOutputFile(void) 
{
	return &output;
}