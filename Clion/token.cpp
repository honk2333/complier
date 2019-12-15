//
// Created by Honk on 2019/11/28.
//

#include"complier.h"

using namespace std;

string a[maxn];   //变量
int len_a;
string C[maxn];  //常数
int len_C;
string s[maxn];  //字符串
int len_s;
string c[maxn];   //字符
int len_c;
string k[maxn];   //关键字
int len_k;
string p[maxn];    //界符
int len_p;
struct token token[maxn];

map<string, int> II;
map<string, int> CC;    //c字符
map<string, int> SS;    //S字符串
map<string, int> cc;    //c常数
map<string, int> KK;    //k关键字
map<string, int> pp;    //p界符

int state_change(int state, char ch) {   //状态转移函数,负数状态代表终止态,正数状态代表非终止态
    //-1代表变量或关键字(后面再进一步区分),-2代表常数,-3代表字符串,-4代表字符,-6代表界符
    if (state == 1) {
        if (ch == ' ' || ch == '\n' || ch == '\t')    //跳过单词开头的空格
            return 1;

        else if (ch <= '9' && ch >= '0') {   //数字
            return 4;
        } else if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a'))  //变量
            return 2;
        else if (ch == '\'')   //字符
            return 9;
        else if (ch == '\"')  //字符串
            return 11;
        else if (ch == '+' || ch == '-')   //数字前面的正负号算入数字中
            return 3;
        else
            return -6;
    }
    if (state == 2) {  //变量
        if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a'))  //继续读下一个自符,状态保持不变
            return 2;
        if (ch <= '9' && ch >= '0') {
            return 2;
        }
        if (ch == '[' || ch == ']')   //遇到数组符号,说明变量输入完毕
            return -1;
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '+' || ch == '<' ||
            ch == '>')   //遇到以上终结符,说明变量输入完毕
            return -1;
    }
    if (state == 3) {    //带符号的数字
        if (ch <= '9' && ch >= '0') {  //至少读入一个数字位
            return 4;
        }
        if (ch == '.') return 8;   //读入了小数点
        else return -6;   //没有数字位,说明是运算符,不能算入数字中
    }
    if (state == 4) {    //数字部分
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == '}' ||
            ch == '{')   //遇到终结符,数字输入完毕
            return -3;
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '&' || ch == '|' || ch == '~')
            return -3;
        if (ch == ']')  //遇到数组符号,说明数字为数组大小,数字输入完毕
            return -2;
        if (ch == '.')  //遇到小数点进入状态8
            return 8;
        if (ch == 'e')  //遇到指数进入状态8
            return 8;
        if (ch <= '9' && ch >= '0') {
            return 4;
        }
    }
    if (state == 8) {  //小数点后至少一位,指数后至少一位，然后跳转到状态7等待结束
        if (ch <= '9' && ch >= '0') {
            return 7;
        } else {    //格式错误,报错
            printf("数据格式输入错误\n");
            return -2;
        }
    }
    if (state == 7) {
        if (ch == 'e')
            return 8;  //底数为小数的指数形式,指数至少为一位,所以要跳转到状态8,至少读入一位数字
        if (ch <= '9' && ch >= '0') {   //多位数字,保持状态7不变读入字符
            return 7;
        }
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == '+' ||
            ch == '-' || ch == '*' || ch == '/')   //遇到终止符,说明数字输入结束
            return -2;
        else {   //格式错误,报错
            printf("数据格式输入错误\n");
            return 7;
        }
    }
    if (state == 9) {   //读完左单引号后,读入任意一个字符,进入状态10,等待读进右单引号
        return 10;
    }
    if (state == 10) {   //等待读进右单引号
        if (ch == '\'')  //读进右单引号,等待读入下一字符,完成字符的输入
            return 16;
        else {     //读进来的不是单引号,说明字符格式错误,报错
            printf("字符格式错误\n");
            return 10;
        }
    }
    if (state == 16) {   //输入终止字符,字符输入完毕
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == '+')
            return -4;
    }
    if (state == 11) {   //字符串处理
        if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a'))
            return 12;
    }
    if (state == 12) {
        if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a'))   //读入任意多个字母字符
            return 12;
        if (ch == '\"')  //读入右双引号.等待输入下一字符,结束字符串的输入
            return 17;
        else {   //没有读入右双引号,说明字符串格式错误,报错
            printf("字符串格式错误\n");
            return 17;
        }
    }
    if (state == 17) { //输入终止字符,字符串输入完毕
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == '+')
            return -5;
    }
}

