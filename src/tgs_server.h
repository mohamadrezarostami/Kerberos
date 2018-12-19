#ifndef TGS_SERVER_H_
#define TGS_SERVER_H_

#include <string>
#include <map>
#include <sstream>
#include "user.h"
#include "sha256.h"
#include "ticket.h"
#include "utils.h"

using namespace std;

class TGS_server
{
public:
    TGS_server(user u);
    string request(string packet);
    user get_user(string id);
private:
    map<string, user> service_server;
    map<string, ticket> tickets;
    sha256 hasher;
    user u;
};

#endif