#include "as_server.h"

using namespace std;

AS_server::AS_server()
{
    vector<string> tgs_ids;
    tgs_ids.push_back("tgs1");

    users.insert(make_pair("admin",user("admin",hasher.generateHash("sdfsvsdv"),tgs_ids)));
    users.insert(make_pair("golabi",user("golabi",hasher.generateHash("asdfgghsd"),tgs_ids)));
    users.insert(make_pair("tgs1",user("tgs1",hasher.generateHash("sdfsfdsdf"),tgs_ids)));
}

user AS_server::get_user(string id)
{
    return users[id];
}

string AS_server::request(string packet)
{
    char* tmp = (char*)packet.c_str();
    string id_c = strtok(tmp, "-");
    string id_tgs = strtok(NULL, "-");
    string ts1 = strtok(NULL, "-");
    string is_triple_str = strtok(NULL, "-");
    bool is_triple = (is_triple_str == "T") ? true : false;

    //generate key
    string key_c_tgs = rand_key_gen(is_triple);
    string AD_C="ADC";
    long ts2 = unix_timestamp();
    string ts2_str;
    stringstream strstream;
    strstream << ts2;
    strstream >> ts2_str;
    //generate key
    string ktgs = users[id_tgs].get_password().substr(0,(is_triple ? 24 : 8));
    string ticket_content = key_c_tgs+"-"+id_c+"-"+AD_C+"-"+id_tgs+"-"+ts2_str+"-"+LIFE_TIME;
    string en_ticket = __des__func(ticket_content, ktgs, false, is_triple);
    tickets[id_c] = ticket(ts2, 128, id_tgs, "AS",id_c, en_ticket, key_c_tgs);

    string kc = users[id_c].get_password().substr(0,(is_triple ? 24 : 8));
    string response_content = key_c_tgs+"-"+id_tgs+"-"+ts2_str+"-"+LIFE_TIME+"-"+en_ticket;
    string reponse = __des__func(response_content, kc, false, is_triple);
    return reponse;
}