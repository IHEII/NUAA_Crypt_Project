//
// Created by 李文博 on 2018-12-20.
//

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include "common.h"

using namespace std;
//|-m1--m2--m3--m4--m5--m6--m7--m8-|
union Unit{
    struct{
        char m8 :1;
        char m7 :1;
        char m6 :1;
        char m5 :1;
        char m4 :1;
        char m3 :1;
        char m2 :1;
        char m1 :1;
    }bit;
    char mem;
}node,node_backup;

string encrypttxt="data/2/text.txt";
string encryptfile="data/2/encrypt.dat";
string decryptfile="data/2/decrypt.dat";
string output="data/2/answer.dat";
string outputtxt="data/2/answer.txt";

void update_lfsr(int num){
    for(int i=0;i<num;i++){
        node.mem<<=1;
        node.bit.m8 = node.bit.m7 ^ node.bit.m4;
    }
}

void set_lfsr(){
    int k;
    cout<<"请输入lfsr初始状态(空格分离)"<<endl;
    cin>>k;
    if(k)node.bit.m4=1;
    else node.bit.m4=0;
    cin>>k;
    if(k)node.bit.m5=1;
    else node.bit.m5=0;
    cin>>k;
    if(k)node.bit.m6=1;
    else node.bit.m6=0;
    cin>>k;
    if(k)node.bit.m7=1;
    else node.bit.m7=0;
    cin>>k;
    if(k)node.bit.m8=1;
    else node.bit.m8=0;
    update_lfsr(3);
    node_backup.mem=node.mem;
}

void show_lfsr(){
    if(node.bit.m1)cout<<1;
    else cout<<0;
    if(node.bit.m2)cout<<1;
    else cout<<0;
    if(node.bit.m3)cout<<1;
    else cout<<0;
    if(node.bit.m4)cout<<1;
    else cout<<0;
    if(node.bit.m5)cout<<1;
    else cout<<0;
    if(node.bit.m6)cout<<1;
    else cout<<0;
    if(node.bit.m7)cout<<1;
    else cout<<0;
    if(node.bit.m8)cout<<1;
    else cout<<0;
}

void show_nlfsr(){
    int k;
    cout<<"请输入展示多少字"<<endl;
    cin>>k;
    for(int i=0;i<k;i++){
        show_lfsr();
        update_lfsr(8);
    }
    cout<<endl;
    node.mem=node_backup.mem;
}

bool rw_file(string infile,string outfile){
    ifstream ifile;
    ifile.open(infile,ios::in|ios::binary);
    ofstream ofile;
    ofile.open(outfile,ios::out|ios::binary);
    if (!ifile.is_open()){
        cout << "Can not find file!"<<endl;
        return false;
    }
    char c;
    while(!ifile.eof()){
        ifile.read((char*)&c,sizeof(c));
        if(ifile.eof())break;
        c = c ^ node.mem;
        ofile.write((char*)&c, sizeof(c));
        update_lfsr(8);
    }
    ifile.close();
    ofile.close();
    node.mem=node_backup.mem;
    return true;
}

int main(){
    set_lfsr();
    show_nlfsr();

    cout<<"进行txt2dat(输入到文件）"<<endl;
    txt_to_dat(encrypttxt,encryptfile);

    cout<<"进行加密(输入到文件）"<<endl;
    rw_file(encryptfile,decryptfile);

    cout<<"进行解密(输入到文件）"<<endl;
    rw_file(decryptfile,output);

    cout<<"进行dat2txt(输入到文件）"<<endl;
    dat_to_txt(output,outputtxt);

    cout<<"完成！"<<endl;
    return 0;
}