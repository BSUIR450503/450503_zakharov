#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32

#include <Windows.h>
#include <conio.h>

#define PID HANDLE

#elif __linux__

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#define PID pid_t

#endif

using namespace std;

class Process{

public:
	Process(int,char**);
};

Process :: Process(int argc,char* argv[]){

#ifdef _WIN32

	int number;
	int result;

	STARTUPINFO StartUpInfo;
	PROCESS_INFORMATION ProcInfo;
	TCHAR SecondProcess[] = TEXT("second");

	LPCTSTR pipeName = TEXT("\\\\.\\pipe\\myPipe");
	DWORD read,write;

	ZeroMemory(&StartUpInfo,sizeof(StartUpInfo));
	StartUpInfo.cb = sizeof(StartUpInfo);
	ZeroMemory(&ProcInfo,sizeof(ProcInfo));

	PID namedPipe = CreateNamedPipe(pipeName,PIPE_ACCESS_DUPLEX,PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,1,1024*8,1024*8,NMPWAIT_USE_DEFAULT_WAIT,NULL);

	if(namedPipe == INVALID_HANDLE_VALUE){
		printf("CreateNamedPipe failed.");
	}

	if(!CreateProcess(NULL,SecondProcess,NULL,NULL,FALSE,0,NULL,NULL,&StartUpInfo,&ProcInfo)){
		printf("CreatProcess failed.");
	}

	ConnectNamedPipe(namedPipe, NULL);

	printf("Input number: ");

	scanf("%d",&number);

	WriteFile(namedPipe, &number, sizeof(int),&write, NULL);

	WaitForSingleObject(ProcInfo.hProcess,INFINITE);

	ReadFile(namedPipe, &result, sizeof(int),&read, NULL);

	printf("Number: %d\n",number);

	printf("Sqrt: %d\n",result);

	CloseHandle(namedPipe);
	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);

#endif

#ifdef __linux__

	PID pid;

	int number;

	int result;

	int fd[2];

	pipe(fd);

	PID = fork();

	if(!pid){

		char readDescriptor[4], writeDescriptor[4];

        sprintf(readDescriptor, "%d", fd[0]);

        sprintf(writeDescriptor, "%d", fd[1]);

        execl("second", readDescriptor, writeDescriptor, NULL);

	}
	if(pid>0){

		printf("Input number: ");

		scanf("%d",&number);

		write([fd[1], &number, sizeof(int));

        close(fd[1]);

        wait(NULL);

        read(fd[0], &result, sizeof(int));

        close(fd[0]);

		printf("Number: %d",number);

		printf("Sqrt: %d",result);
	}

#endif

}