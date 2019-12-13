//
// Created by Honk on 2019/11/28.
//

#include<bits/stdc++.h>

using namespace std;
const int maxn = 1e3;
string a[maxn];   //����
int len_a;
string C[maxn];  //����
int len_C;
string s[maxn];  //�ַ���
int len_s;
string c[maxn];   //�ַ�
int len_c;
string k[maxn];   //�ؼ���
int len_k;
string p[maxn];    //���
int len_p;
struct token {   //token���нṹ��
    int id;
    char group;
} token[maxn];

int state_change(int state, char ch) {   //״̬ת�ƺ���,����״̬������ֹ̬,����״̬�������ֹ̬
    //-1���������ؼ���(�����ٽ�һ������),-2������,-3�����ַ���,-4�����ַ�,-6������
    if (state == 1) {
        if (ch == ' ' || ch == '\n' || ch == '\t')    //�������ʿ�ͷ�Ŀո�
            return 1;
        else if (ch <= '9' && ch >= '0') {   //����
            return 4;
        } else if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a'))  //����
            return 2;
        else if (ch == '\'')   //�ַ�
            return 9;
        else if (ch == '\"')  //�ַ���
            return 11;
        else if (ch == '+' || ch == '-')   //����ǰ�������������������
            return 3;
        else
            return -6;
    }
    if (state == 2) {  //����
        if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a'))  //��������һ���Է�,״̬���ֲ���
            return 2;
        if (ch <= '9' && ch >= '0') {
            return 2;
        }
        if (ch == '[' || ch == ']')   //�����������,˵�������������
            return -1;
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '+' || ch == '<' ||
            ch == '>')   //���������ս��,˵�������������
            return -1;
    }
    if (state == 3) {    //�����ŵ�����
        if (ch <= '9' && ch >= '0') {  //���ٶ���һ������λ
            return 4;
        }
        if (ch == '.') return 8;   //������С����
        else return -6;   //û������λ,˵���������,��������������
    }
    if (state == 4) {    //���ֲ���
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == '}' ||
            ch == '{')   //�����ս��,�����������
            return -3;
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '&' || ch == '|' || ch == '~')
            return -3;
        if (ch == ']')  //�����������,˵������Ϊ�����С,�����������
            return -2;
        if (ch == '.')  //����С�������״̬8
            return 8;
        if (ch == 'e')  //����ָ������״̬8
            return 8;
        if (ch <= '9' && ch >= '0') {
            return 4;
        }
    }
    if (state == 8) {  //С���������һλ,ָ��������һλ��Ȼ����ת��״̬7�ȴ�����
        if (ch <= '9' && ch >= '0') {
            return 7;
        } else {    //��ʽ����,����
            printf("���ݸ�ʽ�������\n");
            return -2;
        }
    }
    if (state == 7) {
        if (ch == 'e')
            return 8;  //����ΪС����ָ����ʽ,ָ������Ϊһλ,����Ҫ��ת��״̬8,���ٶ���һλ����
        if (ch <= '9' && ch >= '0') {   //��λ����,����״̬7��������ַ�
            return 7;
        }
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == '+' ||
            ch == '-' || ch == '*' || ch == '/')   //������ֹ��,˵�������������
            return -2;
        else {   //��ʽ����,����
            printf("���ݸ�ʽ�������\n");
            return 7;
        }
    }
    if (state == 9) {   //���������ź�,��������һ���ַ�,����״̬10,�ȴ������ҵ�����
        return 10;
    }
    if (state == 10) {   //�ȴ������ҵ�����
        if (ch == '\'')  //�����ҵ�����,�ȴ�������һ�ַ�,����ַ�������
            return 16;
        else {     //�������Ĳ��ǵ�����,˵���ַ���ʽ����,����
            printf("�ַ���ʽ����\n");
            return 10;
        }
    }
    if (state == 16) {   //������ֹ�ַ�,�ַ��������
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == '+')
            return -4;
    }
    if (state == 11) {   //�ַ�������
        if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a'))
            return 12;
    }
    if (state == 12) {
        if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a'))   //������������ĸ�ַ�
            return 12;
        if (ch == '\"')  //������˫����.�ȴ�������һ�ַ�,�����ַ���������
            return 17;
        else {   //û�ж�����˫����,˵���ַ�����ʽ����,����
            printf("�ַ�����ʽ����\n");
            return 17;
        }
    }
    if (state == 17) { //������ֹ�ַ�,�ַ����������
        if (ch == ' ' || ch == ';' || ch == '=' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == '+')
            return -5;
    }
}

