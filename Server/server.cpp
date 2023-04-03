#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string>
#include <string.h>
#include <map>
#include <iostream>
using namespace std;
#include "databaseMySQL.h"
#include "helper_functions.h"
#define PORT 4444
#define noOfCommands 4

bool isAdmin = false;
bool loggedIn = false;
string loggedUser;
const char* adminPass = "parola";

void ParseRequest(Database& db, int sd, char* msg);


void Login(Database& db, int sd);
void Signup(Database& db, int sd);
void Post(Database& db, int sd);
void LoadFeed(Database& db, int sd);
void AddFriend(Database& db, int sd);
void Exit(Database& db, int sd);
void Logout(Database& db, int sd);
void SendMessage(Database& db, int sd);
void LoadMessenger(Database& db, int sd);
void DeleteUser(Database& db, int sd);
void DeletePost(Database& db, int sd);
void PermissionToMessage(Database& db, int sd);
void FriendList(Database& db, int sd);

map<string, void(*)(Database&, int)> commLogMenu= {
      {"login", &Login}, {"signup", &Signup}
}, commands{
      {"post", &Post}, {"load_feed", &LoadFeed}, {"login", &Login}, {"signup", &Signup}, {"addfriend", &AddFriend}
      ,{"exit", &Exit}, {"logout", &Logout}, {"send_message", &SendMessage}, {"load_messenger", &LoadMessenger},
       {"delete_user", &DeleteUser}, {"delete_post", &DeletePost}, {"permission_to_message", &PermissionToMessage}, 
       {"friend_list", &FriendList}
};

int main ()
{
	int sd;
	struct sockaddr_in srvAddr, cliAddr;

	if((sd = socket(AF_INET, SOCK_STREAM, 0))==-1){
		perror("[Server] socket() error: ");
		exit(1);
	}

      //if((setsockopt()))

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
                 Database db;
                  if(db.OpenDatabase()==-1) exit(-1);
			while(1){
				char* msg = ReceiveMessage(client);
				//printf("[server]: [main]: %s\n", msg);
                        ParseRequest(db, client, msg);
                        free(msg);
			}
			exit(0);
		}
	}
	return 0;
}	

void ParseRequest(Database& db, int sd, char* msg){
      string str(msg);
      commands[str](db, sd);
}


void Login(Database& db, int sd){
	printf("[Server]: Login!\n");
	char* username = ReceiveMessage(sd);
	char* password = ReceiveMessage(sd);
      int admin = GetInt(sd);

      switch(db.LoginUser(username, password, admin)){
            case 0: {


                  SendResponse(sd, "Login successful!");
                  loggedIn = true;
                  loggedUser = username;
                  isAdmin = admin;
            }
            break;
            case 1: SendResponse(sd, "Account does not exist!");
            break;
            case 2: SendResponse(sd, "Wrong Password!");
            break;
            case 3: SendResponse(sd, "Please use the proper login window for your account type");
            break;
            case -1: SendResponse(sd, "An error occured!\n Please try again!");
            break;
      }
}

void Signup(Database& db, int sd){

	printf("[Server]: Signup!\n");
      char* name = ReceiveMessage(sd);
	char* username = ReceiveMessage(sd);
	char* password = ReceiveMessage(sd);
      int isAdmin = GetInt(sd);
      int privacy;
      if(isAdmin) {
            char* adminPassEntered = ReceiveMessage(sd);
            if(strcmp(adminPassEntered, adminPass)!=0){
                  SendResponse(sd, "The admin password is incorrect\n");
                  free(name); free(username); free(password); free(adminPassEntered);
                  return;
            }
            privacy = 0;
      }
      else privacy = GetInt(sd);
      printf("verificam\n");
      switch (db.SignupUser(username, password, name, isAdmin, privacy)){
            case 0: SendResponse(sd, "Account created successfully!");
            break;
            case 1: SendResponse(sd, "Username already exists!\n Please try again!");
            break;
            case -1: SendResponse(sd, "An error occured!\n Please try again!");
            break;
      }
      free(name); free(username); free(password);
}

