#include"complier.h"

using namespace std;

extern vector<QT_str> QT_arr;
struct target {   //Ŀ�����ṹ��
    int ta;
    string Operand1;
    string Operand2;
    string Operatore;
} tout[maxn * 10];
int index = 0;
QT_str getin[maxn];
target language[maxn];

void convert_for_fout() {
    for (int i = 0; i < QT_arr.size(); i++) {
        getin[index++] = QT_arr[i];
    }
}

void getfouryuan() {
    /*int index = 0;
    for (index = 0; index < 20; index++) {
        cin >> getin[index].Operator;
        if (getin[index].Operator == "#")
            break;
        cin >> getin[index].Operand1;
        cin >> getin[index].Operand2;
        cin >> getin[index].result;
    }*/
    convert_for_fout();
    for (int i = 0; i < index - 1; i++)    //�����ظ����
    {

        for (int j = i + 1; j < index; j++) {
            if ((getin[i].Operand1 == getin[j].Operand1) && (getin[i].Operand2 == getin[j].Operand2) &&
                (getin[i].Operator == getin[j].Operator)) {

                for (int k = j; k < index; k++) {
                    if (getin[j].result == getin[k].Operand1)
                        getin[k].Operand1 = getin[i].result;
                    else if (getin[j].result == getin[k].Operand2)
                        getin[k].Operand2 = getin[i].result;
                    //continue;
                }
                getin[j].result = getin[i].result;

            }
        }
    }

    //�����ظ����
    for (int i = 0; i < index; i++) {

        if ((getin[i].Operator == getin[i + 1].Operator) && (getin[i].Operand1 == getin[i + 1].Operand1) &&
            (getin[i].Operand2 == getin[i + 1].Operand2) &&
            (getin[i].result == getin[i + 1].result) &&
            (getin[i].Operator != "EL" || getin[i].Operator != "IE" || getin[i].Operator != "WE" ||
             getin[i].Operator != "WH")) {
            //for (int k = j + 1; k < index; k++) {
            getin[i].Operator = getin[i + 1].Operator;
            getin[i].Operand1 = getin[i + 1].Operand1;
            getin[i].Operand2 = getin[i + 1].Operand2;
            getin[i].result = getin[i + 1].result;
            // }
            index--;
        }

    }

    //�����ղ���ʽ
    for (int i = 0; i <= index - 1; i++) {
        if (getin[i].Operator == "=") {
            for (int k = 0; k < i - 1; k++) {
                if (getin[i].Operand1 == getin[i - 1].result) {
                    if (getin[i].Operand1 == getin[k].result)
                        getin[i].Operand1 = getin[k].result;
                    else {
                        getin[i - 1].result = getin[i].result;
                    }
                }

            }

        }
    }

    for (int i = 0; i < index; i++) {
        for (int j = i + 1; j < index; j++) {
            //�������ำֵ��Ԫʽ
            if ((getin[i].result == getin[i + 1].result) && (getin[i].result != "_")) {
                for (int j = i + 1; j < index; j++) {
                    getin[j].Operator = getin[j + 1].Operator;
                    getin[j].Operand1 = getin[j + 1].Operand1;
                    getin[j].Operand2 = getin[j + 1].Operand2;
                    getin[j].result = getin[j + 1].result;
                }
                index--;
            }
        }
    }
    cout << "�Ż������Ԫʽ" << endl;
    for (int i = 0; i <= index - 1; i++) {
        cout << "(" << getin[i].Operator;
        cout << ",";
        cout << getin[i].Operand1;
        cout << ",";
        cout << getin[i].Operand2;
        cout << ",";
        cout << getin[i].result << ")" << endl;
    }
    return;
}

void out() {
    for (int i = 0; i <= index - 1; i++) {
        cout << getin[i].Operator;
        cout << ",";
        cout << getin[i].Operand1;
        cout << ",";
        cout << getin[i].Operand2;
        cout << ",";
        cout << getin[i].result << endl;
    } //return;
}

