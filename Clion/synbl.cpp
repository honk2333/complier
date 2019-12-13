//
// Created by Honk on 2019/12/10.
//
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e3;
struct type {  //类型表
    char tval;  //类型代码
    string tpoint;//指针
} type[maxn];
struct synbl {   //符号表总表
    string name;  //名字
    struct type type;  //类型
    string cat;  //种类
    int addr; //地址
} synbl[maxn];
int synbl_size = 0;
int llen[maxn];//长度表
int cconst[maxn];//常数表
void get_synal(){
    void Next();
    freopen("test.cpp","r",stdin);
    //Next();
    //if(word==)
}