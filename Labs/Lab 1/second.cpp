#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <iostream>
#define PID HANDLE
#endif

#ifdef  __linux__
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#define PID pid_t
#endif

using namespace std;

int main(int argc, char* argv[]){

#ifdef _WIN32

	int number;
	int result;

	LPCTSTR pipeName = TEXT("\\\\.\\pipe\\myPipe");
	DWORD read,write;

	PID namedPipe = CreateFile(pipeName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

	ReadFile(namedPipe, &number, sizeof(int), &read, NULL);

	result = number * number;

	WriteFile(namedPipe, &result, sizeof(int), &write, NULL);

	CloseHandle(namedPipe);

#endif

#ifdef  __linux__

	int fd[2];

	int number;

	fd[0] = atoi(argv[0]);

	fd[1] = atoi(argv[1]);

	read(fd[0], &number, sizeof(int));

	close(fd[0]);

	number *= number;

	write(fd[1], &number, sizeof(int));

	close(fd[1]);

#endif
}