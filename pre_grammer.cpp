#include"complier.h"

using namespace std;
const int maxn = 1e2;
struct production {  //产生式结构体
    string li;
    string ri[100];
    set<string> first[100];
    set<string> follow;
    set<string> selected;
    int size;
} p[maxn], np[maxn];//p是从文件中读入的文法,np是消除左递归后的文法
int cnt = 0;
int dnt = 0;

int vt_size = 7;
int vn_size = 7;
string vt[maxn] = {"w0", "w1", "ID", "CONS", "(", ")", "@"}; //终结符集合
string vn[maxn] = {"S", "EVA_SENTENCE", "F", "T", "T'", "E", "E'"}; //非终结符集合
bool judge_vt(string c) {  //判断一个单词是否是终结符
    for (int i = 0; i < vt_size; i++) {
        if (c == vt[i])
            return true;
    }
    return false;
}

bool judge_vn(string c) {  //判断一个单词是否是非终结符
    for (int i = 0; i < vn_size; i++) {
        if (c == vn[i])
            return true;
    }
    return false;
}

void test1_output() {   //打印消除左递归前的文法
    cout << "当前文法为" << endl;
    for (int i = 0; i < cnt; i++) {
        //cout << p[i].size << endl;
        for (int j = 0; j < p[i].size; j++) {
            cout << p[i].li << " -> ";
            cout << p[i].ri[j] << endl;
        }
    }
}

void test2_output() {   //打印消除左递归后的文法
    for (int i = 0; i < dnt; i++) {
        //cout << p[i].size << endl;
        for (int j = 0; j < np[i].size; j++) {
            cout << np[i].li << " -> ";
            cout << np[i].ri[j] << endl;
        }
    }
}

void Left_Recursion() {  //判断有无左递归并消除左递归
    bool flag = false;
    for (int i = 0; i < cnt; i++) {    //判断左递归
        if (flag)
            break;
        for (int j = 0; j < p[i].size; j++) {
            if (p[i].li[0] == p[i].ri[j][0]) {
                flag = true;   //有左递归
                break;
            }
        }
    }
    if (flag) {    //消除左递归
        cout << "当前文法有左递归，消除左递归后的文法为：" << endl;
        string ss;
        for (int i = 0; i < cnt; i++) {
            bool tag = false;
            for (int j = 0; j < p[i].size; j++) {
                if (p[i].li[0] == p[i].ri[j][0]) {   //当前产生式有左递归
                    tag = true;
                    for (int l = 0; l < p[i].size; l++) {
                        if (p[i].li[0] != p[i].ri[l][0]) {
                            ss = p[i].ri[l];   //ss存不含有左递归的产生式
                            cout << p[i].li << " -> ";
                            int len = p[i].ri[j].length();
                            cout << ss + " {" + p[i].ri[j].substr(1, len - 1) + " }" << endl;
                            np[dnt].li = p[i].li;
                            np[dnt].ri[np[dnt].size++] = ss + " " + np[dnt].li + "\'";
                            dnt++;
                            np[dnt].li = np[dnt - 1].li + "\'";
                            np[dnt].ri[np[dnt].size++] = p[i].ri[j].substr(2, len - 1) + " " + np[dnt].li;
                            np[dnt].ri[np[dnt].size++] = "@"; //空
                            dnt++;
                            break;
                        }
                    }
                }
            }
            if (!tag) {
                np[dnt].li = p[i].li;
                np[dnt].size = p[i].size;
                for (int j = 0; j < p[i].size; j++) {
                    np[dnt].ri[j] = p[i].ri[j];
                }
                dnt++;
            }
        }
        /*for (int i = 0; i < cnt; i++) {  //p中剩余产生式加入到np数组中
            bool tag = false;
            for (int j = 0; j < dnt; j++) {
                if (p[i].li == np[j].li) {
                    tag = true;
                    break;
                }
            }
            *//* if (!tag) {
                 np[dnt].li = p[i].li;
                 np[dnt].size = p[i].size;
                 for (int j = 0; j < p[i].size; j++) {
                     np[dnt].ri[j] = p[i].ri[j];
                 }
                 dnt++;
             }*//*
        }*/
        cout << "消除括号后的文法为：" << endl;
        //test1_output();
        test2_output();
    } else
        return;
}

void First(int i, int j, string c) {   //递归求first集
    if (judge_vt(c) && c != "@") {  //非空终结符,加入first集
        np[i].first[j].insert(c);
    } else {
        for (int k = 0; k < dnt; k++) {
            if (np[k].li == c) {  //找到左部
                for (int l = 0; l < np[k].size; l++) {  //遍历右部
                    string tmp = "";
                    stringstream ss(np[k].ri[j]);  //产生式右部
                    ss >> tmp;
                    First(i, j, tmp);
                }
            }
        }
    }
    return;
}

void get_first() {   //求产生式的first集
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            string tmp = "";
            stringstream ss(np[i].ri[j]);  //产生式右部
            ss >> tmp;
            First(i, j, tmp);
        }
    }
    return;
}

void test_first() {  //打印first集
    get_first();
    cout << "First集如下:" << endl;
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            cout << "First of " << np[i].ri[j] << endl;
            int len = np[i].first[j].size();
            //cout << len << endl;
            for (set<string>::iterator it = np[i].first[j].begin(); it != np[i].first[j].end(); it++) {
                cout << *it << " ";
            }
            cout << endl;

        }
    }
}


