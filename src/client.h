#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <map>
#include "ticket.h"
#include "as_server.h"
#include "tgs_server.h"
#include "utils.h"
#include "user.h"
#include "service_server.h"

using namespace std;

class client
{
private:
    map<string, ticket> tickets;
    user u;
    ticket* get_ticket_from_as(AS_server* as, string id_tgs,bool is_triple);
    ticket* get_ticket_from_tgs(AS_server* as, TGS_server* tgs, string id_tgs, string id_server,bool is_triple);
public:
    client(user u);
    bool connect(AS_server* as,TGS_server* tgs, string id_tgs, service_server* server, string id_server,bool is_triple);
    
};

#endif