#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string.h>

#include "helper_functions.h"



char* ReceiveMessage(int sd){
      int len=0;
	if((read(sd, &len, sizeof(len)))<0){
		perror("[Server] Error at reading from client, terminating connnection");
		close(sd);
		exit(-1);
	}

	char* msg = (char*)malloc(sizeof(char)*len + 1);
	bzero(msg, len+1);
	int bytesRead = read(sd, msg, len);
	if (bytesRead == -1) {
		perror("[Server] Error at reading from client, terminating connnection");
		close(sd);
		exit(-1);
	}
	if (bytesRead == 0) {
		printf("[Server] Client has disconnected!");
		exit(-1);
	}
	printf("[Server]: %d: %s\n", len, msg);
	return msg;
}

int GetInt(int sd){
      int msg;
	if((read(sd, &msg, sizeof(msg)))==0){
		perror("[Server] Error at reading from client, terminating connnection");
		close(sd);
		exit(-1);
	}
       printf("am primit %d\n", msg);
      return msg;
}

void SendInt(int sd, int msg){
      printf("am trimis %d\n", msg);
    if ((write (sd, &msg, sizeof(msg))) <= 0)
    {
        perror ("[Server]Eroare la write() spre client.\n");
        close(sd);
        exit(1);
    }
}

void SendResponse(int client, const char* rasp){
      int len = strlen(rasp);
      int bytesWritten = write(client, &len, sizeof(len)); 
	if (bytesWritten <=0) {
		perror ("[Server]Eroare la write() spre client.\n");
            close(client);
            exit(1);
	}

	bytesWritten = write(client, rasp, len); 
	if (bytesWritten <= 0) {
		perror ("[Server]Eroare la write() spre client.\n");
            close(client);
            exit(1);
	}
	printf("[Server] am trimis : %d: %s\n", len, rasp);
}


char* CurrentTime(){
      time_t curr_time;
      tm* curr_tm;
      char* time_string = (char*) malloc (30);

      time(&curr_time);
      curr_tm = localtime(&curr_time);
      strftime(time_string, 30, "%B %d %Y, %R", curr_tm);

      return time_string;
}