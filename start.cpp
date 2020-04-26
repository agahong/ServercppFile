//start.cpp

#include "std.h"

extern vector<MEMBER> g_memlist;
CRITICAL_SECTION cs;

void ui_Print(SOCKET clientsock, const char* buf)
{
	SOCKADDR_IN clientaddr;
	int addrsize = sizeof(clientaddr);
	getpeername(clientsock, (SOCKADDR*)&clientaddr, &addrsize);

	printf("[수신 %s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
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

	//0. 데이터베이스 접근 처리
	if (wbdb_DBConnect() == 0)
	{
		printf("데이터 베이스 연결 오류\n");
		return -1;
	}
	printf("데이터 베이스 연결\n");

	//1.시작
	if (wbsock_Init() == 0)
		return -1;

	//2.소켓 생성 및 연결 
	SOCKET listensock;
	listensock = wbsock_CreateSocket(SERVER_PORT);
	if (listensock == -1)
	{
		printf("소켓 생성 오류\n");
		return -1;
	}

	//3.서버 실행(쓰레드가 호출되면 accept무한 반복 호출)	
	printf("61.81.99.67:9000\n");
	HANDLE handle = wbsock_Run(listensock);

	//4. ListenThread가 종료될때까지 대기
	WaitForSingleObject(handle, INFINITE);
	CloseHandle(handle);

	//5.종료
	wbsock_Close(listensock, 0);
	wbsock_Exit();

	//6.DB종료
	wbdb_DBDisConnect();

	DeleteCriticalSection(&cs);

	return 0;
}

