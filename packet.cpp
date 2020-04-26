//packet.cpp
#include "std.h"

tagP_ADDMEMBER::tagP_ADDMEMBER(TCHAR* _id, TCHAR* _pw, TCHAR* _name, TCHAR* _phone)
{
	flag = PACK_ADDMEMBER;
	_tcscpy_s(id, _countof(id), _id);
	_tcscpy_s(pw, _countof(pw), _pw);
	_tcscpy_s(name, _countof(name), _name);
	_tcscpy_s(phone, _countof(phone), _phone);
}

void ack_addmember(P_ADDMEMBER* pdata, bool ischeck)
{
	if (ischeck == true)
		pdata->flag = ACK_ADDMEMBER_S;
	else
		pdata->flag = ACK_ADDMEMBER_F;
}



tagP_LOGIN::tagP_LOGIN(TCHAR* _id, TCHAR* _pw)
{
	flag = PACK_LOGIN;
	_tcscpy_s(id, _countof(id), _id);
	_tcscpy_s(pw, _countof(pw), _pw);
	_tcscpy_s(name, _countof(name), TEXT(""));
	_tcscpy_s(phone, _countof(phone), TEXT(""));
	GetLocalTime(&dt);
}

void ack_login(P_LOGIN* pdata, bool ischeck)
{
	if (ischeck == true)
		pdata->flag = ACK_LOGIN_S;
	else
		pdata->flag = ACK_LOGIN_F;
}

void ack_logout(P_LOGIN* pdata, bool ischeck)
{
	if (ischeck == true)
		pdata->flag = ACK_LOGOUT_S;
	else
		pdata->flag = ACK_LOGOUT_F;
}