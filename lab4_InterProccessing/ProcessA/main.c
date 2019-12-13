#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include <TlHelp32.h>

#define BUFFER_LENGTH 256
#define INVALID_PROCESS_ID 0

DWORD GetProcessID(LPCTSTR process) {
	PROCESSENTRY32 processEntry;
	DWORD processID = INVALID_PROCESS_ID;
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, INVALID_PROCESS_ID);

	if (Snapshot != INVALID_HANDLE_VALUE) {
		processEntry.dwSize = sizeof(processEntry);
		Process32First(Snapshot, &processEntry);
		do {
			if (lstrcmp(processEntry.szExeFile, process) == 0) {
				processID = processEntry.th32ProcessID;
				break;
			}
		} while (Process32Next(Snapshot, &processEntry));

		CloseHandle(Snapshot);
	}

	return processID;
}

BOOL SendAdress(DWORD szAdress) {
	LPCTSTR pipeName = TEXT("\\\\.\\pipe\\namedpipe");
	HANDLE hNamedPipe = CreateFile(pipeName, GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		_tprintf_s(TEXT("Connecting to pipe\n"));
		return FALSE;
	}
	_tprintf_s(TEXT("Writing to pipe\n"));
	if (!WriteFile(hNamedPipe, &szAdress, sizeof(szAdress),
		NULL, NULL)) {

		_tprintf_s(TEXT("Writing to the named pipe failed\n"));
		CloseHandle(hNamedPipe);
		return FALSE;

	}
	CloseHandle(hNamedPipe);
	_tprintf_s(TEXT("Server read data\n"));
	return TRUE;
}

INT main(void) {
	DWORD szBufferSize = BUFFER_LENGTH * sizeof(TCHAR);
	LPTSTR buffer = calloc(BUFFER_LENGTH, sizeof(TCHAR));
	_tprintf_s(TEXT("Enter your string: "));
	_tscanf_s("%s", buffer, BUFFER_LENGTH - 1);

	DWORD dwProcessID = GetProcessID("ProcessB.exe");
	HANDLE hProcessB = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);

	if (hProcessB == NULL) {
		_tprintf_s(TEXT("Server process cannot be opened!\n"));
		free(buffer);
		return -1;
	}
	else {
		_tprintf_s(TEXT("Server is opened!\n"));
	}

	LPVOID szAddress = VirtualAllocEx(hProcessB, NULL, szBufferSize,
		MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	BOOL isWrited = WriteProcessMemory(hProcessB, szAddress, buffer, szBufferSize, NULL);

	if (!isWrited) {
		_tprintf_s(TEXT("Memory was not writed\n"));
		free(buffer);
		VirtualFreeEx(hProcessB, szAddress, 0, MEM_RELEASE);
		return -1;
	}
	else {
		_tprintf_s(TEXT("Memory was allocated\n"));
	}

	if (SendAdress((DWORD)szAddress)) {
		_tprintf_s(TEXT("OK!\n"));
	}
	else {
		_tprintf_s(TEXT("FAILURE!\n"));
	}

	free(buffer);
	VirtualFreeEx(hProcessB, szAddress, 0, MEM_RELEASE);
	CloseHandle(hProcessB);
	return 0;
}