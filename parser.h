//parser.h
#pragma once

void AddMember_Parser(SOCKET clientsock, P_ADDMEMBER* recvdata);
void Login_Parser(SOCKET clientsock, P_LOGIN* recvdata);
void Logout_Parser(SOCKET clientsock, P_LOGIN* recvdata);

void parser_RecvData(SOCKET clientsock,char* data);
