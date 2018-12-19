#include "ticket.h"

using namespace std;

ticket::ticket(){}
ticket::ticket(long ts2, long life_time, string owner, string exporter, string user, string t, string key)
{
    this->ts2 = ts2;
    this->life_time = life_time;
    this->owner = owner;
    this->exporter = exporter;
    this->user = user;
    this->t = t;
    this->key = key;
}

bool ticket::is_ticket_valid()
{
    return ((ts2 + life_time*1000) > unix_timestamp());
}

string ticket::get_ticket()
{
    return t;
}

string ticket::get_owner()
{
    return owner;
}

string ticket::get_key()
{
    return key;
}