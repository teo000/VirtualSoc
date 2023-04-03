char* ReceiveMessage(int sd);
void SendResponse(int sd, const char* msg);
int GetInt(int sd);
void SendInt(int sd, int msg);
char* CurrentTime();