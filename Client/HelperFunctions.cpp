#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "HelperFunctions.h"
int sd = 0;
void SendRequest(int sd, char* msg){
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
    printf("[Client] am trimis: %s\n", msg);
}
void ReceiveResponse(int sd, char* msg){
    if ((read(sd, msg, 100)) < 0)
    {
        perror ("[client]Eroare la read() dinspre server.\n");
        exit(1);
    }
    printf("[Client] am primit: %s\n", msg);
}

