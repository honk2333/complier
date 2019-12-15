//
// Created by Honk on 2019/11/28.
//

#include"complier.h"

using namespace std;
string sentence;  //要处理的句子
string word;  //当前单词
int pos;//下一个单词的起始位置*/


void Next() {   //下一个单词
    char c;
    word = "";
    int len = sentence.size();
    //cout << sentence << endl;
    while (sentence[pos] == ' ' || sentence[pos] == '\n') pos++;
    for (int i = len - 1; i >= pos; i--) {   //查找单词表，优先匹配长度长的单词
        string tmp = sentence.substr(pos, i - pos + 1);
        //cout<<tmp<<endl;
        if (tmp == "if") putchar('!');
        if (is_word(tmp) > 0 || tmp == "#") {
            word = tmp;
            pos = i + 1;
            //cout <<"word  "<< word << endl;
            return;
        }
    }
    return;
}

extern production np[], pron[];
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
    for (int i = 0; i <= (int) (letter.size()) * 10 - 1; i++)
        printf("-");
    puts("");
    printf("|%15s", "|");
    for (int i = 0; i < (int) letter.size(); i++)
        printf("%7s%s", letter[i].c_str(), "|");
    puts("");
    for (int i = 0; i <= (int) (letter.size()) * 10 - 1; i++)
        printf("-");
    puts("");
    map<string, int>::iterator it_table = Left.begin();
    for (; it_table != Left.end(); it_table++) {
        printf("|%-14s%s", it_table->first.c_str(), "|");
        for (int j = 0; j < (int) letter.size(); j++) {
            if (L_table.count(make_pair(it_table->first, letter[j])))
                printf("%7d%s", L_table[make_pair(it_table->first, letter[j])], "|");
            else printf("%8s", "|");
        }
        puts("");
        for (int i = 0; i <= (int) (letter.size()) * 10 - 1; i++)
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
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            stringstream ss(np[i].ri[j]);
            string right_next;
            while (ss >> right_next) {
                //cout << right_next << endl;
                //cout<<"11111111111111"<<endl;
                if (right_next == "id") {
                    int idx = np[i].ri[j].find(right_next, 0);
                    np[i].ri[j].insert(idx + right_next.size(), " P");
                } else if (right_next == "if") {
                    int idx = np[i].ri[j].find(')', 0);
                    np[i].ri[j].insert(idx + 1, " {IF}");
                    np[i].ri[j].insert(np[i].ri[j].size(), " {IE}");
                } else if (right_next == "w0") {
                    int idx = np[i].ri[j].find("w0", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {w0}");
                } else if (right_next == "w1") {
                    int idx = np[i].ri[j].find("w1", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {w1}");
                } else if (right_next == "cons") {
                    int idx = np[i].ri[j].find(right_next, 0);
                    np[i].ri[j].insert(idx + right_next.size(), " P");
                } else if (right_next == "=") {
                    int idx = np[i].ri[j].find("=", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {=}");
                } else if (right_next == "else") {
                    int idx = np[i].ri[j].find("else", 0);
                    np[i].ri[j].insert(idx + right_next.size(), " {EL}");
                } else if (right_next == "while") {
                    int idx = np[i].ri[j].find("while", 0);
                    np[i].ri[j].insert(idx + right_next.size(), " {WH}");
                    np[i].ri[j].insert(np[i].ri[j].size(), " {WE}");
                } else if (right_next == ">") {
                    int idx = np[i].ri[j].find(">", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {>}");
                } else if (right_next == "<") {
                    int idx = np[i].ri[j].find("<", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {<}");
                } else if (right_next == "<=") {
                    int idx = np[i].ri[j].find("<=", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {<=}");
                } else if (right_next == ">=") {
                    int idx = np[i].ri[j].find(">=", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {>=}");
                } else if (right_next == "==") {
                    int idx = np[i].ri[j].find("==", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {==}");
                } else if (right_next == "!=") {
                    int idx = np[i].ri[j].find("!=", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {!=}");
                }
            }
        }
    }
}

string rec_str;
stack<string> SEM;
stack<QT_str> QT;
int QT_cnt = 1;


vector<QT_str> QT_arr;

//get QT_arr
vector<QT_str> get_QT() {
    stack<QT_str> QT_tmp;
    while (!QT.empty()) {
        QT_str qt_tmp = QT.top();
        //cout<<"("<<qt_tmp.Operator<<","<<qt_tmp.Operand1<<","<<qt_tmp.Operand2<<","<<qt_tmp.result<<")"<<endl;
        QT.pop();
        QT_tmp.push(qt_tmp);
    }
    while (!QT_tmp.empty()) {
        QT_str qt_tmp = QT_tmp.top();
        QT_tmp.pop();
        QT_arr.push_back(qt_tmp);
    }
    for (int i = 0; i < (int) QT_arr.size(); i++) {
        cout << "(" << QT_arr[i].Operator << "," << QT_arr[i].Operand1 << "," << QT_arr[i].Operand2 << ","
             << QT_arr[i].result << ")" << endl;

    }
    return QT_arr;
}

//输出分析过程前部
void print_front(int steps, stack<string> stk, string x, string w) {
    printf("%-5d", steps);
    string out = "";
    while (!stk.empty()) {
        out = stk.top() + out;
        stk.pop();
    }
    printf("%-35s", out.c_str());
    printf("%-10s", x.c_str());
    printf("%-10s", w.c_str());
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
    QT_str Q_str;
    string str = to_string(QT_cnt);
    if (oper == "=") {
        string oper2 = SEM.top();
        SEM.pop();
        string oper1 = SEM.top();
        SEM.pop();
        Q_str.Operator = oper;
        Q_str.Operand1 = oper2;
        Q_str.Operand2 = "_";
        Q_str.result = oper1;
    } else if (oper == "IF") {
        string oper2 = SEM.top();
        SEM.pop();
        Q_str.Operator = oper;
        Q_str.Operand1 = oper2;
        Q_str.Operand2 = "_";
        Q_str.result = "_";

    } else if (oper == "IE" || oper == "WH") {
        Q_str.Operator = oper;
        Q_str.Operand1 = "_";
        Q_str.Operand2 = "_";
        Q_str.result = "_";
    } else if (oper == "EL" || oper == "WE") {
        Q_str.Operator = oper;
        Q_str.Operand1 = "_";
        Q_str.Operand2 = "_";
        Q_str.result = "_";
    } else {
        string oper2 = SEM.top();
        SEM.pop();
        string oper1 = SEM.top();
        SEM.pop();
        //cout << "oper1: "<<oper1 << "oper2: " << oper2 << endl;
        Q_str.Operator = oper;
        Q_str.Operand1 = oper1;
        Q_str.Operand2 = oper2;
        Q_str.result = "t" + str;
        SEM.push("t" + str);
        QT_cnt++;
    }
    QT.push(Q_str);
    //QT_cnt_num++;
}

extern map<string, int> II;
extern map<string, int> CC;    //c字符
extern map<string, int> SS;    //S字符串
extern map<string, int> cc;    //c常数
extern map<string, int> KK;    //k关键字
extern map<string, int> pp;    //p界符
string w0 = "w0";
string w1 = "w1";


//四元式分析过程
void solve_QT() {
    string w, x, front_w;
    //NEXT(w)
    freopen("test.txt", "r", stdin);
    char ch;
    sentence = "";
    pos = 0;
    while ((ch = getchar()) != '#') {
        sentence += ch;
        //putchar(ch);
    }
    sentence += '#';
    //cout << sentence << endl;
    Next();
    w = word;
    cout << w << endl;
    //w = stack_str.top();
    string str_end = "#";
    string str_$ = "$";
    int steps = 0;
    s_solve.push("#");
    s_solve.push("MAINPRO");
    printf("%-10s%-35s%-10s%-10s%-20s%-20s%-20s\n", "步骤", "符号栈", "x", "w", "所用产生式", "SEM[m]", "QT");
    while (!s_solve.empty()) {
        string temp_w;
        if (II.count(w))temp_w = "id";
        else if (is_word(w) == 2)temp_w = "cons";
        else if (match("w0", w))temp_w = "w0";
        else if (match("w1", w))temp_w = "w1";
        else temp_w = w;
        //temp_w=w;
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
                //stack_str.pop();
                Next();
                w = word;
                //w = stack_str.top();
            } else {
                cout << "error" << endl;
                break;
            }
        }
            //x属于Vn?
        else if (Left.count(x)) {
            //查表
            int id = L_table[make_pair(x, temp_w)];
            //压栈
            //cout << "idx "<<idx << endl;
            for (int i = 0; i < dnt; i++) {
                for (int j = 0; j < np[i].size; j++) {
                    if (np[i].id[j] == id)tmp = np[i].ri[j];
                }
            }
            stack<string> tmp_right;
            //tmp = np[id];
            stringstream ss(tmp);
            string right_next;
            while (ss >> right_next) {
                if (right_next == "{w0}" || right_next == "{w1}") tmp_right.push("{" + w + "}");
                else if (right_next != "@") tmp_right.push(right_next);
            }
            while (!tmp_right.empty()) {
                string v = tmp_right.top();
                tmp_right.pop();
                s_solve.push(v);
            }

        } else if (x == "P") {
            SEM.push(front_w);
        } else if (x == "{-}" || x == "{+}" || x == "{*}" || x == "{/}" || x == "{=}" || x == "{IF}" || x == "{EL}" ||
                   x == "{IE}" || x == "{WH}" || x == "{WE}" || x == "{>}" || x == "{<}" || x == "{>=}" ||
                   x == "{<=}" || x == "{!=}" || x == "{==}") {
            string temp_GEQ;
            //if (x[1] == 'w') temp_GEQ = w;
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

bool match(string x, string word) {   //判断x和word是否是同种单词（例如+与w0是同种单词）
    int is_word(string);
    return x == word || (x == "cons" && (is_word(word) == 2)) || (x == "id" && (is_word(word) == 1)) ||
           ((x == "w0") && (word == "+" || word == "-")) || ((x == "w1") && (word == "*" || word == "/"));
}

void ll1() {
    make_table();
    new_Right();
    /*  for (int i = 0; i < dnt; i++) {
          //cout << p[i].size << endl;
          for (int j = 0; j < np[i].size; j++) {
              cout << np[i].li << " -> ";
              cout << np[i].ri[j] << endl;
          }
      }*/
    solve_QT();
    get_QT();
}
