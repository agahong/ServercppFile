//wbsock.h

#pragma once

#define SERVER_PORT 9000
#define BUFFERSIZE 1024

int wbsock_Init();

void wbsock_Exit();

int wbsock_CreateSocket(int port);

int wbsock_Send(SOCKET clientsock, const char* buf, int size);

int wbsock_Recv(SOCKET clientsock, char* buf, int size);

void wbsock_Close(SOCKET sock, int flag); //flag :0Έ΄½Ό,1:Ελ½Ε

DWORD __stdcall WorkThead(void* data);

DWORD __stdcall ListenThread(void* data);

HANDLE wbsock_Run(SOCKET listensock);

int recvn(SOCKET s, char* buf, int len, int flags);