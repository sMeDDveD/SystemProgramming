#include "stdafx.h"
#include "MiniGame.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

const char* author = "Someone Noname";
const char* description = "Plugin with the AAA+ GameProject";


void Intro() {
	cout << R"(
 ______________________________________________________________
|  _                   _               __                      |
| |_) _. ._   _  ._   |_)  _   _ |    (_   _ o  _  _  _  ._ _  |
| |  (_| |_) (/_ |    | \ (_) (_ |<   __) (_ | _> _> (_) | _>  |
|________|_____________________________________________________|    
)" << '\n';

}

void YouLose() {
	cout << R"(
      _            _   __  _         
 \_/ / \ | |   |  / \ (_  |_ | | | | 
  |  \_/ |_|   |_ \_/ __) |_ o o o o 
                                     
)" << '\n';
}
void YouWin() {
	cout << R"(
      _                __              
 \_/ / \ | |   \    / |  | |\ | | | | | 
  |  \_/ |_|    \/\/  |__| | \| o o o o 
                                      
)" << '\n';

}

void Tie() {
	cout << R"(
 ___ ___  _         
  |   |  |_ | | | | 
  |  _|_ |_ o o o o 
                    
)" << '\n';


}

string GetAiShoot() {


	string AiShoot = "";
	srand(time(0));
	int pick = (rand() % 3);

	switch (pick)
	{
	case 0: AiShoot = "paper";
		break;
	case 1: AiShoot = "rock";
		break;
	case 2: AiShoot = "scissors";
		break;
	default: cout << "Something is wrong" << endl;
		break;
	}
	cout << "AI picks:" << AiShoot << endl;

	return AiShoot;

}

string GetPlayerShoot() {

	string PlayerShoot = "";

	cout << "Enter rock, paper, or scissors:" << endl;
	cin >> PlayerShoot;
	cout << "You chose:" << PlayerShoot << endl;
	return PlayerShoot;

}

// Who Won
void Rules()
{

	string PlayerChose = GetPlayerShoot();
	string AiChose = GetAiShoot();

	if (PlayerChose == "rock") {
		if (AiChose == "rock") {
			Tie();
		}
		if (AiChose == "paper") {
			YouLose();
		}
		if (AiChose == "scissors") {
			YouWin();
		}

	}
	if (PlayerChose == "paper") {
		if (AiChose == "paper") {
			Tie();
		}
		if (AiChose == "scissors") {
			YouLose();
		}
		if (AiChose == "rock") {
			YouWin();
		}

	}
	if (PlayerChose == "scissors") {
		if (AiChose == "scissors") {
			Tie();
		}
		if (AiChose == "rock") {
			YouLose();
		}
		if (AiChose == "paper") {
			YouWin();
		}

	}
}

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
	Intro();

	Rules();
}