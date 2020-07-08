#include "pch.h"
#include <iostream>
#include <string>
#include <tchar.h>
#include <ctime>
#include "Winsock2.h" // заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
#include "WS2tcpip.h"

using namespace std;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);

int main()
{
	setlocale(LC_ALL, "rus");
	SOCKET cC;
	WSADATA wsaData;
	//const int k = 1000;
	try {
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup: ", WSAGetLastError());

		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;	//Server socket parameters 
		serv.sin_family = AF_INET;	//Using IP-adressation
		serv.sin_port = htons(3000);	//TCP-port of Server
		inet_pton(AF_INET, "127.0.0.1", &(serv.sin_addr)); //Server adress
		if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw  SetErrorMsgText("connect:", WSAGetLastError());

		char ibuf[50], //input buffer
			obuf[50] = "Hello from client";  //output buffer
		int  libuf = 0, //quantity of bytes recived
			lobuf = 0;  //quantity of bytes sent
		char b[] = "Hello from client ";

		int count;
		cout << "Enter count of messages" << endl;
		cin >> count;


		for (int i = 0; i < count; i++)
		{

			if (SOCKET_ERROR == send(cC, obuf, sizeof(obuf), NULL))
			{
				cout << "Send : " << GetLastError() << endl;;
			}

			if (SOCKET_ERROR == recv(cC, ibuf, sizeof(ibuf), NULL))
			{
				cout << "Recv : " << GetLastError() << endl;
			}

			cout << (i + 1) << " Serv: " << ibuf << endl;
		}
		if (SOCKET_ERROR == send(cC, "CLOSE", sizeof("CLOSE"), NULL))
		{
			cout << "send exit : " << GetLastError() << endl;
		}

		if (closesocket(cC) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;

	}
	system("pause");
	return 0;

}

string GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	case WSAEINTR: msgText = "Function interrupted";
		break;
	case WSAEACCES: msgText = "Permission denied";
		break;
	case WSAEFAULT: msgText = "Wrong address";
		break;
	case WSASYSCALLFAILURE: msgText = "System call abort";
		break;
	default: msgText = "***ERROR***";
		break;
	}

	return msgText;
}


string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
}
