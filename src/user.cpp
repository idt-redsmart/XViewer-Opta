#include "user.h"

bool Users::addUser(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE], bool isAdmin, bool autologin)
{
    sprintf(_buffer, "Add user\tUser: %32s | Pass: %32s | Is admin: %1u | ALogin: %1u", username, pass, isAdmin, autologin);
    _dbg(_buffer);

    // Scan user vector and check if the username added is already in use.
    for (uint8_t index = 0; index < _users.size(); index++)
    {
        if (strcmp(username, _users.at(index).username) == 0)
        {
            sprintf(_buffer, "Username %32s already in use. Pass: %32s | Is admin: %1u | ALogin: %1u",
                    _users.at(index).username, _users.at(index).pass, _users.at(index).isAdmin, _users.at(index).autologin);
            _dbg(_buffer);

            return false;
        }
    }

    // Check vector max size
    if (_users.size() + 1 > USER_MAX_NUMBER)
    {
        _dbg("Maximum number of users reached. Can't add");
        return false;
    }

    // Username is not used - add
    userStruct_t tmp;
    memcpy(tmp.username, username, USER_USERNAME_SIZE);
    memcpy(tmp.pass, pass, USER_PASS_SIZE);
    tmp.isAdmin = isAdmin;
    tmp.autologin = autologin;

    _users.push_back(tmp);

    sprintf(_buffer, "User added\tUser: %32s | Pass: %32s | Is admin: %1d | ALogin: %1d", username, pass, isAdmin, autologin);
    _dbg(_buffer);

    return true;
}

bool Users::removeUser(char username[USER_USERNAME_SIZE])
{
    bool userFound = false;
    int8_t indexUser = -1;
    std::vector<userStruct_t> tmpVect(_users);

    _dbg("Trying to remove user: " + String(username));

    for (uint8_t index = 0; index < tmpVect.size(); index++)
    {
        if (strcmp(tmpVect.at(index).username, username) == 0)
        {
            userFound = true;
            indexUser = index;
            break;
        }
    }

    if (userFound)
    {
        tmpVect.erase(tmpVect.begin() + indexUser);
    }
    else
    {
        _dbg("User to remove not found. Looking for user: " + String(username));
        return false;
    }

    _users.clear();

    for (uint8_t index = 0; index < tmpVect.size(); index++)
    {
        userStruct_t tmp;

        memcpy(tmp.username, tmpVect.at(index).username, USER_USERNAME_SIZE);
        memcpy(tmp.pass, tmpVect.at(index).pass, USER_PASS_SIZE);
        tmp.isAdmin = tmpVect.at(index).isAdmin;
        tmp.autologin = tmpVect.at(index).autologin;

        _users.push_back(tmp);
    }
    return true;
}

bool Users::updatePass(char username[USER_USERNAME_SIZE], char newPass[USER_PASS_SIZE])
{
    // Scan user vector and check if the username is in vector
    for (uint8_t index = 0; index < _users.size(); index++)
    {
        if (strcmp(username, _users.at(index).username) == 0)
        {
            sprintf(_buffer, "User: %32s found. Updating pass: %32s with new pass: %32s",
                    _users.at(index).username, _users.at(index).pass, newPass);
            _dbg(_buffer);

            memcpy(_users.at(index).pass, newPass, USER_PASS_SIZE);

            return true;
        }
    }

    sprintf(_buffer, "User: %32s not found.", username);
    _dbg(_buffer);
    return false;
}

bool Users::updateIsAdmin(char username[USER_USERNAME_SIZE], bool newIsAdmin)
{
    // Scan user vector and check if the username is in vector
    for (uint8_t index = 0; index < _users.size(); index++)
    {
        if (strcmp(username, _users.at(index).username) == 0)
        {
            sprintf(_buffer, "User: %32s found. Updating isAdmin: %1d with new isAdmin: %1d",
                    _users.at(index).username, _users.at(index).isAdmin, newIsAdmin);
            _dbg(_buffer);

            _users.at(index).isAdmin = newIsAdmin;

            return true;
        }
    }

    sprintf(_buffer, "User: %32s not found.", username);
    _dbg(_buffer);
    return false;
}