char Findgroup(int state, string word) {  //根据当前的终止状态确定单词类别
    for (int i = 0; i < len_k; i++) {
        if (word == k[i])
            return 'k';
    }
    if (state == -1) {
        return 'a';
    }
    if (state == -2)
        return 'c';
    if (state == -3)
        return 'c';
    if (state == -4)
        return 'C';
    if (state == -5)
        return 's';
    if (state == -6)
        return 'p';
}

int Findid(char group, string word) {  //根据单词类别,查找单词是否已经出现过,如果出现过则直接返回其在数组中对应的下标
    // 如果没有出现过,就将该单词添加到对应数组末尾
    if (group == 'k') {
        for (int i = 0; i < len_k; i++) {
            if (word == k[i])
                return i;
        }
    }
    if (group == 'C') {
        for (int i = 0; i < len_C; i++) {
            if (C[i] == word) {
                return i;
            }
        }
        cc[word] = len_C;  //字符
        C[len_C++] = word;
        return len_C - 1;
    }
    if (group == 's') {
        for (int i = 0; i < len_s; i++) {
            if (s[i] == word) {
                return i;
            }
        }
        SS[word] = len_s;
        s[len_s++] = word;
        return len_s - 1;
    }
    if (group == 'c') {
        for (int i = 0; i < len_c; i++) {
            if (c[i] == word) {
                return i;
            }
        }
        CC[word] = len_c;
        c[len_c++] = word;
        return len_c - 1;
    }
    if (group == 'p') {
        for (int i = 0; i < len_p; i++) {
            if (p[i] == word) {
                return i;
            }
        }
        pp[word] = len_p;
        p[len_p++] = word;
        return len_p - 1;
    }
    if (group == 'a') {
        for (int i = 0; i < len_a; i++) {
            if (a[i] == word) {
                return i;
            }
        }
        II[word] = len_a;
        a[len_a++] = word;
        return len_a - 1;
    }
}

void Init() { //初始化关键词表,出现在关键词表中的单词就是关键字
    k[0] = "int";
    k[1] = "main";
    k[2] = "void";
    k[3] = "if";
    k[4] = "else";
    k[5] = "char";
    k[6] = "while";
    k[7] = "return";
    k[8] = "double";
    k[9] = "bool";
    // k[9] = "include";
    KK["int"] = 0;
    KK["main"] = 1;
    KK["void"] = 2;
    KK["if"] = 3;
    KK["else"] = 4;
    KK["char"] = 5;
    KK["while"] = 6;
    KK["return"] = 7;
    KK["double"] = 8;
    KK["bool"] = 9;
    len_k = 10;
}

int is_word(string word) {  //用于语法分析器,返回当前单词类别
    for (int i = 0; i < len_a; i++) {  //标志符
        if (a[i] == word) return 1;
    }
    for (int i = 0; i < len_C; i++) {
        if (C[i] == word) return 4;
    }
    for (int i = 0; i < len_c; i++) {
        if (c[i] == word) return 2;
    }
    for (int i = 0; i < len_p; i++) {
        if (p[i] == word) return 6;
    }
    for (int i = 0; i < len_s; i++) {
        if (s[i] == word) return 5;
    }
    for (int i = 0; i < len_k; i++) {
        if (k[i] == word) return 7;
    }
    if (word == "#") return 3;
    return -1;
}

