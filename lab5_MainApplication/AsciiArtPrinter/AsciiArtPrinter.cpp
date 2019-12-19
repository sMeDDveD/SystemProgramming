#include "stdafx.h"
#include "AsciiArtPrinter.h"
#include <iostream>

const char* author = "Leonardo Da Vinci";
const char* description = "This DLL prints masterpieces";

using namespace std;

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
	cout << "#############################################################" << endl;
	cout << "#                    _                                      #" << endl;
	cout << "#                  -=\\`\\                                    #" << endl;
	cout << "#              |\\ ____\\_\\__                                 #" << endl;
	cout << "#            -=\\c`\"\"\"\"\"\"\" \"`)                               #" << endl;
	cout << "#               `~~~~~/ /~~`\                                #" << endl;
	cout << "#                 -==/ /                                    #" << endl;
	cout << "#                   '-'                                     #" << endl;
	cout << "#                  _  _                                     #" << endl;
	cout << "#                 ( `   )_                                  #" << endl;
	cout << "#                (    )    `)                               #" << endl;
	cout << "#              (_   (_ .  _) _)                             #" << endl;
	cout << "#                                             _             #" << endl;
	cout << "#                                            (  )           #" << endl;
	cout << "#             _ .                         ( `  ) . )        #" << endl;
	cout << "#           (  _ )_                      (_, _(  ,_)_)      #" << endl;
	cout << "#         (_  _(_ ,)                                        #" << endl;
	cout << "#############################################################" << endl;

}
