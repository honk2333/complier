//
// Created by Honk on 2019/12/10.
//
#include"complier.h"
#include"QMessageBox"

enum {  //变量类型联合体
    INT, DOUBLE, CHAR, BOOL, ARRAY, FUNCTION, STRUCT
};

struct Record {
    string name;  //名称
    int type;  //类型
    int range;
    string addr;
    //存值
    int ival;
    double dval;
    char cval;
    bool bval;


    int lenl; //长度表
    int low, up, ctp, clen;  //数组表参数
    int level = 0, foff, fn = 0, param, entry; //函数表参数
    void Init_Arr(int a, int b, int c, int d) {  //数组表初始化
        type = ARRAY;
        low = a, up = b, ctp = c, clen = d;
    }

    void Init_Fun(int a, int b, int c, int d) {  //函数表初始化
        type = FUNCTION;
        foff = a;
        fn = b;
        param = c;
        entry = d;
    }
};

struct SymTable {
    vector<map<int, vector<Record>>> Infos;
    set<string> Def{"int", "double", "char", "bool"};
    vector<Record> synbl;
    vector<Record> arr;
    int offset;
};
struct SymTable table;
bool Find(string name){
    for(int i=0;i<table.synbl.size();i++){
        if(table.synbl[i].name==name&&(table.synbl[i].type<=3)) return true;
    }
    return false;
}
void Get_NewRecord(const string type, int range, const string Name, Record& rcd,MyWindow* w) {
    if(Find(Name)){
        string dlgtitle="语法错误";
        string strinfo="单词\""+string(Name)+"\"重复定义";
        QMessageBox::warning(w,QString::fromLocal8Bit(dlgtitle.c_str()),QString::fromLocal8Bit(strinfo.c_str()),QMessageBox::Ok);
        return ;
    }
    else{
        rcd.range = range;
        if (type == "int") {
            rcd.type = INT;
        } else if (type == "double") {
            rcd.type = DOUBLE;
        } else if (type == "bool") {
            rcd.type = BOOL;
        } else if (type == "char") {
            rcd.type = CHAR;
        }
        rcd.name = Name;
        //cout << rcd.name << endl;
        table.synbl.push_back(rcd);
    }

}

void Pop_Record(int range, map<int, vector<Record>> &InfoTable) {
    for (auto &p : InfoTable) {
        vector<Record> &S = p.second;
        if (S.size()) {
            if (S.back().range == range) S.pop_back();
        }
    }
}

int get_type(string s) {
    if (s == "int") return INT;
    else if (s == "double") return DOUBLE;
    else if (s == "char") return CHAR;
    else if (s == "bool") return BOOL;
    return -1;
}

int get_size(int i) {
    if (i == 3 || i == 2) return 1;  //bool类型占1个字节
    if (i == 1) return 8; //double占8个字节
    return 4;  //int占4个字节
}

