//
// Created by Honk on 2019/11/28.
//

#include"complier.h"

using namespace std;

/*
string sentence;  //要处理的句子
string word;  //当前单词
int pos;//下一个单词的起始位置*/

/*
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
}*/

/*struct fouryuan {   //四元式结构体
    string s1;
    string s2;
    string op;
    string res;
} fout[maxn * 10];
int fout_size = 0;  //四元式数量
vector <string> sem;*/

extern production np[], p[];
extern int cnt, dnt;
extern int vt_size;
extern string vt[];
map<pair<string, string>, int> L_table;
map<int, set<string>> select;
map<int, string> Left_fan;
map<string, int> Vt; //终结符
map<string, int> Left;  //可做非终结符

void convert_for_table() {
    int ccnt = 0;
    for (int i = 0; i < dnt; i++) {  //给产生式标号
        for (int j = 0; j < np[i].size; j++) {
            np[i].id[j] = ccnt++;
        }
    }
    for (int i = 0; i < dnt; i++) {  //改变select集数据结构
        for (int j = 0; j < np[i].size; j++) {
            select[np[i].id[j]] = np[i].select[j];
            Left_fan[np[i].id[j]] = np[i].li;
        }
        Left[np[i].li] = i;
    }
/*    for (int i = 0; i < ccnt; i++) {
        for (auto it = select[i].begin(); it != select[i].end(); it++)
            cout << (*it) << endl;
    }*/
    for (int i = 0; i < vt_size; i++) {
        Vt[vt[i]] = i;
    }
};


//生成分析表
void make_table() {
    convert_for_table();
    cout << "\n分析表\n\n";
    map<int, set<string >>::iterator
            it = select.begin();
    for (; it != select.end(); it++) {
        string left_str;
        left_str = Left_fan[it->first];
        set<string> &temp = it->second;
        set<string>::iterator itl = temp.begin();
        for (; itl != temp.end(); itl++) {
            L_table[make_pair(left_str, *itl)] = it->first;
        }
    }
    set<string> visi_table;
    vector<string> letter;
    map<string, int>::iterator it_Vt = Vt.begin();
    for (; it_Vt != Vt.end(); it_Vt++) {
        letter.push_back(it_Vt->first);
    }
    letter.push_back("#");
    for (int i = 0; i <= (int) (letter.size() + 1) * 10; i++)
        printf("-");
    puts("");
    printf("|%9s", "|");
    for (int i = 0; i < (int) letter.size(); i++)
        printf("%5s%5s", letter[i].c_str(), "|");
    puts("");
    for (int i = 0; i <= (int) (letter.size() + 1) * 10; i++)
        printf("-");
    puts("");
    map<string, int>::iterator it_table = Left.begin();
    for (; it_table != Left.end(); it_table++) {
        //if (visi_table.count(it_table->first.first))break;
        //visi_table.insert(it_table->first.first);
        //cout << it_table->first.first << it_table->first.second << it_table->second << endl;
        printf("|%5s%4s", it_table->first.c_str(), "|");
        for (int j = 0; j < (int) letter.size(); j++) {
            if (L_table.count(make_pair(it_table->first, letter[j])))
                printf("%7d%3s", L_table[make_pair(it_table->first, letter[j])], "|");
            else printf("%10s", "|");
        }
        puts("");
        for (int i = 0; i <= (int) (letter.size() + 1) * 10; i++)
            printf("-");
        puts("");
    }
}

vector<string> Right;
stack<string> s_solve;
stack<string> stack_str;
extern struct token token[];

void convert_for_ll1() {
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            Right.push_back(np[i].ri[j]);
        }
    }

}

//生成带翻译文法的Right集
void new_Right() {
    for (int i = 0; i < (int) Right.size(); i++) {
        for (int j = 0; j < (int) Right[i].size(); j++) {
            if (Right[i][j] == 'i' && Right[i][j + 1] != 'f')Right[i].insert(j + 1, "P");
            else if (Right[i][j] == '+') {
                Right[i].insert(j + 2, "{+}");
                j = j + 4;
            } else if (Right[i][j] == '-') {
                Right[i].insert(j + 2, "{-}");
                j = j + 4;
            } else if (Right[i][j] == '*') {
                Right[i].insert(j + 2, "{*}");
                j = j + 4;
            } else if (Right[i][j] == '/') {
                Right[i].insert(j + 2, "{/}");
                j = j + 4;
            } else if (Right[i][j] == '=') {
                Right[i].insert(j + 2, "{=}");
                j = j + 4;
            } else if (Right[i][j] == 'if') {
                int idx = Right[i].find(')', j);
                Right[i].insert(idx + 1, "{IF}");
                j = idx + 4;
            }
        }
    }
}

typedef struct {
    string Operator;
    string Operand1;
    string Operand2;
    string result;
} QT_str;

string rec_str;
stack<string> SEM;
stack<QT_str> QT;
int QT_cnt = 1;


//输出分析过程前部
void print_front(int steps, stack<string> stk, string x, string w) {
    printf("%-5d", steps);
    string out = "";
    while (!stk.empty()) {
        out = stk.top() + out;
        stk.pop();
    }
    printf("%-25s", out.c_str());
    printf("%-5s", x.c_str());
    printf("%-5s", w.c_str());
}

