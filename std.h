//std.h
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <vector>
using namespace std;
//=== ���� ��� h/dll
#include <WinSock2.h>					//�Լ��� �����
#pragma comment(lib, "ws2_32.lib")		//�Լ��� �����ΰ� �ִ�
										//������ ����
#include <tchar.h>
#include <conio.h>

#include "packet.h"
#include "member.h"
#include "wbsock.h"
#include "wbdb.h"
#include "parser.h"



