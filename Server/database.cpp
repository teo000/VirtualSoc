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
      sprintf(sql, "INSERT INTO Users VALUES('%s', '%s', '%s');", username, password, name);
      rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
      if (rc != SQLITE_OK ) {
            
            printf("SQL error: %s\n", err_msg);
            
            sqlite3_free(err_msg);        
            sqlite3_close(db);
            return -1;
      } 
      return 0;
}