//输出分析过程后部
void print_end(string wf, stack<string> SEM_temp, stack<QT_str> QT_temp) {
    printf("%-20s", wf.c_str());
    string out2 = "";
    while (!SEM_temp.empty()) {
        out2 = SEM_temp.top() + out2;
        SEM_temp.pop();
    }
    printf("%-20s", out2.c_str());
    QT_str qt;
    if (!QT_temp.empty()) {
        qt = QT_temp.top();
        cout << "(" << qt.Operator << "," << qt.Operand1 << "," << qt.Operand2 << "," << qt.result << ")";
    }
    cout << endl;
}

//生成四元式
void GEQ(string oper) {
    string oper2 = SEM.top();
    SEM.pop();
    string oper1 = SEM.top();
    SEM.pop();
    QT_str Q_str;
    string str = to_string(QT_cnt);
    if (oper == "=") {
        Q_str.Operator = oper;
        Q_str.Operand1 = oper2;
        Q_str.Operand2 = "_";
        Q_str.result = oper1;
    } else if (oper == "IF") {
        Q_str.Operator = oper;
        Q_str.Operand1 = oper2;
        Q_str.Operand2 = "_";
        Q_str.result = "_";
    } else {
        //cout << "oper1: "<<oper1 << "oper2: " << oper2 << endl;
        Q_str.Operator = oper;
        Q_str.Operand1 = oper1;
        Q_str.Operand2 = oper2;
        Q_str.result = "t" + str;
        SEM.push("t" + str);
    }
    QT.push(Q_str);
    QT_cnt++;
}

extern map<string, int> II;
extern map<string, int> CC;    //c字符
extern map<string, int> SS;    //S字符串
extern map<string, int> cc;    //c常数
extern map<string, int> KK;    //k关键字
extern map<string, int> pp;    //p界符
//四元式分析过程
void solve_QT() {
    //printf("1111\n");
    new_Right();
    string w, x, front_w;
    //NEXT(w)
    w = stack_str.top();
    string str_end = "#";
    string str_$ = "$";
    int steps = 0;
    printf("%-5s%-25s%-5s%-5s%-20s%-20s%-20s\n", "步骤", "符号栈", "x", "w", "所用产生式", "SEM[m]", "QT");
    while (!s_solve.empty()) {
        string temp_w;
        if (II.count(w) || cc.count(w))temp_w = "i";
        else temp_w = w;
        string tmp = "";
        //POP(x)
        x = s_solve.top();
        print_front(steps++, s_solve, x, w);
        s_solve.pop();
        //x属于Vt
        if (Vt.count(x)) {
            //x=w?
            if (x == temp_w) {
                //NEXT(w)
                front_w = w;
                tmp = "NEXT(W)";
                stack_str.pop();
                w = stack_str.top();
            } else {
                cout << "error" << endl;
                break;
            }
        }
            //x属于Vn?
        else if (Left.count(x)) {
            //查表
            int idx = L_table[make_pair(x, temp_w)];
            //压栈
            //cout << "idx "<<idx << endl;
            tmp = Right[idx];
            for (int i = tmp.length() - 1; i >= 0; i--) {
                if (tmp[i] == '\'') {
                    string v = tmp.substr(i - 1, 2);
                    //cout << "v：" << v << endl;
                    s_solve.push(v);
                    i = i - 1;
                } else if (tmp[i] == '}') {
                    string v = tmp.substr(i - 2, 3);
                    s_solve.push(v);
                    i = i - 2;
                } else {
                    string v = tmp.substr(i, 1);
                    // << "v：" << v << endl;
                    if (v != str_$)s_solve.push(v);
                }
            }
            tmp = Left_fan[idx] + "->" + tmp;
        } else if (x == "P") {
            SEM.push(front_w);
        } else if (x == "{+}" || x == "{-}" || x == "{*}" || x == "{/}" || x == "{=}" || x == "{IF}") {
            string temp_GEQ;
            for (int i = 0; i < (int) x.size(); i++) {
                if (x[i] != '{' && x[i] != '}') {
                    temp_GEQ.push_back(x[i]);
                    //cout << temp_GEQ << endl;
                    //system("pause");
                }
            }
            //temp_GEQ.push_back(x[1]);
            GEQ(temp_GEQ);
        } else if (w == str_end) {
            cout << "succeed" << endl;
            break;
        } else {
            cout << "error" << endl;
            break;
        }
        //print(steps++, s_solve, x, w, tmp, SEM, QT);
        print_end(tmp, SEM, QT);
    }
}

/*
void test_fout() {   //打印四元式
    //get_fout();
    cout << "四元式如下:" << endl;
    for (int i = 0; i < fout_size; i++) {
        cout << "( " << fout[i].op << " " << fout[i].s1 << " " << fout[i].s2 << " " << fout[i].res << " )" << endl;
    }
}*/

bool match(string x, string word) {   //判断x和word是否是同种单词（例如+与w0是同种单词）
    int is_word(string);
    return x == word || (x == "I" && (is_word(word) == 2 || is_word(word) == 1)) ||
           ((x == "w0") && (word == "+" || word == "-")) || ((x == "w1") && (word == "*" || word == "/"));
}

void ll1() {
    make_table();

}
