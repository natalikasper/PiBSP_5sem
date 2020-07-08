// ClientNPct.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	BOOL check;
	char wbuf[30] = "HelloFromCallNamedPipe";
	char rbuf[30];
	DWORD dwRead;


	check = CallNamedPipe
	(
		L"\\\\BNT\\pipe\\ConsolePipe",   // [in] ��������� �� ��� ������
		wbuf,   // [in] ��������� �� ����� ��� ������  
		sizeof(wbuf),   // [in] ������ ������ ��� ������
		rbuf,   // [out] ��������� �� ����� ��� ������ 
		sizeof(rbuf),   // [in] ������ ������ ��� ������
		&dwRead,  // [out] ���������� ����������� ����
		NMPWAIT_USE_DEFAULT_WAIT  //  [in] �������� ��������         
	);


	if (check || GetLastError() == ERROR_MORE_DATA)
	{
		cout << "����������:   " << wbuf << endl;
		cout << "�������� �� �������:   " << rbuf << endl;
	}


	return 0;
}

