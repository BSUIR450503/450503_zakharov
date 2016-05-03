#include "Client.h"
#define END_PRINT 1
#define EXIT 2

Client::Client() {
	semaphores[0] = OpenSemaphore(SEMAPHORE_ALL_ACCESS,TRUE, "START_PRINT");
	semaphores[1] = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "END_PRINT");
	semaphores[2] = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "EXIT");

	system("title Client");

	sharedMemory = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "LAB3");
	if (sharedMemory == NULL) {
		std::cout << "Could not open file mapping object " << GetLastError() << std::endl;
	}
}

int Client::start() {
	while (1) {
		std::string message;

		std::cout << "Waiting message from server...";
		int result = WaitForMultipleObjects(3, semaphores, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (result == EXIT)
			break;

		buffer = (LPTSTR)MapViewOfFile(sharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, 80);
		if (buffer == NULL) {
			std::cout << "Could not map view of file " << GetLastError() << std::endl;
		}

		message = std::string(buffer);

		std::cout << "\nMessage from server: ";
		for (int i = 0; i < message.length(); i++) {
			std::cout << message[i];
			Sleep(100);
		}
		std::cout << std::endl;

		CopyMemory((PVOID)buffer, " ", 80);

		char str[80];
		printf("\nMessage for server: ");
		fflush(stdin);
		fgets(str, 80, stdin);

		if (strcmp(str, "exit\n") == 0) {
			ReleaseSemaphore(semaphores[2], 1, NULL);
			break;
		}

		CopyMemory((PVOID)buffer, str, 80 * sizeof(char));

		ReleaseSemaphore(semaphores[END_PRINT], 1, NULL);
	}
	return 0;
}

Client :: ~Client() {
	UnmapViewOfFile(buffer);
	CloseHandle(sharedMemory);
	CloseHandle(semaphores[0]);
	CloseHandle(semaphores[1]);
}