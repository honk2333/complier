//
// Created by Honk on 2019/11/28.
//

#include"complier.h"
#include"QDebug"
#include"QMessageBox"
#include"QFile"

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
            if(word=="//") Next();
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

//vector<int>S_idx;
vector<QT_str> QT_arr;
stack<int>FE_idx;
stack<int>C_idx;
int num_for=0;
int B_idx=0;
int S_idx=0;
int FUN_idx=0;
stack<int>break_idx;
stack<int>continue_idx;
stack<int>FOR_AND_WH_idx;
string FUN_str;
map<string,int>FUN_str_idx;

void change_QT(MyWindow* ww){
    bool first_eq=1;
    for(int i=0;i<QT_arr.size();i++){
        //cout <<"["<<QT_arr[i].QT_num<<"]"<<"(" << QT_arr[i].Operator << "," << QT_arr[i].Operand1 << "," << QT_arr[i].Operand2 << ","
             //<< QT_arr[i].result << ")" << endl;
        if(QT_arr[i].Operator=="PROGRAM"){
            FUN_idx=QT_arr[i].QT_num;
            FUN_str=QT_arr[i].Operand1;
        }
        if(QT_arr[i].Operator=="JMP"&&QT_arr[i].Operand1=="FUN"){
            FUN_str_idx[QT_arr[i].result]=QT_arr[i].QT_num;
        }
        if(QT_arr[i].Operand1=="RETURN"&&FUN_str!="main"){
            string return_str=to_string(FUN_str_idx[FUN_str]);
            QT_arr[i].result=return_str;
        }
        if(QT_arr[i].Operator=="END"){
            QT_arr[i].Operand1=QT_arr[FUN_idx].Operand1;
        }
        if(QT_arr[i].Operand1=="BREAK"){
            break_idx.push(i);
        }
        if(QT_arr[i].Operator=="WH") {
            FOR_AND_WH_idx.push(i);
        }
        if(QT_arr[i].Operand1=="CONTINUE"){
            if(!FOR_AND_WH_idx.empty()){
                string str_FE_AND_WE = to_string(FOR_AND_WH_idx.top());
                QT_arr[i].result=str_FE_AND_WE;
                FOR_AND_WH_idx.pop();
            }
        }
        if(QT_arr[i].Operator=="FOR"){
            FOR_AND_WH_idx.push(i);
            num_for++;
            first_eq=1;
        }
        if(QT_arr[i].Operator=="WE"){

            if(!break_idx.empty()) {
                string str_BRE = to_string(i);
                QT_arr[break_idx.top()].result = str_BRE;
                break_idx.pop();
            }
        }
        if(num_for>0){
            if(QT_arr[i].Operator=="="&&first_eq){
                B_idx=QT_arr[i+1].QT_num;
                first_eq=0;
            }
            if(QT_arr[i].result=="FE"){
                C_idx.push(QT_arr[i+1].QT_num);
                FE_idx.push(i);
            }
            if(QT_arr[i].result=="B"){
                S_idx=QT_arr[i+1].QT_num;
                string str_B = to_string(B_idx);
                QT_arr[i].result= str_B;
                //修J<
                string str_S=to_string(S_idx);
                QT_arr[B_idx].Operator="J"+QT_arr[B_idx].Operator;
                QT_arr[B_idx].result=str_S;
            }
            if(QT_arr[i].result=="C"){
                string str_C=to_string(C_idx.top());
                C_idx.pop();
                QT_arr[i].result=str_C;
            }
            if(QT_arr[i].Operator=="FE"){
                num_for--;
                string str_FE=to_string(i);
                QT_arr[FE_idx.top()].result=str_FE;
                FE_idx.pop();
                //修改break的result
                if(!break_idx.empty()) {
                    string str_BRE = to_string(i);
                    QT_arr[break_idx.top()].result = str_BRE;
                    break_idx.pop();
                }
            }
        }
    }

    ww->clear_fout();
    for (int i = 0; i < (int) QT_arr.size(); i++) {
        string ss = "[" +  to_string(QT_arr[i].QT_num) + "]" + "(" + QT_arr[i].Operator + "," + QT_arr[i].Operand1 +","+QT_arr[i].Operand2+ "," + QT_arr[i].result + ")";
        QString sss=QString::fromStdString(ss);
        ww->print_fout(sss);
    }
}

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
    //cout << "\n分析表\n\n";
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
    /*set<string> visi_table;
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
    }*/

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
extern production save_np[];
//生成带翻译文法的Right集
void new_Right() {
    for(int i=0;i<dnt;i++){
        save_np[i].size=np[i].size;
        save_np[i].follow=np[i].follow;
        save_np[i].li=np[i].li;
        for(int j=0;j<np[i].size;j++){
            save_np[i].ri[j]=np[i].ri[j];
            save_np[i].first[j]=np[i].first[j];
            save_np[i].select[j]=np[i].select[j];
            save_np[i].id[j]=np[i].id[j];
        }
    }
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            stringstream ss(np[i].ri[j]);
            string right_next;
            while (ss >> right_next) {
                //cout << right_next << endl;
                //cout<<"11111111111111"<<endl;
                if (right_next == "id"&&np[i].li!="SUBPRO"&&np[i].li!="DEF_SEN"&&np[i].li!="SUBPRO_") {
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
                    int idx2 = np[i].ri[j].find(')', 0);
                    np[i].ri[j].insert(idx2 + 1, " {DO}");
                    np[i].ri[j].insert(idx + right_next.size(), " {WH}");
                    np[i].ri[j].insert(np[i].ri[j].size(), " {WE}");
                } else if (right_next == ">"&&np[i].li!="Head_file") {
                    int idx = np[i].ri[j].find(">", 0);
                    np[i].ri[j].insert(idx + right_next.size() + 2, " {>}");
                } else if (right_next == "<"&&np[i].li!="Head_file") {
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
                }else if (right_next == "id"&&np[i].li=="SUBPRO") {
                    int idx = np[i].ri[j].find("id", 0);
                    np[i].ri[j].insert(idx + right_next.size(), " {id}");
                    np[i].ri[j].insert(np[i].ri[j].size()," {EN}");
                    //np[i].ri[j].insert(np[i].ri[j].size(), " {EN}");
                }else if (right_next == "main"&&np[i].li=="MAINPRO") {
                    int idx = np[i].ri[j].find("main", 0);
                    np[i].ri[j].insert(idx + right_next.size(), " {main}");
                    np[i].ri[j].insert(np[i].ri[j].size()," {EN}");
                }else if(right_next=="break"){
                    np[i].ri[j].insert(np[i].ri[j].size()," {BREAK}");
                }
                else if(right_next=="continue") {
                    np[i].ri[j].insert(np[i].ri[j].size(), " {CONTINUE}");
                }
                else if(right_next=="return"){
                    np[i].ri[j].insert(np[i].ri[j].size()," {RETURN}");
                }
                else if(right_next=="ELIST"&&np[i].li=="SUBPRO_"){
                    np[i].ri[j].insert(0,"{FUN} ");
                }
                else if(right_next=="E"&&np[i].li=="arr_num"){
                    int idx=np[i].ri[j].find("]",0);
                    np[i].ri[j].insert(idx+1," {ARR_Dn}");
                }
                else if (right_next == "for") {
                    int idx = np[i].ri[j].find("for", 0);
                    np[i].ri[j].insert(idx + right_next.size(), " {FOR}");
                    string com_e="COM_E";
                    int idx2=np[i].ri[j].find("COM_E",0);
                    np[i].ri[j].insert(idx2+com_e.size()," {JFE}");
                    int idx3=np[i].ri[j].find("EVA_SENTENCE",idx2);
                    string eva ="EVA_SENTENCE";
                    np[i].ri[j].insert(idx3+eva.size()," {JB}");
                    np[i].ri[j].insert(np[i].ri[j].size()," {JC}");
                    np[i].ri[j].insert(np[i].ri[j].size(), " {FE}");
                }
            }
        }
    }
}

