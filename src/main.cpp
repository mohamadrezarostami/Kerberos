#include <iostream>
#include <vector>
#include "tests.h"
#include "utils.h"
#include "rsa.h"
#include "sha256.h"
#include "ticket.h"
#include "as_server.h"
#include "tgs_server.h"
#include "utils.h"
#include "user.h"
#include "service_server.h"
#include "client.h"

using namespace std;


void test_convertions()
{
    string a = "salamHaj";
    ui64 t = convert_8char_to_ui64(&a,0);
    string out = convert_ui64_to_string(t);
    if( a!= out){
        cout<<"test_convertions:"<<endl;
        cout<<"out: "<<out<<" in: "<<a<<endl;
    }
}

void __SHA256__TEST()
{
    string input = "Hello World!";
    string expect_output = "7f83b1657ff1fc53b92dc18148a1d65dfc2d4b1fa3d677284addd200126d9069";
    string output;


    sha256 sha;
    output = sha.generateHash(input);
    
    if(output != expect_output)
    {
        cout<<"Error in SHA-256"<<endl;
    }
    else
    {
        cout<<"SHA-256 PASS"<<endl;
    }
}

void __RSA__TEST()
{
    RSA client = RSA(3, 7);
    RSA server = RSA(5, 7);
    uint64_t plain_text = 20;
    uint64_t cipher_text = client.encrypte(plain_text, server.get_public_key(), server.get_n());
    uint64_t de_plain_text = server.decrypte(cipher_text);
    if(de_plain_text != plain_text)
    {
        cout<<"Error in RSA"<<endl;
    }
    else
    {
        cout<<"RSA PASS"<<endl;
    }
}

void __DES__TEST()
{
    string input = "slslslslslsllslsl";
    string key = "salasalasalasalasalasala";
    string output;
    output = __des__func(input,key,false,true);
    output = __des__func(output,key,true,true);
    if(output != input)
    {
        cout<<"Error in DES"<<endl;
    }
    else
    {
        cout<<"DES PASS"<<endl;
    }
}

void __KERBROUS__TEST()
{
    sha256 hasher;
    vector<string> tgs_ids;
    tgs_ids.push_back("tgs1");

    AS_server* as_server = new AS_server();

    TGS_server* tgs_server = new TGS_server(as_server->get_user("tgs1"));

    service_server* a = new service_server(tgs_server->get_user("a"));
    service_server* b = new service_server(tgs_server->get_user("b"));
    service_server* c = new service_server(tgs_server->get_user("c"));

    client* admin = new client(as_server->get_user("admin"));
    client* golabi = new client(as_server->get_user("golabi"));

    admin->connect(as_server, tgs_server, "tgs1", a, "a",true);
}

int main(int argc, char **argv)
{
    test_convertions();
    __RSA__TEST();
    __DES__TEST();
    __SHA256__TEST();
    __KERBROUS__TEST();
    return 0;
}
