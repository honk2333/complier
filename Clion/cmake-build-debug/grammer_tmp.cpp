#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<map>
#include<set>
#include<stack>
#include<string>

using namespace std;

const int maxn = 1010;


stack<string> s_solve;
stack<string> stack_str;
char sstr[maxn];
map<string, int> Vt; //终结符

map<string, int> I{make_pair("a", 1), make_pair("b", 2), make_pair("c", 3), make_pair("d", 4),
                   make_pair("ch", 5), make_pair("x", 6), make_pair("v", 7)};    //标识符
map<string, int> C{make_pair("'a'", 1)};    //c字符
map<string, int> S{make_pair("\"ok\"", 1)};    //S字符串
map<string, int> c{make_pair("1", 1), make_pair("2", 2), make_pair("10", 3)};    //c常数
map<string, int> K{make_pair("int", 1), make_pair("main", 2), make_pair("void", 3), make_pair("if", 4),
                   make_pair("char", 5), make_pair("else", 6)};    //k关键字
map<string, int> p{make_pair("<=", 1), make_pair("=", 2), make_pair("(", 3), make_pair(")", 4), make_pair("{", 5),
                   make_pair("}", 6),
                   make_pair("+", 7), make_pair("==", 8), make_pair("[", 9), make_pair("]", 10), make_pair(";", 11),
                   make_pair(",", 12), make_pair("*", 13),
                   make_pair("-", 14), make_pair("/", 15), make_pair("#", 16), make_pair("!=", 17), make_pair(">=", 18),
                   make_pair(">", 19), make_pair("<", 20)};    //p界符

char reprograme[maxn];

typedef struct {
    string tnow_w;
    string tnow_kind;
    int tnow_num;
} T;

T token[maxn];

int now_pointer;
int C_num = 2, I_num = 8, S_num = 2, c_num = 4, K_num = 6, p_num = 13;


/*
void print(int steps, stack<string> stk, string x, string w, string wf, stack<string> SEM_temp, stack<QT_str> QT_temp) {
    printf("%-5d", steps);
    printf("%-5s", x.c_str());
    printf("%-5s", w.c_str());
    printf("%-20s", wf.c_str());
    string out2 = "";
    string out = "";
    while (!stk.empty()) {
        out = stk.top() + out;
        stk.pop();
    }
    printf("%-20s", out.c_str());
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
}*/






int main() {

    make_table();
    cout << "输入字符串" << endl;
    scanf("%s", sstr);

    solve2(sstr);

    for (int i = now_pointer - 1; i >= 0; i--) {
        //cout << "[" << token[i].tnow_w << "," << token[i].tnow_kind << "," << token[i].tnow_num << "]" << endl;
        string temp_sstr;
        if (token[i].tnow_kind == "c" || token[i].tnow_kind == "I") {
            stack_str.push(token[i].tnow_w);
        } else {
            stack_str.push(token[i].tnow_w);
        }
    }
    /*for (int i = sstr.length() - 1; i >= 0; i--) {
    string temp_sstr;
    temp_sstr.push_back(sstr[i]);
    stack_str.push(temp_sstr);
    }*/
    s_solve.push(temp_str);
    s_solve.push(start_word);
    solve_QT();
    cout << "输入产生式条数: " << endl;

    return 0;
}

