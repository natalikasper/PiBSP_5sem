//програмн.интерфейс для обмена д-ми между процессами в распределенном приложении
#include "pch.h"
#include <Winsock2.h>
#include <string>
#define PIPE TEXT("\\\\.\\pipe\\Tube")
#define MAX_SIZE_OF_BUFFER 64

int main()
{
	DWORD dwRead;
	char* buffer = new char[MAX_SIZE_OF_BUFFER];
	try
	{
		HANDLE hPipe = CreateNamedPipe( PIPE,	PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,	
			1,						//макс.кол-во экземпляров канала
			1024 * 16,				//вых
			1024 * 16,				//вход
			NMPWAIT_USE_DEFAULT_WAIT,	//t ожид.связи с клиентом
			NULL);
		
		if (hPipe == INVALID_HANDLE_VALUE)
			throw "Named Pipe doesn't create";
		
		if (!ConnectNamedPipe(hPipe, NULL))
			throw "Troubles with client connection";
		
		while (ReadFile(hPipe, buffer, strlen(buffer) - 1, &dwRead, NULL) != FALSE)
		{
			buffer[dwRead] = '\0';
			puts(buffer);
			WriteFile(hPipe, buffer, strlen(buffer), &dwRead, NULL);
		}
		//законить обмен д-ми
		DisconnectNamedPipe(hPipe);
		//закрыть канал
		CloseHandle(hPipe);
	}
	catch (char* err)
	{
		puts(err);
	}
	delete[] buffer;
	system("pause");
	return 0;
}

//сервер по 1 ИК с неск.клиентами - созд.экземпляры этого канала

//по умолч.лок формат имени
//удал.клиент + сет.формат - обмен сообщениями
//лок.клиент + сет.формат - сообщениями
//лок.клиент + лок.формат - потоком.