//packet.h
#pragma once

//=================== 애플리케이션 프로토콜 =======================

//1. 패킷 flag 정의
//Client -> Server
#define PACK_ADDMEMBER		1
#define PACK_LOGIN			2
#define PACK_LOGOUT			3

//Server -> Client 
#define ACK_ADDMEMBER_S		50
#define ACK_ADDMEMBER_F		51
#define ACK_LOGIN_S			52
#define ACK_LOGIN_F			53
#define ACK_LOGOUT_S		54
#define ACK_LOGOUT_F		55

typedef struct tagP_ADDMEMBER
{
	int   flag;
	TCHAR id[20];
	TCHAR pw[20];
	TCHAR name[20];
	TCHAR phone[20];

	tagP_ADDMEMBER(TCHAR* _id, TCHAR* _pw, TCHAR* _name, TCHAR* _phone);
}P_ADDMEMBER;

void ack_addmember(P_ADDMEMBER* pdata, bool ischeck);



typedef struct tagP_LOGIN
{
	int   flag;
	TCHAR id[20];
	TCHAR pw[20];
	TCHAR name[20];
	TCHAR phone[20];
	SYSTEMTIME dt;
	tagP_LOGIN() {}
	tagP_LOGIN(TCHAR* _id, TCHAR* _pw);
}P_LOGIN;

void ack_login(P_LOGIN* pdata, bool ischeck);
void ack_logout(P_LOGIN* pdata, bool ischeck);
