#ifndef UTILS_H_
#define UTILS_H_

#define CONCAT3(x1,x2,x3) "temptemptemptemptemptemp"
#define CONCAT1(x) "temptemp"
#define MKSTR(x) #x

#include <ctime>
#include <string>
#include <iostream>
#include "des.h"
#include "des3.h"

const ui64 MASK = 0x00000000000000FF;

void usage();
string convert_ui64_to_string(ui64 in);
string convert_ui64_to_char_string(ui64 in);
uint64_t convert_8char_to_ui64(string* cs,int idx);
string __des__func(string input, string key, bool mode, bool isTriple);
long int unix_timestamp();
string rand_key_gen(bool isTriple);


#endif