//
// Created by Honk on 2019/12/10.
//
#include<bits/stdc++.h>

using namespace std;
const int maxn = 1e3;
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

void add_synbl(string name) {
    synbl[synbl_size++].name = name;
}

void test_synbl() {
    cout << "当前符号表如下:" << endl;
    for (int i = 0; i < synbl_size; i++) {
        cout << i << " " << synbl[i].name << " " << synbl[i].cat << " " << synbl[i].addr << endl;
    }
}