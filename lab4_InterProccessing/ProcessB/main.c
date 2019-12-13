#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include <TlHelp32.h>

#define BUFFER_LENGTH 256
#define INVALID_PROCESS_ID 0

static LPCTSTR sPipeName = "\\\\.\\pipe\\namedpipe";

INT main(void) {
	HANDLE hNamedPipe;
	hNamedPipe = CreateNamedPipe(
		sPipeName,
		PIPE_ACCESS_INBOUND,
		PIPE_TYPE_MESSAGE |
		PIPE_READMODE_MESSAGE |
		PIPE_WAIT,
		1,
		sizeof(DWORD),
		sizeof(DWORD),
		1,
		NULL
	);

	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		_tprintf_s(TEXT("Cannot create named pipe.\n"));
		return 1;
	}

	_tprintf_s("Wating for client data!\n");
	ConnectNamedPipe(hNamedPipe, NULL);

	DWORD address;
	if (ReadFile(hNamedPipe, &address,
		sizeof(address), NULL, NULL
	)) {
		_tprintf_s(TEXT("Your message: %s\n"), (LPTSTR)address);
	}
	else {
		_tprintf_s(TEXT("You get nothing\n"));
	}

	CloseHandle(hNamedPipe);
	return 0;
}