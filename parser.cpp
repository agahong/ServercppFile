//parser.cpp
#include "std.h"

extern CRITICAL_SECTION cs;

void ui_Print();

void parser_RecvData(SOCKET clientsock,char* data)
{
	EnterCriticalSection(&cs);
	int* flag = (int*)data;
	if (*flag == PACK_ADDMEMBER)
		AddMember_Parser(clientsock, (P_ADDMEMBER*)data);
	else if (*flag == PACK_LOGIN)
		Login_Parser(clientsock, (P_LOGIN*)data);
	else if (*flag == PACK_LOGOUT)
		Logout_Parser(clientsock, (P_LOGIN*)data);

	ui_Print();
	LeaveCriticalSection(&cs);
}

void AddMember_Parser(SOCKET clientsock, P_ADDMEMBER* recvdata)
{
//	printf("데이터 수신 :%ls, %ls, %ls, %ls\n",
//		recvdata->id, recvdata->pw, recvdata->name, recvdata->phone);
	if (member_addmember(recvdata) == true)
	{
		ack_addmember(recvdata, true);
	}
	else
	{
		ack_addmember(recvdata, false);
	}
	//전송
	wbsock_Send(clientsock, (char*)recvdata, sizeof(P_ADDMEMBER));
}

void Login_Parser(SOCKET clientsock, P_LOGIN* recvdata)
{
	P_LOGIN data;
	data = *recvdata;
	
	if (member_login(&data) == true)
	{
		ack_login(&data, true);
	}
	else
	{
		ack_login(&data, false);
	}

	wbsock_Send(clientsock, (char*)&data, sizeof(P_LOGIN));
}

void Logout_Parser(SOCKET clientsock, P_LOGIN* recvdata)
{
	P_LOGIN data;
	data = *recvdata;

	if (member_logout(&data) == true)
	{
		ack_logout(&data, true);
	}
	else
	{
		ack_logout(&data, false);
	}

	wbsock_Send(clientsock, (char*)&data, sizeof(P_LOGIN));
}