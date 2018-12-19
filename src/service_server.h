#ifndef SERVICE_SERVER_H_
#define SERVICE_SERVER_H_

#include <string>
#include <map>
#include <sstream>
#include "user.h"
#include "sha256.h"
#include "ticket.h"
#include "utils.h"

using namespace std;

class service_server
{
public:
    service_server(user u);
    string request(string packet);
    string get_status(string packet);
private:    
    map<string, ticket> tickets;
    sha256 hasher;
    user u;
};

#endif