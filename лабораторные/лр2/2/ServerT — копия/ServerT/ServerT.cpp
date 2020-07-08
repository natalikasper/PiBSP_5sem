#include "pch.h"
#include <string>
#include <tchar.h>
#include "Winsock2.h"
#include <iostream>
#pragma comment(lib, "WS2_32.lib") 
using namespace std;
//...................................................................
string  GetErrorMsgText(int code)    // cформировать текст ошибки 
{
	string msgText;
	switch (code)                      // проверка кода возврата  
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
	case WSAEFAULT:         msgText = "WSAEFAULT";        break;
	case WSAEINVAL:         msgText = "WSAEINVAL";        break;
	case WSAEMFILE:         msgText = "WSAEMFILE";        break;
	case WSAEWOULDBLOCK:         msgText = "WSAEWOULDBLOCK";        break;
	case WSAEINPROGRESS:         msgText = "WSAEINPROGRESS";        break;
	case WSAEALREADY:         msgText = "WSAEALREADY";        break;
	case WSAENOTSOCK:         msgText = "WSAENOTSOCK";        break;
	case WSAEDESTADDRREQ:         msgText = "WSAEDESTADDRREQ";        break;
	case WSAEMSGSIZE:         msgText = "WSAEMSGSIZE";        break;
	case WSAEPROTOTYPE:         msgText = "WSAEPROTOTYPE";        break;
	case WSAENOPROTOOPT:         msgText = "WSAENOPROTOOPT";        break;
	case WSAEPROTONOSUPPORT:         msgText = "WSAEPROTONOSUPPORT";        break;
	case WSAESOCKTNOSUPPORT:         msgText = "WSAESOCKTNOSUPPORT";        break;
	case WSAEOPNOTSUPP:         msgText = "WSAEOPNOTSUPP";        break;
	case WSAEPFNOSUPPORT:         msgText = "WSAEPFNOSUPPORT";        break;
	case WSAEAFNOSUPPORT:         msgText = "WSAEAFNOSUPPORT";        break;
	case WSAEADDRINUSE:         msgText = "WSAEADDRINUSE";        break;
	case WSAEADDRNOTAVAIL:         msgText = "WSAEADDRNOTAVAIL";        break;
	case WSAENETDOWN:         msgText = "WSAENETDOWN";        break;
	case WSAENETUNREACH:         msgText = "WSAENETUNREACH";        break;
	case WSAENETRESET:         msgText = "WSAENETRESET";        break;
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	case WSAECONNABORTED: msgText = "WSAECONNABORTED"; break;
	case WSAECONNRESET: msgText = "WSAECONNRESET"; break;
	case WSAENOBUFS: msgText = "WSAENOBUFS"; break;
	case WSAEISCONN: msgText = "WSAEISCONN"; break;
	case WSAENOTCONN: msgText = "WSAENOTCONN"; break;
	case WSAESHUTDOWN: msgText = "WSAESHUTDOWN"; break;
	case WSAECONNREFUSED: msgText = "WSAECONNREFUSED"; break;
	case WSAEHOSTDOWN: msgText = "WSAEHOSTDOWN"; break;
	case WSAEHOSTUNREACH: msgText = "WSAEHOSTUNREACH"; break;
	case WSAEPROCLIM: msgText = "WSAEPROCLIM"; break;
	case WSASYSNOTREADY: msgText = "WSASYSNOTREADY"; break;
	case WSAVERNOTSUPPORTED: msgText = "WSAVERNOTSUPPORTED"; break;
	case WSANOTINITIALISED: msgText = "WSANOTINITIALISED"; break;
	case WSAEDISCON: msgText = "WSAEDISCON"; break;
	case WSATYPE_NOT_FOUND: msgText = "WSATYPE_NOT_FOUND"; break;
	case WSAHOST_NOT_FOUND: msgText = "WSAHOST_NOT_FOUND"; break;
	case WSATRY_AGAIN: msgText = "WSATRY_AGAIN"; break;
	case WSANO_RECOVERY: msgText = "WSANO_RECOVERY"; break;
	case WSANO_DATA: msgText = "WSANO_DATA"; break;
	case WSA_INVALID_HANDLE: msgText = "WSA_INVALID_HANDLE"; break;
	case WSA_INVALID_PARAMETER: msgText = "WSA_INVALID_PARAMETER"; break;
	case WSA_IO_INCOMPLETE: msgText = "WSA_IO_INCOMPLETE"; break;
	case WSA_IO_PENDING: msgText = "WSA_IO_PENDING"; break;
	case WSA_NOT_ENOUGH_MEMORY: msgText = "WSA_NOT_ENOUGH_MEMORY"; break;
	case WSA_OPERATION_ABORTED: msgText = "WSA_OPERATION_ABORTED"; break;
	default:                msgText = "***ERROR***";      break;
	};
	return msgText;
};
 