void get_token() {
    freopen("test.txt", "r", stdin);
    Init();
    //freopen("output.txt","w",stdout);
    int state = 1;   //当前状态,开始状态为1
    int last_state = 0;   //读入该字符之前的状态
    char ch;
    string word = "";
    int cnt = 0; //单词个数
    while ((ch = getchar()) != '#') {
        //putchar(ch);
        if (state == 0) {   //处理注释,注释后面的字符不需要读
            if (ch != '\n')
                continue;
            else {
                state = 1;
                continue;
            }
        }
        if (state == 1 && (ch == ' ' || ch == '\n' || ch == '\t')) { //过滤掉单词开头的空格换行等字符
            continue;
        }
        if (state == 1 && word == "" &&
            (ch == '(' || ch == '{' || ch == ')' || ch == '}')) { //单词开头即为括号的情况，直接将括号作为一个单词分出来
            word = "";
            word += ch; //右括号单独算一个单词
            cout << word << endl;
            token[cnt].group = 'p';    //不用加符号表
            token[cnt].id = Findid(token[cnt].group, word);
            cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
            cnt++;
            word = "";
            state = 1;
            continue;
        }
        if (word == "<" || word == "=" || word == ">") {  //用于处理一些特殊的单词,例如>=,<=
            if (ch == '>' || ch == '<' || ch == '=') {
                word += ch;
                cout << word << endl;
                token[cnt].group = 'p';  //不用加符号表
                token[cnt].id = Findid(token[cnt].group, word);
                cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                cnt++;
                word = "";
                state = 1;
                continue;
            } else {
                cout << word << endl;
                token[cnt].group = 'p'; //不用加符号表
                token[cnt].id = Findid(token[cnt].group, word);
                cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                cnt++;
                word = "";
                state = 1;
            }
        }
        if (word == "/") {   //注释的处理
            if (ch == '/') {
                word += ch;
                cout << word << endl;
                token[cnt].group = 'p'; //不用加符号表
                token[cnt].id = Findid(token[cnt].group, word);
                cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                cnt++;
                word = "";
                state = 0;
                continue;
            }
        }
        if (((word == "+") && (ch == '+' || ch == '=')) || ((word == "-") && (ch == '-' || ch == '=')) ||
            ((word == "/" || word == "*") && (ch == '='))) {  //一些特殊运算符的处理,++,--,+=
            word += ch;
            cout << word << endl;
            token[cnt].group = 'p'; //不用加符号表
            token[cnt].id = Findid(token[cnt].group, word);
            cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
            cnt++;
            word = "";
            state = 1;
            continue;
        } else if ((word == "*" || (word == "/" && ch != '/'))) {  //*,/作为算数运算符
            cout << word << endl;
            token[cnt].group = 'p'; //不用加符号表
            token[cnt].id = Findid(token[cnt].group, word);
            cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
            cnt++;
            word = "";
            state = 1;
        } else if ((word == "+" || word == "-") && state != 3) {  //+,-号不作为数字开头,作为算术运算符
            cout << word << endl;
            token[cnt].group = 'p'; //不用加符号表
            token[cnt].id = Findid(token[cnt].group, word);
            cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
            cnt++;
            word = "";
        }
        int state_before = state;
        state = state_change(state_before, ch);
        //cout << ch << " ! " << state << endl;
        if (state > 0) {
            //单词未终结
            if (ch != ' ' && ch != '\n')
                word.push_back(ch);
        }
        if (state < 0) {   //进入了终止状态
            //cout<<word<<endl;
            //word.erase(word.end() - -1);
            if (word != "") {   //已经读入单词的部分不为空,先把单词存入
                cout << word << endl;
                token[cnt].group = Findgroup(state, word);
                token[cnt].id = Findid(token[cnt].group, word);
                cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                cnt++;
            }
            if (ch != ' ' && ch != '\n') {   //当前字符不为空格或换行,说明当前字符也要作为一个单词处理
                if (ch == '>' || ch == '<' || ch == '=' || ch == '/' || ch == '+' || ch == '-' ||
                    ch == '*') {   //遇到逻辑运算符，不能直接作为单词，可能和后面的字符共同构成单词
                    word = "";
                    word += ch;
                    //cout << word << endl;
                    last_state = state;
                    state = 1;
                    continue;
                } else {   //其他情况,将当前字符作为单词存入
                    word = "";
                    word.push_back(ch);
                    cout << word << endl;
                    token[cnt].group = 'p';
                    token[cnt].id = Findid(token[cnt].group, word);
                    cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                    cnt++;
                }
            }
            word = "";   //初始化,清空单词部分
            last_state = state;  //状态初始化为1
            state = 1;
        }
    }

    printf("单词表如下：-----------------------\n");   //打印单词表
    for (int i = 0; i < len_a; i++) {
        cout << i << " " << "a标志符 " << a[i] << endl;;
    }
    for (int i = 0; i < len_C; i++) {
        cout << i << " " << "C字符 " << C[i] << endl;;
    }
    for (int i = 0; i < len_s; i++) {
        cout << i << " " << "S字符串 " << s[i] << endl;;
    }
    for (int i = 0; i < len_c; i++) {
        cout << i << " " << "c常数 " << c[i] << endl;;
    }
    for (int i = 0; i < len_k; i++) {
        cout << i << " " << "k关键字 " << k[i] << endl;;
    }
    for (int i = 0; i < len_p; i++) {
        cout << i << " " << "p界符 " << p[i] << endl;;
    }
    for (int i = 0; i < len_a; i++) {
        add_synbl(a[i]);  //填符号表
    }

}
