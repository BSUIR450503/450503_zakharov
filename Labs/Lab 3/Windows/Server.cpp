#include "Server.h"

Server :: Server() {
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	ZeroMemory(&clientProcessInfo, sizeof(clientProcessInfo));

	semaphores[0] = CreateSemaphore(NULL, 0, 1, "START_PRINT");
	semaphores[1] = CreateSemaphore(NULL, 0, 1, "END_PRINT");
	semaphores[2] = CreateSemaphore(NULL, 0, 1, "EXIT");

	sharedMemoryName = NULL;
	sharedMemory = NULL;
	buffer = NULL;

	system("title Server");
}

int Server::createSharedMemory() {
	sharedMemoryName = TEXT("LAB3");

	sharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 80, sharedMemoryName);
	if (sharedMemory == NULL) {
		printf("Could not create file mapping object (%d).\n", GetLastError());
		return 1;
	}

	buffer = (LPTSTR)MapViewOfFile(sharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, 80);
	if (buffer == NULL) {
		printf("Could not map view of file (%d).\n", GetLastError());
		CloseHandle(sharedMemory);
		return 1;
	}
}

int Server::start(char* path) {
	CreateProcess(path, " 2", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &clientProcessInfo);

	while (1) {
		char message[80];
		std::cout << "\nEnter message for client: ";
		fflush(stdin);
		fgets(message, 80, stdin);

		if (strcmp(message,"exit\n") == 0) {
			ReleaseSemaphore(semaphores[2], 1, NULL);
			WaitForSingleObject(clientProcessInfo.hProcess, INFINITE);
			break;
		}

		CopyMemory((PVOID)buffer, message, 80 * sizeof(char));

		ReleaseSemaphore(semaphores[0], 1, NULL);

		std::cout << "Waiting message from client...";

		int result = WaitForMultipleObjects(3, semaphores, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (result == 2) {
			break;
		}

		std::string messageFromClient = std::string(buffer);

		std::cout << "\nMessage from client: ";
		for (int i = 0; i < messageFromClient.length(); i++) {
			std::cout << messageFromClient[i];
			Sleep(100);
		}

		CopyMemory((PVOID)buffer, " ", 80);
	}
	return 0;
}

Server::~Server() {
	UnmapViewOfFile(buffer);
	CloseHandle(sharedMemory);
	CloseHandle(semaphores[0]);
	CloseHandle(semaphores[1]);
}