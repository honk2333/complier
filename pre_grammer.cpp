#include"complier.h"

using namespace std;

struct production np[maxn], pron[maxn];//p�Ǵ��ļ��ж�����ķ�,np��������ݹ����ķ�
int cnt = 0;
int dnt = 0;

int vt_size = 5;
int vn_size = 12;
string vt[maxn] = {"w0", "w1", "id", "cons", "@"}; //�ս������
string vn[maxn] = {"MAINPRO", "S_LIST", "TYPE", "S", "EVA_SENTENCE", "SEL_SENTENCE", "ITE_SENTENCE", "F", "E", "T",
                   "T'",
                   "E'"}; //���ս������
bool Find(string s) {
    for (int i = 0; i < vt_size; i++) {
        if (vt[i] == s) return true;
    }
    return false;
}

void Union() {   //ͨ���ʷ��������������ս����
    extern string k[maxn];   //�ؼ���
    extern int len_k;
    extern string p[maxn];    //���
    extern int len_p;
    for (int i = 0; i < len_k; i++) {
        if (!Find(k[i])) vt[vt_size++] = k[i];
    }
    for (int i = 0; i < len_p; i++) {
        if (!Find(p[i])) vt[vt_size++] = p[i];
    }
    cout << vt_size << endl;
}

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
        for (int j = 0; j < pron[i].size; j++) {
            cout << pron[i].li << " -> ";
            cout << pron[i].ri[j] << endl;
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
        for (int j = 0; j < pron[i].size; j++) {
            stringstream ss(pron[i].ri[j]);
            string ri_first;
            ss >> ri_first;
            //cout << ri_first << endl;
            if (pron[i].li == ri_first) {
                flag = true;   //����ݹ�
                break;
            }
        }
    }
    if (flag) {    //������ݹ�
        cout << "��ǰ�ķ�����ݹ飬������ݹ����ķ�Ϊ��" << endl;
        string left;
        for (int i = 0; i < cnt; i++) {
            bool tag = false;
            for (int j = 0; j < pron[i].size; j++) {
                stringstream ss(pron[i].ri[j]);
                string ri_first;
                ss >> ri_first;
                //cout << ri_first << endl;
                if (pron[i].li == ri_first) {
                    //��ǰ����ʽ����ݹ�
                    tag = true;
                    for (int l = 0; l < pron[i].size; l++) {
                        stringstream sss(pron[i].ri[l]);
                        string rri_first;
                        sss >> rri_first;
                        if (pron[i].li != rri_first) {
                            left = pron[i].ri[l];   //ss�治������ݹ�Ĳ���ʽ
                            cout << pron[i].li << " -> ";
                            int len = pron[i].ri[j].length();
                            string tmp = pron[i].ri[j].substr(ri_first.size());
                            //cout << tmp << endl;
                            cout << left + " {" + tmp + " }" << endl;
                            np[dnt].li = pron[i].li;
                            np[dnt].ri[np[dnt].size++] = left + " " + np[dnt].li + "\'";
                            dnt++;
                            np[dnt].li = np[dnt - 1].li + "\'";
                            np[dnt].ri[np[dnt].size++] =
                                    pron[i].ri[j].substr(ri_first.size() + 1, len - 1) + " " + np[dnt].li;
                            np[dnt].ri[np[dnt].size++] = "@"; //��
                            dnt++;
                            break;
                        }
                    }
                }
            }
            if (!tag) {
                np[dnt].li = pron[i].li;
                np[dnt].size = pron[i].size;
                for (int j = 0; j < pron[i].size; j++) {
                    np[dnt].ri[j] = pron[i].ri[j];
                }
                dnt++;
            }
        }
        cout << "�������ź���ķ�Ϊ��" << endl;
        test2_output();
    } else {
        dnt = cnt;
        for (int i = 0; i < cnt; i++) {
            np[i].li = pron[i].li;
            np[i].size = pron[i].size;
            for (int j = 0; j < pron[i].size; j++) {
                np[dnt].ri[j] = pron[i].ri[j];
            }
        }
    }

}

