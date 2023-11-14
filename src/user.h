#ifndef USER_H
#define USER_H

#include "def.h"
#include "XViewerStruct.h"

#define USER_STRUCT_USERNAME_NOT_FOUND "USER NOT FOUND"
#define USER_STRUCT_PASS_NOT_FOUND "PASS NOT FOUND"
#define USER_STRUCT_IS_ADMIN_NOT_FOUND false
#define USER_STRUCT_ALOGIN_NOT_FOUND false

class Users
{
public:
    void init();

    std::vector<userStruct_t> &getUserVect() { return _users; }

    bool addUser(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE], bool isAdmin, bool autologin);
    bool removeUser(char username[USER_USERNAME_SIZE]);
    bool updatePass(char username[USER_USERNAME_SIZE], char newPass[USER_PASS_SIZE]);
    bool updateIsAdmin(char username[USER_USERNAME_SIZE], bool newIsAdmin);
    bool updateAutologin(char username[USER_USERNAME_SIZE], bool newAutologin);
    userStruct_t getUser(char username[USER_USERNAME_SIZE]);

    void printUsers();

    String login(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE]);
    bool setWorkingUser(char username[USER_USERNAME_SIZE]);
    userStruct_t getWorkingUser();
    bool setDefaultWorkingUser(char username[USER_USERNAME_SIZE]);
    userStruct_t getDefaultWorkingUser();
    int8_t getIndexWorkingUser() { return _indexWorkingUser; }

    DynamicJsonDocument vectToJson();

    void setDbg(bool dbgOn) { _dbgOn = dbgOn; }

private:
    std::vector<userStruct_t> _users;
    int8_t _indexWorkingUser = -1;
    int8_t _indexDefaultWorkingUser = -1;

    char _buffer[256];

    bool _dbgOn = false;

    void _dbg(String str = "", bool header = true);
};

#endif