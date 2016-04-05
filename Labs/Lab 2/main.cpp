#include "Process.h"

int main(int argc, char *argv[]){
#ifdef _WIN32
    if (argc == 3){
        HANDLE printEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, argv[1]);
        HANDLE closeEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, argv[2]);

        while (1){
            if (WaitForSingleObject(printEvent, 1) == WAIT_OBJECT_0){
                print(atoi(argv[1]));
                ResetEvent(printEvent);
            }
            if (WaitForSingleObject(closeEvent, 0) == WAIT_OBJECT_0){
                CloseHandle(closeEvent);
                CloseHandle(printEvent);
                return 0;
            }
        }
    }
    else{
        char key;
        int processCounter = 0;
        int currentPrinting = 1;
        Process processes[10];

        while (1){
            key = getch_noblock();
            Sleep(1);
            switch (key){
            case '+':
                if (processCounter < 10){
                    processCounter++;
                    processes[processCounter - 1].createNewProcess(processCounter, argv[0]);
                }
                break;
            case '-':
                if (processCounter > 0){
                    SetEvent(processes[--processCounter].getCloseEvent());
                }
                break;
            case 'q':
                if (processCounter > 0){
                    for (int i = 0; i < processCounter; i++)
                        SetEvent(processes[i].getCloseEvent());
                }
                return 0;
            }

            if (processCounter && WaitForSingleObject(processes[currentPrinting - 1].getPrintEvent(), 0) == WAIT_TIMEOUT){
                if (currentPrinting >= processCounter)
                    currentPrinting = 1;
                else
                    currentPrinting++;
                SetEvent(processes[currentPrinting - 1].getPrintEvent());
            }
        }
    }

#elif __linux__
    pid_t processes[10];
    char key;
    int processCounter = 0;
    int currentPrinting = 1;

    initscr();
    clear();
    noecho();
    refresh();

    printSignal.sa_handler = setPrintFlag;
    sigaction(SIGUSR1, &printSignal, NULL);

    endSignal.sa_handler = setEndFlag;
    sigaction(SIGUSR2, &endSignal, NULL);

    while (1){
        key = getchar();
        usleep(1000);
        fflush(stdout);
        refresh();

        switch (key){
        case '+':
            if (processCounter < 10){
                processes[processCounter] = fork();
                processCounter++;
                if(processes[processCounter - 1] == 0){
                     endFlag = 0;
                    while (!endFlag){
                        usleep(1000);
                        fflush(stdout);
                        refresh();

                        if (printFlag){
                            print(processCounter);
                            refresh();
                            printFlag = 0;
                            kill(getppid(), SIGUSR2);
                        }
                    }
                    return 0;
                }
            }
            break;

        case '-':
            if (processCounter > 0)
            {
                kill(processes[--processCounter], SIGUSR2);
                waitpid(processes[processCounter], NULL, 0);
                kill(processes[processCounter], SIGKILL);
                waitpid(processes[processCounter], NULL, 0);
            }
            break;

        case 'q':
            if (processCounter > 0)
            {
                for (; processCounter > 0; processCounter--)
                {
                    kill(processes[processCounter - 1], SIGUSR2);
                    waitpid(processes[processCounter - 1], NULL, 0);
                    kill(processes[processCounter - 1], SIGKILL);
                    waitpid(processes[processCounter - 1], NULL, 0);
                }
            }
            clear();
            endwin();
            return 0;
        }

        if (processCounter && endFlag)
        {
            endFlag = 0;
            if (currentPrinting >= processCounter)
                currentPrinting = 1;
            else
                currentPrinting++;
            kill(processes[currentPrinting - 1], SIGUSR1);
        }
        refresh();
    }
#endif
}