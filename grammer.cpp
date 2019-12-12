//
// Created by Honk on 2019/11/28.
//

#include"grammer.h"


struct anaylist {   //ll(1)分析表
    string s1;
    string s2;
    string ans;
} List[100];
int List_size = 0;  //ll(1)分析表长度

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


