//wbdb.h
#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>

BOOL wbdb_DBConnect();
void wbdb_DBDisConnect();
BOOL DirectQuery(TCHAR* sql);

BOOL wbdb_AddMember(MEMBER mem);
BOOL wbdb_Login(P_LOGIN* data);

void StringToSystemTime(SYSTEMTIME* dt, TCHAR* date);
