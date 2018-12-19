#include "user.h"

user::user(){}
user::user(string username, string password, vector<string> tgs_ids)
{
    this->username = username;
    this->password = password;
    this->tgs_ids = tgs_ids;
}

    