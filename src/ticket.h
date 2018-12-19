#ifndef TICKET_H_
#define TICKET_H_

#include <string>
#include "utils.h"

#define LIFE_TIME "128"

using namespace std;

class ticket
{
public:
    ticket();
    ticket(long ts2, long life_time, string owner, string exporter, string user, string t, string key);
    bool is_ticket_valid();
    string get_ticket();
    string get_owner();
    string get_key();
private:
    long ts2;
    long life_time;
    string owner;
    string exporter;
    string user;
    string t;
    string key;

};

#endif