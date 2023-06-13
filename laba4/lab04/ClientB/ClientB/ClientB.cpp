#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <clocale>
#include <ctime>

#include "ErrorMsgText.h"
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;

bool GetServer(
    const char* call,
    short            port, 
    struct sockaddr* from, 
    int* flen 
);

int main()
{
    setlocale(LC_ALL, "rus");

    WSADATA wsaData;

    try {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw  SetErrorMsgText("Startup: ", WSAGetLastError());
        }

        SOCKADDR_IN clnt;
        int lc = sizeof(clnt);

        cout << "ClientB\n\n";

        GetServer("hello", 2000, (LPSOCKADDR)&clnt, &lc);

        if (WSACleanup() == SOCKET_ERROR) {
            throw  SetErrorMsgText("Cleanup: ", WSAGetLastError());
        }
    }
    catch (string errorMsgText) {
        cout << endl << errorMsgText;
    }

    return 0;
}

bool GetServer(const char* call, short port, struct sockaddr* from, int* flen) {
    try {
        SOCKET cC;
        int optval = 1, lb = 0, lobuf = 0;
        char buf[50];

        SOCKADDR_IN all;                     
        all.sin_family = AF_INET;               
        all.sin_port = htons(port);            
        all.sin_addr.s_addr = INADDR_BROADCAST; 

        if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
            throw  SetErrorMsgText("socket:", WSAGetLastError());
        }
        if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR) {
            throw  SetErrorMsgText("setsocketopt:", WSAGetLastError());
        }
        if ((lb = sendto(cC, call, sizeof(call), NULL, (sockaddr*)&all, sizeof(all))) == SOCKET_ERROR) {
            throw SetErrorMsgText("sendto:", WSAGetLastError());
        }
        std::cout << "Sent a bc message: " << call << "\n";
        std::cout << "Receiving...\n";
        if (lobuf = recvfrom(cC, buf, sizeof(buf), NULL, (sockaddr*)from, flen) == SOCKET_ERROR) {
            throw SetErrorMsgText("recvfrom:", WSAGetLastError());
        }

        struct sockaddr_in* server = (struct sockaddr_in*) from;
        cout << "Server port: " << ntohs(server->sin_port) << endl;
        cout << "Server IP: " << inet_ntoa(server->sin_addr) << endl;

        if (closesocket(cC) == SOCKET_ERROR) {
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());
        }

        return strcmp(call, buf) == 0 ? true : false;
    }
    catch (int errCode) {
        return errCode == WSAETIMEDOUT ? false : throw  SetErrorMsgText("recvfrom:", WSAGetLastError());
    }
}