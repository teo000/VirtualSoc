#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "database.h"
#define PORT 4444
#define noOfCommands 4

// struct Commands{
// 	char* name;
// 	int length;
// }command[noOfCommands] = {{"", 0}, {"login", 6}, {"sign-up", 7}, {"exit", 4}};

char* ReceiveMessage(int sd, int& len);
void ParseRequest(Database& db, int sd, char* msg, int& len);
void SendResponse(int sd, char* msg, int& len);

void Login(Database& db, int sd);
void Signup(Database& db, int sd);

int main ()
{
	int sd;
	struct sockaddr_in srvAddr, cliAddr;

	if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
		perror("[Server] socket() error: ");
		exit(1);
	}

	bzero(&srvAddr, sizeof(srvAddr));
    	bzero(&cliAddr, sizeof(cliAddr));

	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(PORT);
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((bind(sd, (struct sockaddr*)&srvAddr, sizeof (struct sockaddr))) == -1){
		perror("[Server] bind() error: ");
		exit(1);
	}

	if(listen(sd, 5)==-1){
		perror("[Server] bind() error: ");
		exit(1);
	}

	while(1){
		int client;
		int length = sizeof(cliAddr);
		client = accept(sd, (struct sockaddr*)&cliAddr, (socklen_t*)&length);
		if(client == -1){
			perror("[Server] accept() error: ");
			continue;
		}
		pid_t pid;
		if((pid = fork())==-1){
			perror("[Server] fork() error: ");
			continue;
		} else if(pid > 0){
			close(client);
			continue;
		} else {
			close(sd);
                 // Database *db = new Database();
                 Database db;
                  if(db.OpenDatabase()==-1) exit(-1);
			while(1){
				int len=0;
				char* msg = ReceiveMessage(client, len);
				
				//printf("[server]: [main]: %s\n", msg);
				ParseRequest(db, client, msg, len);
				//SendResponse(client, msg, len);
			}
			exit(0);
		}
	}
	return 0;
}	

char* ReceiveMessage(int sd, int& len){
	if((read(sd, &len, sizeof(len)))==0){
		perror("[Server] Error at reading from client 1!");
		if (-1 == close(sd)) {
			perror("[Server] Error at closing the client1!\n");
			printf("[Server] Connection terminated unsuccessfuly!1\n");
			exit(-1);
		}
		printf("[Server] Connection terminated successfuly!1");
		exit(-1);
	}
	char* msg = (char*)malloc(sizeof(char)*len + 1);
	bzero(msg, len+1);
	int bytesRead = read(sd, msg, len);
	if (bytesRead == -1) {
		perror("[Server] Error at reading from client!2");
		if (-1 == close(sd)) {
			perror("[Server] Error at closing the client!2\n");
			printf("[Server] Connection terminated unsuccessfuly!2\n");
			exit(-1);
		}
		printf("[Server] Connection terminated successfuly!2");
	}
	if (bytesRead == 0) {
		printf("[Server] Client has disconnected!2");
		exit(-1);
	}
	printf("[Server]: %d: %s\n", len, msg);
	return msg;
}

void ParseRequest(Database& db, int sd, char* msg, int& len){
	if((strncmp(msg, "quit", 4))==0){
		char rasp[] = "quit\0";
		SendResponse(sd, rasp, len);
		exit(0);
	}
	if((strncmp(msg, "login", 5))==0){
		char rasp[] = "login\0";
		//SendResponse(sd, rasp, len); 
		Login(db, sd);
	}
      else if((strncmp(msg, "signup", 5))==0){
		char rasp[] = "signup\0";
		//SendResponse(sd, rasp, len); 
		Signup(db, sd);
	}
	else{
		char rasp[] = "buna\0";
		SendResponse(sd, rasp, len);
	}
}

void SendResponse(int client, char* rasp, int& len){
	int bytesWritten = write(client, rasp, len); 
	if (bytesWritten == -1) {
		perror("[Server] Error at writing to client!");
		if (-1 == close(client)) {
			perror("[Server] Error at closing the client!\n");
			printf("[Server] Connection terminated unsuccessfuly!\n");
			exit(-1);
		}
		printf("[Server] Connection terminated successfuly!");
	}
	if (bytesWritten == 0) {
		printf("[Server] Client has disconnected!");
		exit(-1);
	}
	printf("[Server] am trimis : %d: %s\n", len, rasp);
}

void Login(Database& db, int sd){
	printf("[Server]: Login!\n");
	int userLength=0, passLength=0;
	char* username = ReceiveMessage(sd, userLength);
	char* password = ReceiveMessage(sd, passLength);

	if((strcmp(username, "username"))==0 && (strcmp(password, "password"))==0)
	{
		char rasp[10] = "success"; 
		printf("[Server]: succes: %s\n", rasp);
		int len = strlen(rasp);
		SendResponse(sd, rasp, len);
	}
	else {
		char rasp[10] = "failure"; 
		printf("[Server]: failure %s\n", rasp);
		int len = strlen(rasp);
		SendResponse(sd, rasp, len);
	}
}

void Signup(Database& db, int sd){

	printf("[Server]: Signup!\n");
	int userLength=0, passLength=0;
	char* username = ReceiveMessage(sd, userLength);
	char* password = ReceiveMessage(sd, passLength);
      printf("[Server]: am ajuns aici 1!\n");
      char nume[20]="nume frumos\0";
      printf("[Server]: am ajuns aici 2!\n");

      if((db.SignupUser(username, password, nume)) ==-1){
            printf("[Server]: am ajuns aici 3!\n");
            char rasp[10] = "failure"; 
            printf("[Server]: failure %s\n", rasp);
            int len = strlen(rasp);
            SendResponse(sd, rasp, len);
      }
      else {
            printf("[Server]: am ajuns aici 4!\n");
            char rasp[10] = "success"; 
		printf("[Server]: succes: %s\n", rasp);
		int len = strlen(rasp);
		SendResponse(sd, rasp, len);
      }
      printf("[Server]: am ajuns aici 5!\n");
}