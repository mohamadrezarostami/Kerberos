#include "rsa.h"

int gcd(int a, int h)
{
    int temp;
    while(1)
    {
        temp = a%h;
        if(temp==0)
        return h;
        a = h;
        h = temp;
    }
}

uint64_t RSA::encrypte(uint64_t mtext, uint64_t dest_public_key,uint64_t dest_n)
{
    uint64_t c = pow(mtext,dest_public_key);
    c=fmod(c,dest_n);
    return c;
}

uint64_t RSA::decrypte(uint64_t ctext)
{
    uint64_t m = pow(ctext,d);
    m=fmod(m,n);
    return m;
}

uint64_t RSA::get_n()
{
    return n;
}

uint64_t RSA::get_private_key()
{
    return d;
}

uint64_t RSA::get_public_key()
{
    return e;
}
RSA::RSA(uint64_t p, uint64_t q)
{
    this->p = p;
    this->q = q;

    this->n = p*q;
    uint64_t totient = (p-1)*(q-1);
 
    //public key
    this->e=2;
    uint64_t count;
    while(this->e < totient)
    {
        count = gcd(this->e, totient);
        if(count==1)
            break;
        else
            this->e++;
    }
 
    
    uint64_t k=2;
    //private key
    this->d = (1 + (k*totient))/this->e;
}