void Print_RegularTable(MyWindow* w) {
    table.offset=0;//   the address
    string ss;
    char data[100];
    QString out;
    sprintf(data,"%-8s%-8s%-8s%-8s\n", "NAME", "TYP", "CAT", "ADDR");
    ss=data;
    w->clear_synbl();
    out+=QString::fromStdString(ss);
    for (int i = 0; i < table.synbl.size(); ++i) {
        Record r = table.synbl[i];
        sprintf(data,"%-8s",r.name.c_str());
        ss=data;
        out+=QString::fromStdString(ss);
        //printf("%-8s", r.name.c_str());
        if (r.type == 0) {
            sprintf(data,"%-8s", "int");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 1) {
            sprintf(data,"%-8s", "double");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 2) {
            sprintf(data,"%-8s", "char");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 3) {
            sprintf(data,"%-8s", "bool");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 4) {
            sprintf(data,"%-8s", "AINFL");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 5) {
            sprintf(data,"%-8s", "PFINFL");
            ss=data;
            out+=QString::fromStdString(ss);
        }
        if (r.type == 0) {
            sprintf(data,"%-8s", "v");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 1) {
            sprintf(data,"%-8s", "v");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 2) {
            sprintf(data,"%-8s", "v");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 3) {
            sprintf(data,"%-8s", "v");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 4) {
            sprintf(data,"%-8s", "t");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.type == 5) {
            sprintf(data,"%-8s", "f");
            ss=data;
            out+=QString::fromStdString(ss);
        }
        if (0 <= r.type && r.type < 4) {
            char addr[10];
            sprintf(addr, "(%d,%d)", r.range, table.offset);
            table.offset += get_size(r.type);
            sprintf(data,"%-8s\n", addr);
            ss=data;
            r.addr=ss;
            if(table.synbl[i-1].type==4) table.offset+=table.synbl[i].lenl;
            out+=QString::fromStdString(ss);
            //cout<<1<<endl;
        } else if (r.type == 4) {
            sprintf(data,"%-8s\n", "LENL");
            ss=data;
            out+=QString::fromStdString(ss);
            //r.low = d, r.up = d + r.clen * r.lenl;
            //table.offset += r.lenl;

        } else if (r.type == 5) {
            sprintf(data,"%-8s\n", "PFINFL");
            ss=data;
            out+=QString::fromStdString(ss);
        }
    }
    out+="\nLENL\n";
    sprintf(data,"%-8s%-8s\n", "NAME", "LENL");
    ss=data;
    out+=QString::fromStdString(ss);
    for (auto r : table.arr) {
        if(r.type!=4) continue;
        sprintf(data,"%-8s", r.name.c_str());
        ss=data;
        out+=QString::fromStdString(ss);
        sprintf(data,"%d\n", r.lenl);
        ss=data;
        out+=QString::fromStdString(ss);
    }
    out+="\nTYPEL\n";
    //Type Table
    sprintf(data,"%-8s%-8s\n", "NAME", "TYP");
    ss=data;
    out+=QString::fromStdString(ss);
    for (auto r : table.arr) {
        sprintf(data,"%-8s%-8s\n", r.name.c_str(),"AINFL");
        ss=data;
        out+=QString::fromStdString(ss);
    }
    out+="\nAINFL\n";
    sprintf(data,"%-8s%-8s%-8s%-8s%-8s\n", "NAME", "LOW", "UP", "CTP", "CLEN");
    ss=data;
    out+=QString::fromStdString(ss);
    for (auto r : table.arr) {
        sprintf(data,"%-8s%-8d%-8d", r.name.c_str(), r.low, r.up);
        ss=data;
        out+=QString::fromStdString(ss);
        if (r.ctp == 0) {
            sprintf(data,"%-8s", "int");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.ctp == 1) {
            sprintf(data,"%-8s", "double");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.ctp == 2) {
            sprintf(data,"%-8s", "char");
            ss=data;
            out+=QString::fromStdString(ss);
        } else if (r.ctp == 3) {
            sprintf(data,"%-8s", "bool");
            ss=data;
            out+=QString::fromStdString(ss);
        }else if(r.ctp==4){
            sprintf(data,"%-8s", "array");
            ss=data;
            out+=QString::fromStdString(ss);
        }
        sprintf(data,"%-8d\n", r.clen);
        ss=data;
        out+=QString::fromStdString(ss);
    }
    out+="\nPFINFL\n";
    sprintf(data,"%-8s%-8s%-8s%-8s%-8s\n", "LEVEL", "OFF", "FN", "ENTYR", "PARAM");
    ss=data;
    out+=QString::fromStdString(ss);
    sprintf(data,"%-8s%-8s%-8s%-8s%-8s\n", "0", "0", "0", "0", "0");
    ss=data;
    out+=QString::fromStdString(ss);
    out+="\n";
    w->print_synbl(out);
}