string rec_str;
stack<string> SEM;
stack<QT_str> QT;
int QT_cnt = 1;


vector<QT_str> get_QT(){
    return QT_arr;
}

//get QT_arr
void switch_QT() {
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

int QT_cnt_num=0;
string Dn;
bool define=0;
int get_id=1;
int add_arr=1;

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
        Q_str.QT_num=QT_cnt_num++;
    } else if (oper == "IF") {
        string oper2 = SEM.top();
        SEM.pop();
        Q_str.Operator = oper;
        Q_str.Operand1 = oper2;
        Q_str.Operand2 = "_";
        Q_str.result = "_";
        Q_str.QT_num=QT_cnt_num++;
    } else if (oper == "IE" || oper == "WH" || oper=="FOR"||oper=="FE") {
        Q_str.Operator = oper;
        Q_str.Operand1 = "_";
        Q_str.Operand2 = "_";
        Q_str.result = "_";
        Q_str.QT_num=QT_cnt_num++;
    } else if (oper == "EL" || oper == "WE") {
        Q_str.Operator = oper;
        Q_str.Operand1 = "_";
        Q_str.Operand2 = "_";
        Q_str.result = "_";
        Q_str.QT_num=QT_cnt_num++;
    }else if (is_word(oper)==1||oper=="main") {
        Q_str.Operator = "PROGRAM";
        Q_str.Operand1 = oper;
        Q_str.Operand2 = "_";
        Q_str.result = "_";
        Q_str.QT_num = QT_cnt_num++;
    }else if (oper == "EN") {
        Q_str.Operator = "END";
        Q_str.Operand1 = "main";
        Q_str.Operand2 = "_";
        Q_str.result = "_";
        Q_str.QT_num=QT_cnt_num++;
    }
    else if (oper == "DO") {
        string oper2 = SEM.top();
        SEM.pop();
        Q_str.Operator = "DO";
        Q_str.Operand1 = oper2;
        Q_str.Operand2 = "_";
        Q_str.result = "_";
        Q_str.QT_num=QT_cnt_num++;
    }
    else if(oper=="RETURN"){
        Q_str.Operator = "JMP";
        Q_str.Operand1 = oper;
        Q_str.Operand2 = "_";
        Q_str.result = "L";
        Q_str.QT_num=QT_cnt_num++;
    }
    else if(oper=="JFE"){
        Q_str.Operator = "JMP";
        Q_str.Operand1 = "_";
        Q_str.Operand2 = "_";
        Q_str.result = "FE";
        Q_str.QT_num=QT_cnt_num++;
    }
    else if(oper=="JB"){
        Q_str.Operator = "JMP";
        Q_str.Operand1 = "_";
        Q_str.Operand2 = "_";
        Q_str.result = "B";
        Q_str.QT_num=QT_cnt_num++;
    }
    else if(oper=="JC"){
        Q_str.Operator = "JMP";
        Q_str.Operand1 = "_";
        Q_str.Operand2 = "_";
        Q_str.result = "C";
        Q_str.QT_num=QT_cnt_num++;
    }
    else if(oper=="BREAK"){
        Q_str.Operator="JMP";
        Q_str.Operand1=oper;
        Q_str.Operand2="_";
        Q_str.result="L";
        Q_str.QT_num=QT_cnt_num++;
    }
    else if(oper=="CONTINUE"){
        Q_str.Operator="JMP";
        Q_str.Operand1=oper;
        Q_str.Operand2="_";
        Q_str.result="L";
        Q_str.QT_num=QT_cnt_num++;
    }
    else if(oper=="FUN"){
        string oper2 = SEM.top();
        SEM.pop();
        Q_str.Operator="JMP";
        Q_str.Operand1="FUN";
        Q_str.Operand2="_";
        Q_str.result=oper2;
        Q_str.QT_num=QT_cnt_num++;
    }
    else if(oper=="ARR_Dn"){
        string oper2 = SEM.top();
        SEM.pop();
        string oper1 = SEM.top();
        SEM.pop();
        string str=oper1+"["+oper2+"]";
        SEM.push(str);
    }
    else {
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
        Q_str.QT_num=QT_cnt_num++;
    }
    if(oper!="ARR_Dn")QT.push(Q_str);
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

