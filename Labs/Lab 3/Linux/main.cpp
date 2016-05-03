#include <iostream>
#include <stdio.h>
#include <termios.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>
#include <string.h>
#include <math.h>

using namespace std;

void WaitSemaphore(int sem_id, int num);
void ReleaseSemaphore(int sem_id, int num);

int main(int argc, char *argv[]){  
	int shm_id = shmget(IPC_PRIVATE, 80, IPC_CREAT | 0666);

	if (shm_id < 0) {
		printf("shmget error\n");
		return(0);
	}

	system("clear");

	int sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
	semctl(sem_id, 0, SETALL, 0);
	
	if(sem_id < 0){
		printf("Semaphores is not created.");
		return 0;
	}

	int pid = fork();
	switch(pid){
	case -1:
		printf("New process is not created\n");
		return 0;
		
	case 0:{
			void *buffer = shmat(shm_id, NULL, 0);
			while(1)
			{
				char messageFromServer[80];
				
				WaitSemaphore(sem_id, 0);					
				memcpy(messageFromServer,buffer,80);
				
				cout << "\nClient process:\n";

				for(int i = 0; messageFromServer[i] != '\0'; i++){
					putchar(messageFromServer[i]);
					fflush(stdout);
					usleep(100000);
				}
				
				char messageForServer[80];
				
				fflush(stdin);
				fgets(messageForServer,80,stdin);
				
				memcpy(buffer,messageForServer,80);
				
				cout<<"\n\n\n";
				
				ReleaseSemaphore(sem_id, 1);		  
			}
			
			
		}break;
		
	default:{
			void *buffer = shmat(shm_id, NULL, 0);
			cout<<"Server process:\n";
			while(1){
				char str[80];
				fflush(stdin);
				fgets(str,80,stdin);	
								
				memcpy(buffer,str,80);	
				ReleaseSemaphore(sem_id, 0);
				
				WaitSemaphore(sem_id, 1);

				char messageFromClient[80];
				memcpy(messageFromClient,buffer,80);
				
				cout<<"Server process:\n";
				
				for(int i = 0; messageFromClient[i] != '\0'; i++){
					putchar(messageFromClient[i]);
					fflush(stdout);
					usleep(100000);
				}
			}
		}break;
	}

	system("clear"); 
	system("stty echo");
	return 0;
}

void WaitSemaphore(int sem_id, int num){
	struct sembuf buf;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;
	buf.sem_num = num;
	semop(sem_id, &buf, 1);	
}

void ReleaseSemaphore(int sem_id, int num){
	struct sembuf buf;
	buf.sem_op = 1;
	buf.sem_flg = SEM_UNDO;
	buf.sem_num = num;
	semop(sem_id, &buf, 1); 
}