bool follow_vis[maxn];

//生成follow集
set<string> make_follow(int id, string left_head) {
    if (follow_vis[id]) return np[id].follow;  //若当前左部的follow集已经求完,直接返回

    //查找含left_head的产生式
    for (int i = 0; i < dnt; i++) {
        string left_str = np[i].li;
        for (int j = 0; j < np[i].size; j++) {
            int idx = np[i].ri[j].find(left_head, 0);   //下标
            if (idx != np[i].ri[j].npos) {
                if (idx > 0 && np[i].ri[j][idx - 1] != ' ') idx = np[i].ri[j].npos;
                if (idx + left_head.size() < np[i].ri[j].size() && np[i].ri[j][idx + left_head.size()] != ' ')
                    idx = np[i].ri[j].npos;
                if (np[i].ri[j][idx + 1] == '\'' && left_head[left_head.size() - 1] != '\'') idx = np[i].ri[j].npos;
            }
            //查到

            if (idx != np[i].ri[j].npos) {
                //cout << np[i].ri[j] << " " << idx << endl;
                //查下一字符
                //没字符
                if (idx + left_head.size() == np[i].ri[j].length()) {
                    if (left_head != np[i].li) {
                        set<string> temp;
                        //cout << "!!!" << left_str << endl;
                        temp = make_follow(i, left_str);  //单词末尾,加上该产生式的follow(li);
                        set<string>::iterator it = temp.begin();
                        for (; it != temp.end(); it++) {
                            np[i].follow.insert(*it);
                            np[id].follow.insert(*it);
                        }

                    }
                } else {
                    string tmp = np[i].ri[j].substr(idx + left_head.size());
                    stringstream ss(tmp);
                    string right_next;
                    ss >> right_next;
                    //为终结符
                    // cout << "right " << right_next << endl;
                    if (judge_vt(right_next)) {
                        //cout << "produ" << np[i].ri[j][idx] << endl;
                        np[id].follow.insert(right_next);
                    }
                        //为非终结符,可能出现E'T'，这种情况
                    else {
                        //查非终结符对应的产生式
                        //if(left_head==right_next)
                        int left_next_num;
                        for (int k = 0; k < dnt; k++) {
                            if (np[k].li == right_next) {
                                left_next_num = k;
                                break;
                            }
                        }
                        string next_test;
                        for (int k = 0; k < np[left_next_num].size; k++) {
                            //可推出空
                            if (np[left_next_num].ri[k] == "@") {
                                //if(visi_$.count(right_next)){
                                //Follow[i]加入Follow[left_head]
                                set<string> temp;
                                temp = make_follow(i, left_str);
                                set<string>::iterator it = temp.begin();
                                for (; it != temp.end(); it++) {
                                    np[id].follow.insert(*it);
                                }
                            } else {
                                //first[right_next]加入Follow[left_head]
                                set<string>::iterator it1 = np[left_next_num].first[k].begin();
                                for (; it1 != np[left_next_num].first[k].end(); it1++) {
                                    np[id].follow.insert(*it1);
                                }

                            }

                        }
                    }
                }

            }
        }
    }
    return np[id].follow;
}

void get_follow() {
    memset(follow_vis, false, sizeof(follow_vis));
    np[0].follow.insert("#");
    for (int i = 0; i < dnt; i++) {
        //cout << np[i].li << endl;
        make_follow(i, np[i].li);
        follow_vis[i] = true;
    }
}

void test_follow() {   //输入follow集并输出
    get_follow();
    cout << "Follow集如下:" << endl;
    for (int i = 0; i < dnt; i++) {
        cout << "Follow of " << np[i].li << endl;
        int len = np[i].follow.size();
        //cout << len << endl;
        for (set<string>::iterator it = np[i].follow.begin(); it != np[i].follow.end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    }
}


void pre_grammer() {
    void get_token();
    freopen("input3.txt", "r", stdin);
    string ss;
    int start = 0;
    while (getline(cin, ss)) {
        cout << ss << endl;
        int len = ss.length();
        for (int i = 0; i < len; i++) {
            if (ss[i] == ' ')
                continue;
            else {   //读产生式左部
                int j = i;
                for (j = i; ss[j] != ' ' && ss[j] != '|'; j++);
                p[cnt].li = ss.substr(i, j - i);
                break;
            }
        }
        for (int i = 0; i < len - 1; i++) {
            if (ss[i] == '-' && ss[i + 1] == '>') {   //读到产生式符号
                start = i + 2;
            }
        }
        p[cnt].size = 0;
        int li = start, ri;
        while (ss[li] == ' ')
            li++;
        for (int i = start; i < len; i++) { //将产生式分开，便于后续处理
            if (ss[i] == '|') {
                ri = i - 1;
                while (ss[ri] == ' ')
                    ri--;
                p[cnt].ri[p[cnt].size] = ss.substr(li, ri - li + 1);
                li = i + 1;
                while (ss[li] == ' ')
                    li++;
                p[cnt].size = p[cnt].size + 1;
            }
        }
        ri = len - 1;
        while (ss[ri] == ' ')
            ri--;
        p[cnt].ri[p[cnt].size] = ss.substr(li, ri - li + 1);
        p[cnt].size = p[cnt].size + 1;
        cnt++;
    }
    test1_output();
    Left_Recursion();  //判断并消除左递归
    test_first();
    test_follow();
}
