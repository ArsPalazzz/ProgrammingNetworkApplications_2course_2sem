#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable:4996) 

#include <iostream>
#include <string>
#include "Winsock2.h" 
#include "ErrorMsgText.h"
#pragma comment(lib, "WS2_32.lib")
using namespace std;

bool get_server(char* call, short port, sockaddr* from, int* flen);


int main(int argc, char* argv[])
{
    char CALL[6] = "Hello";
    WSADATA wsaData;
    SOCKET cS;
    SOCKADDR_IN server;
    int lserver = sizeof(server);

    cout << "Client: " << CALL << endl;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw set_error_msg_text("WSAStartup:", WSAGetLastError());

        if (!get_server(CALL, 2000, (sockaddr*)&server, &lserver))
            cout << "GetServer returned false" << endl;

        if (WSACleanup() == SOCKET_ERROR)
            throw set_error_msg_text("WSACleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }
    return 0;
}

// отправка широковещательного сигнала ко всем серверам
bool get_server(char* call, short port, sockaddr* from, int* flen)
{
    SOCKET cC;
    SOCKADDR_IN all;

    int optval = 1;
    char bfrom[50];

    int timeout = 1000;

    try {
        if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw set_error_msg_text("socket: ", WSAGetLastError());
        if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
            throw set_error_msg_text("opt: ", WSAGetLastError());
        if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) == SOCKET_ERROR)
            throw set_error_msg_text("opt:", WSAGetLastError());

        all.sin_family = AF_INET;
        all.sin_port = htons(port);
        all.sin_addr.s_addr = INADDR_BROADCAST;

        if (sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
            throw set_error_msg_text("send:", WSAGetLastError());
        if (recvfrom(cC, bfrom, sizeof(bfrom), NULL, from, flen) == SOCKET_ERROR)
            throw set_error_msg_text("recvfrom:", WSAGetLastError());
        char obuf[1] = "";

          if (sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
              throw set_error_msg_text("send:", WSAGetLastError());
          if (recvfrom(cC, bfrom, sizeof(bfrom), NULL, from, flen) == SOCKET_ERROR)
              throw set_error_msg_text("recvfrom:", WSAGetLastError());


              // если позывной и ответ не совпали
        if (strcmp(call, bfrom) != 0)
            return false;


        string sfrom(bfrom);
        cout << endl << "Server sends: " << sfrom << endl;

        if (closesocket(cC) == SOCKET_ERROR)
            throw set_error_msg_text("CloseSocket: ", WSAGetLastError());

        cout << "Server IP: " << inet_ntoa(((struct sockaddr_in*)from)->sin_addr) << endl;
        cout << "Server Port: " << ntohs(((struct sockaddr_in*)from)->sin_port) << endl;

        return true;
    }
    catch (string errorMsgText)
    {
        if (WSAGetLastError() == WSAETIMEDOUT) {
            cout << "There is no server with that callsign" << endl;
            return false;
        }
        throw set_error_msg_text("GetServer:", WSAGetLastError());
    }
}