bool Users::updateAutologin(char username[USER_USERNAME_SIZE], bool newAutologin)
{
    // Scan user vector and check if the username is in vector
    for (uint8_t index = 0; index < _users.size(); index++)
    {
        if (strcmp(username, _users.at(index).username) == 0)
        {
            sprintf(_buffer, "User: %32s found. Updating ALogin: %1d with new ALogin: %1d",
                    _users.at(index).username, _users.at(index).autologin, newAutologin);
            _dbg(_buffer);

            _users.at(index).autologin = newAutologin;

            return true;
        }
    }

    sprintf(_buffer, "User: %32s not found.", username);
    _dbg(_buffer);
    return false;
}

userStruct_t Users::getUser(char username[USER_USERNAME_SIZE])
{
    userStruct_t res;
    // Scan sensor vector and check if the username is in vector
    for (uint8_t index = 0; index < _users.size(); index++)
    {
        if (strcmp(username, _users.at(index).username) == 0)
        {
            memcpy(res.username, _users.at(index).username, USER_USERNAME_SIZE);
            memcpy(res.pass, _users.at(index).pass, USER_PASS_SIZE);
            res.isAdmin = _users.at(index).isAdmin;
            res.autologin = _users.at(index).autologin;

            sprintf(_buffer, "Found user struct | User:%32s | Pass:%32s | Is admin:%1u | ALogin:%1u",
                    res.username, res.pass, res.isAdmin, res.autologin);
            _dbg(_buffer);

            return res;
        }
    }

    memcpy(res.username, USER_STRUCT_USERNAME_NOT_FOUND, USER_USERNAME_SIZE);
    memcpy(res.pass, USER_STRUCT_PASS_NOT_FOUND, USER_PASS_SIZE);
    res.isAdmin = USER_STRUCT_IS_ADMIN_NOT_FOUND;
    res.autologin = USER_STRUCT_ALOGIN_NOT_FOUND;

    return res;
}

String Users::login(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE])
{
    String res = String();

    sprintf(_buffer, "Login | User: %32s | Pass: %32s", username, pass);
    _dbg(_buffer);

    for (uint8_t index = 0; index < _users.size(); index++)
    {
        if (strcmp(username, _users[index].username) == 0) // check if the user in present
        {
            if (strcmp(pass, _users[index].pass) == 0) // check pass
            {
                if (index == _indexWorkingUser) // check if user is already logged in
                {
                    res = "User already logged in";
                    break;
                }
                else
                {
                    res = "Done";
                    break;
                }
            }
            else // wrong pass
            {
                res = "Wrong password";
                break;
            }
        }
        else // user not found
        {
            if (index == _users.size() - 1)
                res = "Username not found in flash";
        }
    }
    return res;
}

bool Users::setWorkingUser(char username[USER_USERNAME_SIZE])
{
    bool res = false;

    for (uint8_t index = 0; index < _users.size(); index++)
    {
        if (strcmp(username, _users[index].username) == 0)
        {
            _indexWorkingUser = index;
            _dbg("Working user set: " + String(_users[_indexWorkingUser].username));
            res = true;

            if (_indexDefaultWorkingUser >= 0 && _indexDefaultWorkingUser != _indexWorkingUser) // indexDefaultWorkingUser = null
            {
                _indexDefaultWorkingUser = -1;
                _users[index].autologin = false;

                // _storeUsersInFlash(_users);
            }

            break;
        }
    }
    if (!res)
        _dbg("Working user to set not found");

    return res;
}

