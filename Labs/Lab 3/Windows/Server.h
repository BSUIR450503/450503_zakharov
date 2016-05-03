#pragma once

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<iostream>
#include<string>

class Server {

public:
	Server();
	int createSharedMemory();
	int start(char*);
	~Server();
private:
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION clientProcessInfo;
	HANDLE semaphores[3];
	HANDLE sharedMemory;
	TCHAR* sharedMemoryName;
	LPCTSTR buffer;
};