int cnt_arr=0;

//四元式分析过程
void solve_QT(MyWindow* ww) {
    string w, x, front_w;
    string front_warr;
    //NEXT(w)

    char ch;
    sentence = "";
    pos = 0;
    bool state=1;
    while ((ch = getchar()) != '#') {
        if(!state){
            if(ch=='\n') state=1;
        }
        if(state) sentence += ch;
        if(ch=='/'&&sentence[sentence.size()-2]=='/') {
            state=0;
        }
        //putchar(ch);
    }
    sentence += '#';
    //cout << sentence << endl;
    Next();

    w = word;

    //cout << w << endl;
    //w = stack_str.top();
    string str_end = "#";
    string str_$ = "$";
    int steps = 0;
    s_solve.push("#");
    s_solve.push(np[0].li);
    printf("%-10s%-35s%-10s%-10s%-20s%-20s%-20s\n", "步骤", "符号栈", "x", "w", "所用产生式", "SEM[m]", "QT");
    while (!s_solve.empty()) {
        string temp_w;
        if (II.count(w))temp_w = "id";
        else if (match("cons",w))temp_w = "cons";
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
                string dlgtitle="语法错误";
                string strinfo="执行ll1过程发现语法错误,请查看LL1分析表";
                QMessageBox::warning(ww,QString::fromLocal8Bit(dlgtitle.c_str()),QString::fromLocal8Bit(strinfo.c_str()),QMessageBox::Ok);
                break;
            }
        }
            //x属于Vn?
        else if (Left.count(x)) {
            //查表
            if(L_table.find(make_pair(x, temp_w))==L_table.end()){
                cout << "error" << endl;
                string dlgtitle="语法错误";
                string strinfo="执行ll1过程发现语法错误,请查看LL1分析表";
                QMessageBox::warning(ww,QString::fromLocal8Bit(dlgtitle.c_str()),QString::fromLocal8Bit(strinfo.c_str()),QMessageBox::Ok);
                return ;
            }
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
                //else if(right_next=="{main}")tmp_right.push("{"+front_w+"}");
                else if (right_next != "@") tmp_right.push(right_next);
            }
            while (!tmp_right.empty()) {
                string v = tmp_right.top();
                tmp_right.pop();
                s_solve.push(v);
            }

        } else if (x == "P") {
            SEM.push(front_w);
        } else if (x[0]=='{'&&x[x.size()-1]=='}') {
            string temp_GEQ;
            //if (x[1] == 'w') temp_GEQ = w;
            for (int i = 0; i < (int) x.size(); i++) {
                if (x[i] != '{' && x[i] != '}') {
                    temp_GEQ.push_back(x[i]);
                    //cout << temp_GEQ << endl;
                    //system("pause");
                }
            }
            if(temp_GEQ=="id"){cout<<"front_w"<<front_w<<endl;GEQ(front_w);}
            //temp_GEQ.push_back(x[1]);
            else GEQ(temp_GEQ);
        } else if (w == str_end) {
            cout << "succeed" << endl;
            string dlgtitle="语法分析完成";
            string strinfo="执行ll1分析过程成功,该文本内容可以由文法生成";
            QMessageBox::warning(ww,QString::fromLocal8Bit(dlgtitle.c_str()),QString::fromLocal8Bit(strinfo.c_str()),QMessageBox::Ok);
            break;
        } else {
            cout << "error" << endl;
            string dlgtitle="语法错误";
            string strinfo="执行ll1过程发现语法错误,请查看LL1分析表";
            QMessageBox::warning(ww,QString::fromLocal8Bit(dlgtitle.c_str()),QString::fromLocal8Bit(strinfo.c_str()),QMessageBox::Ok);
            break;
        }
        //print(steps++, s_solve, x, w, tmp, SEM, QT);
        print_end(tmp, SEM, QT);
    }
}

