#include "databaseMySQL.h"
#include "helper_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

int Database::OpenDatabase(){
      net_async_status status;
      if (!(this->mysql = mysql_init(NULL))) {
            printf("mysql_init() failed\n");
            return -1;
      }
      while ((status = mysql_real_connect_nonblocking(mysql, "localhost", "teo",
                                                      "parola", "virtualSocDB", 0,
                                                      NULL, 0))
                  == NET_ASYNC_NOT_READY)
      ; 

      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_connect_nonblocking() failed\n");
            return -1;
      }
      printf("e ok\n");
      return 0;
}

int Database::SignupUser(const char* username, const char* password, const char* name, bool isAdmin, bool privacy){
      int exista = CheckUsernameExists(username);
      if(exista != 0) {
            if(exista ==-1) printf("Signup: SQL Error");
            return exista;
      }

      char stmt[100]="";
      sprintf(stmt, "INSERT INTO Users VALUES('%s', '%s', '%s', %d, %d);", username, password, name, isAdmin, privacy);
      
      net_async_status status;
      MYSQL_RES * result = NULL;

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
            status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      sprintf(stmt, "INSERT INTO Friends VALUES('%s', '%s', 2);", username, username);

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
            status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }
      return 0;

      return 0;
}

//1 daca exista, 0 daca nu, -1 pt eroare
int Database::CheckUsernameExists(const char* username){
      char stmt[100]="";
      net_async_status status;
      MYSQL_RES * result = NULL;
      sprintf(stmt, "SELECT username FROM Users WHERE username = \"%s\";", username);

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      status = mysql_fetch_row_nonblocking(result, &row);
      while (status != NET_ASYNC_COMPLETE) {
            status = mysql_fetch_row_nonblocking(result, &row);
      }

      if (row != NULL)
            return 1;
      return 0;
}

//0-ok 1-username does not exist 2-password does not exist -1-eroare
int Database::LoginUser(const char* username, const char* password, bool isAdmin){
      char stmt[100]="";
      net_async_status status;
      MYSQL_RES * result = NULL;
      sprintf(stmt, "SELECT password, admin FROM Users WHERE username = \"%s\";", username);

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      status = mysql_fetch_row_nonblocking(result, &row);
      while (status != NET_ASYNC_COMPLETE) {
            status = mysql_fetch_row_nonblocking(result, &row);
      }

      if (row == NULL)
            return 1; //username does not exist
      if((strcmp(row[0], password))==0){
            if(atoi(row[1])==isAdmin) 
                   return 0;
            else return 3; //use proper login
      }
      else return 2;

      while (mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE) ;
}

int Database::Post(const char* username, const char* post, int privacy){
      char stmt[100]="";
      sprintf(stmt, "INSERT INTO Posts VALUES('%s', '%s', %d, now());", username, post, privacy);
      
      net_async_status status;
      MYSQL_RES * result = NULL;

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
            status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      return 0;
}

int Database::LoadPosts(int sd, const char* username){
      char stmt[200]="";
      net_async_status status;
      MYSQL_RES * result = NULL;
      sprintf(stmt, "SELECT * FROM Posts p  WHERE privacy = 0 OR privacy <= (SELECT f.status FROM Friends f WHERE p.username = f.user AND f.friend = '%s') ORDER BY time DESC;"
      ,username);

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      while(1){
            status = mysql_fetch_row_nonblocking(result, &row);
            while (status != NET_ASYNC_COMPLETE) {
                  status = mysql_fetch_row_nonblocking(result, &row);
            }
            if (status == NET_ASYNC_ERROR) {
                  printf("mysql_fetch_row_nonblocking() failed\n");
                  return -1;
            }
            if (row == NULL){
                  printf("!@#");
                  SendResponse(sd, "!@#");
                  return 0;
            }
            printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
            SendResponse(sd, row[0]);
            SendResponse(sd, row[3]);
            SendResponse(sd, row[1]);
            SendResponse(sd, row[2]); //privacy = int

      }
      while (mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE) ;

}

