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
#include "mainwindow.h"
#include "HelperFunctions.h"
#define PORT 4444


int main(int argc, char *argv[]){
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

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
  //  close(sd);
    return a.exec();
}
