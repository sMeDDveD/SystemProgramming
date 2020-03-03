#ifdef MINIGAME_EXPORTS
#define MINIGAME_API __declspec(dllexport) 
#else
#define MINIGAME_API __declspec(dllimport) 
#endif

#include <Windows.h>

#pragma comment(linker, "/export:GetAuthor=_GetAuthor@12")
#pragma comment(linker, "/export:GetDescription=_GetDescription@12")
#pragma comment(linker, "/export:Execute=_Execute@0")

#ifdef __cplusplus
extern "C" {
#endif

	MINIGAME_API BOOLEAN __stdcall GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	MINIGAME_API BOOLEAN __stdcall GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	MINIGAME_API VOID __stdcall Execute();

#ifdef __cplusplus
}
#endif