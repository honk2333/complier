//
// Created by Honk on 2019/12/10.
//
#include<bits/stdc++.h>

using namespace std;
const int maxn = 1e3;
struct type {  //���ͱ�
    char tval;  //���ʹ���
    string tpoint;//ָ��
} type[maxn];
struct synbl {   //���ű��ܱ�
    string name;  //����
    struct type type;  //����
    string cat;  //����
    int addr; //��ַ
} synbl[maxn];
int synbl_size = 0;
int llen[maxn];//���ȱ�
int cconst[maxn];//������

void add_synbl(string name) {
    synbl[synbl_size++].name = name;
}

void test_synbl() {
    cout << "��ǰ���ű�����:" << endl;
    for (int i = 0; i < synbl_size; i++) {
        cout << i << " " << synbl[i].name << " " << synbl[i].cat << " " << synbl[i].addr << endl;
    }
}