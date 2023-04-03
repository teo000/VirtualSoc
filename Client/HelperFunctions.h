#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string>

extern int sd;
void SendRequest(int sd, const char* msg);
char* ReceiveResponse(int sd);
void SendInt(int sd, int msg);
int GetInt(int sd);
bool isAlphanumeric(std::string str);
#endif // HELPERFUNCTIONS_H
