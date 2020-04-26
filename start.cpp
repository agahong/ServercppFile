//start.cpp

#include "std.h"

extern vector<MEMBER> g_memlist;
CRITICAL_SECTION cs;

void ui_Print(SOCKET clientsock, const char* buf)
{
	SOCKADDR_IN clientaddr;
	int addrsize = sizeof(clientaddr);
	getpeername(clientsock, (SOCKADDR*)&clientaddr, &addrsize);

	printf("[���� %s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
		ntohs(clientaddr.sin_port), buf);
}

void ui_Print()
{
	EnterCriticalSection(&cs);
	system("cls");
	printf("ID\tPW\tNAME\tPHONE\t\tDATETIME\tISLOGIN\n");
	printf("-----------------------------------------\n");
	for (int i = 0; i < (int)g_memlist.size(); i++)
	{
		MEMBER mem = g_memlist[i];

		_tprintf(TEXT("%s\t%s\t%s\t%s\t\t%02d:%02d:%02d\t%c\n"),
			mem.id, mem.pw, mem.name, mem.phone,
			mem.st.wHour, mem.st.wMinute, mem.st.wSecond,
			((mem.islogin == true)? 'O': 'X') );
	}
	LeaveCriticalSection(&cs);
}

int main()
{
	InitializeCriticalSection(&cs);

	//0. �����ͺ��̽� ���� ó��
	if (wbdb_DBConnect() == 0)
	{
		printf("������ ���̽� ���� ����\n");
		return -1;
	}
	printf("������ ���̽� ����\n");

	//1.����
	if (wbsock_Init() == 0)
		return -1;

	//2.���� ���� �� ���� 
	SOCKET listensock;
	listensock = wbsock_CreateSocket(SERVER_PORT);
	if (listensock == -1)
	{
		printf("���� ���� ����\n");
		return -1;
	}

	//3.���� ����(�����尡 ȣ��Ǹ� accept���� �ݺ� ȣ��)	
	printf("61.81.99.67:9000\n");
	HANDLE handle = wbsock_Run(listensock);

	//4. ListenThread�� ����ɶ����� ���
	WaitForSingleObject(handle, INFINITE);
	CloseHandle(handle);

	//5.����
	wbsock_Close(listensock, 0);
	wbsock_Exit();

	//6.DB����
	wbdb_DBDisConnect();

	DeleteCriticalSection(&cs);

	return 0;
}