char Findgroup(int state, string word) {  //���ݵ�ǰ����ֹ״̬ȷ���������
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

int Findid(char group, string word) {  //���ݵ������,���ҵ����Ƿ��Ѿ����ֹ�,������ֹ���ֱ�ӷ������������ж�Ӧ���±�
    // ���û�г��ֹ�,�ͽ��õ�����ӵ���Ӧ����ĩβ
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
        C[len_C++] = word;
        return len_C - 1;
    }
    if (group == 's') {
        for (int i = 0; i < len_s; i++) {
            if (s[i] == word) {
                return i;
            }
        }
        s[len_s++] = word;
        return len_s - 1;
    }
    if (group == 'c') {
        for (int i = 0; i < len_c; i++) {
            if (c[i] == word) {
                return i;
            }
        }
        c[len_c++] = word;
        return len_c - 1;
    }
    if (group == 'p') {
        for (int i = 0; i < len_p; i++) {
            if (p[i] == word) {
                return i;
            }
        }
        p[len_p++] = word;
        return len_p - 1;
    }
    if (group == 'a') {
        for (int i = 0; i < len_a; i++) {
            if (a[i] == word) {
                return i;
            }
        }
        a[len_a++] = word;
        return len_a - 1;
    }
}

void Init() { //��ʼ���ؼ��ʱ�,�����ڹؼ��ʱ��еĵ��ʾ��ǹؼ���
    k[0] = "int";
    k[1] = "main";
    k[2] = "void";
    k[3] = "if";
    k[4] = "else";
    k[5] = "char";
    k[6] = "while";
    k[7] = "return";
    k[8] = "double";
    // k[9] = "include";
    len_k = 9;
}

