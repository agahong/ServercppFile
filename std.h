//std.h
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <vector>
using namespace std;
//=== 소켓 통신 h/dll
#include <WinSock2.h>					//함수의 선언부
#pragma comment(lib, "ws2_32.lib")		//함수의 구현부가 있는
										//파일의 정보
#include <tchar.h>
#include <conio.h>

#include "packet.h"
#include "member.h"
#include "wbsock.h"
#include "wbdb.h"
#include "parser.h"