void targetlan() {
    //���뻯������Ԫʽ
    int index = 0;
    for (index = 0; index < 20; index++) {
        cin >> getin[index].Operator;
        if (getin[index].Operator == "#")
            break;
        cin >> getin[index].Operand1;
        cin >> getin[index].Operand2;
        cin >> getin[index].result;

    }

    int seml[20];//Ŀ�����Ե�semջ
    int s = 0;//semջָ��

    int L[3][index - 1];//����L��־λ
    string RDL = "0";//��ʼ���Ĵ���R��ֵΪ0
    for (int m = 0; m <= 2; m++) {
        for (int n = 0; n < index; n++) {
            L[m][n] = 1;
        }
    }
    //��ʼ��ȫ����ԪʽL״̬Ϊyes��1��

    for (int i = 0; i < index; i++) {
        for (int j = i + 1; j < index; j++) {
            //�������

            if (getin[i].result == getin[j].Operand1) {
                for (int k = j + 1; k < index; k++) {
                    if (getin[j].Operand1 != getin[k].Operand1 || getin[j].Operand1 != getin[k].Operand2)
                        L[0][j] = 0;
                }

            }
            if (getin[i].result == getin[j].Operand2) {
                for (int k = j + 1; k < index; k++) {
                    if (getin[j].Operand2 != getin[k].Operand1 || getin[j].Operand2 != getin[k].Operand1)
                        L[1][j] = 0;
                }
            }
        }
    }

//����Ŀ����룬����Ŀ�����ṹ��
    int m = 0;
    for (int i = 0; i < index; i++) {

        if (getin[i].Operator == "+") {

            if (RDL == "0") {
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].ta = m + 1;
                language[m].Operatore = "ADD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ADD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ADD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ADD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    //����������
                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ADD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                }
            } else {
                for (int j = 0; j < i; j++) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ADD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;


                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ADD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ADD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ADD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        }
                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "-") {

            if (RDL == "0") {
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].ta = m + 1;
                language[m].Operatore = "SUB";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "SUB";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "SUB";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "SUB";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    //����������
                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "SUB";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                }
            } else {
                for (int j = 0; j < i; j++) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "SUB";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;


                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "SUB";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "SUB";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "SUB";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        }
                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "*") {

            if (RDL == "0") {
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].ta = m + 1;
                language[m].Operatore = "MUL";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "MUL";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "MUL";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "MUL";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    //����������
                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "MUL";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                }
            } else {
                for (int j = 0; j < i; j++) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "MUL";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;


                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "MUL";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "MUL";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "MUL";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        }
                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "/") {

            if (RDL == "0") {
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].ta = m + 1;
                language[m].Operatore = "DIV";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "DIV";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "DIV";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "DIV";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    //����������
                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "DIV";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                }
            } else {
                for (int j = 0; j < i; j++) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "DIV";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;


                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "DIV";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "DIV";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "DIV";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                        }
                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "<") {

            if (L[0][i] == 1) {
                cout << "LD R,";
                cout << getin[i].Operand1 << endl;
                //����������


            }
            cout << "LT R,";
            cout << getin[i].Operand2 << endl;
            RDL = getin[i].result;
        } else if (getin[i].Operator == ">") {

            if (L[0][i] == 1) {
                cout << "LD R,";
                cout << getin[i].Operand1 << endl;
                //����������


            }
            cout << "GT R,";
            cout << getin[i].Operand2 << endl;
            RDL = getin[i].result;
        } else if (getin[i].Operator == "<=") {

            if (L[0][i] == 1) {
                cout << "LD R,";
                cout << getin[i].Operand1 << endl;
                //����������


            }
            cout << "LE R,";
            cout << getin[i].Operand2 << endl;
            RDL = getin[i].result;
        } else if (getin[i].Operator == ">=") {

            if (L[0][i] == 1) {
                cout << "LD R,";
                cout << getin[i].Operand1 << endl;
                //����������
            }
            cout << "GE R,";
            cout << getin[i].Operand2 << endl;
            RDL = getin[i].result;
        } else if (getin[i].Operator == "=") {

            if (RDL == "0") {
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;

            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;


                }
            } else {
                for (int j = 0; j < i; j++) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;

                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;

                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;

                        }
                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "!=") {

            if (L[0][i] == 1) {
                cout << "LD R,";
                cout << getin[i].Operand1 << endl;
                //����������
            }
            cout << "NE R,";
            cout << getin[i].Operand2 << endl;
            RDL = getin[i].result;
        } else if (getin[i].Operator == "IF" || getin[i].Operator == "DO") {

            if (RDL == "0") {
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].ta = m + 1;
                language[m].Operatore = "FJ";
                language[m].Operand1 = "R";
                language[m].Operand2 = "_";
                m++;
                seml[s++] = m;//push p

            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;

                    //����������
                } else {
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;
                }
                seml[s++] = m;//push p

                RDL = "0";
            } else {
                for (int j = 0; j < i; j++) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][j] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                        } else {
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                        }
                    }
                }
                seml[s++] = m;//push p
                RDL = "0";
            }

        } else if (getin[i].Operator == "EL") {
            if ((RDL == getin[i - 1].result) || (getin[i - 1].result == "_")) {
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
                s--;
                int pop = seml[s];
                seml[s] = 0;

                language[pop - 1].Operand2 = to_string(m + 1);

                language[m].ta = m + 1;
                language[m].Operatore = "JMP";
                language[m].Operand1 = "_";
                language[m].Operand2 = "_";
                m++;
                seml[s++] = m;
            }
        } else if (getin[i].Operator == "IE") {

            if (RDL == getin[i - 1].result) {
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;

                s--;
                int pop = seml[s];
                seml[s] = 0;

                language[pop - 1].Operand2 = to_string(m);
            }
        } else if (getin[i].Operator == "WE") {

            if (RDL == getin[i - 1].result) {
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
                RDL = "0";

                s--;
                int pop = seml[s];
                seml[s] = 0;

                language[pop - 1].Operand2 = to_string(m + 1);
                s--;
                pop = seml[s];
                seml[s] = 0;
                language[m].ta = m + 1;
                language[m].Operatore = "JMP";
                language[m].Operand1 = "_";
                language[m].Operand2 = to_string(pop);
                m++;
            }
        } else if (getin[i].Operator == "WH") {
            seml[s++] = m;//push p

        }
    }

    cout << "Ŀ���������:" << endl;
    for (int i = 0; i < m; i++) {
        cout << language[i].ta << "\t";
        cout << language[i].Operatore << "\t";
        cout << language[i].Operand1 << ",";
        cout << language[i].Operand2 << endl;
    }

}

void translate() {
    getfouryuan();
    targetlan();
}

