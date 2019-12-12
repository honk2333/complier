#include"complier.h"

using namespace std;
const int maxn = 1e2;
struct production {  //����ʽ�ṹ��
    string li;
    string ri[100];
    set<string> first[100];
    set<string> follow;
    set<string> selected;
    int size;
} p[maxn], np[maxn];//p�Ǵ��ļ��ж�����ķ�,np��������ݹ����ķ�
int cnt = 0;
int dnt = 0;

int vt_size = 7;
int vn_size = 7;
string vt[maxn] = {"w0", "w1", "ID", "CONS", "(", ")", "@"}; //�ս������
string vn[maxn] = {"S", "EVA_SENTENCE", "F", "T", "T'", "E", "E'"}; //���ս������
bool judge_vt(string c) {  //�ж�һ�������Ƿ����ս��
    for (int i = 0; i < vt_size; i++) {
        if (c == vt[i])
            return true;
    }
    return false;
}

bool judge_vn(string c) {  //�ж�һ�������Ƿ��Ƿ��ս��
    for (int i = 0; i < vn_size; i++) {
        if (c == vn[i])
            return true;
    }
    return false;
}

void test1_output() {   //��ӡ������ݹ�ǰ���ķ�
    cout << "��ǰ�ķ�Ϊ" << endl;
    for (int i = 0; i < cnt; i++) {
        //cout << p[i].size << endl;
        for (int j = 0; j < p[i].size; j++) {
            cout << p[i].li << " -> ";
            cout << p[i].ri[j] << endl;
        }
    }
}

void test2_output() {   //��ӡ������ݹ����ķ�
    for (int i = 0; i < dnt; i++) {
        //cout << p[i].size << endl;
        for (int j = 0; j < np[i].size; j++) {
            cout << np[i].li << " -> ";
            cout << np[i].ri[j] << endl;
        }
    }
}

