#include "utils.h"

using namespace std;

long int unix_timestamp()
{
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}
void usage()
{
    cout << "Usage: DES -e/-d -s/-t key input" << endl;
    cout << "-e : encryption" << endl;
    cout << "-d : decryption" << endl;
    cout << "-s : simple(64 bit key-ascii)" << endl;
    cout << "-t : triple-DES(192 bit key-ascii)" << endl;
}
string convert_ui64_to_string(ui64 in)
{
    string result = "";
    ui64 tmp = 0;
    for(int i=7; i>=0; i--)
    {
        tmp = in & (MASK << (8*i));
        char c = (char) (tmp >> (8*i));
        result += c;
    }
    return result;
}
string convert_ui64_to_char_string(ui64 in)
{
    string result = "";
    ui64 tmp = 0;
    for(int i=7; i>=0; i--)
    {
        tmp = in & (MASK << (8*i));
        char c = (char) (tmp >> (8*i));
        if(c != 0)
            result += c;
    }
    return result;
}
uint64_t convert_8char_to_ui64(string* cs,int idx)
{
    uint64_t result=0;
    uint64_t tmp=0;
    uint64_t padding_check = (*cs).length()-idx;
    ui8 r_size= 8;
    if(padding_check < 8)
    {
        r_size = padding_check;
    }
    for(int i=idx ; i < (r_size + idx) ; i++)
    {
        tmp = (unsigned char)(*cs)[i];
        tmp <<= (8*(7-i-idx));
        result += tmp;
    }
    return result;
}
string __des__func(string input, string key, bool mode, bool isTriple)
{
    DES des;
    DES3 des3;
    string result="";

    if(isTriple && key.length() == 24)
    {
        ui64 key_hex1 = convert_8char_to_ui64(&key,0);
        ui64 key_hex2 = convert_8char_to_ui64(&key,8);
        ui64 key_hex3 = convert_8char_to_ui64(&key,16);
        des3 = DES3(key_hex1, key_hex2, key_hex3);
    }
    else if(!isTriple && key.length() == 8)
    {
        ui64 key_hex = convert_8char_to_ui64(&key,0);
        des = DES(key_hex);
    }
    else
    {
        usage();
        return NULL;
    }

    ui64 buffer;
    ui64 size = input.length();
    ui64 block = size / 8;

    if(mode) block--;

    for(ui64 i = 0; i < block; i++)
    {
        buffer = convert_8char_to_ui64(&input, i * 8);

        if(mode)
            buffer = (isTriple) ? des3.decrypt(buffer) : des.decrypt(buffer);
        else
            buffer = (isTriple) ? des3.encrypt(buffer) : des.encrypt(buffer);
        result += convert_ui64_to_string(buffer);
    }

    if(mode == false)
    {
        // Amount of padding needed
        ui8 padding = 8 - (size % 8);

        // Padding cannot be 0 (pad full block)
        if (padding == 0)
            padding  = 8;

        // Read remaining part of file
        buffer = (ui64) 0;
        if(padding != 8)
            buffer = convert_8char_to_ui64(&input, block * 8);

        // Pad block with a 1 followed by 0s
        ui8 shift = padding * 8;
        buffer  |= (ui64) 0x0000000000000001 << (shift - 1);

        buffer = (isTriple) ? des3.encrypt(buffer) : des.encrypt(buffer);
        result += convert_ui64_to_string(buffer);
    }
    else
    {
        // Read last line of file
        buffer = convert_8char_to_ui64(&input, block * 8);
        buffer = (isTriple) ? des3.decrypt(buffer) :des.decrypt(buffer);
        // Amount of padding on file
        ui8 padding = 0;

        // Check for and record padding on end
        while(!(buffer & 0x00000000000000ff))
        {
            buffer >>= 8;
            padding++;
        }

        buffer >>= 8;
        padding++;

        if(padding != 8)
            result += convert_ui64_to_char_string(buffer);
    }
    return result;
}
uint64_t rand_uint64_slow(void)
{
  uint64_t r = 0;
  for (int i=0; i<64; i++) {
    r = r*2 + rand()%2;
  }
  return r;
}

string rand_key_gen(bool isTriple)
{
    if(isTriple)
    {
        ui64 k1 =rand_uint64_slow();
        ui64 k2 =rand_uint64_slow();
        ui64 k3 =rand_uint64_slow();
        return CONCAT3(k1,k2,k3);
    }
    else
    {
        ui64 k1 =rand_uint64_slow();
        return CONCAT1(k1);
    }
}