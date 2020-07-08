#include "pch.h"
#include <iostream>
#include <string>
#include "Winsock2.h"

#pragma comment(lib, "WS2_32.lib")

SOCKET cC;

std::string  GetErrorMsgText(int code)
{
	switch (code)
	{
	case WSAEINTR: return "������ ������� �������� ";
	case WSAEACCES: return "���������� ����������";
	case WSAEFAULT:	return "��������� �����";
	case WSAEINVAL:	return "������ � ���������";
	case WSAEMFILE: return "������� ����� ������ �������";
	case WSAEWOULDBLOCK: return "������ �������� ����������";
	case WSAEINPROGRESS: return "�������� � �������� ��������";
	case WSAEALREADY: return "�������� ��� �����������";
	case WSAENOTSOCK: return "����� ����� �����������";
	case WSAEDESTADDRREQ: return "��������� ����� ������������";
	case WSAEMSGSIZE: return "��������� ������� �������";
	case WSAEPROTOTYPE: return "������������ ��� ��������� ��� ������";
	case WSAENOPROTOOPT: return "������ � ����� ���������";
	case WSAEPROTONOSUPPORT: return "�������� �� ��������������";
	case WSAESOCKTNOSUPPORT: return "��� ������ �� ��������������";
	case WSAEOPNOTSUPP:	return "�������� �� ��������������";
	case WSAEPFNOSUPPORT: return "��� ���������� �� ��������������";
	case WSAEAFNOSUPPORT: return "��� ������� �� �������������� ����������";
	case WSAEADDRINUSE:	return "����� ��� ������������";
	case WSAEADDRNOTAVAIL: return "����������� ����� �� ����� ���� �����������";
	case WSAENETDOWN: return "���� ���������";
	case WSAENETUNREACH: return "���� �� ���������";
	case WSAENETRESET: return "���� ��������� ����������";
	case WSAECONNABORTED: return "����������� ����� �����";
	case WSAECONNRESET:	return "����� �������������";
	case WSAENOBUFS: return "�� ������� ������ ��� �������";
	case WSAEISCONN: return "����� ��� ���������";
	case WSAENOTCONN: return "����� �� ���������";
	case WSAESHUTDOWN: return "������ ��������� send : ����� �������� ������";
	case WSAETIMEDOUT: return "���������� ���������� ��������  �������";
	case WSAECONNREFUSED: return "���������� ���������";
	case WSAEHOSTDOWN: return "���� � ����������������� ���������";
	case WSAEHOSTUNREACH: return "��� �������� ��� �����";
	case WSAEPROCLIM: return "������� ����� ���������";
	case WSASYSNOTREADY: return "���� �� ��������";
	case WSAVERNOTSUPPORTED: return "������ ������ ����������";
	case WSANOTINITIALISED:	return "�� ��������� ������������� WS2_32.DLL";
	case WSAEDISCON: return "����������� ����������";
	case WSATYPE_NOT_FOUND: return "����� �� ������";
	case WSAHOST_NOT_FOUND:	return "���� �� ������";
	case WSATRY_AGAIN: return "������������������ ���� �� ������";
	case WSANO_RECOVERY: return "��������������  ������";
	case WSANO_DATA: return "��� ������ ������������ ����";
	case WSA_INVALID_HANDLE: return "��������� ���������� �������  � �������";
	case WSA_INVALID_PARAMETER: return "���� ��� ����� ���������� � �������";
	case WSA_IO_INCOMPLETE:	return "������ ����� - ������ �� � ���������� ���������";
	case WSA_IO_PENDING: return "�������� ���������� �����";
	case WSA_NOT_ENOUGH_MEMORY: return "�� ���������� ������";
	case WSA_OPERATION_ABORTED: return "�������� ����������";
	case WSASYSCALLFAILURE: return "��������� ���������� ���������� ������";
	default: return "**ERROR**";
	};
};

std::string  SetErrorMsgText(std::string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};

bool GetServer(char* call, short port, sockaddr* from, int* flen)
{
	memset(from, 0, sizeof(flen));

	if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
	{
		throw  SetErrorMsgText("socket:", WSAGetLastError());
	}

	int optval = 1;			//��������� enabled
	//���������� ����� ������
	if (setsockopt(cC,
		SOL_SOCKET,		//������� �-��� ������
		SO_BROADCAST,	//����� ������ ��� ���-��� ���������.������
		(char*)&optval, //�������� ������ ������
		sizeof(int)) == SOCKET_ERROR)	//����� ������ ������
	{
		throw  SetErrorMsgText("opt:", WSAGetLastError());
	}

	SOCKADDR_IN all;
	all.sin_family = AF_INET;
	all.sin_port = htons(2000);
	all.sin_addr.s_addr = INADDR_BROADCAST;

	if ((sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all))) == SOCKET_ERROR)
	{
		throw  SetErrorMsgText("sendto:", WSAGetLastError());
	}

	char nameServer[50];
	if ((recvfrom(cC, nameServer, sizeof(nameServer), NULL, from, flen)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
		{
			return false;
		}
		else
		{
			throw  SetErrorMsgText("recv:", WSAGetLastError());
		}
	}

	SOCKADDR_IN* addr = (SOCKADDR_IN*)&from;
	std::cout << std::endl << "���� �������: " << addr->sin_port;
	std::cout << std::endl << "IP-����� �������: " << inet_ntoa(addr->sin_addr);

	if (!strcmp(nameServer, call))
	{
		std::cout << std::endl << "������ � ����� ������ ������.";
		return true;
	}
	else
	{
		std::cout << std::endl << "������ � ����� ������ �� ������.";
		return false;
	}

	return true;
}

int main()
{
	setlocale(LC_CTYPE, "rus");
	WSADATA wsaData;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		}

		char call[] = "Hello";

		SOCKADDR_IN clnt;
		int lc = sizeof(clnt);

		GetServer(call, 2000, (sockaddr*)&clnt, &lc);

		if (closesocket(cC) == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		}
		if (WSACleanup() == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
		}
	}
	catch (std::string errorMsgText)
	{
		std::cout << std::endl << errorMsgText;
	}

	std::cout << std::endl;
	system("pause");
	return 0;
}