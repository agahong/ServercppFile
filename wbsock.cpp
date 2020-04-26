//wbsock.cpp
#include "std.h"

vector<SOCKET> g_socklist;

int wbsock_Init()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("������ ���� �ʱ�ȭ ����\n");
		return 0;
	}
	printf("������ ���� �ʱ�ȭ ����\n");
	return 1;
}

void wbsock_Exit()
{
	WSACleanup();
}

int wbsock_CreateSocket(int port)
{
	//1. ���� ����(��������)
	SOCKET listensock;
	listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listensock == INVALID_SOCKET)
		return -1;

	//2. ���� �ּ� �Ҵ�(SOCKADDR_IN, ip, port)
	SOCKADDR_IN serveraddr;
	//�����ּҺ��� size��� ����Ʈ���� 0���� �ʱ�ȭ
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//<==

	int retval;
	retval = bind(listensock, (SOCKADDR*)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return -1;

	//3. ���� ��� ����
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

	//Header�� �о���.
	int header;
	retval = recvn(clientsock, (char*)&header, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		printf("���� ����\n");
		return -1;
	}
	else if (retval == 0)
	{
		printf("������ ���� : ������ ������ ����\n");
		return -1;
	}

	//2. �� �����͸� �о�� ��
	retval = recvn(clientsock, buf, header, 0);
	if (retval == SOCKET_ERROR)
	{
		printf("���� ����\n");
		return -1;
	}
	else if (retval == 0)
	{
		printf("������ ���� : ������ ������ ����\n");
		return -1;
	}

	else // �޽��� ó��
		return retval;
}

void wbsock_Close(SOCKET sock, int flag) //flag :0����,1:���
{
	SOCKADDR_IN clientaddr;
	int addrsize = sizeof(clientaddr);
	getpeername(sock, (SOCKADDR*)&clientaddr, &addrsize);

	closesocket(sock);
	if (flag == 1)
		printf("[�������� %s:%d\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));
	else if (flag == 0)
		printf("��� ���� ����\n");
}

DWORD __stdcall WorkThead(void* data)
{
	SOCKET clientsock = (SOCKET)data;
	char buf[BUFFERSIZE];
	int retval;
	while (1)
	{
		//1. ������ ����
		retval = wbsock_Recv(clientsock, buf, sizeof(buf));
		if (retval == -1)
			break;

		parser_RecvData(clientsock, buf);

		//2. ȭ�� ���
//		ui_Print(clientsock, buf);

		//3. ������ �۽�
//		retval = wbsock_Send(clientsock, buf, strlen(buf));

		//4. 1�� �� �۽�
		//for (int i = 0; i < (int)g_socklist.size(); i++)
		//{
		//	SOCKET stemp = g_socklist[i];
		//	if (stemp != clientsock)
		//		retval = wbsock_Send(stemp, buf, strlen(buf));
		//}
	}

	//5. ���� ����
	wbsock_Close(clientsock, 1);

	//6. ���� ���� (�˻�)
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
	int addrlen = sizeof(clientaddr);//�ݵ�� �ʱ�ȭ�� �ؼ� ����

	SOCKET clientsock;

	while (1)
	{
		//1. accept
		clientsock = accept(listensock, (SOCKADDR*)&clientaddr, &addrlen);

		//2.���� ���� ���
		printf("[Ŭ���̾�Ʈ����] : %s:%d\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));

		//3. ���� ����
		g_socklist.push_back(clientsock);

		//4.���thread ����
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


// ����� ���� ������ ���� �Լ�
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
