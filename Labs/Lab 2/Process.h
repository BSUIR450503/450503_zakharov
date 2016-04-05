#ifdef _WIN32
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#endif

#ifdef linux
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

static const char* processesNames[] = { "1.First process\n\r", "2.Second process\n\r", "3.Third process\n\r",
    "4.Fourth process\n\r", "5.Fifth process\n\r", "6.Sixth process\n\r", "7.Seventh process\n\r", "8.Eighth process\n\r",
    "9.Ninth process\n\r", "10.Tenth process\n\r" };

using namespace std;

void print(int number);

#ifdef _WIN32
class Process
{
private:
    PROCESS_INFORMATION processInformation;
    HANDLE printEvent;
    HANDLE closeEvent;
public:
    PROCESS_INFORMATION createProcess(int, char*);
    HANDLE getPrintEvent();
    HANDLE getCloseEvent();
};

PROCESS_INFORMATION Process::createProcess(int number, char* name){
    STARTUPINFO startupInformation;
    PROCESS_INFORMATION processInformation;

    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    char printEventName[10];
    char closeEventName[10];
    char commandLine[10] = "-n ";

    itoa(number, printEventName, 10);
    itoa(number+10, closeEventName, 10);

    strcat(commandLine, printEventName);
    strcat(commandLine, " ");
    strcat(commandLine, closeEventName);

    if (!CreateProcess(name,commandLine,NULL,NULL,FALSE,NULL,NULL,NULL,&startupInformation,&processInformation)){
        cout << "Error" << GetLastError();
    }

    printEvent = CreateEvent(NULL, TRUE, FALSE, printEventName);
    closeEvent = CreateEvent(NULL, FALSE, FALSE, closeEventName);

    return processInformation;
}

HANDLE Process :: getPrintEvent(){
	return printEvent;
}

HANDLE Process :: getCloseEvent(){
	return closeEvent;
}
#endif

#ifdef __linux__
int printFlag = 0;
int endFlag = 1;

struct sigaction printSignal;
struct sigaction endSignal;

void setPrintFlag(int signal) {
    printFlag = 1;
}

void setEndFlag(int signal) {
    endFlag = 1;
}
#endif

void print(int number){
    for (int i = 0; i < strlen(processesNames[number - 1]); i++){
        cout << processesNames[number - 1][i];
#ifdef _WIN32
        Sleep(50);
#elif __linux__
        usleep(50000);
        fflush(stdout);
        refresh();
#endif
    }
    return;
}