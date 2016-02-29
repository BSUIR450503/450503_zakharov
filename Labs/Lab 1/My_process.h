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

	int number = 8;

	int result = 0;

	int fd[2];

	pipe(fd);

	FILE *input = fdopen(fd[1],"w");

	FILE *output = fdopen(fd[0],"r");

	PID = fork();

	if(!pid){

		int sqrt = number*number;

		fwrite(&sqrt,1,sizeof(int),input);

	}
	if(pid>0){

		fread(&result,sizeof(int),1,output);

		printf("Number = %d\n",number);

	    printf("Sqrt = %d\n",result);
	}

#endif

}