void Post(Database& db, int sd){
      char* post = ReceiveMessage(sd);
      int privacy;
      if(!isAdmin) privacy = GetInt(sd);
      else privacy = 0;
      switch(db.Post(loggedUser.c_str(), post, privacy)){
            case 0: SendResponse(sd, "Success");
            break;
            case -1: SendResponse(sd, "Error");
            break;
      }
      free(post);
}

void LoadFeed(Database& db, int sd){
      printf("LoadFeed");
      if(loggedIn = true){
            if(isAdmin)
                  switch(db.LoadAllPosts(sd)){
                        case 0: SendResponse(sd, "Success");
                        break;
                        case -1: SendResponse(sd, "Error");
                  }
            else
                  switch(db.LoadPosts(sd, loggedUser.c_str())){
                        case 0: SendResponse(sd, "Success");
                        break;
                        case -1: SendResponse(sd, "Error");
                  }
      }
      else
            switch(db.LoadPosts(sd)){
                  case 0: SendResponse(sd, "Success");
                  break;
                  case -1: SendResponse(sd, "Error");
            }
}
void AddFriend(Database& db, int sd){
      char* frnd = ReceiveMessage(sd);
      int status = GetInt(sd);
      printf("%s %d", frnd, status);
      switch(db.AddFriend(loggedUser.c_str(), frnd, status)){
            case 0: SendResponse(sd, "Success");
            break;
            case -1: SendResponse(sd, "User does not exit");
            break;
            case 1: SendResponse(sd, "The user is already in your friend list!");
            break;
            case 2: SendResponse(sd, "Contul dumneavoastra a fost sters de catre un administrator");
      }
      free(frnd);
}
void Exit(Database& db, int sd){
      printf("Conection closed successfully!");
      close(sd);
      exit(0);
}

void Logout(Database& db, int sd){
      isAdmin = false;
      loggedIn = false;
      loggedUser.erase();
}

void SendMessage(Database& db, int sd){
      char* receiver = ReceiveMessage(sd);
      char* text = ReceiveMessage(sd);
       switch(db.SendMessage(loggedUser.c_str(),receiver, text)){
            case 0: SendResponse(sd, "Success");
            break;
            case -1: SendResponse(sd, "Error");
            break;
            case 1: SendResponse(sd, "User does not exist");
            break;
      }
      free(receiver);
      free(text);
}

void LoadMessenger(Database& db, int sd){
      switch(db.LoadMessenger(sd, loggedUser.c_str())){
            case 0: SendResponse(sd, "Success");
            break;
            case -1: SendResponse(sd, "Error");
            break;
      }
}

void DeleteUser(Database& db, int sd){
      printf("am ajuns in delete user");
      char* user = ReceiveMessage(sd);
      switch(db.DeleteUser(user)){
            case 0: SendResponse(sd, "Success");
            break;
            case -1: SendResponse(sd, "Error");
            break;
            case 1: SendResponse(sd, "User does not exist");
      }
      free(user);
}

void DeletePost(Database& db, int sd){
      char* user = ReceiveMessage(sd);
      char* time = ReceiveMessage(sd);
      switch(db.DeletePost(user, time)){
            case 0: SendResponse(sd, "Success");
            break;
            case -1: SendResponse(sd, "Error");
      }
      free(user); free(time);
}

void PermissionToMessage(Database& db, int sd){
      char* user1 =  ReceiveMessage(sd);
      char* user2 =  ReceiveMessage(sd);
      //SendInt(sd, db.PermissionToMessage(user1, user2))
      switch(db.IsPublic(user2)){
            case 1: SendInt(sd, 1);
            break;
            case 0: {
                  switch(db.IsFriends(user2, user1)){
                        case 1: SendInt(sd, 1);
                        break;
                        case 0: SendInt(sd, 0);
                        break;
                        case -1: SendInt(sd, -1);
                  }
            }
            break;
            case -1: SendInt(sd, -1);
            break;
            case 2: SendInt(sd, 2);
      }
      free(user1); free(user2);
}

void FriendList(Database& db, int sd){
      printf("am ajuns aici\n");
      switch(db.FriendList(sd, loggedUser.c_str())){
            case 0: SendResponse(sd, "Success");
            break;
            case -1: SendResponse(sd, "Error");
      }
}