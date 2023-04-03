#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "HelperFunctions.h"
int sd = 0;
void SendRequest(int sd, const char* msg){
    int len = strlen(msg);
    if ((write (sd, &len, sizeof(len))) <= 0)
    {
        perror ("[client]Eroare la write() spre server.\n");
        exit(1);
    }

    if ((write (sd, msg, len)) <= 0)
    {
        perror ("[client]Eroare la write() spre server.\n");
        exit(1);
    }
    printf("[Client] am trimis: %d %s\n",len, msg);
}
void SendInt(int sd, int msg){
    printf("%d", msg);
    if ((write (sd, &msg, sizeof(msg))) <= 0)
    {
        perror ("[client]Eroare la write() spre server.\n");
        exit(1);
    }
}
int GetInt(int sd){
      int msg;
    if((read(sd, &msg, sizeof(msg)))==0){
        perror("[Client] Error at reading from server!");
        if (-1 == close(sd)) {
            perror("[Client] Error at closing the the connection!\n");
            printf("[Client] Connection terminated unsuccessfuly!\n");
            exit(-1);
        }
        printf("[Client] Connection terminated successfuly!");
        exit(-1);
    }
    printf("am primit %d\n", msg);
      return msg;
}

char* ReceiveResponse(int sd){
    int len=0;
    if((read(sd, &len, sizeof(len)))==0){
        perror("[Client] Error at reading from server!");
        if (-1 == close(sd)) {
            perror("[Client] Error at closing the server!\n");
            printf("[Client] Connection terminated unsuccessfuly!\n");
            exit(-1);
        }
        printf("[Client] Connection terminated successfuly!");
        exit(-1);
    }
    char* msg = (char*)malloc(sizeof(char)*len + 1);
    bzero(msg, len+1);
    int bytesRead = read(sd, msg, len);
    if (bytesRead == -1)
    {
        perror ("[Client]Eroare la read() dinspre server.\n");
        exit(1);
    }
    printf("[Client] am primit: %d, %s\n", len, msg);
    return msg;
}

bool isAlphanumeric(std::string str){
    return std::all_of(str.begin(), str.end(), [](char const &c) {
            return std::isalnum(c);
        });
}
