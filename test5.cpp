//
// Created by 李文博 on 2018-12-23.
//

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include "common.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>

using namespace std;
using namespace NTL;

int SEED = 5125735;
string P = "36D953ECAE4F281DF5A5D0FF3CAD6371F76D5C29B6D953ECAE4F281DF5A5D0FF3CAD6371F76D5C29B6D95470416909F306C5D9EF3A5C7F054B552561B4974CD1";
string Q = "AE4F281DF5A5D0FF3CAD6371F76D5C29B6D953FF";


ZZ p,q,g,x,y,r,s,Hm,M;
unsigned int A,AA,B,BB,C,CC,D,DD,E,EE,k1,k2,k3,k4;
unsigned int X[80];


ZZ PowMod_ZZ(ZZ x,ZZ r,ZZ n){
    ZZ a = x,b = r;
    ZZ c;
    c = 1;
    while(b!=0){
        while((b&0x1)==0){
            b>>=1;
            a = MulMod(a,a,n);
        }
        b = b-1;
        c = MulMod(c,a,n);
    }
    return c;
}

int char2int(char c){
    if(c<='9'&&c>='0'){
        return c-'0';
    } else if(c>='a'&&c<='f'){
        return c-'a'+10;
    } else{
        return c-'A'+10;
    }
}

bool Miller_Rabin(ZZ n){

    if(n<2)return false;
    if(n == 2 || n == 3 || n == 5 || n == 7)return true;
    if(n%2==0 || n%3==0 || n%5==0 || n%7==0)return false;
    int i=0;
    while(i!=50){
        ZZ temp = RandomBnd(n);
        if(MillerWitness(n,temp)==1)
            return false;
        i++;
    }
    return true;
}

void Set_ZZ(ZZ *p,string str){
    int length = str.length();
    *p = char2int(str[0]);
    for(int i = 1;i<=length-1;i++){
        *p <<= 4;
        *p += char2int(str[i]);
    }
}

ZZ Pollard_rho(ZZ n,ZZ c) {
    ZZ x, y, d;
    x = 2, y = 2, d = 1;
    while (d == 1) {
        x = MulMod(x, x, n) + c;
        y = MulMod(y, y, n) + c;
        y = MulMod(y, y, n) + c;
        d = GCD(abs(x - y), n);
    }
    if (d == n) {
        return Pollard_rho(n, c + 1);
    }
    return d;
}

void find_factor(ZZ n){
    if(n == 1)return;
    if(Miller_Rabin(n)){
        if(NumBits(n)==160)q = n;
        cout<<n<<endl;
        return;
    }
    ZZ temp;
    ZZ one;
    one = 1;
    temp = Pollard_rho(n,one);
    find_factor(temp);
    find_factor(n/temp);
}

void Set_DSA(){
    //设置随机种子
    ZZ ranseed;
    ranseed<<8;
    ranseed|=SEED;
    SetSeed(ranseed);

    //p = RandomPrime_ZZ(512,10);
    Set_ZZ(&p,P);
    cout<<"p: "<<p<<endl;

    //find q
    Set_ZZ(&q,Q);
    //find_factor(p-1);
    cout<<"q: "<<q<<endl;

    ZZ h = RandomBnd(p-1);
    while(PowMod_ZZ(h,(p-1)/q,p)<=1)h = RandomBnd(p-1);
    g = PowMod_ZZ(h,(p-1)/q,p);
    cout<<"g: "<<g<<endl;

    x = RandomBnd(q);
    cout<<"x: "<<x<<endl;

    y = PowMod_ZZ(g,x,p);
    cout<<"y: "<<y<<endl;
}

void Creat_M(){
    ifstream file;
    long length=0;
    file.open("data/5/text.txt",ios::in);
    file>>noskipws;
    M = 0;
    char c;
    while(!file.eof()){
        file>>c;
        if(file.eof())break;
        M += c;
        M <<= 8;
        length+=8;
    }
    M>>=7;
    M+=1;
    M<<=SubMod(448,length+1,512);
    M<<=64;
    M+=length;
}

void Set_SHA(){
    A = 0x67452301;
    B = 0xefcdab89;
    C = 0x98badcfe;
    D = 0x10325476;
    E = 0xc3d2e1f0;
    k1 = 0x5a827999;
    k2 = 0x6ed9eba1;
    k3 = 0x8f1bbcdc;
    k4 = 0xca62c1d6;
}

