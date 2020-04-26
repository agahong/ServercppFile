//member.cpp
#include "std.h"


vector<MEMBER> g_memlist;

bool member_addmember(P_ADDMEMBER* data)
{

	MEMBER mem = { 0 };
	_tcscpy_s(mem.id, _countof(mem.id), data->id);
	_tcscpy_s(mem.pw, _countof(mem.pw), data->pw);
	_tcscpy_s(mem.name, _countof(mem.name), data->name);
	_tcscpy_s(mem.phone, _countof(mem.phone), data->phone);
	GetLocalTime(&mem.st);
	mem.islogin = false;
//	g_memlist.push_back(mem);

	return wbdb_AddMember(mem);
}

//select
//아이디, 패스워드와 동일한 회원이 있는지 확인
//그리고 있다면, 회원정보를 반환 

//update
//아이디, 패스워드와 동일한 회원이 있는지 확인
//있다면 그 사람의 islogin을 1로 처리 
bool member_login(P_LOGIN* data)
{	
	//로그인 성공 실패 처리
	//성공시 member 정보를 selelct 전송
	return wbdb_Login(data);

	//for (int i = 0; i < (int)g_memlist.size(); i++)
	//{
	//	MEMBER mem = g_memlist[i];
	//	if (_tcscmp(mem.id, data->id)==0 && _tcscmp(mem.pw, data->pw)==0)
	//	{		
	//		_tcscpy_s(data->name, _countof(data->name), mem.name);
	//		_tcscpy_s(data->phone, _countof(data->phone), mem.phone);
	//		data->dt = mem.st;
	//		g_memlist[i].islogin = true;

	//		return true;
	//	}
	//}
	
	return false;
}

bool member_logout(P_LOGIN* data)
{
	for (int i = 0; i < (int)g_memlist.size(); i++)
	{
		MEMBER mem = g_memlist[i];
		if (_tcscmp(mem.id, data->id)==0)
		{
			g_memlist[i].islogin = false;

			return true;
		}
	}
	return false;
}