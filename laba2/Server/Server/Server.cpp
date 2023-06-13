#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <clocale>
#include <ctime>

#include "ErrorMsgText.h"
#include "Winsock2.h"                
#pragma comment(lib, "WS2_32.lib")   


int main()
{
    setlocale(LC_ALL, "rus");

    WSADATA wsa_data; // данные
    SOCKADDR_IN serv; // адрес

    serv.sin_family = AF_INET;  //protocol     
    serv.sin_port = htons(2000);
    serv.sin_addr.s_addr = INADDR_ANY;  //ip    

    try {
        SOCKET s_s; // объявление
        std::cout << "ServerT\n\n";

        if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) { //load lib (ver lib, )
            throw  SetErrorMsgText("Startup: ", WSAGetLastError());
        }

        if ((s_s = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) { // пробуем инициализировать
            throw  SetErrorMsgText("socket: ", WSAGetLastError());
        }

        if (bind(s_s, reinterpret_cast<LPSOCKADDR>(&serv), sizeof(serv)) == SOCKET_ERROR) { // привязка порта, хоста, сем проток к сокету
            throw  SetErrorMsgText("bind: ", WSAGetLastError());
        }


        if (listen(s_s, SOMAXCONN) == SOCKET_ERROR) {   // слушаем пока придут с клиента сообщения
            throw SetErrorMsgText("listen: ", WSAGetLastError());
        }

        SOCKET c_s;   // объявляем клиенсткий сокет   
        SOCKADDR_IN client = {};   // сюда придет адрес сокета   
        int lclnt = sizeof(client);

        clock_t start, end;
        char
            obuf[50] = "server: принято ";
        bool flag = true;

        while (true) {
            if ((c_s = accept(s_s, reinterpret_cast<sockaddr*>(&client), &lclnt)) == INVALID_SOCKET) { // получение клиентского сокета
                throw SetErrorMsgText("accept: ", WSAGetLastError());
            }
            std::cout << "\n---- NEW CLIENT ACCEPTED ----\n\n";

            
            std::cout << inet_ntoa(client.sin_addr) << ":" << htons(client.sin_port) << "\n";
      
            while (true) {
                int lobuf = 0;
                int libuf = 0;
                char ibuf[50];


                if ((libuf = recv(c_s, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR) { // получения сообщения
                    if (WSAGetLastError() == WSAECONNRESET) {
                        end = clock();
                        flag = true;
                        std::cout << "\n---- CLIENT CONNECTION WAS RESET after " << (static_cast<double>(end - start) / CLK_TCK) << " seconds of recv/send excange ----\n";
                        break;
                    }
                    throw SetErrorMsgText("recv: ", WSAGetLastError());
                }
                if (flag) {
                    start = clock();
                    flag = !flag;
                }

                std::string obuf = ibuf;
                if ((lobuf = send(c_s, obuf.c_str(), strlen(obuf.c_str()) + 1, NULL)) == SOCKET_ERROR) {    // отправка того же сообщения
                    throw SetErrorMsgText("send: ", WSAGetLastError());
                }

                if (strcmp(ibuf, "") == 0) {
                    flag = true;
                    end = clock();
                    std::cout << "Full time of recv/send exchange: " << ((double)(end - start) / CLK_TCK) << " c\n\n";
                    break;
                }
                std::cout << ibuf << std::endl;
            }
        }

    }
    catch (std::string error_msg_text) {
        WSACleanup();   // очистка библиотеки
        std::cout << '\n' << error_msg_text;
    }

    system("pause");
    return 0;
}