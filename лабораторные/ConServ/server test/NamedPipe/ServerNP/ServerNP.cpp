// ServerNP.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <ctime>
using namespace std;
// SERWER

string  GetErrorMsgText(int code)    // c����������� ����� ������ 
{
	string msgText;

	switch (code)                      // �������� ���� ��������  
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
	case WSAEFAULT:         msgText = "WSAEFAULT";        break;
	case WSAEINVAL:         msgText = "WSAEINVAL";        break;
	case WSAEMFILE:         msgText = "WSAEMFILE";        break;
	case WSAEWOULDBLOCK:    msgText = "WSAEWOULDBLOCK";   break;
	case WSAEINPROGRESS:    msgText = "WSAEINPROGRESS";   break;
	case WSAEALREADY:       msgText = "WSAEALREADY";      break;
	case WSAENOTSOCK:       msgText = "WSAENOTSOCK";      break;
	case WSAEDESTADDRREQ:   msgText = "WSAEDESTADDRREQ";  break;
	case WSAEMSGSIZE:       msgText = "WSAEMSGSIZE";      break;
	case WSAEPROTOTYPE:     msgText = "WSAEPROTOTYPE";    break;
	case WSAENOPROTOOPT:    msgText = "WSAENOPROTOOPT";   break;
	case WSAEPROTONOSUPPORT:msgText = "WSAEPROTONOSUPPORT"; break;
	default:                msgText = "***ERROR***";      break;
	};
	return msgText;
};
string  SetPipeError(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "");
	char rbuf[40]; //����� ��� ������
	char wbuf[40]; //����� ��� ������
	DWORD dwRead; // ���������� �������� ����
	DWORD dwWrite; // ���������� ���������� ����
	HANDLE hPipe; // ���������� ������
	DWORD myTime = 5;
	try
	{
		if ((hPipe = CreateNamedPipe(L"\\\\.\\pipe\\BOX",
			PIPE_ACCESS_DUPLEX,           //���������� ����� 
			PIPE_TYPE_MESSAGE | PIPE_WAIT,  // ���������|����������
			1, NULL, NULL,               // �������� 1 ���������
			INFINITE, NULL)) == INVALID_HANDLE_VALUE)throw SetPipeError("create:", GetLastError());
		if (!ConnectNamedPipe(hPipe, NULL))   throw SetPipeError("connect:", GetLastError());

	}
	catch (string ErrorPipeText)
	{
		printf("\n%s", ErrorPipeText.c_str());
	}
	while (true) {
		ConnectNamedPipe(hPipe, NULL);
		for (;;) {
			bool check = ReadFile
			(
				hPipe,   // [in] ����������  ������
				rbuf,   // [out] ��������� �� �����  �����
				sizeof(rbuf),   // [in] ���������� �������� ����
				&dwRead,   // [out] ���������� ����������� ����  
				NULL    // [in,out] ��� ����������� ��������� 
			);
			if (check == FALSE) break;
			printf("��������� �� �������:  %s\n", rbuf);
			WriteFile
			(
				hPipe,   // [in] ����������  ������
				rbuf,   // [in] ��������� �� �����  ������
				sizeof(rbuf),   // [in] ���������� ������������ ����
				&dwWrite,   // [out] ���������� ���������� ����  
				NULL    // [in,out] ��� ����������� ��������� 
			);
		}
		puts("--------------����� ������-----------------");
		DisconnectNamedPipe(hPipe);
	}
	CloseHandle(hPipe);
	system("pause");
	return 0;
}