bool generate_empty(string ri) {
    //int len=ri.size();
    stringstream ss(ri);
    string ww;
    while (ss >> ww) {
        //cout << ww << endl;
        if (judge_vt(ww) && ww != "@") {
            return false;
        } else if (judge_vn(ww)) {
            bool tag = false;
            for (int i = 0; i < dnt; i++) {
                if (tag) break;
                if (np[i].li == ww) {
                    for (int j = 0; j < np[i].size; j++) {
                        if (generate_empty(np[i].ri[j])) {
                            tag = true;
                            break;
                        }
                    }
                    break;
                }
            }
            if (tag) continue;
            else return false;
        }
    }
    return true;
}

//�ݹ���first��
set<string> First(string c) {
    //cout << "c " << c << endl;
    set<string> ret;
    if (c == "@") {
        return ret;
    } else if (judge_vt(c)) {  //�ǿ��ս��,����first��
        ret.insert(c);
    } else {
        for (int k = 0; k < dnt; k++) {
            if (np[k].li == c) {  //�ҵ���
                for (int l = 0; l < np[k].size; l++) {  //�����Ҳ�
                    string tmp = "";
                    stringstream ss(np[k].ri[l]);  //����ʽ�Ҳ�
                    ss >> tmp;
                    set<string> rett = First(tmp);
                    for (auto it = rett.begin(); it != rett.end(); it++) {
                        ret.insert(*it);
                    }
                    while (!ss.eof() && generate_empty(tmp)) { //�����ǰ�ַ�����Ϊ��
                        ss >> tmp;
                        rett = First(tmp);
                        for (auto it = rett.begin(); it != rett.end(); it++) {
                            ret.insert(*it);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

//�����ʽ��first��
void get_first() {
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            string tmp = "";
            stringstream ss(np[i].ri[j]);  //����ʽ�Ҳ�
            ss >> tmp;
            //cout << tmp << endl;
            np[i].first[j] = First(tmp);
            while (!ss.eof() && generate_empty(tmp)) { //�����ǰ�ַ�����Ϊ��
                ss >> tmp;
                //cout << "ss " << tmp << endl;
                set<string> rett = First(tmp);
                for (auto it = rett.begin(); it != rett.end(); it++) {
                    np[i].first[j].insert(*it);
                }
            }
        }
    }
    return;
}

//��ӡfirst��
void test_first() {
    get_first();
    cout << "First_set:" << endl;
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
int nowid; //��ֹ��ѭ����
//����follow��
set<string> make_follow(int id, string left_head) {
    if (follow_vis[id]) return np[id].follow;  //����ǰ�󲿵�follow���Ѿ�����,ֱ�ӷ���
    //cout << left_head << endl;
    //���Һ�left_head�Ĳ���ʽ
    for (int i = 0; i < dnt; i++) {
        string left_str = np[i].li;
        for (int j = 0; j < np[i].size; j++) {
            int start = 0;
            int idx;
            while (start != np[i].ri[j].npos && (idx = np[i].ri[j].find(left_head, start)) != np[i].ri[j].npos) {
                {//�±�
                    if (idx > 0 && np[i].ri[j][idx - 1] != ' ') {
                        idx = (int) np[i].ri[j].npos;
                        break;
                    }
                    if (idx + left_head.size() < np[i].ri[j].size() && np[i].ri[j][idx + left_head.size()] != ' ') {
                        idx = (int) np[i].ri[j].npos;
                        break;
                    }
                    if (np[i].ri[j][idx + 1] == '\'' && left_head[left_head.size() - 1] != '\'') {
                        idx = (int) np[i].ri[j].npos;
                        break;
                    }
                }
                //�鵽
                //cout <<"idx"<< idx << endl;
                if (idx != np[i].ri[j].npos) {
                    //cout << "left_head" << left_head << "  np[i].ri[j]" << np[i].ri[j] << "  idx" << idx << endl;
                    //����һ�ַ�
                    //û�ַ�
                    if (idx + left_head.size() == np[i].ri[j].length()) {
                        if (left_head != np[i].li && nowid != i) {
                            set<string> temp;
                            //cout << "no word:" << left_str << endl;
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
                        if (judge_vt(right_next)) np[id].follow.insert(right_next);
                            //cout << right_next << endl;
                        else {
                            if (generate_empty(tmp)) {  //�Ҳ����Ŵ�����Ϊ��
                                set<string> temp;
                                temp = make_follow(i, left_str);
                                set<string>::iterator it = temp.begin();
                                for (; it != temp.end(); it++) {
                                    np[id].follow.insert(*it);
                                }
                            }
                            {  //�Ҳ����Ŵ���Ϊ��
                                set<string> ret = First(right_next);
                                for (auto it = ret.begin(); it != ret.end(); it++)
                                    np[id].follow.insert(*it);
                                while (!ss.eof() && generate_empty(right_next)) { //�����ǰ�ַ�����Ϊ��
                                    ss >> right_next;
                                    //cout << "ss " << tmp << endl;
                                    set<string> rett = First(right_next);
                                    for (auto it = rett.begin(); it != rett.end(); it++) {
                                        np[id].follow.insert(*it);
                                    }
                                }
                            }
                        }
                    }
                    start = idx + left_head.size();
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
        nowid = i;
        //cout << np[i].li << endl;
        make_follow(i, np[i].li);
        follow_vis[i] = true;
    }
}

//����follow�������
void test_follow() {
    get_follow();
    cout << "Follow_set:" << endl;
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

//����select��
void make_select() {
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            //cout << np[i].li << " " << np[i].ri[j] << endl;
            set<string> &temp = np[i].first[j];
            set<string>::iterator it = temp.begin();
            for (; it != temp.end(); it++) {
                np[i].select[j].insert(*it);
            }
            if (generate_empty(np[i].ri[j])) {
                if (!follow_vis[i]) make_follow(i, np[i].li);
                set<string> &temp = np[i].follow;
                set<string>::iterator it2 = temp.begin();
                for (; it2 != temp.end(); it2++) {
                    np[i].select[j].insert(*it2);
                }
            }
        }
    }
}

void get_select() {
    make_select();
}

void test_select() {
    get_select();
    cout << "Select_set:" << endl;
    for (int i = 0; i < dnt; i++) {
        for (int j = 0; j < np[i].size; j++) {
            cout << "Select of " << np[i].li << " -> " << np[i].ri[j] << endl;
            int len = np[i].select[j].size();
            //cout << len << endl;
            for (set<string>::iterator it = np[i].select[j].begin(); it != np[i].select[j].end(); it++) {
                cout << *it << " ";
            }
            cout << endl;
        }
    }
}

void pre_grammer() {
    freopen("input3.txt", "r", stdin);
    string ss;
    int start = 0;
    while (getline(cin, ss)) {
        //cout << ss << endl;
        int len = ss.length();
        for (int i = 0; i < len; i++) {
            if (ss[i] == ' ')
                continue;
            else {   //������ʽ��
                int j = i;
                for (j = i; ss[j] != ' ' && ss[j] != '|'; j++);
                pron[cnt].li = ss.substr(i, j - i);
                //cout<<"left: "<<pron[cnt].li<<endl;
                break;
            }
        }
        for (int i = 0; i < len - 1; i++) {
            if (ss[i] == '-' && ss[i + 1] == '>') {   //��������ʽ����
                start = i + 2;
            }
        }
        pron[cnt].size = 0;
        int li = start, ri;
        while (ss[li] == ' ')
            li++;
        for (int i = start; i < len; i++) { //������ʽ�ֿ������ں�������
            if (ss[i] == '|') {
                ri = i - 1;
                while (ss[ri] == ' ')
                    ri--;
                pron[cnt].ri[pron[cnt].size] = ss.substr(li, ri - li + 1);
                //cout<<"right "<<pron[cnt].ri[pron[cnt].size]<<endl;
                li = i + 1;
                while (ss[li] == ' ')
                    li++;
                pron[cnt].size = pron[cnt].size + 1;
            }
        }
        //
        ri = len - 1;
        while (ss[ri] == ' ')
            ri--;
        pron[cnt].ri[pron[cnt].size] = ss.substr(li, ri - li + 1);
        //cout<<"right "<<pron[cnt].ri[pron[cnt].size]<<endl;
        pron[cnt].size = pron[cnt].size + 1;
        cnt++;
    }
    test1_output();
    Left_Recursion();  //�жϲ�������ݹ�
    test_first();
    test_follow();
    test_select();
}
