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
//���̵�, �н������ ������ ȸ���� �ִ��� Ȯ��
//�׸��� �ִٸ�, ȸ�������� ��ȯ 

//update
//���̵�, �н������ ������ ȸ���� �ִ��� Ȯ��
//�ִٸ� �� ����� islogin�� 1�� ó�� 
bool member_login(P_LOGIN* data)
{	
	//�α��� ���� ���� ó��
	//������ member ������ selelct ����
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