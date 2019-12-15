#include"complier.h"

using namespace std;

struct production np[maxn], pron[maxn];//p是从文件中读入的文法,np是消除左递归后的文法
int cnt = 0;
int dnt = 0;

int vt_size = 5;
int vn_size = 12;
string vt[maxn] = {"w0", "w1", "id", "cons", "@"}; //终结符集合
string vn[maxn] = {"MAINPRO", "S_LIST", "TYPE", "S", "EVA_SENTENCE", "SEL_SENTENCE", "ITE_SENTENCE", "F", "E", "T",
                   "T'",
                   "E'"}; //非终结符集合
bool Find(string s) {
    for (int i = 0; i < vt_size; i++) {
        if (vt[i] == s) return true;
    }
    return false;
}

void Union() {   //通过词法分析器来扩充终结符表
    extern string k[maxn];   //关键字
    extern int len_k;
    extern string p[maxn];    //界符
    extern int len_p;
    for (int i = 0; i < len_k; i++) {
        if (!Find(k[i])) vt[vt_size++] = k[i];
    }
    for (int i = 0; i < len_p; i++) {
        if (!Find(p[i])) vt[vt_size++] = p[i];
    }
    cout << vt_size << endl;
}

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
        for (int j = 0; j < pron[i].size; j++) {
            cout << pron[i].li << " -> ";
            cout << pron[i].ri[j] << endl;
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
        for (int j = 0; j < pron[i].size; j++) {
            stringstream ss(pron[i].ri[j]);
            string ri_first;
            ss >> ri_first;
            //cout << ri_first << endl;
            if (pron[i].li == ri_first) {
                flag = true;   //有左递归
                break;
            }
        }
    }
    if (flag) {    //消除左递归
        cout << "当前文法有左递归，消除左递归后的文法为：" << endl;
        string left;
        for (int i = 0; i < cnt; i++) {
            bool tag = false;
            for (int j = 0; j < pron[i].size; j++) {
                stringstream ss(pron[i].ri[j]);
                string ri_first;
                ss >> ri_first;
                //cout << ri_first << endl;
                if (pron[i].li == ri_first) {
                    //当前产生式有左递归
                    tag = true;
                    for (int l = 0; l < pron[i].size; l++) {
                        stringstream sss(pron[i].ri[l]);
                        string rri_first;
                        sss >> rri_first;
                        if (pron[i].li != rri_first) {
                            left = pron[i].ri[l];   //ss存不含有左递归的产生式
                            cout << pron[i].li << " -> ";
                            int len = pron[i].ri[j].length();
                            string tmp = pron[i].ri[j].substr(ri_first.size());
                            //cout << tmp << endl;
                            cout << left + " {" + tmp + " }" << endl;
                            np[dnt].li = pron[i].li;
                            np[dnt].ri[np[dnt].size++] = left + " " + np[dnt].li + "\'";
                            dnt++;
                            np[dnt].li = np[dnt - 1].li + "\'";
                            np[dnt].ri[np[dnt].size++] =
                                    pron[i].ri[j].substr(ri_first.size() + 1, len - 1) + " " + np[dnt].li;
                            np[dnt].ri[np[dnt].size++] = "@"; //空
                            dnt++;
                            break;
                        }
                    }
                }
            }
            if (!tag) {
                np[dnt].li = pron[i].li;
                np[dnt].size = pron[i].size;
                for (int j = 0; j < pron[i].size; j++) {
                    np[dnt].ri[j] = pron[i].ri[j];
                }
                dnt++;
            }
        }
        cout << "消除括号后的文法为：" << endl;
        test2_output();
    } else {
        dnt = cnt;
        for (int i = 0; i < cnt; i++) {
            np[i].li = pron[i].li;
            np[i].size = pron[i].size;
            for (int j = 0; j < pron[i].size; j++) {
                np[dnt].ri[j] = pron[i].ri[j];
            }
        }
    }

}

