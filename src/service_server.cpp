#include "service_server.h"

service_server::service_server(user u)
{
    this->u = u;
}

string service_server::request(string packet)
{
    char* tmp = (char*)packet.c_str();
    string en_ticket = strtok(tmp, "-");
    string en_auth_c = strtok(NULL, "-");
    string is_triple_str = strtok(NULL, "-");
    bool is_triple = (is_triple_str == "T") ? true : false;

    string kv = u.get_password().substr(0,(is_triple ? 24 : 8));
    string de_ticket = __des__func(en_ticket, kv, true, is_triple);

    tmp = (char*)de_ticket.c_str();
    string key_c_v = strtok(tmp, "-");
    string id_c = strtok(NULL, "-");
    string AD_c = strtok(NULL, "-");
    string id_v = strtok(NULL, "-");
    string ts4_str = strtok(NULL, "-");
    string life_time_str = strtok(NULL, "-");

    string de_auth_c = __des__func(en_auth_c, key_c_v, true, is_triple);
    tmp = (char*)de_auth_c.c_str();
    string id_c_auth = strtok(tmp, "-");
    string AD_c_auth = strtok(NULL, "-"); 
    string ts5_str = strtok(NULL, "-");
    
    std::string::size_type sz;
    long ts4 = std::stol (ts4_str,&sz);
    long ts5 = std::stol (ts5_str,&sz);
    long life_time = std::stol (life_time_str,&sz);
    
    if(id_c != id_c_auth || AD_c_auth != AD_c || id_v != u.get_username() || ts4 > ts5 || (ts4+life_time) < unix_timestamp()){
        return "";
    }

    tickets[id_c] = ticket(ts4, life_time, id_v, "", id_c, en_ticket, key_c_v);

    ts5++;
    
    stringstream strstream;
    strstream << ts5;
    strstream >> ts5_str;

    string response_content = ts5_str;
    string reponse = __des__func(response_content, key_c_v, false, is_triple);
    return reponse;
}

string service_server::get_status(string packet)
{
    char* tmp = (char*)packet.c_str();
    string id_c = strtok(tmp, "-");
    string en_req = strtok(NULL, "-");

    ticket t = tickets[id_c];
    if(!t.is_ticket_valid())
    {
        return "";
    }

    bool is_triple = (t.get_key().length() == 24) ? true : false;

    string req = __des__func(en_req, t.get_key(), true, is_triple);
    if(req == "salam")
    {
        return __des__func("salam! chetor?", t.get_key(), false, is_triple);
    }

    return "you are not valid user.";

}