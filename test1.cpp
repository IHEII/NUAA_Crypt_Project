//
// Created by 李文博 on 2018-12-20.
//
#include <iostream>
#include <fstream>
#include <string>
#include "common.h"

using namespace std;

string encryptfile="data/1/encrypt.txt";
string decryptfile="data/1/decrypt.txt";
string output="data/1/answer.txt";

int encryption(int k1, int k2,int mod,int m){
    return quick_mod(k1+k2*m,1,mod);
}

int decryption(int k1,int k2,int mod,int m){
    return quick_pow(anti_mod(k2,mod)*(m-k1),1,mod);
}

bool rw_file(int k1, int k2,bool is_encrypt,string infile,string outfile){
    ifstream ifile;
    ifile.open(infile,ios::in);
    ofstream ofile;
    ofile.open(outfile,ios::out);
    if (!ifile.is_open()){
        cout << "Can not find file!"<<endl;
        return false;
    }

    while(!ifile.eof()){
        string temp;
        getline(ifile,temp);
        for(int i = 0;temp[i]!='\0';i++){
            if(temp[i]>='a'&&temp[i]<='z'){
                temp[i]-='a';
                if(is_encrypt)temp[i]=encryption(k1,k2,26,temp[i]);
                else temp[i]=decryption(k1,k2,26,temp[i]);
                temp[i]+='a';
            }
            else if(temp[i]>='A'&&temp[i]<='Z'){
                temp[i]-='A';
                if(is_encrypt)temp[i]=encryption(k1,k2,26,temp[i]);
                else temp[i]=decryption(k1,k2,26,temp[i]);
                temp[i]+='A';
            }
        }
        ofile<<temp<<endl;
    }

    ifile.close();
    ofile.close();
    return true;
}

bool cal_alp(string filename){
    int alp[256];
    memset(alp,0, sizeof(alp));

    ifstream ifile;
    ifile.open(filename,ios::in);
    if (!ifile.is_open()){
        cout << "Can not find file!"<<endl;
        return false;
    }

    int sum = 0;

    while(!ifile.eof()){
        string temp;
        getline(ifile,temp);
        for(int i = 0;temp[i]!='\0';i++){
            if(temp[i]>='a'&&temp[i]<='z'){
                alp[temp[i]]++;
                sum++;
            }
            else if(temp[i]>='A'&&temp[i]<='Z'){
                alp[temp[i]]++;
                sum++;
            }
        }
    }

    cout<<"字母   "<<"出现频率"<<endl;
    for(int i=0;i<150;i++){
        if(i >= 'a' && i <= 'z'){
            cout<<char(i)<<"     "<<double(alp[i])/sum<<endl;
        }
        else if( i >='A' && i <= 'Z'){
            cout<<char(i)<<"     "<<double(alp[i])/sum<<endl;
        }
    }

    ifile.close();
    return true;
}

int main(){
    int k1=1;
    int k2=3;
    rw_file(k1,k2,true,encryptfile,decryptfile);
    cout<<"加密成功！"<<endl;
    rw_file(k1,k2,false,decryptfile,output);
    cout<<"解密成功！"<<endl;

    cal_alp(encryptfile);
    cout<<endl;

    cal_alp(decryptfile);
    cout<<endl;

    cal_alp(output);
    cout<<endl;

    return 0;
}