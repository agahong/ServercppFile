//mydb.cpp
/*
ODBC API(Database)��� ���α׷���
������\�ý��� �� ����\���� ����>>ODBC������
//wb31db
*/

#define  _CRT_SECURE_NO_WARNINGS
#include "std.h"
#include "wbdb.h"

#define DBNAME TEXT("wb31db")
#define ID	   TEXT("c##ccm")
#define PW		TEXT("1111")

SQLHSTMT hStmt;
SQLHENV hEnv;
SQLHDBC hDbc;

BOOL wbdb_DBConnect()
{
	// ���� ������ ���� ������
	SQLRETURN Ret;

	// 1, ȯ�� �ڵ��� �Ҵ��ϰ� ���� �Ӽ��� �����Ѵ�.(p1741)
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
		return false;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return false;

	// 2. ���� �ڵ��� �Ҵ��ϰ� �����Ѵ�.
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
		return false;
	// ����Ŭ ������ �����ϱ�
	Ret = SQLConnect(hDbc, (SQLTCHAR*)DBNAME, SQL_NTS, (SQLTCHAR*)ID, SQL_NTS, (SQLTCHAR*)PW, SQL_NTS);

	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
		return false;

	// ��� �ڵ��� �Ҵ��Ѵ�.
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
		return false;

	return true;
}

void wbdb_DBDisConnect()
{
	// ������
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

BOOL DirectQuery(TCHAR* sql)
{
	if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return FALSE;
	}
	else
	{
		SQLExecDirect(hStmt, (SQLTCHAR*)TEXT("commit"), SQL_NTS);
		return TRUE;
	}
}

/*
INSERT INTO wb_member VALUES( sequence_wbmember_id.nextval, 0, '111', 'ȫ�浿', '010-1111-1111', sysdate);
*/
BOOL wbdb_AddMember(MEMBER mem)
{
	TCHAR sql[1024];
	wsprintf(sql, TEXT("INSERT INTO wb_member VALUES(sequence_wbmember_id.nextval,%d,'%s','%s','%s','%s',sysdate)"),
		mem.islogin, mem.id, mem.pw, mem.name, mem.phone);

	return DirectQuery(sql);
}

void StringToSystemTime(SYSTEMTIME* dt, TCHAR* date)
{
	TCHAR result[4][100];		//�������� ���ڿ��� ���� ���� 2���� �迭
	//char* : 20/04/24
	TCHAR sp[20];
	TCHAR* p = date;
	for (int i = 0; i < 4; i++)
	{
		int value = wsprintf(sp, TEXT("%s"), _tcstok(p, TEXT("/")));
		p = p + 3;
		wsprintf(result[i], TEXT("%s"), sp);
	}

	//���ڿ��� ������ ��ȯ
	dt->wYear = _ttoi(result[0]);
	dt->wMonth = _ttoi(result[1]);
	dt->wDay = _ttoi(result[2]);
}

BOOL wbdb_Login(P_LOGIN* data)
{
	SQLTCHAR name[20], phone[20];// , dt[20];
	SQLTCHAR dt[50];
	SQLLEN nName, nPhone, ndt;

	SQLBindCol(hStmt, 1, SQL_C_TCHAR, name, sizeof(name), &nName);
	SQLBindCol(hStmt, 2, SQL_C_TCHAR, phone, sizeof(phone), &nPhone);
	SQLBindCol(hStmt, 3, SQL_C_TCHAR, dt, sizeof(dt), &ndt);

	TCHAR sql[1024];
	wsprintf(sql, TEXT("select name, phone, st from wb_member where id='%s' and pw='%s'"),
		data->id, data->pw);

	if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return FALSE;
	}

	for (int i = 0; SQLFetch(hStmt) != SQL_NO_DATA; i++)
	{

		wsprintf(data->name, TEXT("%ws"), name);
		wsprintf(data->phone, TEXT("%s"), phone);

		TCHAR temp[20];
		wsprintf(temp, TEXT("%s"), dt);
		StringToSystemTime(&data->dt, temp);
	}
	return TRUE;
}

/*
BOOL mydb_selectmember(TCHAR* id, MEMBER* member)
{
	SQLCHAR Id[20], Name[20], Phone[50];
	SQLLEN nId, nName, nPhone;

	SQLBindCol(hStmt, 1, SQL_C_CHAR, Id, sizeof(Id), &nId);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, Name, sizeof(Name), &nName);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, Phone, sizeof(Phone), &nPhone);

	TCHAR sql[256];
	wsprintf(sql, TEXT("select id,name,phone from member where id = '%s';"),
		id);

	if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS) //���������
	{
		return FALSE;
	}

	while (SQLFetch(hStmt) != SQL_NO_DATA) //��� ��ü���� �ϳ��� �������. ������ ���� ����ִ´�.
	{
		wsprintf(member->id, TEXT("%s"), Id);
		wsprintf(member->name, TEXT("%s"), Name);
		wsprintf(member->phone, TEXT("%s"), Phone);
	}
	return TRUE;
}
*/
