#include "tgs_server.h"

TGS_server::TGS_server(user u)
{
    vector<string> tgs_ids;
    tgs_ids.push_back("tgs1");

    service_server.insert(make_pair("a",user("a",hasher.generateHash("asdadsasd"),tgs_ids)));
    service_server.insert(make_pair("b",user("b",hasher.generateHash("asdadsasd"),tgs_ids)));
    service_server.insert(make_pair("c",user("c",hasher.generateHash("asdadasdas"),tgs_ids)));

    this->u = u;
}

user TGS_server::get_user(string id)
{
    return service_server[id];
}

string TGS_server::request(string packet)
{
    char* tmp = (char*)packet.c_str();
    string id_v = strtok(tmp, "-");
    string en_ticket = strtok(NULL, "-");
    string en_auth_c = strtok(NULL, "-");
    string is_triple_str = strtok(NULL, "-");
    bool is_triple = (is_triple_str == "T") ? true : false;

    string ktgs = u.get_password().substr(0,(is_triple ? 24 : 8));
    string de_ticket = __des__func(en_ticket, ktgs, true, is_triple);

    tmp = (char*)de_ticket.c_str();
    string key_c_tgs = strtok(tmp, "-");
    string id_c = strtok(NULL, "-");
    string AD_c = strtok(NULL, "-");
    string id_tgs = strtok(NULL, "-");
    string ts2_str = strtok(NULL, "-");
    string life_time_str = strtok(NULL, "-");

    string de_auth_c = __des__func(en_auth_c, key_c_tgs, true, is_triple);
    tmp = (char*)de_auth_c.c_str();
    string id_c_auth = strtok(tmp, "-");
    string AD_c_auth = strtok(NULL, "-"); 
    string ts3_str = strtok(NULL, "-");

    std::string::size_type sz; 
    long ts2 = std::stol (ts2_str,&sz);
    long ts3 = std::stol (ts3_str,&sz);
    long life_time = std::stol (life_time_str,&sz);
    
    if(id_c != id_c_auth || AD_c_auth != AD_c || id_tgs != u.get_username() || ts2 > ts3 || (ts2+life_time) < unix_timestamp()){
        return "";
    }

    string key_c_v=rand_key_gen(is_triple);
    
    long ts4 = unix_timestamp();
    string ts4_str;

    stringstream strstream;
    strstream << ts4;
    strstream >> ts4_str;


    string kv = service_server[id_v].get_password().substr(0,(is_triple ? 24 : 8));
    string ticket_content = key_c_v+"-"+id_c+"-"+AD_c+"-"+id_v+"-"+ts4_str+"-"+LIFE_TIME;
    string en_ticket_v = __des__func(ticket_content, kv, false, is_triple);
    tickets[id_c] = ticket(ts4, 128, id_v, id_tgs, id_c, en_ticket_v, key_c_v);

    string response_content = key_c_v+"-"+id_v+"-"+ts4_str+"-"+en_ticket_v;
    string reponse = __des__func(response_content, key_c_tgs, false, is_triple);
    return reponse;
}