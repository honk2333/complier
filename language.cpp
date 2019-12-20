#include"complier.h"
#include"qDebug"

using namespace std;

extern vector<QT_str> QT_arr;
struct target {   //目标语句结构体
    int t;
    int ta;
    string Operand1;
    string Operand2;
    string Operatore;
} tout[maxn * 10];
int index = 0;
QT_str getin[maxn];
target language[maxn];
void Init_language(){
    index=0;
}
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
    for (int i = 0; i < index - 1; i++)    //消除重复语句
    {

        for (int j = i + 1; j < index; j++) {
            if ((getin[i].Operand1 == getin[j].Operand1) && (getin[i].Operand2 == getin[j].Operand2) &&
                (getin[i].Operator == getin[j].Operator) && (getin[i].Operator != "JMP") &&
                (getin[i].Operator != "J<") &&
                (getin[i].Operator != "J>") && (getin[i].Operator != "J>=") && (getin[i].Operator != "J<=")) {

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
    cout << "消除重复定义成功" << endl;

    //消除重复语句
    for (int i = 0; i < index; i++) {
        for(int j=i+1;j < index; j++)
        {
        if ((getin[i].Operator == getin[j].Operator) && (getin[i].Operand1 == getin[j].Operand1) &&
            (getin[i].Operand2 == getin[j].Operand2) &&
            (getin[i].result == getin[j].result) &&
            (getin[i].Operator != "EL" && getin[i].Operator != "IE" &&  getin[i].Operator != "WE" &&
                     getin[i].Operator != "WH"&& getin[i].Operator != "JMP" &&  getin[i].Operator != "J<" &&  getin[i].Operator != "J>" &&
                     getin[i].Operator != "J<=" &&  getin[i].Operator != "J>=")) {
            for (int k = j + 1; k < index; k++) {
            getin[k-1].Operator = getin[k].Operator;
            getin[k-1].Operand1 = getin[k].Operand1;
            getin[k-1].Operand2 = getin[k].Operand2;
            getin[k-1].result = getin[k].result;}
            for (int k = 0; k < index; k++) {
                if (getin[k].Operator == "JMP" || getin[k].Operator == "J<" || getin[k].Operator == "J>" ||
                    getin[k].Operator == "J<=" || getin[k].Operator == "J>=") {
                    int g;
                    g = stoi(getin[k].result);
                    if (g > j+1) { getin[k].result = to_string(g - 1); }

                }
            }
            // }
            index--;
        }
        }

    }
    cout << "消除重复语句成功" << endl;
    cout << "优化后的四元式" << endl;
    //消除空产生式
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

    cout << "消除多余赋值语句成功" << endl;
    for (int i = 0; i < index; i++) {
        for (int j = i + 1; j < index; j++) {
            //消除多余赋值四元式
            if ((getin[i].result == getin[i + 1].result) && (getin[i].result != "_")) {
                for (int j = i + 1; j < index; j++) {
                    getin[j].Operator = getin[j + 1].Operator;
                    getin[j].Operand1 = getin[j + 1].Operand1;
                    getin[j].Operand2 = getin[j + 1].Operand2;
                    getin[j].result = getin[j + 1].result;
                    for (int k = 0; k < index; k++) {
                        if ((getin[k].Operator == "JMP" && getin[k].Operand1 != "FUN") || getin[k].Operator == "J<" ||
                            getin[k].Operator == "J>" || getin[k].Operator == "J<=" || getin[k].Operator == "J>=") {
                            int g;
                            g = stoi(getin[k].result);
                            if (g > j + 1) { getin[k].result = to_string(g - 1); }

                        }
                    }
                }
                index--;
            }
        }
    }
    cout << "优化后的四元式" << endl;
    for (int i = 0; i <= index - 1; i++) {
        cout << "(" << getin[i].Operator;
        cout << ",";
        cout << getin[i].Operand1;
        cout << ",";
        cout << getin[i].Operand2;
        cout << ",";
        cout << getin[i].result << ")" << endl;
    }


    cout << "消除空产生式成功" << endl;
    //消除常数产生式
     string cons[50][100];//定义常数字符串数组
      int m,n;//定义常数字符串数组指针
      m=n=0;
      int co[50][100];
      int co1,co2;
      for(int i=0;i<index;i++)
      {



  //形如同w,B,C,A的四元式

          if (getin[i].Operator == "+"||getin[i].Operator == "-"||getin[i].Operator == "*"||getin[i].Operator == "/") {
              int c1, c2;
              c1 = c2 = 0;
              for (int c=0; c < getin[i].Operand1.size(); c++) {//s1.s1为常数
                  int n = (int) getin[i].Operand1[c];
                  if (n >= 48 && n <= 57) {
                      continue;
                  } else {
                      c1 = 1;

                      break;
                  }

              }
              for (int c=0; c < getin[i].Operand2.size(); c++) {
                  int n = (int) getin[i].Operand2[c];
                  if (n >= 48 && n <= 57) {
                      continue;
                  } else {
                      c2 = 1;
                      break;
                  }

              }
              for(int j=0;j<n;j++)//s1.s2在常数列表内
              {

                  if(getin[i].Operand1==cons[m][j])
                  {
                      co1=co[m][j];
                      c1=2;
                      break;
                  }
              }
              for(int j=0;j<n;j++)
              {

                  if(getin[i].Operand2==cons[m][j])
                  {
                      co2=co[m][j];
                      c2=2;
                      break;
                  }
              }

              if (c1 == 0 && c2 == 0) {
                  int s;
                  if(getin[i].Operator=="+")
                  {
                      s = stoi(getin[i].Operand1) + stoi(getin[i].Operand2);
                  }else if(getin[i].Operator=="-")
                  {
                      s = stoi(getin[i].Operand1) - stoi(getin[i].Operand2);
                  }else if(getin[i].Operator=="*")
                  {
                      s = stoi(getin[i].Operand1) * stoi(getin[i].Operand2);
                  }else if(getin[i].Operator=="/")
                  {
                      s = stoi(getin[i].Operand1) / stoi(getin[i].Operand2);
                  }
                  getin[i].Operator = "=";
                  getin[i].Operand1 = to_string(s);
                  getin[i].Operand2 = "_";
                  cons[m][n] = getin[i].result;
                  co[m][n]=s;

                  n++;
              }else if(c1==2&&c2==0){
                  int s;
                  if(getin[i].Operator=="+")
                  {
                      s = co1 + stoi(getin[i].Operand2);
                  }else if(getin[i].Operator=="-")
                  {
                      s = co1 - stoi(getin[i].Operand2);
                  }else if(getin[i].Operator=="*")
                  {
                      s = co1 * stoi(getin[i].Operand2);
                  }else if(getin[i].Operator=="/")
                  {
                      s = co1 / stoi(getin[i].Operand2);
                  }
                  getin[i].Operator = "=";
                  getin[i].Operand1 = to_string(s);
                  getin[i].Operand2 = "_";
                  cons[m][n] = getin[i].result;
                  co[m][n]=s;

                  n++;
              }else if(c1==0&&c2==2){
                  int s;
                  if(getin[i].Operator=="+")
                  {
                      s = stoi(getin[i].Operand1) + co2;
                  }else if(getin[i].Operator=="-")
                  {
                      s = stoi(getin[i].Operand1) - co2;
                  }else if(getin[i].Operator=="*")
                  {
                      s = stoi(getin[i].Operand1) * co2;
                  }else if(getin[i].Operator=="/")
                  {
                      s = stoi(getin[i].Operand1) / co2;
                  }
                  getin[i].Operator = "=";
                  getin[i].Operand1 = to_string(s);
                  getin[i].Operand2 = "_";
                  cons[m][n] = getin[i].result;
                  co[m][n]=s;

                  n++;
              }else if(c1==2&&c2==2){
                  int s;
                  if(getin[i].Operator=="+")
                  {
                      s = co1 + co2;
                  }else if(getin[i].Operator=="-")
                  {
                      s = co1 - co2;
                  }else if(getin[i].Operator=="*")
                  {
                      s = co1 * co2;
                  }else if(getin[i].Operator=="/")
                  {
                      s = co1 / co2;
                  }
                  getin[i].Operator = "=";
                  getin[i].Operand1 = to_string(s);
                  getin[i].Operand2 = "_";
                  cons[m][n] = getin[i].result;
                  co[m][n]=s;

                  n++;
              }

          }else if(getin[i].Operator=="PROGRAM"||getin[i].Operator=="WH"||getin[i].Operator=="FOR")
          {
              m++;
          }else if(getin[i].Operator=="END"||getin[i].Operator=="WE"||getin[i].Operator=="FE")
          {
              m--;
          }
          /*else if(getin[i].Operator=="="){//形如同=,B,_,A的四元式

                  for(int j=0;j<n;j++)//s1在常数列表内
              {

                  if(getin[i].Operand1==cons[m][j])
                  {
                      co1=co[m][j];
                      c1=2;
                      break;
                  }
              }

              if (c1 == 2) {
                  getin[i].Operator = "=";
                  getin[i].Operand1 = to_string(co1);
                  getin[i].Operand2 = "_";
                  cons[m][n] = getin[i].result;
                  co[m][n]=co1;
                  n++;

              }
          }*/





      }
    cout << "消除常数表达式成功" << endl;

    cout << "优化后的四元式" << endl;
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


int targetnum;

void targetlan(MyWindow* ww) {
    //输入化简后的四元式


    int seml[50];//目标语言的sem栈
    int s = 0;//sem栈指针
    int forl[50];//for语句栈
    int f = 0;//for语句栈指针

    int L[3][index - 1];//设立L标志位
    string RDL = "0";//初始化寄存器R的值为0
   // int depth,p;
   // depth=p=0;
    for (int p = 0; p <= 2; p++) {
        for (int n = 0; n < index; n++) {
            //for(depth = 0; depth <1000;depth++)

            L[p][n] = 1;

        }
    }
    //初始化全部四元式L状态为yes（1）

    for (int i = 0; i < index; i++) {
        /*if(getin[i].Operator=="PROGRAM"||getin[i].Operator=="EL")
        {
            p++;
            L[0][i][p]=L[1][i][p]=L[2][i][p]=2;
        }else if(getin[i].Operator=="DO"||getin[i].Operator=="IF")
        {
            p++;
            L[1][i][p]=L[2][i][p]=2;
            L[0][i][p]=0;
        } else if (getin[i].Operator=="END"){
            p++;
            L[0][i][p]=L[1][i][p]=L[2][i][p]=2;
        }else if (getin[i].Operator=="WE"||getin[i].Operator=="FE")){
            p--;
            L[0][i][p]=L[1][i][p]=L[2][i][p]=2;
        }*/

        for (int j = i + 1; j < index; j++) {     //基本块的内部活跃信息
            if(getin[j].Operator=="PROGRAM"||getin[j].Operator=="DO"||getin[j].Operator=="WH"||getin[j].Operator=="WE"||
            getin[j].Operator=="IF"||getin[j].Operator=="EL"||getin[j].Operator=="IE"||getin[j].Operator=="END"||
            getin[j].Operator=="FOR"||getin[j].Operator=="FE")
            {
                break;
            }
            if (getin[i].result == getin[j].Operand1) {
                for (int k = j + 1; k < index; k++) {
                    if (getin[j].Operand1 != getin[k].Operand1 && getin[j].Operand1 != getin[k].Operand2)
                    {L[0][j] = 0;}
                    else{L[0][j]= 1;break;}

                }

            }
            if (getin[i].result == getin[j].Operand2) {
                for (int k = j + 1; k < index; k++) {
                    if (getin[j].Operand2 != getin[k].Operand1 && getin[j].Operand2 != getin[k].Operand1)
                    {L[1][j]= 0;}
                    else{L[1][j] = 1;break;}

                }
            }
            if (getin[i].Operand1 == getin[j].Operand1&&L[0][i]==1) {
                for (int k = j + 1; k < index; k++) {
                    if (getin[j].Operand1 != getin[k].Operand1 && getin[j].Operand1 != getin[k].Operand2)
                    {L[0][j] = 0;}
                    else{L[0][j]= 1;break;}

                }

            }
            if (getin[i].Operand1 == getin[j].Operand2&&L[0][i]==1) {
                for (int k = j + 1; k < index; k++) {
                    if (getin[j].Operand1 != getin[k].Operand1 && getin[j].Operand1 != getin[k].Operand2)
                    {L[1][j] = 0;}
                    else{L[1][j]= 1;break;}

                }

            }
            if (getin[i].Operand2 == getin[j].Operand1&&L[1][i]==1) {
                for (int k = j + 1; k < index; k++) {
                    if (getin[j].Operand1 != getin[k].Operand1 && getin[j].Operand1 != getin[k].Operand2)
                    {L[0][j] = 0;}
                    else{L[0][j]= 1;break;}

                }

            }
            if (getin[i].Operand2 == getin[j].Operand2&&L[1][i]==1) {
                for (int k = j + 1; k < index; k++) {
                    if (getin[j].Operand1 != getin[k].Operand1 && getin[j].Operand1 != getin[k].Operand2)
                    {L[1][j] = 0;}
                    else{L[1][j] = 1;break;}

                }

            }
        }
    }

//生成目标代码，存入目标代码结构体
    int m = 0;
    for (int i = 0; i < index; i++) {

        if (getin[i].Operator == "+") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "ADD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ADD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ADD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ADD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    //输出运算符号
                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ADD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                }
            } else {
                for (int j = i; j >= 0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ADD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ADD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ADD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ADD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ADD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }

                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "-") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "SUB";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "SUB";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "SUB";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }

            } else {
                for (int j = i-1; j >= 0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "SUB";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "SUB";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "SUB";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "SUB";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "SUB";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }

                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "*") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "MUL";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "MUL";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "MUL";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LD";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    //输出运算符号
                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "MUL";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                }
            } else {
                for (int j = i; j >= 0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "MUL";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "MUL";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "MUL";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "MUL";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "MUL";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }

                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "/") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "DIV";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "DIV";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "DIV";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else {
                for (int j = i-1; j >= 0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "DIV";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "DIV";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "DIV";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "DIV";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "DIV";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }

                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "<") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LT";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            }  else {
                for (int j = i-1; j >= 0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LT";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }

                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == ">") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "GT";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "GT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "GT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            }  else {
                for (int j = i-1; j >= 0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "GT";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }

                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "<=") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LE";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            }  else {
                for (int j = i-1; j >= 0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LE";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }

                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == ">=") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "GE";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "GE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "GE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            }  else {
                for (int j = i-1; j >= 0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "GE";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;
                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "=") {
            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
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
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            break;

                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            break;

                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            break;

                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        break;

                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "==") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "EQ";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "EQ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "EQ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "EQ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    //输出运算符号
                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "EQ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                }
            } else {
                for (int j = 0; j < i; j++) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "EQ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "EQ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "EQ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "EQ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "EQ";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "!=") {
            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "NE";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "NE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "NE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "NE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    //输出运算符号
                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "NE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                }
            } else {
                for (int j = 0; j < i; j++) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "NE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "NE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "NE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "NE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "NE";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        break;

                    }
                }
            }
            RDL = getin[i].result;
        } else if (getin[i].Operator == "IF" || getin[i].Operator == "DO") {

            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "FJ";
                language[m].Operand1 = "R";
                language[m].Operand2 = "_";
                m++;
                seml[s++] = m;//push p


            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;

                    //输出运算符号
                } else {
                    language[m].t = i;
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
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "FJ";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = "_";
                        m++;
                        break;

                    }
                }
                seml[s++] = m;//push p
                RDL = "0";
            }

        } else if (getin[i].Operator == "EL") {
            int flag = 0;
            for (int j = i - 1; j >= 0; j--) {
                if (RDL == getin[j].result) { flag = 1; }
                else if (RDL == getin[j].Operand1) {
                    if (L[0][j] == 1) {
                        flag = 1;
                    }
                } else if (RDL == getin[j].Operand2) {
                    if (L[1][j] == 1) {
                        flag = 1;
                    }
                }

            }
            if (RDL != "0" && flag == 1) {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
            }
            s--;
            int pop = seml[s];
            seml[s] = 0;

            language[pop - 1].Operand2 = to_string(m + 1);

            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "JMP";
            language[m].Operand1 = "_";
            language[m].Operand2 = "_";
            m++;
            seml[s++] = m;
            RDL="0";

        } else if (getin[i].Operator == "IE") {

            int flag = 0;
            for (int j = i - 1; j >= 0; j--) {
                if (RDL == getin[j].result) { flag = 1; }
                else if (RDL == getin[j].Operand1) {
                    if (L[0][j] == 1) {
                        flag = 1;
                    }
                } else if (RDL == getin[j].Operand2) {
                    if (L[1][j] == 1) {
                        flag = 1;
                    }
                }

            }
            if (RDL != "0" && flag == 1) {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
            }

            s--;
            int pop = seml[s];
            seml[s] = 0;

            language[pop - 1].Operand2 = to_string(m);
            RDL="0";

        } else if (getin[i].Operator == "WE") {
            int flag = 0;
            for (int j = i - 1; j >= 0; j--) {
                if (RDL == getin[j].result) { flag = 1; }
                else if (RDL == getin[j].Operand1) {
                    if (L[0][j] == 1) {
                        flag = 1;
                    }
                } else if (RDL == getin[j].Operand2) {
                    if (L[1][j] == 1) {
                        flag = 1;
                    }
                }

            }

            if (RDL != "0" && flag == 1) {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
            }
            RDL = "0";

            s--;
            int pop = seml[s];
            seml[s] = 0;

            language[pop - 1].Operand2 = to_string(m + 1);
            s--;
            pop = seml[s];
            seml[s] = 0;
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "JMP";
            language[m].Operand1 = "_";
            language[m].Operand2 = to_string(pop + 1);
            m++;

        } else if (getin[i].Operator == "FE") {

            int flag = 0;
            for (int j = i - 1; j >= 0; j--) {
                if (RDL == getin[j].result) { flag = 1; }
                else if (RDL == getin[j].Operand1) {
                    if (L[0][j] == 1) {
                        flag = 1;
                    }
                } else if (RDL == getin[j].Operand2) {
                    if (L[1][j] == 1) {
                        flag = 1;
                    }
                }

            }
            //if (RDL == getin[i - 2].result) {
            if (RDL != "0"&& flag == 1) {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
            }
            RDL = "0";

            s--;
            int pop = seml[s];
            seml[s] = 0;

            language[pop - 1].Operand2 = to_string(m + 1);
            /*s--;
            pop = seml[s];
            seml[s] = 0;*/
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "JMP";
            language[m].Operand1 = "_";
            language[m].Operand2 = to_string(pop + 1);//
            m++;

        } else if (getin[i].Operator == "WH") {
            seml[s++] = m;//push p
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "_";
            language[m].Operand1 = "_";
            language[m].Operand2 = "_";
            m++;

        } else if (getin[i].Operator == "FOR") {
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "_";
            language[m].Operand1 = "_";
            language[m].Operand2 = "_";
            m++;


        } else if (getin[i].Operator == "JMP" && getin[i].Operand1 != "FUN") {
            // if(RDL=="0"){

            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "JMP";
            language[m].Operand1 = getin[i].Operand1;
            language[m].Operand2 = "_";
            m++;
            /*}else{
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
                RDL = "0";
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "JMP";
                language[m].Operand1 = getin[i].Operand1;
                language[m].Operand2 = "_";
                m++;
            }*/


        } else if (getin[i].Operator == "JMP" && getin[i].Operand1 == "FUN") {
            //if(RDL=="0"){

            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "JMP";
            language[m].Operand1 = getin[i].Operand1;
            language[m].Operand2 = getin[i].result;
            m++;
            /*}else{
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
                RDL = "0";
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "JMP";
                language[m].Operand1 = getin[i].Operand1;
                language[m].Operand2 = getin[i].result;
                m++;
            }*/

        } else if (getin[i].Operator == "J<") {
            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LT";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "FJ";
                language[m].Operand1 = "R";
                language[m].Operand2 = "_";
                m++;

            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;
                }
            } else if (RDL == getin[i].Operand2) {
                if (L[1][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;
                    //输出运算符号
                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;
                }
            } else {
                for (int j = i-1; j >=0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LT";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "FJ";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = "_";
                        m++;
                        break;

                    }
                }
            }

            RDL = "0";
            seml[s++] = m;
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "TJ";
            language[m].Operand1 = "R";
            language[m].Operand2 = "_";
            m++;

        } else if (getin[i].Operator == "J>") {
            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "GT";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "FJ";
                language[m].Operand1 = "R";
                language[m].Operand2 = "_";
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "GT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "GT";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;
                }
            }  else {
                for (int j = i-1; j >=0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GT";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "GT";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "FJ";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = "_";
                        m++;
                        break;

                    }
                }
            }
            RDL = "0";
            seml[s++] = m;
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "TJ";
            language[m].Operand1 = "R";
            language[m].Operand2 = "_";
            m++;

        } else if (getin[i].Operator == "J<=") {
            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LE";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "FJ";
                language[m].Operand1 = "R";
                language[m].Operand2 = "_";
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "LE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;
                }
            }  else {
                for (int j = i-1; j >=0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LE";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "FJ";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = "_";
                        m++;
                        break;

                    }
                }
            }
            RDL = "0";
            seml[s++] = m;
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "TJ";
            language[m].Operand1 = "R";
            language[m].Operand2 = "_";
            m++;

        } else if (getin[i].Operator == "J>=") {
            if (RDL == "0") {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "LD";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand1;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "GE";
                language[m].Operand1 = "R";
                language[m].Operand2 = getin[i].Operand2;
                m++;
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "FJ";
                language[m].Operand1 = "R";
                language[m].Operand2 = "_";
                m++;
            } else if (RDL == getin[i].Operand1) {
                if (L[0][i] == 1) {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "ST";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand1;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "GE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;


                } else {
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "GE";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = getin[i].Operand2;
                    m++;
                    language[m].t = i;
                    language[m].ta = m + 1;
                    language[m].Operatore = "FJ";
                    language[m].Operand1 = "R";
                    language[m].Operand2 = "_";
                    m++;
                }
            } else {
                for (int j = i-1; j >=0; j--) {
                    if (RDL == getin[j].Operand1) {
                        if (L[0][i] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;


                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].Operand2) {
                        if (L[1][j] == 1) {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "ST";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = RDL;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        } else {
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "LD";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand1;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "GE";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = getin[i].Operand2;
                            m++;
                            language[m].t = i;
                            language[m].ta = m + 1;
                            language[m].Operatore = "FJ";
                            language[m].Operand1 = "R";
                            language[m].Operand2 = "_";
                            m++;
                            break;
                        }
                    } else if (RDL == getin[j].result) {

                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "ST";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = RDL;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "LD";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand1;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "GE";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = getin[i].Operand2;
                        m++;
                        language[m].t = i;
                        language[m].ta = m + 1;
                        language[m].Operatore = "FJ";
                        language[m].Operand1 = "R";
                        language[m].Operand2 = "_";
                        m++;
                        break;

                    }
                }
            }
            RDL = "0";
            seml[s++] = m;
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "TJ";
            language[m].Operand1 = "R";
            language[m].Operand2 = "_";
            m++;

        } else if (getin[i].Operator == "PROGRAM") {

            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "PROGRAM";
            language[m].Operand1 = getin[i].Operand1;
            language[m].Operand2 = "_";
            m++;

        } else if (getin[i].Operator == "END") {
            int flag = 0;
            for (int j = i - 1; j >= 0; j--) {
                if (RDL == getin[j].result) { flag = 1; }
                else if (RDL == getin[j].Operand1) {
                    if (L[0][j] == 1) {
                        flag = 1;
                    }
                } else if (RDL == getin[j].Operand2) {
                    if (L[1][j] == 1) {
                        flag = 1;
                    }
                }

            }

            if (RDL != "0" && flag == 1) {
                language[m].t = i;
                language[m].ta = m + 1;
                language[m].Operatore = "ST";
                language[m].Operand1 = "R";
                language[m].Operand2 = RDL;
                m++;
            }
            RDL = "0";
            language[m].t = i;
            language[m].ta = m + 1;
            language[m].Operatore = "END";
            language[m].Operand1 = getin[i].Operand1;
            language[m].Operand2 = "_";
            m++;
        }
    }

    for (int i = 0; i <= m; i++) {
        if (language[i].Operatore == "JMP" && language[i].Operand2 == "_") {
            int f, g;
            f = language[i].t;
            g = stoi(getin[f].result);
            for (int j = 0; j < m; j++) {
                if (g == language[j].t) {
                    language[i].Operand2 = getin[f].result;
                    break;
                }
            }
        } else if (language[i].Operatore == "JMP" && language[i].Operand1 == "FUN") {
            for (int j = 0; j < m; j++) {
                if (language[i].Operand2 == language[j].Operand1 && language[j].Operatore == "PROGRAM") {
                    language[i].Operand2 = to_string(j + 1);
                    break;
                }
            }
        }
    }

    for (int i = 0; i <= m; i++) {
        if (language[i].Operatore == "TJ" && language[i].Operand2 == "_") {
            int f, g;
            f = language[i].t;
            g = stoi(getin[f].result) - 1;
            for (int j = 0; j < m; j++) {
                if (g == language[j].t) {
                    language[i].Operand2 = getin[f].result;
                    break;
                }
            }
        }
    }


    for (int i = 0; i < index; i++) {
        if (getin[i].Operator == "") { break; }

        else if (getin[i].Operator != "IE" && getin[i].Operator != "FE" && getin[i].Operator != "WE" &&
                 getin[i].Operator != "WH" && getin[i].Operator != "PROGRAM" &&
                 getin[i].Operator != "END"&&getin[i].Operator != "DO"&&getin[i].Operator != "IF") {
            cout << getin[i].Operator<< "\t";
            if (getin[i].Operand1 != "_")
                cout << getin[i].Operand1 << "(" << L[0][i] << ")"<< "\t";
            else { cout << getin[i].Operand1<< "\t"; }
            if (getin[i].Operand2 != "_")
                cout << getin[i].Operand2 << "(" << L[1][i] << ")"<< "\t";
            else { cout << getin[i].Operand2<< "\t"; }
            if (getin[i].Operator != "J<" && getin[i].Operator != "J>" && getin[i].Operator != "J>=" &&
                getin[i].Operator != "<=" && getin[i].Operator != "JMP")
                cout << getin[i].result << "(" << L[2][i] << ")" << endl;
            else { cout << getin[i].result << endl; }
        } else if (getin[i].Operator == "DO"||getin[i].Operator == "IF"){
            cout << getin[i].Operator<< "\t";
            cout << getin[i].Operand1<< "(0)"<< "\t";
            cout << getin[i].Operand2<< "\t";
            cout << getin[i].result << endl;

        }else {
                cout << getin[i].Operator<< "\t";
                cout << getin[i].Operand1<< "\t";
                cout << getin[i].Operand2<< "\t";
                cout << getin[i].result << endl;
            }
        }

    //cout << "目标代码如下:" << endl;
    string ss="";
    char data[100];
    for (int i = 0; i < m; i++) {
        sprintf(data,"%-3d%-3d%-8s%-9s%-3s\n",language[i].t,language[i].ta,language[i].Operatore.c_str(),language[i].Operand1.c_str() ,language[i].Operand2.c_str());
        ss=ss+data;
    }
    //qDebug()<<QString::fromStdString(ss)<<endl;
    ww->clear_language();
    ww->print_language(QString::fromLocal8Bit(ss.c_str()));

    //cout << "输出完毕" << endl;

    targetnum=m-1;
}

void translate(MyWindow* ww) {
    Init_language();
    getfouryuan();
    targetlan(ww);
}

