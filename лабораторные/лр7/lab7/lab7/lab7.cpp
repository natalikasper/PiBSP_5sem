//для обмена д-ми между распред.в лок.сети процессами
#include "pch.h"
#include <string>
#include "iostream"
#include "windows.h"
#include "time.h"

using namespace std;

string GetErrorMail(int code)
{
	string msgText = "";
	switch (code)
	{
		case WSAEINTR: msgText = "WSAEINTR"; break;
		case WSAEACCES:	msgText = "WSAEACCES"; break;
		case WSAEFAULT:	msgText = "WSAEFAULT"; break;
		default: msgText = "Error";
	};
	return msgText;
}

string SetErrorMail(string msgText, int code)
{
	return msgText + GetErrorMail(code);
}

int main()
{
	setlocale(LC_CTYPE, "Russian");
	cout << "ServerMS" << endl;

	try
	{
		HANDLE hMS;
		double t1, t2;
		int i = 0;

		if ((hMS = CreateMailslot(L"\\\\.\\mailslot\\BOX",	300, MAILSLOT_WAIT_FOREVER,	NULL)) == INVALID_HANDLE_VALUE)
			throw SetErrorMail("CreateMailslot", GetLastError());

		char rbuf[50];
		DWORD rms;
		do
		{
			i++;
			if (!ReadFile(hMS, rbuf, sizeof(rbuf), &rms, NULL))
				throw SetErrorMail("ReadFile", GetLastError());

			if (i == 1)
				t1 = clock();
			cout << rbuf << endl;
		} while (strcmp(rbuf, " ") != 0);

		t2 = clock();

		cout << "Время передачи: " << (t2 - t1) / 1000 << " сек." << endl;
		cout << "Количество сообщений: " << i - 1 << endl << endl;

		if (!CloseHandle(hMS))
			throw "Error: CloseHandle";

		system("pause");
	}
	catch (string e)
	{
		cout << e << endl;
	}
	return 0;
}