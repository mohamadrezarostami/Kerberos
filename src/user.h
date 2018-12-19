#ifndef USER_H_
#define USER_H_

#include <string>
#include <vector>
#include "sha256.h"

using namespace std;

class user
{
public:
    user();
    user(string username, string password, vector<string> tgs_ids);
    string get_username(){return username;}
    string get_password(){return password;}
    string get_valid_tgs(){return tgs_ids[0];}
private:
    string username;
    string password;
    vector<string> tgs_ids;
};

#endif