#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <QApplication>
#include "StartMenu.h"
#include "HelperFunctions.h"
#define PORT 4444
#define noOfCommands 4

//char command[noOfCommands][20] = {"", "login", "sign-up", "exit"};

//void ReadCommand(char* msg);
//void ParseCommand(char* msg);
//void SendRequest(int sd, char* msg);
//void ReceiveResponse(int sd, char* msg);
void InterpretResponse(char* msg);
//char* intToString(int x);

int main(int argc, char *argv[]){
    //int sd;
    struct sockaddr_in srvAddr;

    if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
          perror("[Server] socket() error: ");
          exit(1);
    }

    memset(&srvAddr, 0, sizeof(srvAddr));

    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(PORT);
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((connect (sd, (struct sockaddr *)&srvAddr, sizeof (struct sockaddr))) == -1)
    {
          perror ("[client] connect() error: ");
          exit(1);
    }

    //aici o sa-i dam drumul la interfata
        QApplication a(argc, argv);
        StartMenu w;
        w.show();
       // close(sd);
        return a.exec();
//        while(1){
//            char msg[100];

//            ReadCommand(msg); //aici o sa astepte sa faca
//            //utilizatorul ceva pe interfata
//            //  ParseCommand(msg); //asta o sa fie nefolositoare

//            SendRequest(sd, msg);
//            ReceiveResponse(sd, msg);
//            InterpretResponse(msg); //?
//        }


        //return 0;
}


//void ReadCommand(char* msg){
//        memset(msg, 0, 100);
//        printf ("[client]Introduceti ceva: ");
//        fflush (stdout);
//        read (0, msg, 100);
//}

/*void ParseCommand(char* msg){
      for(int i = 1; i<=noOfCommands; i++)
            if(strncmp(msg, command[i].name, command[i].length)==0)

}*/

//void SendRequest(int sd, char* msg){
//        int len = strlen(msg);
//        if ((write (sd, &len, sizeof(len))) <= 0)
//        {
//            perror ("[client]Eroare la write() spre server.\n");
//            exit(1);
//        }

//        if ((write (sd, msg, len)) <= 0)
//        {
//            perror ("[client]Eroare la write() spre server.\n");
//            exit(1);
//        }
//}



void InterpretResponse(char* msg){
    printf ("[client]Mesajul primit este: %s\n", msg);
    if(strncmp(msg, "quit", 4) ==0)
        exit(0);
}