extern token token[];
extern int size_token;
void Init_synbl(){
    table.arr.clear();
    table.synbl.clear();
}
void get_synbl(MyWindow* w) {
    Init_synbl();
    int range = 0;  //当前在第几层
    for (int i = 0; i < size_token; i++) {  //定义语句处理
        if (get_type(token[i].word) != -1) {  //变量定义语句;
            if (token[i + 1].group == 'a') {  //定义变量语句
                //cout<<token[i+1].word<<endl;
                Record tmp;
                if (token[i + 2].word != "[") {  //不是数组类型
                    /*if (token[i + 2].word == "=") {  //定义变量并赋值
                        tmp.Init_Var(token[i + 3].word);
                    }*/
                    //cout << token[i + 1].word << endl;
                    Get_NewRecord(token[i].word, range, token[i + 1].word, tmp,w);
                } else {//数组类型
                    int dim = 1; //数组维数
                    int p = i + 5;
                    while (token[p].word != ";" && token[p].word != ",") {
                        if (token[p].word == "[") dim++;
                        p++;
                    }
                    int sum = 1;
                    for(int k=1;k<=dim;k++) {
                        int num = atoi(token[i + k * 3].word.c_str());  //每一维元素个数
                        //tmp.lenl = sum*get_size(get_type(token[i].word));
                        sum = sum * num;
                    }
                    int ssum=sum;
                    for (int k = 1; k<=dim; k++) {
                        int num = atoi(token[i + k * 3].word.c_str());  //每一维元素个数
                        tmp.lenl = sum*get_size(get_type(token[i].word));
                        ssum/=num;
                        tmp.range = range;
                        if (k >1) {
                            tmp.Init_Arr(0, num, get_type(token[i].word), ssum*get_size(get_type(token[i].word)));
                            if(k==dim) tmp.type=get_type(token[i].word);
                            string ss=token[i+1].word;
                            for(int l=1;l<k;l++){
                                ss+=token[i+2*l].word+token[i+2*l+1].word+token[i+2*l+2].word;
                            }
                            //tmp.name=token[i+1].word;
                            //cout<<ss<<endl;
                            Get_NewRecord("array", range, token[i+1].word, tmp,w);
                        } else if(k == 1){
                            //tmp.name=token[i+1].word;
                            tmp.Init_Arr(0, num, 4 ,ssum*get_size(get_type(token[i].word)) );
                            Get_NewRecord("array", range, token[i + 1].word, tmp,w);
                        }
                        table.arr.push_back(tmp);
                    }
                    if(dim==1){
                        //cout<<get_type(token[i].word)<<endl;
                        //tmp.Init_Arr(0, atoi(token[i+3].word.c_str()), get_type(token[i].word), ssum*get_size(get_type(token[i].word)));
                        tmp.type=get_type(token[i].word);
                        Get_NewRecord("array", range, token[i + 1].word, tmp, w);
                        //table.arr.push_back(tmp);
                    }
                }
            }
            int pos = i + 2; //从这个定义的单词往后扫,判断是不是连续定义
            while (token[pos].word != ";" && token[pos].word != ")" && token[pos].word != "(") {
                if (token[pos].word == ",") {
                    if (token[pos + 1].group == 'a') {  //定义变量语句
                        Record tmp;
                        if (token[pos + 2].word != "[") {  //不是数组类型
                            /*if (token[pos + 2].word == "=") {  //定义变量并赋值
                                tmp.Init_Var(token[pos + 3].word);
                            }*/
                            //cout<<token[pos+1].word<<endl;
                            Get_NewRecord(token[i].word, range, token[pos + 1].word, tmp,w);
                        }else{
                            string dlgtitle="语法错误";
                            string strinfo="单词\""+string(token[pos+1].word)+"\"数组类型写在第一个";
                            QMessageBox::warning(w,QString::fromLocal8Bit(dlgtitle.c_str()),QString::fromLocal8Bit(strinfo.c_str()),QMessageBox::Ok);
                            return ;
                        }
                    }
                    pos = pos + 2;
                    continue;
                }
                pos++;
            }
        }
        if(token[i].group=='a'){
            if(!Find(token[i].word)){
                string dlgtitle="语法错误";
                string strinfo="单词\""+string(token[i].word)+"\"未定义";
                QMessageBox::warning(w,QString::fromLocal8Bit(dlgtitle.c_str()),QString::fromLocal8Bit(strinfo.c_str()),QMessageBox::Ok);
                return ;
            }
        }
    }

    Print_RegularTable(w);
}
