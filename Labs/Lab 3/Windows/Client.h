#pragma once

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<iostream>
#include<string>

class Client {
public:
	Client();
	int start();
	~Client();
private:
	HANDLE sharedMemory;
	HANDLE semaphores[3];
	LPCTSTR buffer;
};