int Database::LoadPosts(int sd){
      const char* stmt="SELECT * FROM Posts  WHERE privacy = 0  ORDER BY time DESC;";
      net_async_status status;
      MYSQL_RES * result = NULL;

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      while(1){
            status = mysql_fetch_row_nonblocking(result, &row);
            while (status != NET_ASYNC_COMPLETE) {
                  status = mysql_fetch_row_nonblocking(result, &row);
            }
            if (status == NET_ASYNC_ERROR) {
                  printf("mysql_fetch_row_nonblocking() failed\n");
                  return -1;
            }
            if (row == NULL){
                  printf("!@#");
                  SendResponse(sd, "!@#");
                  return 0;
            }
            printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
            SendResponse(sd, row[0]);
            SendResponse(sd, row[3]);
            SendResponse(sd, row[1]);
            SendResponse(sd, row[2]); //privacy = int

      }
      while (mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE) ;
}

int Database::LoadAllPosts(int sd){
      const char* stmt="SELECT * FROM Posts ORDER BY time DESC;";
      net_async_status status;
      MYSQL_RES * result = NULL;

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      while(1){
            status = mysql_fetch_row_nonblocking(result, &row);
            while (status != NET_ASYNC_COMPLETE) {
                  status = mysql_fetch_row_nonblocking(result, &row);
            }
            if (status == NET_ASYNC_ERROR) {
                  printf("mysql_fetch_row_nonblocking() failed\n");
                  return -1;
            }
            if (row == NULL){
                  printf("!@#");
                  SendResponse(sd, "!@#");
                  return 0;
            }
            printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
            SendResponse(sd, row[0]);
            SendResponse(sd, row[3]);
            SendResponse(sd, row[1]);
            SendResponse(sd, row[2]); //privacy = int

      }
      while (mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE) ;
}

int Database::AddFriend(const char* user, const char* frnd, int friendship){
      if(IsFriends(user, frnd) == 1) return 1;

      int exista = CheckUsernameExists(frnd);
      if(exista ==-1) {
            printf("AddFriend: SQL Error");
            return -1;
      }
      if(exista==0) return 2;

      exista = CheckUsernameExists(user);
      if(exista ==-1) {
            printf("AddFriend: SQL Error");
            return -1;
      }
      if(exista==0) return 2;

      char stmt[100]="";
      sprintf(stmt, "INSERT INTO Friends VALUES('%s', '%s', %d);", user, frnd, friendship);
      
      net_async_status status;
      MYSQL_RES * result = NULL;

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
            status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }
      return 0;
}

int Database::SendMessage(const char* sender, const char* receiver, const char* text){
      int exista = CheckUsernameExists(receiver);
      if(exista ==-1) {
            printf("Signup: SQL Error");
            return -1;
      }
      if(exista==0) return 1;
      
      char stmt[100]="";
      sprintf(stmt, "INSERT INTO Messenger VALUES('%s', '%s', '%s', now());", sender, receiver, text);
      
      net_async_status status;
      MYSQL_RES * result = NULL;

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
            status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }
      return 0;
}

int Database::LoadMessenger(int sd, const char* user){
      char stmt[100]="";
      net_async_status status;
      MYSQL_RES * result = NULL;
      sprintf(stmt, "SELECT * FROM Messenger WHERE receiver = '%s' ORDER BY time DESC;", user);

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      while(1){
            status = mysql_fetch_row_nonblocking(result, &row);
            while (status != NET_ASYNC_COMPLETE) {
                  status = mysql_fetch_row_nonblocking(result, &row);
            }
            if (status == NET_ASYNC_ERROR) {
                  printf("mysql_fetch_row_nonblocking() failed\n");
                  return -1;
            }
            if (row == NULL){
                  printf("!@#");
                  SendResponse(sd, "!@#");
                  return 0;
            }
            printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
            SendResponse(sd, row[0]);
            SendResponse(sd, row[2]);
            SendResponse(sd, row[3]);

      }
      while (mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE) ;

}

