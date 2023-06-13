#include "stdafx.h"
#include <string>
#include <iostream>
#include "Winsock2.h" //заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") //экспорт WS2_32.dll
#include "Error.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;

SOCKET cC;

bool GetServerByName(char* call, char* hostname, sockaddr* from, int* flen)
{
    hostent* server;
    int optval = 1;
    if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket: ", WSAGetLastError());

    if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
        throw SetErrorMsgText("opt: ", WSAGetLastError());
    if (!(server = gethostbyname(hostname)))
        throw SetErrorMsgText("gethostbyname: SERVER DO NOT EXISTS.", 0);

    cout << "Host: " << server->h_name << endl;

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2000);
    addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(in_addr*)(server->h_addr)));

    if (sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        throw SetErrorMsgText("sendto: ", WSAGetLastError());

    char ibuf[50];
    if (recvfrom(cC, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
            return false;
        else
            throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
    }

    cout << "Callsign: " << ibuf << endl;

    if (closesocket(cC) == SOCKET_ERROR)
        throw SetErrorMsgText("closesocket: ", WSAGetLastError());
    return true;
}

void main()
{
    setlocale(LC_CTYPE, "rus");

    WSADATA wsaData;

    char hostname[] = "WIN-OLMAANRL9OB";
    char call[] = "MyName";
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        SOCKADDR_IN from;
        memset(&from, 0, sizeof(from));
        int lfrom = sizeof(from);
        GetServerByName(call, hostname, (sockaddr*)&from, &lfrom);
        cout << "Server socket: " << inet_ntoa(from.sin_addr) << ":" << htons(from.sin_port) << endl;

        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << "WSAGetLastError: " << errorMsgText << endl;
    }
    system("pause");
}
