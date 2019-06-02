//
// Created by 李文博 on 2018-12-20.
//
#include <string>

using namespace std;
#ifndef CRYPT_COMMON_H
#define CRYPT_COMMON_H


//模n快速幂
long long quick_mod(long long base ,long long m ,long long mod);
int quick_pow(int base ,int m ,int mod);

//模n求逆
int anti_mod(int u,int mod);
//long long inv(long long a,long long mod);

//判断两数互质
bool is_coprime(long long x,long long y);

//Miller-Rabin
bool Miller_rabin(int x,int times);

void txt_to_dat(string txt,string dat);
void dat_to_txt(string dat,string txt);

#endif //CRYPT_COMMON_H
