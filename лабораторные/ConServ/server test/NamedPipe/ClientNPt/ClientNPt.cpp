// ClientNPt.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#define _itoa_s insted
#pragma warning(disable : 4996)

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
	HANDLE hPipe;
	bool check;
	char rbuf[50];
	char wbuf[50] = "HelloFromTClient";
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	DWORD dwRead;
	setlocale(LC_ALL, "rus");

	try
	{
		if ((hPipe = CreateFile(
			L"\\\\BNT\\pipe\\ConsolePipe",
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, NULL,
			NULL)) == INVALID_HANDLE_VALUE)
			throw  SetPipeError("createfile:", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}

	try {
		/*	if (check = SetNamedPipeHandleState
		(
		hPipe,    // [in] ���������� ������������ ������
		&dwMode,   // [in]  ��������� �� ����� ��������� ������
		NULL,   // [in]  ��������� �� ����. �-�� ������
		NULL    // [in]  ��������� ��  �������� ��������
		) == FALSE)throw  SetPipeError("SetHendleState:", GetLastError());*/
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
	unsigned int n;
	cout << "������� ���������� ���������:  "; cin >> n;
	for (unsigned int i = 1; i <= n; i++) {
		char wbuf[40] = "HelloFromTClient";
		itoa(i, wbuf + sizeof("HelloFromTClient") - 1, 10);
		check = TransactNamedPipe
		(
			hPipe,   // [in] ����������  ������
			wbuf,   // [in] ��������� �� ����� ��� ������  
			sizeof(wbuf),   // [in] ������ ������ ��� ������
			rbuf,   // [out] ��������� �� ����� ��� ������ 
			sizeof(rbuf),   // [in] ������ ������ ��� ������
			&dwRead,   // [out] ���������� ����������� ����
			NULL    // [in,out] ��� ������������ �������         
		);
		cout << "����������:   " << wbuf << endl;
		cout << "�������� �� �������:   " << rbuf << endl;
	}
	CloseHandle(hPipe);
	system("pause");
	return 0;
}

