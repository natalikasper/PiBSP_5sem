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
	cout << "ClientMS" << endl << endl;
	try
	{
		HANDLE hMS;
		double t1, t2;

		if ((hMS = CreateFile(L"\\\\.\\mailslot\\BOX", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE)
			throw SetErrorMail("CreateFile: ", GetLastError());

		char wbuf[50] = "Hello from Client-Mailslot";
		DWORD wms;

		t1 = clock();

		for (int i = 0; i < 1000; i++)
		{
			char q[6];
			sprintf_s(q, "%d", i + 1);	//вс
			char buf[sizeof(wbuf) + sizeof(q)];
			strcpy_s(buf, wbuf);	//к
			strcat_s(buf, q);		//д

			if (!WriteFile(hMS, buf, sizeof(wbuf), &wms, NULL))
				throw SetErrorMail("WriteFile: ", GetLastError());

			cout << "Message " << i + 1 << endl;
		}

		t2 = clock();

		if (!CloseHandle(hMS))
			throw "Error: CloseHandle";

		cout << endl << "Время передачи: " << (t2 - t1) / 1000 << " сек." << endl << endl;
	}
	catch (string e)
	{
		cout << e << endl;
	}

	system("pause");
	return 0;
}