/*
 * SPLPv1.c
 * The file is part of practical task for System programming course.
 * This file contains validation of SPLPv1 protocol.
 */

 /*
   Солодуха Дмитрий Владимирович
   13
 */



 /*
 ---------------------------------------------------------------------------------------------------------------------------
 # |      STATE      |         DESCRIPTION       |           ALLOWED MESSAGES            | NEW STATE | EXAMPLE
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 1 | INIT            | initial state             | A->B     CONNECT                      |     2     |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 2 | CONNECTING      | client is waiting for con-| A<-B     CONNECT_OK                   |     3     |
   |                 | nection approval from srv |                                       |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 3 | CONNECTED       | Connection is established | A->B     GET_VER                      |     4     |
   |                 |                           |        -------------------------------+-----------+----------------------
   |                 |                           |          One of the following:        |     5     |
   |                 |                           |          - GET_DATA                   |           |
   |                 |                           |          - GET_FILE                   |           |
   |                 |                           |          - GET_COMMAND                |           |
   |                 |                           |        -------------------------------+-----------+----------------------
   |                 |                           |          GET_B64                      |     6     |
   |                 |                           |        ------------------------------------------------------------------
   |                 |                           |          DISCONNECT                   |     7     |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 4 | WAITING_VER     | Client is waiting for     | A<-B     VERSION ver                  |     3     | VERSION 2
   |                 | server to provide version |          Where ver is an integer (>0) |           |
   |                 | information               |          value. Only a single space   |           |
   |                 |                           |          is allowed in the message    |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 5 | WAITING_DATA    | Client is waiting for a   | A<-B     CMD data CMD                 |     3     | GET_DATA a GET_DATA
   |                 | response from server      |                                       |           |
   |                 |                           |          CMD - command sent by the    |           |
   |                 |                           |           client in previous message  |           |
   |                 |                           |          data - string which contains |           |
   |                 |                           |           the following allowed cha-  |           |
   |                 |                           |           racters: small latin letter,|           |
   |                 |                           |           digits and '.'              |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 6 | WAITING_B64_DATA| Client is waiting for a   | A<-B     B64: data                    |     3     | B64: SGVsbG8=
   |                 | response from server.     |          where data is a base64 string|           |
   |                 |                           |          only 1 space is allowed      |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 7 | DISCONNECTING   | Client is waiting for     | A<-B     DISCONNECT_OK                |     1     |
   |                 | server to close the       |                                       |           |
   |                 | connection                |                                       |           |
 ---------------------------------------------------------------------------------------------------------------------------

 IN CASE OF INVALID MESSAGE THE STATE SHOULD BE RESET TO 1 (INIT)

 */

#include <string.h>
#include <ctype.h>
#include "splpv1.h"


enum Statement status = INIT;

const char b64Map[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
						0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
						0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };

const char dataMap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };

const char* connectedLinesStrings[] = {
	"GET_B64",
	"GET_DATA", 
	"GET_FILE", 
	"GET_COMMAND",
	"GET_VER",
	"DISCONNECT",
};

enum Statement connectedLinesStates[] = {
	WAITING_B64_DATA,
	WAITING_DATA,
	WAITING_DATA,
	WAITING_DATA,
	WAITING_VER,
	DISCONNECTING
};

int connectedLinesLength[] = {
	7,
	8,
	8,
	11,
	7,
	9
};




/* FUNCTION:  validate_message
 *
 * PURPOSE:
 *    This function is called for each SPLPv1 message between client
 *    and server
 *
 * PARAMETERS:
 *    msg - pointer to a structure which stores information about
 *    message
 *
 * RETURN VALUE:
 *    MESSAGE_VALID if the message is correct
 *    MESSAGE_INVALID if the message is incorrect or out of protocol
 *    state
 */
enum test_status __vectorcall validate_message(struct Message *msg) {
	static enum Statement status = INIT;
	static unsigned char previousCommand;

	char* text = msg->text_message;

	if (msg->direction == A_TO_B) {
		switch (status) {
		case INIT:
		{
			if (memcmp(text, "CONNECT", 8) == 0) {
				status = CONNECTING;
				return MESSAGE_VALID;
			}
			break;
		}
		case CONNECTED:
		{
			for (int i = 0; i < 6; ++i) {
				if (memcmp(text, connectedLinesStrings[i], connectedLinesLength[i] + 1) == 0) {
					status = connectedLinesStates[i];
					previousCommand = i;
					return MESSAGE_VALID;
				}
			}
		}
		}
	}
	else {
		switch (status) {
		case CONNECTING:
		{
			if (memcmp(text, "CONNECT_OK", 11) == 0) {
				status = CONNECTED;
				return MESSAGE_VALID;
			}
			break;
		}
		case WAITING_VER:
		{
			if (memcmp(text, "VERSION", 7) == 0 && versionChecking(text + 7)) {
				status = CONNECTED;
				return MESSAGE_VALID;
			}
			break;
		}
		case WAITING_B64_DATA:
		{
			if (memcmp("B64: ", text, 5) == 0 && b64Checking(text + 5)) {
				status = CONNECTED;
				return MESSAGE_VALID;
			}
			break;
		}
		case WAITING_DATA:
		{
			if (dataChecking(text, previousCommand)) {
				status = CONNECTED;
				return MESSAGE_VALID;
			}
			break;
		}
		case DISCONNECTING:
		{
			if (memcmp(text, "DISCONNECT_OK", 14) == 0) {
				status = INIT;
				return MESSAGE_VALID;
			}
			break;
		}
		}
	}
	status = INIT;
	return MESSAGE_INVALID;
}


char __vectorcall versionChecking(const char* toCheck) {
	while (*(++toCheck)) {
		if (isdigit(*toCheck) == 0) {
			return 0;
		}
	}
	return 1;
}

char __vectorcall b64Checking(const char *toCheck) {
	char *start = toCheck;
	char numOfEq = 0;
	while (b64Map[*(toCheck)]) ++toCheck;
	while (toCheck[numOfEq] == '=') ++numOfEq;
	return numOfEq < 3 && toCheck[numOfEq] == 0 &&
		(unsigned char)(start - toCheck + 1) & 3;
}

char __vectorcall dataChecking(const char* toCheck, unsigned char previousCommand) {
	char* command = connectedLinesStrings[previousCommand];
	size_t length = connectedLinesLength[previousCommand];
	if (memcmp(toCheck, command, length) == 0)
	{
		toCheck += length;
		if (*toCheck == ' ')
		{
			char* s = skipCorrectData(toCheck);
			return s != NULL && memcmp(s, command, length + 1) == 0;
		}
	}
	return 0;
}

char* __vectorcall skipCorrectData(const char *toSkip) {
	while (dataMap[*(++toSkip)]);
	return *toSkip == ' '? toSkip + 1: NULL;
}