int Database::DeleteUser(const char* user){
      int exista = CheckUsernameExists(user);
      if(exista ==-1) {
            printf("DeleteUser: SQL Error");
            return -1;
      }
      if(exista==0) return 1;


      char stmt[100]="";
      sprintf(stmt, "DELETE FROM Users WHERE username = '%s';", user);
      
      net_async_status status;
      MYSQL_RES * result = NULL;

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
            status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }
      return 0;
}

int Database::DeletePost(const char* user, const char* time){
      char stmt[100]="";
      sprintf(stmt, "DELETE FROM Posts WHERE username = '%s' AND time = '%s' ;", user, time);
      
      net_async_status status;
      MYSQL_RES * result = NULL;

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
            status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }
      return 0;
}

int Database::IsPublic(const char* user){
      //daca user2 e public
      char stmt[100]="";
      net_async_status status;
      MYSQL_RES * result = NULL;
      sprintf(stmt, "SELECT isPrivate FROM Users WHERE username = \"%s\";", user);

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      status = mysql_fetch_row_nonblocking(result, &row);
      while (status != NET_ASYNC_COMPLETE) {
            status = mysql_fetch_row_nonblocking(result, &row);
      }

      if (row == NULL)
            return 2; //username does not exist
      if((strcmp(row[0], "0"))==0)
            return 1; //user2 e public, e ok
      else return 0; 
      while (mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE) ;
}

int Database::IsFriends(const char* user1, const char* user2){
      char stmt[100]="";
      net_async_status status;
      MYSQL_RES * result = NULL;
      sprintf(stmt, "SELECT * FROM Friends WHERE user= \"%s\" AND friend = \"%s\";", user1, user2);

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      status = mysql_fetch_row_nonblocking(result, &row);
      while (status != NET_ASYNC_COMPLETE) {
            status = mysql_fetch_row_nonblocking(result, &row);
      }

      if (row == NULL)
            return 0; //not friends
      else return 1; //friends :) 
      while (mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE) ;

}

 int Database::FriendList(int sd, const char* user){
      char stmt[200]="";
      net_async_status status;
      MYSQL_RES * result = NULL;
      sprintf(stmt, "SELECT friend, status FROM Friends  WHERE user = '%s';"
      ,user);

      status = mysql_real_query_nonblocking(mysql, stmt,
                                        (unsigned long)strlen(stmt));
                  
      while (status == NET_ASYNC_NOT_READY)
             status = mysql_real_query_nonblocking(mysql, stmt, (unsigned long)strlen(stmt));
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_real_query_nonblocking() failed\n");
            return -1;
      }

      status = mysql_store_result_nonblocking(mysql, &result);

      while (status == NET_ASYNC_NOT_READY) {
            status = mysql_store_result_nonblocking(mysql, &result);
      }
      if (status == NET_ASYNC_ERROR) {
            printf("mysql_store_result_nonblocking() failed\n");
            return -1;
      }

      MYSQL_ROW row;
      while(1){
            status = mysql_fetch_row_nonblocking(result, &row);
            while (status != NET_ASYNC_COMPLETE) {
                  status = mysql_fetch_row_nonblocking(result, &row);
            }
            if (status == NET_ASYNC_ERROR) {
                  printf("mysql_fetch_row_nonblocking() failed\n");
                  return -1;
            }
            if (row == NULL){
                  printf("!@#");
                  SendResponse(sd, "!@#");
                  return 0;
            }
            printf("%s %s\n", row[0], row[1]);
            if(strcmp(user, row[0])!=0){
                  SendResponse(sd, row[0]);
                  SendResponse(sd, row[1]); //privacy = int
            }

      }
      while (mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE) ;

 }