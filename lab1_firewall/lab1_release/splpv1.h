/*
 * SPLPv1.c
 * The file is part of practical task for System programming course.
 * This file contains definitions of the data structures and forward
 * declaration of handle_message() function
 */



enum test_status
{
	MESSAGE_INVALID,
	MESSAGE_VALID
};

enum Statement {
	INIT = 1,
	CONNECTING,
	CONNECTED,
	WAITING_VER,
	WAITING_DATA,
	WAITING_B64_DATA,
	DISCONNECTING
};


enum Direction
{
	A_TO_B,
	B_TO_A
};

struct Message /* message */
{
	enum Direction	direction;
	char			*text_message;
};


extern enum test_status __vectorcall validate_message(struct Message* pMessage);

char __vectorcall versionChecking(const char* s);
char __vectorcall b64Checking(const char *s);
char* __vectorcall skipCorrectData(const char *s);
char __vectorcall dataChecking(const char* text, unsigned char previousCommand);
