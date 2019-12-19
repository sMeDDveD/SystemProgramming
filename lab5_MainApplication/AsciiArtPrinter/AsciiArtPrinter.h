#ifdef ASCIIARTPRINTER_EXPORTS
#define ASCIIARTPRINTER_API __declspec(dllexport) 
#else
#define ASCIIARTPRINTER_API __declspec(dllimport) 
#endif

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	ASCIIARTPRINTER_API BOOLEAN __stdcall GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	ASCIIARTPRINTER_API BOOLEAN __stdcall GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	ASCIIARTPRINTER_API VOID __stdcall Execute();

#ifdef __cplusplus
}
#endif