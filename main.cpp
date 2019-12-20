#include "editor.h"
#include"complier.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    pre_grammer();  //文法预处理
    make_table();//生成ll1分析表
    new_Right(); //生成翻译文法
    QApplication a(argc, argv);
    MyWindow w;
    w.show();
    return a.exec();
}