void Left_Recursion() {  //�ж�������ݹ鲢������ݹ�
    bool flag = false;
    for (int i = 0; i < cnt; i++) {    //�ж���ݹ�
        if (flag)
            break;
        for (int j = 0; j < p[i].size; j++) {
            if (p[i].li[0] == p[i].ri[j][0]) {
                flag = true;   //����ݹ�
                break;
            }
        }
    }
    if (flag) {    //������ݹ�
        cout << "��ǰ�ķ�����ݹ飬������ݹ����ķ�Ϊ��" << endl;
        string ss;
        for (int i = 0; i < cnt; i++) {
            bool tag = false;
            for (int j = 0; j < p[i].size; j++) {
                if (p[i].li[0] == p[i].ri[j][0]) {   //��ǰ����ʽ����ݹ�
                    tag = true;
                    for (int l = 0; l < p[i].size; l++) {
                        if (p[i].li[0] != p[i].ri[l][0]) {
                            ss = p[i].ri[l];   //ss�治������ݹ�Ĳ���ʽ
                            cout << p[i].li << " -> ";
                            int len = p[i].ri[j].length();
                            cout << ss + " {" + p[i].ri[j].substr(1, len - 1) + " }" << endl;
                            np[dnt].li = p[i].li;
                            np[dnt].ri[np[dnt].size++] = ss + " " + np[dnt].li + "\'";
                            dnt++;
                            np[dnt].li = np[dnt - 1].li + "\'";
                            np[dnt].ri[np[dnt].size++] = p[i].ri[j].substr(2, len - 1) + " " + np[dnt].li;
                            np[dnt].ri[np[dnt].size++] = "@"; //��
                            dnt++;
                            break;
                        }
                    }
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
        /*for (int i = 0; i < cnt; i++) {  //p��ʣ�����ʽ���뵽np������
            bool tag = false;
            for (int j = 0; j < dnt; j++) {
                if (p[i].li == np[j].li) {
                    tag = true;
                    break;
                }
            }
            *//* if (!tag) {
                 np[dnt].li = p[i].li;
                 np[dnt].size = p[i].size;
                 for (int j = 0; j < p[i].size; j++) {
                     np[dnt].ri[j] = p[i].ri[j];
                 }
                 dnt++;
             }*//*
        }*/
        cout << "�������ź���ķ�Ϊ��" << endl;
        //test1_output();
        test2_output();
    } else
        return;
}

void First(int i, int j, string c) {   //�ݹ���first��
    if (judge_vt(c) && c != "@") {  //�ǿ��ս��,����first��
        np[i].first[j].insert(c);
    } else {
        for (int k = 0; k < dnt; k++) {
            if (np[k].li == c) {  //�ҵ���
                for (int l = 0; l < np[k].size; l++) {  //�����Ҳ�
                    string tmp = "";
                    stringstream ss(np[k].ri[j]);  //����ʽ�Ҳ�
                    ss >> tmp;
                    First(i, j, tmp);
                }
            }
        }
    }
    return;
}

void get_first() {   //�����ʽ��first��
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            string tmp = "";
            stringstream ss(np[i].ri[j]);  //����ʽ�Ҳ�
            ss >> tmp;
            First(i, j, tmp);
        }
    }
    return;
}

void test_first() {  //��ӡfirst��
    get_first();
    cout << "First������:" << endl;
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

//����follow��
set<string> make_follow(int id, string left_head) {
    if (follow_vis[id]) return np[id].follow;  //����ǰ�󲿵�follow���Ѿ�����,ֱ�ӷ���

    //���Һ�left_head�Ĳ���ʽ
    for (int i = 0; i < dnt; i++) {
        string left_str = np[i].li;
        for (int j = 0; j < np[i].size; j++) {
            int idx = np[i].ri[j].find(left_head, 0);   //�±�
            if (idx != np[i].ri[j].npos) {
                if (idx > 0 && np[i].ri[j][idx - 1] != ' ') idx = np[i].ri[j].npos;
                if (idx + left_head.size() < np[i].ri[j].size() && np[i].ri[j][idx + left_head.size()] != ' ')
                    idx = np[i].ri[j].npos;
                if (np[i].ri[j][idx + 1] == '\'' && left_head[left_head.size() - 1] != '\'') idx = np[i].ri[j].npos;
            }
            //�鵽

            if (idx != np[i].ri[j].npos) {
                //cout << np[i].ri[j] << " " << idx << endl;
                //����һ�ַ�
                //û�ַ�
                if (idx + left_head.size() == np[i].ri[j].length()) {
                    if (left_head != np[i].li) {
                        set<string> temp;
                        //cout << "!!!" << left_str << endl;
                        temp = make_follow(i, left_str);  //����ĩβ,���ϸò���ʽ��follow(li);
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
                    //Ϊ�ս��
                    // cout << "right " << right_next << endl;
                    if (judge_vt(right_next)) {
                        //cout << "produ" << np[i].ri[j][idx] << endl;
                        np[id].follow.insert(right_next);
                    }
                        //Ϊ���ս��,���ܳ���E'T'���������
                    else {
                        //����ս����Ӧ�Ĳ���ʽ
                        //if(left_head==right_next)
                        int left_next_num;
                        for (int k = 0; k < dnt; k++) {
                            if (np[k].li == right_next) {
                                left_next_num = k;
                                break;
                            }
                        }
                        string next_test;
                        for (int k = 0; k < np[left_next_num].size; k++) {
                            //���Ƴ���
                            if (np[left_next_num].ri[k] == "@") {
                                //if(visi_$.count(right_next)){
                                //Follow[i]����Follow[left_head]
                                set<string> temp;
                                temp = make_follow(i, left_str);
                                set<string>::iterator it = temp.begin();
                                for (; it != temp.end(); it++) {
                                    np[id].follow.insert(*it);
                                }
                            } else {
                                //first[right_next]����Follow[left_head]
                                set<string>::iterator it1 = np[left_next_num].first[k].begin();
                                for (; it1 != np[left_next_num].first[k].end(); it1++) {
                                    np[id].follow.insert(*it1);
                                }

                            }

                        }
                    }
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
        //cout << np[i].li << endl;
        make_follow(i, np[i].li);
        follow_vis[i] = true;
    }
}

void test_follow() {   //����follow�������
    get_follow();
    cout << "Follow������:" << endl;
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


void pre_grammer() {
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
            else {   //������ʽ��
                int j = i;
                for (j = i; ss[j] != ' ' && ss[j] != '|'; j++);
                p[cnt].li = ss.substr(i, j - i);
                break;
            }
        }
        for (int i = 0; i < len - 1; i++) {
            if (ss[i] == '-' && ss[i + 1] == '>') {   //��������ʽ����
                start = i + 2;
            }
        }
        p[cnt].size = 0;
        int li = start, ri;
        while (ss[li] == ' ')
            li++;
        for (int i = start; i < len; i++) { //������ʽ�ֿ������ں�������
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
    Left_Recursion();  //�жϲ�������ݹ�
    test_first();
    test_follow();
}
