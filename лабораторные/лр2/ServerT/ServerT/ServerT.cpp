#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "pch.h"
#include <string>
#include <tchar.h>
#include "Winsock2.h"
#include <iostream>
#pragma comment(lib, "WS2_32.lib") 
#include <ctime>

using namespace std;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);


int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Server is started" << endl;
	cout << "Server is waiting connections" << endl;
	int i = 0;	//number of client
	WSADATA ws;
	SOCKET s;
	SOCKET c;
	char ibuf[50];	//for writing messages from cients
	char obuf[50] = "Hello from client";	//response for clients
	int t;

	try {
		if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
		{
			cout << "Socket: " << WSAGetLastError() << endl;
		}

		if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
		{
			cout << "Socket: " << WSAGetLastError() << endl;
		}

		sockaddr_in c_adr;
		sockaddr_in s_adr;
		{
			s_adr.sin_port = htons(3000);
			s_adr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			s_adr.sin_family = AF_INET;
		}

		if (SOCKET_ERROR == (bind(s, (LPSOCKADDR)&s_adr, sizeof(s_adr))))
		{
			cout << "Bind: " << WSAGetLastError() << endl;
		}

		if (SOCKET_ERROR == listen(s, 2))
		{
			cout << "Listen: " << WSAGetLastError << endl;
		}

		while (true)
		{
			int lcInt = sizeof(c_adr);
			if (INVALID_SOCKET == (c = accept(s, (sockaddr*)&c_adr, &lcInt)))
			{
				cout << "Accept: " << WSAGetLastError() << endl;
			}

			cout << "         Client connect: " << endl;
			cout << "ADDRES client :          " << inet_ntoa(c_adr.sin_addr) << " : " << htons(c_adr.sin_port) << endl << endl << endl << endl << endl;

			while (true)
			{
				t = clock();

				if (SOCKET_ERROR == recv(c, ibuf, sizeof(ibuf), NULL))
				{
					cout << "Recv: " << WSAGetLastError() << endl;
					break;
				}

				cout << i << " Client : ip " << inet_ntoa(c_adr.sin_addr) << "	Port: " << htons(c_adr.sin_port) << "	Message: " << ibuf << endl;
				i++;

				if (SOCKET_ERROR == send(c, obuf, strlen(obuf) + 1, NULL))
				{
					cout << "Send: " << WSAGetLastError() << endl;
					break;
				}
			}
			i = 0;
			cout << "\t\tClient Diskonect: " << endl;
			cout << "Total time of sending: " << ((float)t) / CLOCKS_PER_SEC << " seconds";
		}
		if (closesocket(c) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
		}
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}

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