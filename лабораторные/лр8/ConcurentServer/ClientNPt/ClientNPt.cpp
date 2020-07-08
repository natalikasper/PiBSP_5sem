// ClientNP.cpp: определяет точку входа для консольного приложения.
//
#include <iostream>
#include <Winsock2.h>
#define PIPE TEXT("\\\\.\\pipe\\Tube")
#define MAX_SIZE_OF_BUFFER 64
int main()
{
	HANDLE hPipe;
	DWORD bytes;
	char buffer[] = "Hello Pipe Transact";
	char* outbuffer = new char[MAX_SIZE_OF_BUFFER];
	memset(outbuffer, NULL, MAX_SIZE_OF_BUFFER);
	try
	{
		//совм.исп-ние запрещено
		//ост.атрибуты не установл.
		hPipe = CreateFile(PIPE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hPipe == INVALID_HANDLE_VALUE)
			throw "Please, check server!";

		DWORD state = PIPE_READMODE_MESSAGE;
		SetNamedPipeHandleState(hPipe, &state, NULL, NULL);//кол-вот байит     интервал задержки

		TransactNamedPipe(hPipe, buffer, sizeof(buffer), outbuffer, MAX_SIZE_OF_BUFFER, &bytes, NULL);
		puts(outbuffer);
		CloseHandle(hPipe);
	}
	catch (char* err)
	{
		puts(err);
	}
	delete[] outbuffer;
	system("pause");
	return (0);
}