userStruct_t Users::getWorkingUser()
{
    userStruct_t res;

    if (_indexWorkingUser < 0)
    {
        memcpy(res.username, USER_STRUCT_USERNAME_NOT_FOUND, USER_USERNAME_SIZE);
        memcpy(res.pass, USER_STRUCT_PASS_NOT_FOUND, USER_PASS_SIZE);
        res.isAdmin = USER_STRUCT_IS_ADMIN_NOT_FOUND;
        res.autologin = USER_STRUCT_ALOGIN_NOT_FOUND;
    }
    else
    {
        memcpy(res.username, _users[_indexWorkingUser].username, USER_USERNAME_SIZE);
        memcpy(res.pass, _users[_indexWorkingUser].pass, USER_PASS_SIZE);
        res.isAdmin = _users[_indexWorkingUser].isAdmin;
        res.autologin = _users[_indexWorkingUser].autologin;
    }

    _dbg("Working user: " + String(res.username));

    return res;
}

bool Users::setDefaultWorkingUser(char username[USER_USERNAME_SIZE])
{
    bool res = false;
    for (uint8_t index = 0; index < _users.size(); index++)
    {
        if (strcmp(username, _users[index].username) == 0)
        {
            if (_indexDefaultWorkingUser == index && _users[index].autologin == true) // default working user already set
                return true;

            _dbg("Index default: " + String(_indexDefaultWorkingUser));

            _indexWorkingUser = index;
            _indexDefaultWorkingUser = index;
            _users[_indexDefaultWorkingUser].autologin = true;
            _dbg(String(_users[_indexDefaultWorkingUser].username) + " set as default user");

            res = true;
        }
        else
        {
            _users[index].autologin = false;
        }
    }

    _dbg("index default working user stored: " + String(_indexDefaultWorkingUser));

    return res;
}

userStruct_t Users::getDefaultWorkingUser()
{
    userStruct_t res;

    if (_indexDefaultWorkingUser < 0)
    {
        memcpy(res.username, USER_STRUCT_USERNAME_NOT_FOUND, USER_USERNAME_SIZE);
        memcpy(res.pass, USER_STRUCT_PASS_NOT_FOUND, USER_PASS_SIZE);
        res.isAdmin = USER_STRUCT_IS_ADMIN_NOT_FOUND;
        res.autologin = USER_STRUCT_ALOGIN_NOT_FOUND;
    }
    else
    {
        memcpy(res.username, _users[_indexDefaultWorkingUser].username, USER_USERNAME_SIZE);
        memcpy(res.pass, _users[_indexDefaultWorkingUser].pass, USER_PASS_SIZE);
        res.isAdmin = _users[_indexDefaultWorkingUser].isAdmin;
        res.autologin = _users[_indexDefaultWorkingUser].autologin;
    }

    _dbg("Default working user: " + String(res.username));

    return res;
}

void Users::printUsers()
{
    _dbg("\nPRINTING USERS", 0);
    if (_indexWorkingUser > 0)
        _dbg("Working user: " + String(_users.at(_indexWorkingUser).username), 0);

    for (uint8_t index = 0; index < _users.size(); index++)
    {
        sprintf(_buffer, "User: %32s | Pass: %32s | Is admin: %1u | ALogin: %1u",
                _users.at(index).username, _users.at(index).pass, _users.at(index).isAdmin, _users.at(index).autologin);
        _dbg(_buffer, 0);
    }
}

DynamicJsonDocument Users::vectToJson()
{
    DynamicJsonDocument json(CONFIG_USER_JSON_SIZE);

    String str;

    json[K_USER_NO] = _users.size() + 1;
    JsonArray userInfoObj = json.createNestedArray("vect");

    for (uint8_t index = 0; index < _users.size(); index++)
    {
        JsonObject userInfoArr = userInfoObj.createNestedObject();

        userInfoArr[K_USER_USERNAME] = _users.at(index).username;
        userInfoArr[K_USER_PASS] = _users.at(index).pass;
        userInfoArr[K_USER_ADMIN] = _users.at(index).isAdmin;
        userInfoArr[K_USER_AL] = _users.at(index).autologin;
    }

    serializeJsonPretty(json, str);
    return json;
}

void Users::_dbg(String str, bool header)
{
    if (_dbgOn)
    {
        if(header)
        Serial.println("[USER]\t" + str);
        else
        Serial.println(str);
    }
}
