#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

extern int sd;
void SendRequest(int sd, char* msg);
void ReceiveResponse(int sd, char* msg);


#endif // HELPERFUNCTIONS_H
