#ifndef AS_SERVER_H_
#define AS_SERVER_H_

#include <string>
#include <map>
#include "user.h"
#include "sha256.h"
#include "ticket.h"
#include "utils.h"
#include <sstream>

using namespace std;

class AS_server
{
public:
    AS_server();
    string request(string packet);
    user get_user(string id);
private:
    map<string, user> users;
    map<string, ticket> tickets;
    sha256 hasher;
};

#endif