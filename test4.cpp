//
// Created by 李文博 on 2018-12-23.
//

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include "common.h"

using namespace std;

long long p,q,n,deltan,d,e;

char plaintext[2];
int plt,cit;

char code2char[27]={' ','A','B','C','D',
                    'E','F','G','H','I',
                    'J','K','L','M','N',
                    'O','P','Q','R','S',
                    'T','U','V','W','X',
                    'Y','Z'};

int char2code(char c){
    switch (c){
        case ' ':return 0;
        case 'A':return 1;
        case 'B':return 2;
        case 'C':return 3;
        case 'D':return 4;
        case 'E':return 5;
        case 'F':return 6;
        case 'G':return 7;
        case 'H':return 8;
        case 'I':return 9;
        case 'J':return 10;
        case 'K':return 11;
        case 'L':return 12;
        case 'M':return 13;
        case 'N':return 14;
        case 'O':return 15;
        case 'P':return 16;
        case 'Q':return 17;
        case 'R':return 18;
        case 'S':return 19;
        case 'T':return 20;
        case 'U':return 21;
        case 'V':return 22;
        case 'W':return 23;
        case 'X':return 24;
        case 'Y':return 25;
        case 'Z':return 26;
    }
}

void txt2plt(){
    plt = char2code(plaintext[0]);
    plt *= 100;
    plt += char2code(plaintext[1]);
}

void plt2txt(){
    plaintext[1] = code2char[plt%100];
    plt /= 100;
    plaintext[0] = code2char[plt%100];
}

void set_rsa(){
    n = p * q;
    cout<<"n: "<<n<<endl;
    deltan = (p - 1) * (q - 1);
    cout<<"(l)n: "<<deltan<<endl;
    for(long long i = 2;;i++){
        if(is_coprime(i,deltan)){
            e = i;
            break;
        }
    }
    cout<<"e: "<<e<<endl;
    for(d=1;;d++){
        if(quick_mod(d*e, 1, deltan) == 1){
            break;
        }
    }
    cout<<"d: "<<d<<endl;
}

void encrypt(){
    cit = quick_mod(plt , e, n);
}

void decrypt(){
    plt = quick_mod(cit, d, n);
}

int main(){
    cout<<"7^563mod561: "<<quick_mod(7,563,561)<<endl;
    cout<<"请输入p 、q"<<endl;
    cin>>p>>q;
    set_rsa();

    ifstream file;

    file.open("data/4/text.txt",ios::in);

    ofstream ofile;
    ofile.open("data/4/encrypt.txt",ios::out);

    while(!file.eof()) {
        file.read((char *) &plaintext, sizeof(plaintext));
        if(file.eof())break;
        txt2plt();
        encrypt();

        ofile.write((char *) &cit, sizeof(cit));
    }
    ofile.close();
    file.close();

    cout<<"加密成功！"<<endl;
    //解密
    file.open("data/4/encrypt.txt",ios::in);

    ofile.open("data/4/answer.txt",ios::out);
    while(!file.eof()) {
        file.read((char *) &cit, sizeof(cit));
        if(file.eof())break;
        decrypt();
        plt2txt();

        ofile.write((char *) &plaintext, sizeof(plaintext));
    }
    ofile.close();
    file.close();

    cout<<"解密成功！"<<endl;
    return 0;
}