void Round1(){
    for(int k = 0;k < 20;k++) {
        unsigned temp = (A << 5) + ((B & C) | (~B & D)) + E + X[k] + k1;
        E = D;
        D = C;
        C = (B << 30);
        B = A;
        A = temp;
    }
}

void Round2(){
    for(int k = 20;k < 40;k++) {
        unsigned temp = (A << 5) + B^C^D + E + X[k] + k2;
        E = D;
        D = C;
        C = (B << 30);
        B = A;
        A = temp;
    }
}

void Round3(){
    for(int k = 40;k < 60;k++) {
        unsigned temp = (A << 5) + (B&C)|(B&D)|(C&D) + E + X[k] + k2;
        E = D;
        D = C;
        C = (B << 30);
        B = A;
        A = temp;
    }
}

void Round4(){
    for(int k = 60;k < 80;k++) {
        unsigned temp = (A << 5) + B^C^D + E + X[k] + k2;
        E = D;
        D = C;
        C = (B << 30);
        B = A;
        A = temp;
    }
}

void SHA(){
    //Set_ZZ(&M,temp);
    int numi;
    if(NumBits(M)%512==0){
        numi = NumBits(M)/512;
    }
    else{
        numi = NumBits(M)/512 + 1;
    }
    ZZ MOD,temp;
    MOD = 1;
    MOD<<=32;
    for(int i = 0;i < numi;i++){

        for(int j = 15;j >= 0;j--){
            X[j] = 0;
            temp = M%MOD;
            unsigned int b = 1;
            for(int k = 0;k<32;k++){
                if(temp%2==1)X[j]+=b;
                temp/=2;
                b*=2;
            }
            M>>=32;
        }

        for(int j=16;j<80;j++){
            X[j] = (X[j-3]^X[j-8]^X[j-14]^X[j-16])<<1;
        }

        AA = A;
        BB = B;
        CC = C;
        DD = D;
        EE = E;

        Round1();
        Round2();
        Round3();
        Round4();

        A = A + AA;
        B = B + BB;
        C = C + CC;
        D = D + DD;
        E = E + EE;
    }
    Hm = A;
    Hm <<= 32;
    Hm += B;
    Hm <<= 32;
    Hm += C;
    Hm <<= 32;
    Hm += D;
    Hm <<= 32;
    Hm += E;
    //cout<<Hm<<endl;
}

void Alice(){
    //设置随机种子
    cout<<endl<<"It is Alice's turn"<<endl;

    ZZ k = RandomBnd(q);
    cout<<"Alice k:"<<k<<endl;

    r = PowMod_ZZ(g,k,p)%q;
    cout<<"Alice r:"<<r<<endl;

    s = (InvMod(k,q) * (Hm + x*r)) %q;
    cout<<"Alice s:"<<s<<endl;
}

void Bob(){
    cout<<endl<<"It is Bob's turn"<<endl;
    ZZ w;
    w = InvMod(s,q) % q;
    cout<<"Bob w:"<<w<<endl;

    ZZ u1,u2;
    u1 = (Hm * w) % q;
    cout<<"Bob u1:"<<u1<<endl;

    u2 = (r * w) % q;
    cout<<"Bob u2:"<<u2<<endl;

    ZZ tempresult;
    tempresult = MulMod(PowMod_ZZ(g,u1,p),PowMod_ZZ(y,u2,p),p) % q;
    if(tempresult == r){
        cout<<"签名有效！Bob验证成功"<<endl;
    }
    else{
        cout<<"签名无效！"<<endl;
    }
}

int main(){
    cout<<"获取 消息M"<<endl;
    Creat_M();
    cout<<"M: "<<M<<endl;


    cout<<"对消息M进行SHA映射"<<endl;
    Set_SHA();
    SHA();
    cout<<"Hm位数: "<<NumBits(Hm)<<endl;

    ZZ ranseed;
    ranseed<<8;
    ranseed|=SEED;
    SetSeed(ranseed);

    cout<<"All is begin!"<<endl;
    cout<<"设置DSA参数"<<endl;
    Set_DSA();
    Alice();
    Bob();

    cout<<InvMod(15,7)<<endl;
    cout<<InvMod(1,7)<<endl;

    //cout<< p <<endl;
}