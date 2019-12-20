//
// Created by Honk on 2019/12/13.
//

#include <bits/stdc++.h>
#include"editor.h"

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
struct QT_str {
    string Operator;
    string Operand1;
    string Operand2;
    string result;
    int QT_num;
};
struct token {   //token序列结构体
    string word;
    int id;
    char group;
};

bool match(string x, string word);

int is_word(string);

void pre_grammer();

int add_synbl(string name);

void get_token(MyWindow* w);

void get_mean(MyWindow* ww);

void ll1(MyWindow* w);

void new_Right();

void make_table();

void get_synbl(MyWindow* w);

bool judge_vn(string c);

bool judge_vt(string c);

void translate(MyWindow* w);

void Union();

#endif //CLION_COMPLIER_H
