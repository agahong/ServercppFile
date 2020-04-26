//wbsock.cpp
#include "std.h"

vector<SOCKET> g_socklist;

int wbsock_Init()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("윈도우 소켓 초기화 실패\n");
		return 0;
	}
	printf("윈도우 소켓 초기화 성공\n");
	return 1;
}

void wbsock_Exit()
{
	WSACleanup();
}

int wbsock_CreateSocket(int port)
{
	//1. 소켓 생성(프로토콜)
	SOCKET listensock;
	listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listensock == INVALID_SOCKET)
		return -1;

	//2. 소켓 주소 할당(SOCKADDR_IN, ip, port)
	SOCKADDR_IN serveraddr;
	//시작주소부터 size모든 바이트값을 0으로 초기화
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//<==

	int retval;
	retval = bind(listensock, (SOCKADDR*)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return -1;

	//3. 소켓 통신 연결
	retval = listen(listensock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return -1;

	return (int)listensock;
}

int wbsock_Send(SOCKET clientsock, const char* buf, int size)
{
	send(clientsock, (char*)&size, sizeof(int), 0);
	return send(clientsock, buf, size, 0);
}

int wbsock_Recv(SOCKET clientsock, char* buf, int size)
{
	int retval;

	//Header를 읽었다.
	int header;
	retval = recvn(clientsock, (char*)&header, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		printf("소켓 오류\n");
		return -1;
	}
	else if (retval == 0)
	{
		printf("정상적 종료 : 상대방이 소켓을 종료\n");
		return -1;
	}

	//2. 본 데이터를 읽어야 함
	retval = recvn(clientsock, buf, header, 0);
	if (retval == SOCKET_ERROR)
	{
		printf("소켓 오류\n");
		return -1;
	}
	else if (retval == 0)
	{
		printf("정상적 종료 : 상대방이 소켓을 종료\n");
		return -1;
	}

	else // 메시지 처리
		return retval;
}

void wbsock_Close(SOCKET sock, int flag) //flag :0릿슨,1:통신
{
	SOCKADDR_IN clientaddr;
	int addrsize = sizeof(clientaddr);
	getpeername(sock, (SOCKADDR*)&clientaddr, &addrsize);

	closesocket(sock);
	if (flag == 1)
		printf("[접속종료 %s:%d\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));
	else if (flag == 0)
		printf("대기 소켓 종료\n");
}

DWORD __stdcall WorkThead(void* data)
{
	SOCKET clientsock = (SOCKET)data;
	char buf[BUFFERSIZE];
	int retval;
	while (1)
	{
		//1. 데이터 수신
		retval = wbsock_Recv(clientsock, buf, sizeof(buf));
		if (retval == -1)
			break;

		parser_RecvData(clientsock, buf);

		//2. 화면 출력
//		ui_Print(clientsock, buf);

		//3. 데이터 송신
//		retval = wbsock_Send(clientsock, buf, strlen(buf));

		//4. 1대 다 송신
		//for (int i = 0; i < (int)g_socklist.size(); i++)
		//{
		//	SOCKET stemp = g_socklist[i];
		//	if (stemp != clientsock)
		//		retval = wbsock_Send(stemp, buf, strlen(buf));
		//}
	}

	//5. 소켓 종료
	wbsock_Close(clientsock, 1);

	//6. 소켓 제거 (검색)
	for (int i = 0; i < (int)g_socklist.size(); i++)
	{
		SOCKET stemp = g_socklist[i];
		if (stemp == clientsock)
		{
			vector<SOCKET>::iterator itr = g_socklist.begin();
			itr = itr + i;
			g_socklist.erase(itr);
		}
	}

	return 0;
}

DWORD __stdcall ListenThread(void* data)
{
	SOCKET listensock = (SOCKET)data;

	SOCKADDR_IN clientaddr;
	memset(&clientaddr, 0, sizeof(clientaddr));
	int addrlen = sizeof(clientaddr);//반드시 초기화를 해서 전달

	SOCKET clientsock;

	while (1)
	{
		//1. accept
		clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addrlen);

		//2.접속 정보 출력
		printf("[클라이언트접속] : %s:%d\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));

		//3. 소켓 저장
		g_socklist.push_back(clientsock);

		//4.통신thread 생성
		HANDLE handle;
		handle = CreateThread(0, 0, WorkThead, (void*)clientsock,
			0, 0);
		CloseHandle(handle);
	}

	return 0;
}

HANDLE wbsock_Run(SOCKET listensock)
{
	HANDLE handle;
	handle = CreateThread(0, 0, ListenThread, (void*)listensock, 0, 0);
	return handle;
}


// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;

	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}
