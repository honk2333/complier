//
// Created by Honk on 2019/11/28.
//

#include<bits/stdc++.h>

using namespace std;
const int maxn = 1e2;
struct production {  //产生式结构体
    string li;
    string ri[100];
    set<string> first[100];
    set<string> follow;
    set<string> selected;
    int size;
} p[maxn], np[maxn];

int cnt = 0;
int dnt = 0;
int vt_size = 6;
int vn_size = 5;
string vt[maxn] = {"w0", "w1", "I", "(", ")", "@"}; //终结符集合
string vn[maxn] = {"F", "T", "T1", "E", "E1"}; //非终结符集合
struct anaylist {   //ll(1)分析表
    string s1;
    string s2;
    string ans;
} List[100];
int List_size = 0;  //ll(1)分析表长度
struct slrlist {    //slr分析表
    string s[100];
    string ans[100];
    int size;
} slrlist[100];
int slrlist_size = 0;   //slr分析表长度
int OPAM[10][10];  //算符优先分析矩阵，0代表=,1代表>,-1代表<,-2代表不存在关系
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
            for (int j = 0; j < p[i].size; j++) {
                if (p[i].li[0] == p[i].ri[j][0]) {   //当前产生式有左递归
                    for (int l = 0; l < p[i].size; l++) {
                        if (p[i].li[0] != p[i].ri[l][0]) {
                            ss = p[i].ri[l];   //ss存不含有左递归的产生式
                            cout << p[i].li << " -> ";
                            int len = p[i].ri[j].length();
                            cout << ss + " {" + p[i].ri[j].substr(1, len - 1) + " }" << endl;
                            np[dnt].li = p[i].li;
                            np[dnt].ri[np[dnt].size++] = ss + " " + np[dnt].li + "1";
                            dnt++;
                            np[dnt].li = np[dnt - 1].li + "1";
                            np[dnt].ri[np[dnt].size++] = p[i].ri[j].substr(2, len - 1) + " " + np[dnt].li;
                            np[dnt].ri[np[dnt].size++] = "@"; //空
                            dnt++;
                            break;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < cnt; i++) {  //p中剩余产生式加入到np数组中
            bool tag = false;
            for (int j = 0; j < dnt; j++) {
                if (p[i].li == np[j].li) {
                    tag = true;
                    break;
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
        cout << "消除括号后的文法为：" << endl;
        //test1_output();
        test2_output();
    } else
        return;
}

bool alpha_word(char w) {   //判断是否是字母或单词
    if (w <= 'z' && w >= 'a')
        return true;
    if (w <= 'Z' && w >= 'A')
        return true;
    if (w <= '9' && w >= '0')
        return true;
    return false;
}

string sentence;  //要处理的句子
string word;  //当前单词
int pos;//下一个单词的起始位置

void Next() {   //下一个单词
    int is_word(string);
    char c;
    word = "";
    int len = sentence.size();
    //cout << sentence << endl;
    for (int i = len - 1; i >= pos; i--) {   //查找单词表，优先匹配长度长的单词
        string tmp = sentence.substr(pos, i - pos + 1);
        if (is_word(tmp) > 0 || tmp == "#") {
            word = tmp;
            pos = i + 1;
            //cout <<"word  "<< word << endl;
            return;
        }
    }
    return;
}

vector<string> subpro;//记录子程序调用顺序

void E() {   //产生式E代表的子程序
    void T();
    void E1();
    subpro.push_back("T");
    T();
    subpro.pop_back();
    subpro.push_back("E1");
    E1();
    subpro.pop_back();
    return;
}

void E1() { //产生式E1代表的子程序
    void T(void);
    if (word == "+" || word == "-") {
        Next();
        if (word != "")
            cout << word;
        subpro.push_back("T");
        T();
        subpro.pop_back();
        subpro.push_back("E1");
        E1();
        subpro.pop_back();
        return;
    } else {
//        w = getchar();
//        putchar(w);
        return;
    }
}

void T() { //产生式T代表的子程序
    void F();
    void T1();
    subpro.push_back("F");
    F();
    subpro.pop_back();
    subpro.push_back("T1");
    T1();
    subpro.pop_back();
    return;
}

void T1() { //产生式T1代表的子程序
    void F();
    if (word == "*" || word == "/") {
        Next();
        if (word != "")
            cout << word;
        subpro.push_back("F");
        F();
        subpro.pop_back();
        subpro.push_back("T1");
        T1();
        subpro.pop_back();
        return;
    } else {
//        w = getchar();
//        putchar(w);
        return;
    }
}

void F() { //产生式F代表的子程序
    int is_word(string);
    if (is_word(word) == 1 || is_word(word) == 2) {
        Next();
        if (word != "")
            cout << word;
        //cerr << w << endl;
        return;
    } else {
        if (word == "(") {
            Next();
            if (word != "")
                cout << word;
            subpro.push_back("E");
            E();
            subpro.pop_back();
            if (word == ")") {
                Next();
                if (word != "")
                    cout << word;
                return;
            } else {
                word = "";
                cout << "error2" << endl;
                return;
            }
        } else {
            word = "";
            cout << "error1" << endl;
            return;
        }
    }
}

void judge1() {   //递归下降子程序分析法判断
    cout << "递归下降子程序分析法" << endl;
    //fflush(stdin);
    sentence = "";
    char ch;
    while ((ch = getchar()) != '#') sentence += ch;
    sentence += "#";    //将待识别的句子先读进来
    //cout << sentence << endl;
    pos = 0;
    Next();
    if (word != "")
        cout << word;
    subpro.push_back("E");
    E();
    subpro.pop_back();
//    w = getchar();
    cout << endl;
    if (word == "#") {   //返回后若当前单词为'#'说明句子是合法的，否则是不合法的
        cout << "该句子是文法的合法语句" << endl;
    } else
        cout << "该句子不是文法的合法语句" << endl;
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

void First(int i, int j, string c) {   //递归求first集
    //cout<<c.size()<<endl;
    if (judge_vt(c) && c != "@") {
        np[i].first[j].insert(c);
    } else {
        for (int k = 0; k < dnt; k++) {
            if (np[k].li == c) {
                for (int l = 0; l < np[k].size; l++) {
                    string tmp = "";
                    tmp += np[k].ri[l][0];
                    //cout << (int) np[k].ri[l][1] << endl;
                    if (np[k].ri[l][1] != 0 && np[k].ri[l][1] != ' ' && np[k].ri[l][1] != '\n')
                        tmp += np[k].ri[l][1];
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
            //cout << np[i].ri[j][0] << endl;
            string tmp = "";
            tmp += np[i].ri[j][0];
            if (np[i].ri[j][1] != 0 && np[i].ri[j][1] != ' ' && np[i].ri[j][1] != '\n')
                tmp += np[i].ri[j][1];
            //cout << tmp << endl;
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

void test_follow() {   //输入follow集并输出
    np[0].follow.insert(")");
    np[0].follow.insert("#");
    np[1].follow.insert(")");
    np[1].follow.insert("#");
    np[2].follow.insert("w0");
    np[2].follow.insert(")");
    np[2].follow.insert("#");
    np[3].follow.insert("w1");
    np[3].follow.insert(")");
    np[3].follow.insert("#");
    np[4].follow.insert("w1");
    np[4].follow.insert(")");
    np[4].follow.insert("#");
    np[4].follow.insert("w0");
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


void test_analyst() {   //输入ll(1)分析表并输出
    List_size = 0;
    List[List_size].s1 = "E";
    List[List_size].s2 = "I";
    List[List_size].ans = "T E1";
    List_size++;

    List[List_size].s1 = "E";
    List[List_size].s2 = "(";
    List[List_size].ans = "T E1";
    List_size++;

    List[List_size].s1 = "E1";
    List[List_size].s2 = "w0";
    List[List_size].ans = "w0 T E1";
    List_size++;

    List[List_size].s1 = "E1";
    List[List_size].s2 = ")";
    List[List_size].ans = "@";
    List_size++;

    List[List_size].s1 = "T";
    List[List_size].s2 = "I";
    List[List_size].ans = "F T1";
    List_size++;

    List[List_size].s1 = "T";
    List[List_size].s2 = "(";
    List[List_size].ans = "F T1";
    List_size++;

    List[List_size].s1 = "T1";
    List[List_size].s2 = "w0";
    List[List_size].ans = "@";
    List_size++;

    List[List_size].s1 = "T1";
    List[List_size].s2 = "w1";
    List[List_size].ans = "w1 F T1";
    List_size++;

    List[List_size].s1 = "T1";
    List[List_size].s2 = ")";
    List[List_size].ans = "@";
    List_size++;

    List[List_size].s1 = "F";
    List[List_size].s2 = "I";
    List[List_size].ans = "I";
    List_size++;

    List[List_size].s1 = "F";
    List[List_size].s2 = "(";
    List[List_size].ans = "( E )";
    List_size++;

    List[List_size].s1 = "E1";
    List[List_size].s2 = "#";
    List[List_size].ans = "@";
    List_size++;

    List[List_size].s1 = "T1";
    List[List_size].s2 = "#";
    List[List_size].ans = "@";
    List_size++;
}

struct fouryuan {   //四元式结构体
    string s1;
    string s2;
    string op;
    string res;
} fout[maxn * 10];
int fout_size = 0;  //四元式数量
vector<string> sem;

//void get_fout() {
//
//    sentence = "";
//    char ch;
//    while ((ch = getchar()) != '#') sentence += ch;
//    sentence += "#";
//    cout << sentence << endl;
//    pos = 0;
//    Next();
//
//};

void test_fout() {   //打印四元式
    //get_fout();
    cout << "四元式如下:" << endl;
    for (int i = 0; i < fout_size; i++) {
        cout << "( " << fout[i].op << " " << fout[i].s1 << " " << fout[i].s2 << " " << fout[i].res << " )" << endl;
    }
}

bool match(string x, string word) {   //判断x和word是否是同种单词（例如+与w0是同种单词）
    int is_word(string);
    return x == word || (x == "I" && (is_word(word) == 2 || is_word(word) == 1)) ||
           ((x == "w0") && (word == "+" || word == "-")) || ((x == "w1") && (word == "*" || word == "/"));
}

string FindList(string s1, string s2) {   //查ll(1)分析表
    for (int i = 0; i < List_size; i++) {
        if (List[i].s1 == s1 && match(List[i].s2, s2)) return List[i].ans;
    }
    return "";
}


void judge2() {   //ll(1)分析法判断句子是否合法
    int is_word(string);
    cout << "LL(1)分析法" << endl;
    sentence = "";
    char ch;
    while ((ch = getchar()) != '#') sentence += ch;
    sentence += "#";         //将待识别的句子先读进来
    cout << sentence << endl;
    pos = 0;

    string x;
    stack<string> sta;   //分析栈
    sta.push("#");
    sta.push("E");
    jmp1:   //标记位置便于后续跳转
    Next();
    jmp2:
    x = sta.top();
    sta.pop();
    cout << "w: " << word << " ";
    cout << "x: " << x << " ";

    if (x[0] == 'P') {   //当前栈顶为PUSH(i)
        cout << endl;
        sem.push_back(x.substr(6));
        goto jmp2;
    }
    if (x[0] == 'G') {  //当前栈顶为GEQ()
        cout << endl;
        string s2 = sem.back();
        sem.pop_back();
        string s1 = sem.back();
        sem.pop_back();
        fout[fout_size].s2 = s2;
        fout[fout_size].s1 = s1;
        fout[fout_size].op = x.substr(5);
        //fout[fout_size].res = "t";
        string xuhao = "";
        std::stringstream sstemp;
        sstemp << fout_size;
        xuhao = sstemp.str();
        fout[fout_size].res = "t" + xuhao;
        sem.push_back(fout[fout_size].res);
        fout_size++;
        goto jmp2;
    }

    if (judge_vt(x)) {  //当前单词是终结符，与栈顶进行匹配
        if (match(x, word)) {
            cout << "匹配: " << word << endl;
            goto jmp1;
        } else {  //若无法匹配，则说明语句不合法，输出错误信息
            cout << "Error1" << endl;
            return;
        }
    } else if (judge_vn(x)) {   //当前单词是非终结符，逆序压栈
        string tmp = FindList(x, word);
        cout << "逆序压栈: " << tmp << endl;
        if (tmp != "") {
            int len = tmp.size();
            string ww = "";
            for (int i = len - 1; i >= 0; i--) {
                if (tmp[i] == ' ') continue;
                ww = tmp[i] + ww;
                //cout << ww << 1 << endl;
                if (ww == "T" && i != 0) {   //将对应的语义动作入栈
                    string ss = "GEQ()" + word;
                    sta.push(ss);
                }
                if (ww == "F" && i != 0) {//将对应的语义动作入栈
                    string ss = "GEQ()" + word;
                    sta.push(ss);
                }
                if (ww == "I") {//将对应的语义动作入栈
                    string ss = "PUSH()" + word;
                    sta.push(ss);
                }
                if ((judge_vt(ww) || judge_vn(ww)) && ww != "@") {  //逆序入栈的过程
                    //cout << ww << endl;
                    sta.push(ww);
                    ww = "";
                }
            }
            goto jmp2;
        } else {   //未找到分析表对应内容，不合法，输出错误信息
            cout << "Error2" << endl;
            return;
        }

    } else if (x == "#") {  //当前栈顶为终结符'#'
        cout << endl;
        if (word == "#")
            cout << "该句子是文法的合法语句" << endl;
        else
            cout << "该句子不是文法的合法语句" << endl;
        return;
    }
}

void get_extend() {   //获得slr分析表
    slrlist[0].s[0] = "E";
    slrlist[0].s[1] = "T";
    slrlist[0].s[2] = "w0";
    slrlist[0].s[3] = "w1";
    slrlist[0].s[4] = "(";
    slrlist[0].s[5] = "I";
    slrlist[0].s[6] = "F";
    slrlist[0].ans[0] = "1";
    slrlist[0].ans[1] = "2";
    slrlist[0].ans[2] = "4";
    slrlist[0].ans[3] = "10";
    slrlist[0].ans[4] = "15";
    slrlist[0].ans[5] = "14";
    slrlist[0].ans[6] = "8";
    slrlist[0].size = 7;
    slrlist[1].s[0] = "#";
    slrlist[1].ans[0] = "OK";
    slrlist[1].size = 1;
    slrlist[2].s[0] = "w0";
    slrlist[2].s[1] = ")";
    slrlist[2].s[2] = "#";
    slrlist[2].s[3] = "E1";
    slrlist[2].ans[0] = "4";
    slrlist[2].ans[1] = "r(3)";
    slrlist[2].ans[2] = "r(3)";
    slrlist[2].ans[3] = "3";
    slrlist[2].size = 4;
    slrlist[3].s[0] = "w0";
    slrlist[3].s[1] = "w1";
    slrlist[3].s[2] = "(";
    slrlist[3].s[3] = ")";
    slrlist[3].s[4] = "I";
    slrlist[3].s[5] = "#";
    slrlist[3].size = 6;
    for (int i = 0; i < slrlist[3].size; i++) slrlist[3].ans[i] = "r(1)";
    slrlist[4].s[0] = "T";
    slrlist[4].s[1] = "F";
    slrlist[4].s[2] = "(";
    slrlist[4].s[3] = "I";
    slrlist[4].ans[0] = "5";
    slrlist[4].ans[1] = "8";
    slrlist[4].ans[2] = "15";
    slrlist[4].ans[3] = "14";
    slrlist[4].size = 4;
    slrlist[5].s[0] = "w0";
    slrlist[5].s[1] = ")";
    slrlist[5].s[2] = "#";
    slrlist[5].s[3] = "E1";
    slrlist[5].ans[0] = "4";
    slrlist[5].ans[1] = "r(3)";
    slrlist[5].ans[2] = "r(3)";
    slrlist[5].ans[3] = "6";
    slrlist[5].size = 4;
    slrlist[6].s[0] = "w0";
    slrlist[6].s[1] = "w1";
    slrlist[6].s[2] = "(";
    slrlist[6].s[3] = ")";
    slrlist[6].s[4] = "I";
    slrlist[6].s[5] = "#";
    slrlist[6].size = 6;
    for (int i = 0; i < slrlist[6].size; i++) slrlist[6].ans[i] = "r(2)";
    slrlist[8].s[0] = "w0";
    slrlist[8].s[1] = "w1";
    slrlist[8].s[2] = ")";
    slrlist[8].s[3] = "#";
    slrlist[8].s[4] = "T1";
    slrlist[8].ans[0] = "r(6)";
    slrlist[8].ans[1] = "10";
    slrlist[8].ans[2] = "r(6)";
    slrlist[8].ans[3] = "r(6)";
    slrlist[8].ans[4] = "9";
    slrlist[8].size = 5;
    slrlist[9].s[0] = "w0";
    slrlist[9].s[1] = "w1";
    slrlist[9].s[2] = "(";
    slrlist[9].s[3] = ")";
    slrlist[9].s[4] = "I";
    slrlist[9].s[5] = "#";
    slrlist[9].size = 6;
    for (int i = 0; i < slrlist[9].size; i++) slrlist[9].ans[i] = "r(4)";
    slrlist[10].s[0] = "(";
    slrlist[10].s[1] = "I";
    slrlist[10].s[2] = "F";
    slrlist[10].ans[0] = "15";
    slrlist[10].ans[1] = "14";
    slrlist[10].ans[2] = "11";
    slrlist[10].size = 3;
    slrlist[11].s[0] = "w0";
    slrlist[11].s[1] = "w1";
    slrlist[11].s[2] = ")";
    slrlist[11].s[3] = "#";
    slrlist[11].s[4] = "T1";
    slrlist[11].ans[0] = "r(6)";
    slrlist[11].ans[1] = "10";
    slrlist[11].ans[2] = "r(6)";
    slrlist[11].ans[3] = "r(6)";
    slrlist[11].ans[4] = "12";
    slrlist[11].size = 5;
    slrlist[12].s[0] = "w0";
    slrlist[12].s[1] = "w1";
    slrlist[12].s[2] = "(";
    slrlist[12].s[3] = ")";
    slrlist[12].s[4] = "I";
    slrlist[12].s[5] = "#";
    slrlist[12].size = 6;
    for (int i = 0; i < slrlist[12].size; i++) slrlist[12].ans[i] = "r(5)";
    slrlist[14].s[0] = "w0";
    slrlist[14].s[1] = "w1";
    slrlist[14].s[2] = "(";
    slrlist[14].s[3] = ")";
    slrlist[14].s[4] = "I";
    slrlist[14].s[5] = "#";
    slrlist[14].size = 6;
    for (int i = 0; i < slrlist[14].size; i++) slrlist[14].ans[i] = "r(7)";
    slrlist[15].s[0] = "E";
    slrlist[15].s[1] = "T";
    slrlist[15].s[2] = "w0";
    slrlist[15].s[3] = "w1";
    slrlist[15].s[4] = "(";
    slrlist[15].s[5] = "I";
    slrlist[15].s[6] = "F";
    slrlist[15].ans[0] = "16";
    slrlist[15].ans[1] = "2";
    slrlist[15].ans[2] = "4";
    slrlist[15].ans[3] = "10";
    slrlist[15].ans[4] = "15";
    slrlist[15].ans[5] = "14";
    slrlist[15].ans[6] = "8";
    slrlist[15].size = 7;
    slrlist[16].s[0] = ")";
    slrlist[16].ans[0] = "17";
    slrlist[16].size = 1;
    slrlist[17].s[0] = "w0";
    slrlist[17].s[1] = "w1";
    slrlist[17].s[2] = "(";
    slrlist[17].s[3] = ")";
    slrlist[17].s[4] = "I";
    slrlist[17].s[5] = "#";
    slrlist[17].size = 6;
    for (int i = 0; i < slrlist[17].size; i++) slrlist[17].ans[i] = "r(8)";
}

string Findslrlist(string x, int sta) {  //查找slr分析表
    for (int i = 0; i < slrlist[sta].size; i++) {
        if (match(slrlist[sta].s[i], x)) {
            return slrlist[sta].ans[i];
        }
    }
    return "";
}

int count_popnum(int rr) {  //查找对应的产生式要弹出多少变量
    if (rr == 0 || rr == 7) return 1;
    if (rr == 1 || rr == 4) return 2;
    if (rr == 2 || rr == 5 || rr == 8) return 3;
    return 0;
}

void judge3() {    //slr分析法
    get_extend();
    stack<string> sta;   //分析栈
    sta.push("#");
    sta.push("0");
    int status = 0;
    cout << "SLR(1)分析法" << endl;
    sentence = "";
    char ch;
    while ((ch = getchar()) != '#') sentence += ch;
    sentence += "#";   //将要判断的句子读进来
    cout << sentence << endl;
    pos = 0;
    jmp1:
    Next();
    jmp2:
    cout << "x: " << word << " status: " << status;
    string tmp = Findslrlist(word, status);
    cout << " action: " << tmp << endl;
    if (tmp == "") {  //slr分析表中没有这一项
        cout << "ERROR" << endl;
        return;
    } else {
        int len = tmp.size();
        if (tmp == "OK") {  //分析表中对应内容为“OK"，表示句子读完
            cout << "该句子是文法的合法语句" << endl;
        } else if (tmp[len - 1] <= '9' && tmp[len - 1] >= '0') {  //状态转换
            sta.push(tmp);
            if (tmp[len - 1] >= '0' && tmp[len - 1] <= '9') {
                if (tmp[len - 2] >= '0' && tmp[len - 2] <= '9')
                    status = ((tmp[len - 2] - '0') * 10 + tmp[len - 1] - '0');
                else status = tmp[len - 1] - '0';
            }
            goto jmp1;
        } else {  //分析表中的内容为规约状态
            int rr = tmp[2] - '0';   //规约态对应的产生式
            //cout << rr << endl;
            int num = count_popnum(rr);  //弹出元素的个数
            //cout << num << endl;
            while (num > 0) {
                num--;
                //cout << sta.top() << endl;
                sta.pop();
            }
            if (sta.empty()) {
                cout << "该句子不是文法的合法语句" << endl;
                return;
            }
            string ss = sta.top();
            //cout << ss << endl;
            len = ss.size();
            if (ss[len - 1] >= '0' && ss[len - 1] <= '9') {
                if (ss[len - 2] >= '0' && ss[len - 2] <= '9')
                    status = ((ss[len - 2] - '0') * 10 + ss[len - 1] - '0');
                else status = ss[len - 1] - '0';
            }
            //cout << status << endl;
            string li;
            if (rr == 1) li = "E";
            if (rr == 2 || rr == 3) li = "E1";
            if (rr == 4) li = "T";
            if (rr == 5 || rr == 6) li = "T1";
            if (rr == 7 || rr == 8) li = "F";
            string ttmp = Findslrlist(li, status);
            //cout << ttmp << endl;
            if (ttmp != "") {
                sta.push(ttmp);
                int len = ttmp.size();
                if (ttmp[len - 1] >= '0' && ttmp[len - 1] <= '9') {
                    if (ttmp[len - 2] >= '0' && ttmp[len - 2] <= '9')
                        status = ((ttmp[len - 2] - '0') * 10 + ttmp[len - 1] - '0');
                    else status = ttmp[len - 1] - '0';
                }
                //cout << status << endl;
            }
            goto jmp2;
        }

    }
}

void get_OPAM() {  //得到算符优先分析表
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            OPAM[i][j] = -2;
        }
    }
    OPAM[0][0] = 1;
    OPAM[0][1] = -1;
    OPAM[0][2] = -1;
    OPAM[0][3] = -1;
    OPAM[0][4] = 1;
    OPAM[0][5] = 1;

    OPAM[1][0] = 1;
    OPAM[1][1] = 1;
    OPAM[1][2] = -1;
    OPAM[1][3] = -1;
    OPAM[1][4] = 1;
    OPAM[1][5] = 1;

    OPAM[2][0] = 1;
    OPAM[2][1] = 1;
    OPAM[2][4] = 1;
    OPAM[2][5] = 1;

    OPAM[3][0] = -1;
    OPAM[3][1] = -1;
    OPAM[3][2] = -1;
    OPAM[3][3] = -1;
    OPAM[3][4] = 0;

    OPAM[4][0] = 1;
    OPAM[4][1] = 1;
    OPAM[4][4] = 1;
    OPAM[4][5] = 1;

    OPAM[5][0] = -1;
    OPAM[5][1] = -1;
    OPAM[5][2] = -1;
    OPAM[5][3] = -1;
    OPAM[5][5] = 0;
}

int Findid(string s1) {  //终结符单词对应的下标
    if (s1 == "#") return 5;
    for (int i = 0; i < vt_size; i++) {
        if (match(vt[i], s1)) return i;
    }
    return -1;//不是终结符
}

int Find_OPAM(int a, int b) {
    return OPAM[a][b];
}

bool Generate(string s1, string s2) {    //判断s1能否产生s2
    if (s1 == s2) return true;
    int len = s1.size();
    for (int i = 0; i < len; i++) {
        string tmp = "";
        tmp += s1[i];
        if (s1[i + 1] <= '9' && s1[i + 1] >= '0')
            tmp += s1[i + 1];
        if (judge_vt(tmp)) {
            if (tmp == s2) return true;
        }
    }
}

void judge4() {   //算符优先分析判断句子是否合法
    get_OPAM();
    vector<string> sta;  //分析栈
    sta.push_back("#");
    int top = 0;  //栈顶非终结符
    cout << "算符优先分析法" << endl;
    sentence = "";
    char ch;
    while ((ch = getchar()) != '#') sentence += ch;
    sentence += "#";   //将要判断的句子读进来
    cout << sentence << endl;
    pos = 0;
    jmp1:
    Next();
    int kk = top;   //第一个非终结符
    if (Findid(sta[top]) == -1) kk = top - 1;
    jmp2:
    cout << "栈顶非终结符: " << sta[kk] << " x: " << word;
    if (word == "#" && sta[kk] == "#") {
        cout << endl;
        cout << "该句子是文法的合法语句" << endl;
        return;
    }
    int tmp = Find_OPAM(Findid(sta[kk]), Findid(word));
    if (tmp == -2) {   //未查到对应表项
        cout << "ERRor" << endl;
        return;
    } else {
        if (tmp == -1) {  //表项为<
            cout << " priority: <" << endl;
            sta.push_back(word);
            top++;
            //cout << top << endl;
            goto jmp1;
        } else if (tmp == 0) {  //表项为=
            cout << " priority: =" << endl;
            sta.push_back(word);
            top++;
            goto jmp1;
        } else if (tmp == 1) {   //表项为>
            cout << " priority: >" << endl;
            //cout << sta[top] << endl;
            int pp;
            do {   //找到左边第一个<
                pp = kk;
                if (Findid(sta[kk - 1]) == -1)
                    kk = kk - 2;
                else kk = kk - 1;
            } while (Find_OPAM(Findid(sta[kk]), Findid(sta[pp])) == 1 ||
                     Find_OPAM(Findid(sta[kk]), Findid(sta[pp])) == 0);
            //cout << sta[kk] << endl;
            string ri = "";
            string key;
            for (int i = kk + 1; i <= top; i++) {  //<与>中间的部分构成的表达式
                if (i > kk + 1) ri += " ";
                int id = Findid(sta[i]);
                if (id == -1) {
                    ri += sta[i];
                } else {
                    ri += vt[id];
                    key = vt[id];
                }
            }
            cout << ri << endl;
            for (int i = kk + 1; i <= top; i++) {  //将这部分表达式从栈中弹出来
                sta.pop_back();
            }
            //test1_output();
            bool tag = false;
            for (int i = 0; i < cnt; i++) {
                if (tag) break;
                for (int j = 0; j < p[i].size; j++) {
                    //cout << p[i].ri[j] << endl;
                    if (Generate(p[i].ri[j], key)) {   //判断这个右部能否产生目标表达式
                        cout << p[i].li << endl;
                        sta.push_back(p[i].li);
                        tag = true;
                        break;
                    }
                }
            }
            if (!tag) {  //无法产生则输出错误信息
                cout << "ERRor2" << endl;
                return;
            }
            top = kk + 1;
            goto jmp2;
        }
    }
}


int main() {
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
        //cerr<<p[cnt].li<<endl;
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
    test_analyst();
    cout << "词法分析器运行结果如下：" << endl;
    get_token();
    freopen("input2.txt", "r", stdin);
    judge1();  //递归下降子程序法
    freopen("input2.txt", "r", stdin);
    judge2(); //LL(1)分析
    //freopen("input2.txt", "r", stdin);
    test_fout();
    freopen("input2.txt", "r", stdin);
    judge3(); //SLR(0)分析法
    freopen("input2.txt", "r", stdin);
    judge4();
    return 0;
}
