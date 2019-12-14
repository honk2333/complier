//
// Created by Honk on 2019/12/13.
//

#include <bits/stdc++.h>

using namespace std;
#ifndef CLION_COMPLIER_H
#define CLION_COMPLIER_H
const int maxn = 1000;
struct production {  //产生式结构体
    string li;
    string ri[100];
    set<string> first[100];
    set<string> follow;
    set<string> select[100];
    int size;
    int id[100]; //产生式序号
};
struct token {   //token序列结构体
    int id;
    char group;
};

int is_word(string);

void pre_grammer();

void add_synbl(string name);

void get_token();

void ll1();

void test_synbl();

bool judge_vn(string c);

bool judge_vt(string c);

#endif //CLION_COMPLIER_H