bool generate_empty(string ri) {
    //int len=ri.size();
    stringstream ss(ri);
    string ww;
    while (ss >> ww) {
        //cout << ww << endl;
        if (judge_vt(ww) && ww != "@") {
            return false;
        } else if (judge_vn(ww)) {
            bool tag = false;
            for (int i = 0; i < dnt; i++) {
                if (tag) break;
                if (np[i].li == ww) {
                    for (int j = 0; j < np[i].size; j++) {
                        if (generate_empty(np[i].ri[j])) {
                            tag = true;
                            break;
                        }
                    }
                    break;
                }
            }
            if (tag) continue;
            else return false;
        }
    }
    return true;
}

//递归求first集
set<string> First(string c) {
    //cout << "c " << c << endl;
    set<string> ret;
    if (c == "@") {
        return ret;
    } else if (judge_vt(c)) {  //非空终结符,加入first集
        ret.insert(c);
    } else {
        for (int k = 0; k < dnt; k++) {
            if (np[k].li == c) {  //找到左部
                for (int l = 0; l < np[k].size; l++) {  //遍历右部
                    string tmp = "";
                    stringstream ss(np[k].ri[l]);  //产生式右部
                    ss >> tmp;
                    set<string> rett = First(tmp);
                    for (auto it = rett.begin(); it != rett.end(); it++) {
                        ret.insert(*it);
                    }
                    while (!ss.eof() && generate_empty(tmp)) { //如果当前字符可以为空
                        ss >> tmp;
                        rett = First(tmp);
                        for (auto it = rett.begin(); it != rett.end(); it++) {
                            ret.insert(*it);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

//求产生式的first集
void get_first() {
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            string tmp = "";
            stringstream ss(np[i].ri[j]);  //产生式右部
            ss >> tmp;
            //cout << tmp << endl;
            np[i].first[j] = First(tmp);
            while (!ss.eof() && generate_empty(tmp)) { //如果当前字符可以为空
                ss >> tmp;
                //cout << "ss " << tmp << endl;
                set<string> rett = First(tmp);
                for (auto it = rett.begin(); it != rett.end(); it++) {
                    np[i].first[j].insert(*it);
                }
            }
        }
    }
    return;
}

//打印first集
void test_first() {
    get_first();
    cout << "First_set:" << endl;
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
int nowid; //防止死循环用
//生成follow集
set<string> make_follow(int id, string left_head) {
    if (follow_vis[id]) return np[id].follow;  //若当前左部的follow集已经求完,直接返回
    //cout << left_head << endl;
    //查找含left_head的产生式
    for (int i = 0; i < dnt; i++) {
        string left_str = np[i].li;
        for (int j = 0; j < np[i].size; j++) {
            int start = 0;
            int idx;
            while (start != np[i].ri[j].npos && (idx = np[i].ri[j].find(left_head, start)) != np[i].ri[j].npos) {
                {//下标
                    if (idx > 0 && np[i].ri[j][idx - 1] != ' ') {
                        idx = (int) np[i].ri[j].npos;
                        break;
                    }
                    if (idx + left_head.size() < np[i].ri[j].size() && np[i].ri[j][idx + left_head.size()] != ' ') {
                        idx = (int) np[i].ri[j].npos;
                        break;
                    }
                    if (np[i].ri[j][idx + 1] == '\'' && left_head[left_head.size() - 1] != '\'') {
                        idx = (int) np[i].ri[j].npos;
                        break;
                    }
                }
                //查到
                //cout <<"idx"<< idx << endl;
                if (idx != np[i].ri[j].npos) {
                    //cout << "left_head" << left_head << "  np[i].ri[j]" << np[i].ri[j] << "  idx" << idx << endl;
                    //查下一字符
                    //没字符
                    if (idx + left_head.size() == np[i].ri[j].length()) {
                        if (left_head != np[i].li && nowid != i) {
                            set<string> temp;
                            //cout << "no word:" << left_str << endl;
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
                        if (judge_vt(right_next)) np[id].follow.insert(right_next);
                            //cout << right_next << endl;
                        else {
                            if (generate_empty(tmp)) {  //右部符号串可以为空
                                set<string> temp;
                                temp = make_follow(i, left_str);
                                set<string>::iterator it = temp.begin();
                                for (; it != temp.end(); it++) {
                                    np[id].follow.insert(*it);
                                }
                            }
                            {  //右部符号串不为空
                                set<string> ret = First(right_next);
                                for (auto it = ret.begin(); it != ret.end(); it++)
                                    np[id].follow.insert(*it);
                                while (!ss.eof() && generate_empty(right_next)) { //如果当前字符可以为空
                                    ss >> right_next;
                                    //cout << "ss " << tmp << endl;
                                    set<string> rett = First(right_next);
                                    for (auto it = rett.begin(); it != rett.end(); it++) {
                                        np[id].follow.insert(*it);
                                    }
                                }
                            }
                        }
                    }
                    start = idx + left_head.size();
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
        nowid = i;
        //cout << np[i].li << endl;
        make_follow(i, np[i].li);
        follow_vis[i] = true;
    }
}

//输入follow集并输出
void test_follow() {
    get_follow();
    cout << "Follow_set:" << endl;
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

//生成select集
void make_select() {
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            //cout << np[i].li << " " << np[i].ri[j] << endl;
            set<string> &temp = np[i].first[j];
            set<string>::iterator it = temp.begin();
            for (; it != temp.end(); it++) {
                np[i].select[j].insert(*it);
            }
            if (generate_empty(np[i].ri[j])) {
                if (!follow_vis[i]) make_follow(i, np[i].li);
                set<string> &temp = np[i].follow;
                set<string>::iterator it2 = temp.begin();
                for (; it2 != temp.end(); it2++) {
                    np[i].select[j].insert(*it2);
                }
            }
        }
    }
}

void get_select() {
    make_select();
}

void test_select() {
    get_select();
    cout << "Select_set:" << endl;
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            cout << "Select of " << np[i].li << " -> " << np[i].ri[j] << endl;
            int len = np[i].select[j].size();
            //cout << len << endl;
            for (set<string>::iterator it = np[i].select[j].begin(); it != np[i].select[j].end(); it++) {
                cout << *it << " ";
            }
            cout << endl;
        }
    }
}

void pre_grammer() {
    freopen("input3.txt", "r", stdin);
    string ss;
    int start = 0;
    while (getline(cin, ss)) {
        //cout << ss << endl;
        int len = ss.length();
        for (int i = 0; i < len; i++) {
            if (ss[i] == ' ')
                continue;
            else {   //读产生式左部
                int j = i;
                for (j = i; ss[j] != ' ' && ss[j] != '|'; j++);
                pron[cnt].li = ss.substr(i, j - i);
                //cout<<"left: "<<pron[cnt].li<<endl;
                break;
            }
        }
        for (int i = 0; i < len - 1; i++) {
            if (ss[i] == '-' && ss[i + 1] == '>') {   //读到产生式符号
                start = i + 2;
            }
        }
        pron[cnt].size = 0;
        int li = start, ri;
        while (ss[li] == ' ')
            li++;
        for (int i = start; i < len; i++) { //将产生式分开，便于后续处理
            if (ss[i] == '|') {
                ri = i - 1;
                while (ss[ri] == ' ')
                    ri--;
                pron[cnt].ri[pron[cnt].size] = ss.substr(li, ri - li + 1);
                //cout<<"right "<<pron[cnt].ri[pron[cnt].size]<<endl;
                li = i + 1;
                while (ss[li] == ' ')
                    li++;
                pron[cnt].size = pron[cnt].size + 1;
            }
        }
        //
        ri = len - 1;
        while (ss[ri] == ' ')
            ri--;
        pron[cnt].ri[pron[cnt].size] = ss.substr(li, ri - li + 1);
        //cout<<"right "<<pron[cnt].ri[pron[cnt].size]<<endl;
        pron[cnt].size = pron[cnt].size + 1;
        cnt++;
    }
    test1_output();
    Left_Recursion();  //判断并消除左递归
    test_first();
    test_follow();
    test_select();
}
