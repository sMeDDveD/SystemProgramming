#include "stdafx.h"
#include "ProcessorInfo.h"
#include <iostream>

const char* author = "Linus Torvalds";
const char* description = "This DLL shows some information about your processor";

BOOLEAN __stdcall ReturnCall(LPSTR buffer, const char* info, DWORD dwBufferSize, DWORD* pdwBytesWritten) {
	if (strlen(info) + 1 <= dwBufferSize) {
		strcpy_s(buffer, dwBufferSize, info);
		*pdwBytesWritten = strlen(info) + 1;
		return (TRUE);
	}

	*pdwBytesWritten = 0;
	return (FALSE);
}

BOOLEAN __stdcall GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
	return ReturnCall(buffer, author, dwBufferSize, pdwBytesWritten);
}

BOOLEAN __stdcall GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
	return ReturnCall(buffer, description, dwBufferSize, pdwBytesWritten);
}

VOID __stdcall Execute()
{
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);

	const char* processor_architecture;

	switch (system_info.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		processor_architecture = "x64 (AMD or Intel)";
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		processor_architecture = "ARM";
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		processor_architecture = "ARM64";
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		processor_architecture = "Intel Itanium-based";
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		processor_architecture = "x86";
		break;
	default:
		processor_architecture = "Unknown architecture";
	}

	std::cout << "Your processor architecture: " << processor_architecture << std::endl <<
		"The number of logical processors: " << 
		system_info.dwNumberOfProcessors;
}