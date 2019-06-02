//
// Created by 李文博 on 2018-12-20.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

typedef long long ll;

int quick_pow(int base ,int m ,int mod){
    int ans = 1;
    while(m){
        if( m & 1 ){
            ans = ( base * ans ) % mod;
        }
        base = ( base * base) % mod;
        m >>= 1;
    }
    if(ans < 0){
        ans = mod + ans;
    }
    return ans;
}

long long mod_mul(long long base, long long m, long long n) {
    long long res = 0;
    while (m) {
        if(m & 1)
            res = (res + base) % n;
        base = (base + base) % n;
        m >>= 1;
    }
    return res;
}

long long quick_mod(long long base, long long m, long long mod) {
    long long res = 1;
    while(m) {
        if(m & 1)
            res = mod_mul(res, base, mod);
        base = mod_mul(base, base, mod);
        m >>= 1;
    }
    return res;
}

/*
ll exgcd(ll a,ll b,ll &x,ll &y){
    if(!b){
        x=1;
        y=0;
        return a;
    }
    ll d=exgcd(b,a%b,x,y);
    ll tmp=x;
    x=y;
    y=tmp-a/b*y;
    return d;
}

ll inv(ll a,ll m){
    ll x,y;
    ll d=exgcd(a,m,x,y);
    if(d==1){
        //处理负数
        return (x%m+m)%m;
    }
    return -1;
}
*/
int anti_mod(int u, int mod){
    int n1 = mod;
    int n2 = u;
    int b1 = 0,b2 = 1;
    int q = n1 / n2;
    int r = n1 - q * n2;
    while(r!=0){
        n1=n2;
        n2=r;
        int t = b2;
        b2 = b1 - q * b2;
        b1 = t;
        q = n1 / n2;
        r = n1 - q * n2;
    }
    if(n2 != 1)return 0x7FFFFFFF;
    return quick_mod(b2,1,mod);
}

bool is_coprime(long long x, long long y){
    if(x==1 || y==1)
        return true;
    else if(x<=0 || y<=0)
        return false;
    else{
        int tmp=0;
        while(true){
            tmp = x % y;
            if(!tmp)break;
            else{
                x = y;
                y = tmp;
            }
        }
        if(y == 1)
            return true;
        else
            return false;
    }
}

bool Miller_rabin(int n, int times){
    if(n == 2 | n == 1)return true;
    if(!(n & 1))return false;
    int s,m,temp;
    s = 0;
    temp = n-1;
    while ((temp & 0x1) == 0 && temp){
        temp >>= 1;
        s++;
    }
    m = temp;

    srandom((unsigned)time(0));
    int count = 0;
    for(int i=0;i < times;i++){
        int b = rand()%(n-2)+2;

        int r = 0, z = quick_mod(b,m,n);

        if(z == 1 | z == n-1){
            count++;
            continue;
        }
        while(z != (n-1) && r<(s-1)){
           r++;
           z = quick_mod(z,2,n);
        }
        if(z == n-1){
            count++;
            continue;
        }
        break;
    }
    if(count!=times)return false;
    return true;
}

void txt_to_dat(string txt,string dat){
    fstream ifile;
    ifile.open(txt,ios::in);
    fstream ofile;
    ofile.open(dat,ios::out|ios::binary);
    //不省略空格和换行
    ifile>>noskipws;
    char c;
    while(!ifile.eof()){
        ifile>>c;
        if(ifile.eof())break;
        ofile.write((char*)&c,sizeof(c));
    }
    ifile.close();
    ofile.close();
}

void dat_to_txt(string dat,string txt){
    ifstream ifile;
    ifile.open(dat,ios::in|ios::binary);
    ofstream ofile;
    ofile.open(txt,ios::out);
    char c;
    while(!ifile.eof()){
        ifile.read((char*)&c,sizeof(c));
        if(ifile.eof())break;
        ofile<<c;
    }
    ifile.close();
    ofile.close();
}