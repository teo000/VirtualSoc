#include <stdio.h>
#include "sqlite3.h"

class Database{
private: 
      sqlite3 *db;
public:
      int OpenDatabase();
      int SignupUser(char* username, char* password, char* name);
};
