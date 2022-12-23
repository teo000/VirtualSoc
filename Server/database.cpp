#include "database.h"


int Database::OpenDatabase(){
      int rc = sqlite3_open("db.sqlite", &db);

      if (rc != SQLITE_OK) {
        
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return -1;
    }

    return 0;
}

int Database::SignupUser(char* username, char* password, char* name){
      char* sql, *err_msg = 0;
      int rc;
      
      int status = CheckUsernameExists(username);
      if(status!=0) return status;

      sprintf(sql, "INSERT INTO Users VALUES('%s', '%s', '%s');", username, password, name);
      rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
      if (rc != SQLITE_OK ) {
            printf("SignupUser: SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);        
            return -1;
      } 
      return 0;
}

int Database::CheckUsernameExists(char* username){
      sqlite3_stmt *stmt;
      char *sql;
      sprintf(sql, "SELECT username FROM Users WHERE username = \"%s\";", username);
      int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
      //rc = sqlite3_exec(db, sql, callback, (void*)data, &err_msg);
      if (rc != SQLITE_OK ) {
            printf("CheckUsernameExists: SQL error: %s", sqlite3_errmsg(db));
            return -1;
      } 
      while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
            return 1;
      if (rc != SQLITE_DONE) {
            printf("error: %s", sqlite3_errmsg(db));
            return -1;
      }
      sqlite3_finalize(stmt);
      return 0;
}