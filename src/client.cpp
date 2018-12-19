#include "client.h"

client::client(user u)
{
    this->u = u;
}

ticket* client::get_ticket_from_as(AS_server* as, string id_tgs, bool is_triple)
{

    long ts1 = unix_timestamp();
    string ts1_str;
    stringstream strstream;
    strstream << ts1;
    strstream >> ts1_str;
    string packet = u.get_username()+"-"+id_tgs+"-"+ts1_str+"-"+(is_triple ? "T" : "S");
    string response =  as->request(packet);
    string kc = u.get_password().substr(0,(is_triple ? 24 : 8));
    string d_res = __des__func(response, kc, true, is_triple);
 
    char* tmp = (char*)d_res.c_str(); 
    string key_c_tgs = strtok(tmp, "-");
    string id_tgs_r = strtok(NULL, "-");
    string ts2_str = strtok(NULL, "-");
    string life_time_str = strtok(NULL, "-");
    string en_ticket = strtok(NULL, "-");

    std::string::size_type sz; 
    long ts2 = std::stol (ts2_str,&sz);
    long life_time = std::stol (life_time_str,&sz);
    
    if(ts2 < ts1 || (ts2+life_time*1000) < unix_timestamp())
    {
        cout<<"ERROR in hand shaking with protocol with AS "+id_tgs+" ...."<<endl;
        return NULL;
    }

    ticket* t = new ticket(ts2, life_time, id_tgs_r, "AS",this->u.get_username(), en_ticket, key_c_tgs);
    tickets[id_tgs_r]= *t;
    return t;

}

ticket* client::get_ticket_from_tgs(AS_server* as, TGS_server* tgs, string id_tgs, string id_server, bool is_triple)
{
    ticket tgs_ticket = this->tickets[id_tgs];
    if(!tgs_ticket.is_ticket_valid()){
        cout<<"TGS "+id_tgs+" ticket is not valid"<<endl;
        cout<<"try to get from AS..."<<endl;
        ticket* result = get_ticket_from_as(as, id_tgs, is_triple);
        if(result != NULL)
        {
            tgs_ticket = *result;
            cout<<"got TGS "+tgs_ticket.get_owner()+" ticket from AS..."<<endl;
        }
        else
        {
            cout<<"AS server not responing...."<<endl;
            return NULL;
        }
    }
    string AD_C="ADC";
    long ts3 = unix_timestamp();
    string ts3_str;
    stringstream strstream;
    strstream << ts3;
    strstream >> ts3_str;

    string auth_palin = u.get_username()+"-"+AD_C+"-"+ts3_str;
    string en_auth = __des__func(auth_palin, tgs_ticket.get_key(), false, is_triple);

    string packet = id_server+"-"+tgs_ticket.get_ticket()+"-"+en_auth+"-"+(is_triple ? "T" : "S");
    string response =  tgs->request(packet);

    if(response == "")
    {
        cout<<"server "+id_tgs+" server not responing...."<<endl;
        return NULL;
    }
    string d_res = __des__func(response, tgs_ticket.get_key(), true, is_triple);

    char* tmp = (char*)d_res.c_str();
    string key_c_v = strtok(tmp, "-");
    string id_v = strtok(NULL, "-");
    string ts4_str = strtok(NULL, "-");
    string en_ticket_v = strtok(NULL, "-");

    std::string::size_type sz; 
    long ts4 = std::stol (ts4_str,&sz);
    
    if(ts4 < ts3)
    {
        cout<<"ERROR in hand shaking with protocol with TGS"+id_tgs+" ...."<<endl;
        return NULL;
    }

    ticket* t = new ticket(ts4, 128, id_v, id_tgs,this->u.get_username(), en_ticket_v, key_c_v);

    tickets[id_v]= *t;
    return t;
}

bool client::connect(AS_server* as,TGS_server* tgs, string id_tgs, service_server* server, string id_server, bool is_triple)
{
    ticket server_ticket = this->tickets[id_server];
    if(!server_ticket.is_ticket_valid()){
        cout<<"server "+id_server+" ticket is not valid"<<endl;
        cout<<"try to get from tgs..."<<endl;
        ticket* result = get_ticket_from_tgs(as, tgs, id_tgs, id_server, is_triple);
        if(result != NULL)
        {
            server_ticket = *result;
            cout<<"got server "+server_ticket.get_owner()+" ticket from TGS..."<<endl;
        }
        else
        {
            cout<<"TGS server not responing...."<<endl;
            return false;
        }
    }
    else
    {
        cout<<"find ticket "+server_ticket.get_owner()<<endl;
    }

    string AD_C="ADC";
    long ts5 = unix_timestamp();
    string ts5_str;
    stringstream strstream;
    strstream << ts5;
    strstream >> ts5_str;
    
    string auth_palin = u.get_username()+"-"+AD_C+"-"+ts5_str;
    string en_auth = __des__func(auth_palin, server_ticket.get_key(), false, is_triple);
    string packet = server_ticket.get_ticket()+"-"+en_auth+"-"+(is_triple ? "T" : "S");
    string response =  server->request(packet);
    if(response == "" )
    {
        cout<<"server "+id_server+" server not responing...."<<endl;
        return false;
    }

    string d_res = __des__func(response, server_ticket.get_key(), true, is_triple);

    char* tmp = (char*)d_res.c_str();
    string ts5_1_str = strtok(tmp, "-");

    std::string::size_type sz; 
    long ts5_1 = std::stol (ts5_1_str,&sz);
    
    if(ts5+1 != ts5_1)
    {
        cout<<"ERROR: not get good response from server"+id_server+" ...."<<endl;
        return false;
    }
    cout<<"connect to server "+id_server+" ...."<<endl;

    string get_status_p = "salam";
    cout<<"Request to server "+id_server+":\""+get_status_p+"\"...."<<endl;

    string req_en = u.get_username()+"-"+__des__func(get_status_p, server_ticket.get_key(), false, is_triple);
    cout<<"Encrypted Request to server "+id_server+":\""+req_en+"\"...."<<endl;

    string response_en = server->get_status(req_en);
    cout<<"Encrypted Response from server "+id_server+":\""+response_en+"\"...."<<endl;

    string req_de = __des__func(response_en, server_ticket.get_key(), true, is_triple);
    cout<<"Response from server "+id_server+":\""+req_de+"\"...."<<endl;

    return true;
}