string  SetErrorMsgText(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};

int _tmain(int argc, _TCHAR* argv[])
{
	SOCKET  sS;	
	WSADATA wsaData;		//стр-ра в котор.возвращ параметры инициализации
	try
	{
		if (WSAStartup
					(MAKEWORD(2, 0),
					&wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		
		if ((sS = socket
					(AF_INET,					//формат адреса - для TCP/IP af_inet
					SOCK_STREAM,				//тип сокета - ориент.на поток
					NULL)) == INVALID_SOCKET)	//протокол - для TCP/IP null
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;						// параметры сокета sS
		serv.sin_family = AF_INET;				// используется IP-адресация  
		serv.sin_port = htons(2000);			// порт 2000 (преобр порт в формат tcp/ip)
		serv.sin_addr.s_addr = INADDR_ANY;		// любой собственный IP-адрес 

		//связать сокет с параметрами из стр-ры sockaddr_in
		if (bind(
				sS,								
				(LPSOCKADDR)&serv,					
				sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("bind:", WSAGetLastError());
		
		while (true) {
			if (listen(
				sS,							
				SOMAXCONN) == SOCKET_ERROR)	//макс.длина очереди
				throw  SetErrorMsgText("listen:", WSAGetLastError());

			SOCKET cS;							// сокет для обмена данными с клиентом 
			SOCKADDR_IN clnt;					// параметры  сокета клиента
			memset(&clnt, 0, sizeof(clnt));		// обнулить память
			int lclnt = sizeof(clnt);			// размер SOCKADDR_IN

			//разрешить подключ
			if ((cS = accept(
				sS,								//дескриптор сокета
				(sockaddr*)&clnt,				//указатель на sockaddr_in
				&lclnt)) == INVALID_SOCKET)		//указ.на длину sockaddr_in
				throw  SetErrorMsgText("accept:", WSAGetLastError());

			cout << "Port(TCP/IP):" << htons(clnt.sin_port);
			cout << "\nClient's IP:" << inet_ntoa(clnt.sin_addr) << "\n";
			
			char ibuf[50];						//буфер ввода 
			char obuf[50] = "sever: принято ";	//буфер вывода
			int		libuf = 0,					//количество принятых байт
					lobuf = 0;					//количество отправленных байь 

			while (strlen(ibuf) != 0)
			{
				char bufnumber2[10] = "";
				char bufnumber[10] = "";
				char buf[60] = "Hello from Client ";

				if ((libuf = recv(
						cS,							//канал приема
						ibuf,						//буфер данных
						sizeof(ibuf),				//кол-во байт в буфере
						NULL)) == SOCKET_ERROR)		//индикатор - null вх.буфер очищ после счит
															//msg_peek - не очищается
					throw SetErrorMsgText("recv:", WSAGetLastError());
				
					cout << ibuf << endl;
					strcpy(bufnumber, &ibuf[18]);
					_itoa(atoi(bufnumber) + 1, bufnumber2, 10);
					strcat(buf, bufnumber2);
					Sleep(10);
					strcpy(obuf, buf);

					if ((lobuf = send(	
							cS,							//дескриптор сокета
							obuf,						//указ.буфер данных
							strlen(obuf) + 1,			//кол-во байт в буфере
							NULL)) == SOCKET_ERROR)		//индикатор особого редима маршрутиз
														//null по примечанию
						throw SetErrorMsgText("send:", WSAGetLastError());
			}
		}

		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());

	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}
	return 0;
}