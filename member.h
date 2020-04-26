//member.h
#pragma once

typedef struct tagMEMBER
{
	bool  islogin;
	TCHAR id[20];
	TCHAR pw[20];
	TCHAR name[20];
	TCHAR phone[20];
	SYSTEMTIME st;
}MEMBER;


bool member_addmember(P_ADDMEMBER* data);
bool member_login(P_LOGIN* data);
bool member_logout(P_LOGIN* data);