int is_word(string word) {  //�����﷨������,���ص�ǰ�������
    for (int i = 0; i < len_a; i++) {
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
    if (word == "#") return 3;
    return -1;
}

int get_token() {
    freopen("input2.txt", "r", stdin);
    Init();
    //freopen("output.txt","w",stdout);
    int state = 1;   //��ǰ״̬,��ʼ״̬Ϊ1
    int last_state = 0;   //������ַ�֮ǰ��״̬
    char ch;
    string word = "";
    int cnt = 0; //���ʸ���
    while ((ch = getchar()) != '#') {
        //putchar(ch);
        if (state == 0) {   //����ע��,ע�ͺ�����ַ�����Ҫ��
            if (ch != '\n')
                continue;
            else {
                state = 1;
                continue;
            }
        }
        if (state == 1 && (ch == ' ' || ch == '\n' || ch == '\t')) { //���˵����ʿ�ͷ�Ŀո��е��ַ�
            continue;
        }
        if (state == 1 && word == "" &&
            (ch == '(' || ch == '{' || ch == ')' || ch == '}')) { //���ʿ�ͷ��Ϊ���ŵ������ֱ�ӽ�������Ϊһ�����ʷֳ���
            word = "";
            word += ch; //�����ŵ�����һ������
            cout << word << endl;
            token[cnt].group = 'p';
            token[cnt].id = Findid(token[cnt].group, word);
            cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
            cnt++;
            word = "";
            state = 1;
            continue;
        }
        if (word == "<" || word == "=" || word == ">") {  //���ڴ���һЩ����ĵ���,����>=,<=
            if (ch == '>' || ch == '<' || ch == '=') {
                word += ch;
                cout << word << endl;
                token[cnt].group = 'p';
                token[cnt].id = Findid(token[cnt].group, word);
                cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                cnt++;
                word = "";
                state = 1;
                continue;
            } else {
                cout << word << endl;
                token[cnt].group = 'p';
                token[cnt].id = Findid(token[cnt].group, word);
                cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                cnt++;
                word = "";
                state = 1;
            }
        }
        if (word == "/") {   //ע�͵Ĵ���
            if (ch == '/') {
                word += ch;
                cout << word << endl;
                token[cnt].group = 'p';
                token[cnt].id = Findid(token[cnt].group, word);
                cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                cnt++;
                word = "";
                state = 0;
                continue;
            }
        }
        if (((word == "+") && (ch == '+' || ch == '=')) || ((word == "-") && (ch == '-' || ch == '=')) ||
            ((word == "/" || word == "*") && (ch == '='))) {  //һЩ����������Ĵ���,++,--,+=
            word += ch;
            cout << word << endl;
            token[cnt].group = 'p';
            token[cnt].id = Findid(token[cnt].group, word);
            cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
            cnt++;
            word = "";
            state = 1;
            continue;
        } else if ((word == "*" || (word == "/" && ch != '/'))) {  //*,/��Ϊ���������
            cout << word << endl;
            token[cnt].group = 'p';
            token[cnt].id = Findid(token[cnt].group, word);
            cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
            cnt++;
            word = "";
            state = 1;
        } else if ((word == "+" || word == "-") && state != 3) {  //+,-�Ų���Ϊ���ֿ�ͷ,��Ϊ���������
            cout << word << endl;
            token[cnt].group = 'p';
            token[cnt].id = Findid(token[cnt].group, word);
            cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
            cnt++;
            word = "";
        }
        int state_before = state;
        state = state_change(state_before, ch);
        //cout << ch << " ! " << state << endl;
        if (state > 0) {
            //����δ�ս�
            if (ch != ' ' && ch != '\n')
                word.push_back(ch);
        }
        if (state < 0) {   //��������ֹ״̬
            //cout<<word<<endl;
            //word.erase(word.end() - -1);
            if (word != "") {   //�Ѿ����뵥�ʵĲ��ֲ�Ϊ��,�Ȱѵ��ʴ���
                cout << word << endl;
                token[cnt].group = Findgroup(state, word);
                token[cnt].id = Findid(token[cnt].group, word);
                cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                cnt++;
            }
            if (ch != ' ' && ch != '\n') {   //��ǰ�ַ���Ϊ�ո����,˵����ǰ�ַ�ҲҪ��Ϊһ�����ʴ���
                if (ch == '>' || ch == '<' || ch == '=' || ch == '/' || ch == '+' || ch == '-' ||
                    ch == '*') {   //�����߼������������ֱ����Ϊ���ʣ����ܺͺ�����ַ���ͬ���ɵ���
                    word = "";
                    word += ch;
                    //cout << word << endl;
                    last_state = state;
                    state = 1;
                    continue;
                } else {   //�������,����ǰ�ַ���Ϊ���ʴ���
                    word = "";
                    word.push_back(ch);
                    cout << word << endl;
                    token[cnt].group = 'p';
                    token[cnt].id = Findid(token[cnt].group, word);
                    cout << "{" << token[cnt].group << "," << token[cnt].id << "}" << endl;
                    cnt++;
                }
            }
            word = "";   //��ʼ��,��յ��ʲ���
            last_state = state;  //״̬��ʼ��Ϊ1
            state = 1;
        }
    }
    printf("���ʱ����£�-----------------------\n");   //��ӡ���ʱ�
    for (int i = 0; i < len_a; i++) {
        cout << i << " " << "a��־�� " << a[i] << endl;;
    }
    for (int i = 0; i < len_C; i++) {
        cout << i << " " << "C�ַ� " << C[i] << endl;;
    }
    for (int i = 0; i < len_s; i++) {
        cout << i << " " << "S�ַ��� " << s[i] << endl;;
    }
    for (int i = 0; i < len_c; i++) {
        cout << i << " " << "c���� " << c[i] << endl;;
    }
    for (int i = 0; i < len_k; i++) {
        cout << i << " " << "k�ؼ��� " << k[i] << endl;;
    }
    for (int i = 0; i < len_p; i++) {
        cout << i << " " << "p��� " << p[i] << endl;;
    }
    return 0;
}
