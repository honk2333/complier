//
// Created by Honk on 2019/11/28.
//

#include"grammer.h"


struct anaylist {   //ll(1)������
    string s1;
    string s2;
    string ans;
} List[100];
int List_size = 0;  //ll(1)��������

bool alpha_word(char w) {   //�ж��Ƿ�����ĸ�򵥴�
    if (w <= 'z' && w >= 'a')
        return true;
    if (w <= 'Z' && w >= 'A')
        return true;
    if (w <= '9' && w >= '0')
        return true;
    return false;
}

string sentence;  //Ҫ����ľ���
string word;  //��ǰ����
int pos;//��һ�����ʵ���ʼλ��

void Next() {   //��һ������
    int is_word(string);
    char c;
    word = "";
    int len = sentence.size();
    //cout << sentence << endl;
    for (int i = len - 1; i >= pos; i--) {   //���ҵ��ʱ�����ƥ�䳤�ȳ��ĵ���
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





void test_analyst() {   //����ll(1)���������
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

struct fouryuan {   //��Ԫʽ�ṹ��
    string s1;
    string s2;
    string op;
    string res;
} fout[maxn * 10];
int fout_size = 0;  //��Ԫʽ����
vector<string> sem;

void test_fout() {   //��ӡ��Ԫʽ
    //get_fout();
    cout << "��Ԫʽ����:" << endl;
    for (int i = 0; i < fout_size; i++) {
        cout << "( " << fout[i].op << " " << fout[i].s1 << " " << fout[i].s2 << " " << fout[i].res << " )" << endl;
    }
}

bool match(string x, string word) {   //�ж�x��word�Ƿ���ͬ�ֵ��ʣ�����+��w0��ͬ�ֵ��ʣ�
    int is_word(string);
    return x == word || (x == "I" && (is_word(word) == 2 || is_word(word) == 1)) ||
           ((x == "w0") && (word == "+" || word == "-")) || ((x == "w1") && (word == "*" || word == "/"));
}

string FindList(string s1, string s2) {   //��ll(1)������
    for (int i = 0; i < List_size; i++) {
        if (List[i].s1 == s1 && match(List[i].s2, s2)) return List[i].ans;
    }
    return "";
}


void judge2() {   //ll(1)�������жϾ����Ƿ�Ϸ�
    int is_word(string);
    cout << "LL(1)������" << endl;
    sentence = "";
    char ch;
    while ((ch = getchar()) != '#') sentence += ch;
    sentence += "#";         //����ʶ��ľ����ȶ�����
    cout << sentence << endl;
    pos = 0;

    string x;
    stack<string> sta;   //����ջ
    sta.push("#");
    sta.push("E");
    jmp1:   //���λ�ñ��ں�����ת
    Next();
    jmp2:
    x = sta.top();
    sta.pop();
    cout << "w: " << word << " ";
    cout << "x: " << x << " ";

    if (x[0] == 'P') {   //��ǰջ��ΪPUSH(i)
        cout << endl;
        sem.push_back(x.substr(6));
        goto jmp2;
    }
    if (x[0] == 'G') {  //��ǰջ��ΪGEQ()
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

    if (judge_vt(x)) {  //��ǰ�������ս������ջ������ƥ��
        if (match(x, word)) {
            cout << "ƥ��: " << word << endl;
            goto jmp1;
        } else {  //���޷�ƥ�䣬��˵����䲻�Ϸ������������Ϣ
            cout << "Error1" << endl;
            return;
        }
    } else if (judge_vn(x)) {   //��ǰ�����Ƿ��ս��������ѹջ
        string tmp = FindList(x, word);
        cout << "����ѹջ: " << tmp << endl;
        if (tmp != "") {
            int len = tmp.size();
            string ww = "";
            for (int i = len - 1; i >= 0; i--) {
                if (tmp[i] == ' ') continue;
                ww = tmp[i] + ww;
                //cout << ww << 1 << endl;
                if (ww == "T" && i != 0) {   //����Ӧ�����嶯����ջ
                    string ss = "GEQ()" + word;
                    sta.push(ss);
                }
                if (ww == "F" && i != 0) {//����Ӧ�����嶯����ջ
                    string ss = "GEQ()" + word;
                    sta.push(ss);
                }
                if (ww == "I") {//����Ӧ�����嶯����ջ
                    string ss = "PUSH()" + word;
                    sta.push(ss);
                }
                if ((judge_vt(ww) || judge_vn(ww)) && ww != "@") {  //������ջ�Ĺ���
                    //cout << ww << endl;
                    sta.push(ww);
                    ww = "";
                }
            }
            goto jmp2;
        } else {   //δ�ҵ��������Ӧ���ݣ����Ϸ������������Ϣ
            cout << "Error2" << endl;
            return;
        }

    } else if (x == "#") {  //��ǰջ��Ϊ�ս��'#'
        cout << endl;
        if (word == "#")
            cout << "�þ������ķ��ĺϷ����" << endl;
        else
            cout << "�þ��Ӳ����ķ��ĺϷ����" << endl;
        return;
    }
}


