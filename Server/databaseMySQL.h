#include <mysql/mysql.h>

class Database{
private:
      MYSQL *mysql;
public:
      int OpenDatabase();
      int SignupUser(const char* username, const char* password, const char* name, bool isAdmin, bool privacy);
      int CheckUsernameExists(const char* username);
      int LoginUser(const char* username, const char* password, bool isAdmin);
      int Post(const char* username, const char* post, int privacy);
      int LoadPosts(int sd, const char* username);
      int LoadPosts(int sd); //not signed in
      int LoadAllPosts(int sd);
      int AddFriend(const char* user, const char* frnd, int status);
      int SendMessage(const char* sender, const char* receiver, const char* text);
      int LoadMessenger(int sd, const char* user);
      int DeleteUser(const char* user);
      int DeletePost(const char* user, const char* time);
      int IsPublic(const char* user);
      int IsFriends(const char* user1, const char* user2);
      int FriendList(int sd, const char* user);
};