bool match(string x, string word) {   //判断x和word是否是同种单词（例如+与w0是同种单词）
    int is_word(string);
    return x == word || (x == "cons" && (is_word(word) == 2||is_word(word)==4||is_word(word)==5)) || (x == "id" && (is_word(word) == 1)) ||
           ((x == "w0") && (word == "+" || word == "-")) || ((x == "w1") && (word == "*" || word == "/"));
}
void Init_ll1(){
    QT_arr.clear();
    while(!FE_idx.empty()) FE_idx.pop();
    while(!C_idx.empty()) C_idx.pop();
    num_for=0;
    B_idx=0;
    S_idx=0;
    FUN_idx=0;
    while(!break_idx.empty()) break_idx.pop();
    while(!continue_idx.empty()) continue_idx.pop();
    while(!FOR_AND_WH_idx.empty()) FOR_AND_WH_idx.pop();
    FUN_str="";
    FUN_str_idx.clear();
    rec_str="";
    while (!SEM.empty()) {
        SEM.pop();
    }
    while(!QT.empty()){
        QT.pop();
    }
    QT_cnt=1;
    QT_cnt_num=0;
    Dn="";
    define=0;
    get_id=1;
    add_arr=1;
    cnt_arr=0;
}
void ll1(MyWindow* ww) {
    Init_ll1();
    QFile file("ll1_process");
    if(file.remove()){
        freopen("cache", "r", stdin);
        freopen("ll1_process","w",stdout);
        solve_QT(ww);
        fclose(stdin);
        fclose(stdout);
    }
    else{
        QString dlgtitle="语法分析消息框";
        QString strinfo="发生了一些缓存错误";
        QMessageBox::information(ww,dlgtitle,strinfo,QMessageBox::Ok);
        exit(0);
    }

}
void get_mean(MyWindow* ww){
    switch_QT();
    change_QT(ww);
}
