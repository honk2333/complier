#include "editor.h"
#include "ui_mywindow.h"
#include "QString"
#include"QTextCursor"
#include"QDebug"
#include"QDir"
#include"QFileDialog"
#include"QLineEdit"
#include"QFile"
#include"QListWidget"
#include"QTableWidget"
#include"QTextCodec"
#include"QMessageBox"
#include"complier.h"
#include"QProcess"

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    filename=new QLineEdit(tr("请打开文件"));
    ui->statusBar->addWidget(filename);
    ui->centralWidget->setFixedSize(1400,680);

    ui->fontComboBox->setCurrentFont(ui->codeditor->font());
    ui->spinBox->setValue(12);
    ui->boldcheck->setCheckState(Qt::Checked);
    setInsertTextColor(Qt::yellow);

    connect(ui->blueradio,SIGNAL(clicked()),this,SLOT(setTextFontColor()));
    connect(ui->yellowradio,SIGNAL(clicked()),this,SLOT(setTextFontColor()));
    connect(ui->greenradio,SIGNAL(clicked()),this,SLOT(setTextFontColor()));
    connect(ui->blackradio,SIGNAL(clicked()),this,SLOT(setTextFontColor()));
    connect(ui->codeditor,SIGNAL(textChanged()),this,SLOT(show_line()));
}

MyWindow::~MyWindow()
{
    delete ui;
}



void MyWindow::show_line(){
    QString s= ui->codeditor->toPlainText();
    int num=1;
    for(int i=0;i<s.size();i++){
        if(s[i]=='\n') num++;
    }
    //qDebug()<<ss;
    ui->margin->clear();
    for(int i=1;i<=num;i++){
        ui->margin->appendPlainText(QString::number(i,10));
    }

}


void MyWindow::on_boldcheck_clicked(bool checked)
{
    QFont font=ui->codeditor->font();
    font.setBold(checked);
    ui->codeditor->setFont(font);
}

void MyWindow::on_italiccheck_clicked(bool checked)
{
    QFont font=ui->codeditor->font();
    font.setItalic(checked);
    ui->codeditor->setFont(font);
}

void MyWindow::on_undercheck_clicked(bool checked)
{
    QFont font=ui->codeditor->font();
    font.setUnderline(checked);
    ui->codeditor->setFont(font);
}
void MyWindow::print_token(QString s)
{
    ui->tokentext->appendPlainText(s);
}

void MyWindow::clear_token()
{
    ui->tokentext->clear();
}

void MyWindow::print_wordlist(QString s)
{
    ui->wordlist->appendPlainText(s);
}

void MyWindow::clear_wordlist()
{
    ui->wordlist->clear();
}

void MyWindow::print_fout(QString s)
{
    ui->foutext->appendPlainText(s);
}

void MyWindow::clear_fout()
{
    ui->foutext->clear();
}

void MyWindow::print_language(QString s)
{
    ui->codetext->appendPlainText(s);
}

void MyWindow::clear_language()
{
    ui->codetext->clear();
}

void MyWindow::print_synbl(QString s)
{
    ui->synbltext->appendPlainText(s);
}

void MyWindow::clear_synbl()
{
    ui->synbltext->clear();
}

void MyWindow::setInsertTextColor(QColor col)
{
             QTextCharFormat fmt;
             fmt.setForeground(col);
             QTextCursor cursor = ui->codeditor->textCursor();
             cursor.mergeCharFormat(fmt);
             ui->codeditor->mergeCurrentCharFormat(fmt); //调用QTextEdit或者QPlianTextEdit的api;
}

void MyWindow::setTextFontColor()
{
    //QPalette plet=ui->codeditor->palette();
    if(ui->blueradio->isChecked()){
        setInsertTextColor(Qt::blue);
    }
    else if(ui->yellowradio->isChecked()){
        setInsertTextColor(Qt::yellow);
    }
    else if(ui->greenradio->isChecked()){
        setInsertTextColor(Qt::green);
    }
    else setInsertTextColor(Qt::black);
    //ui->codeditor->setPalette(plet);
}

void MyWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
     ui->codeditor->setFont(f);
}

void MyWindow::on_groundcomboBox_activated(const QString &arg1)
{
    if(arg1=="黑色背景") {
        ui->codeditor->setStyleSheet("selection-background-color: rgb(149, 149, 149);selection-color: rgb(255, 255, 255);background-color: rgb(56, 56, 56);");
        setInsertTextColor(Qt::yellow);
    }
    else{
        ui->codeditor->setStyleSheet("selection-background-color: rgb(56, 56, 56);selection-color: rgb(149, 149, 149);background-color: rgb(255, 255, 255);");
        setInsertTextColor(Qt::black);
    }

}

void MyWindow::on_spinBox_valueChanged(int arg1)
{
    QFont font=ui->codeditor->font();
    font.setPointSize(arg1);
    ui->codeditor->setFont(font);
}

void MyWindow::on_codeditor_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu=ui->codeditor->createStandardContextMenu();
    menu->exec(pos);
}

void MyWindow::on_actionopenfile_triggered()
{
    QString curpath=QDir::currentPath();
    QString dlgTitle="选择一个文件";
    QString filter="文本文件(*.txt)";
    QString aFilename=QFileDialog::getOpenFileName(this,dlgTitle,curpath,filter);
    if(!aFilename.isEmpty()){
        filename->clear();
        filename->setText(aFilename);
        QFile file(aFilename);
        file.open(QIODevice::ReadOnly);
        QString text=file.readAll();
        ui->codeditor->clear();
        ui->codeditor->appendPlainText(text);
        file.close();
    }
}

void MyWindow::on_actionsavefile_triggered()
{
    QString curpath=QDir::currentPath();
    QString dlgTitle="保存一个文件";
    QString filter="文本文件(*.txt)";
    QString aFilename=QFileDialog::getSaveFileName(this,dlgTitle,curpath,filter);
    if(!aFilename.isEmpty()){
        filename->clear();
        filename->setText(aFilename);
        QFile file(aFilename);
        file.open(QIODevice::WriteOnly);
        QString text=ui->codeditor->toPlainText();
        file.write(text.toUtf8());
        file.close();
    }
}

void MyWindow::on_actionlook_triggered()  //查看最初的文法
{
    QListWidget* grammerlist=new QListWidget();
    grammerlist->resize(800,600);
    QString curpath=QDir::currentPath();
    curpath=curpath+"/grammerlist.txt";
    //qDebug()<<curpath;
    QFile file(curpath);
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        QString s=file.readLine();
        grammerlist->addItem(s);
    }
    grammerlist->show();
}

void MyWindow::on_actiontoken_triggered()    //词法分析按钮
{
    if(done_token){
        QString dlgtitle="词法分析消息框";
        QString strinfo="已经进行过词法分析了";
        QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
    }
    else{
        if(ui->codeditor->toPlainText()!=""){
            QString curpath=QDir::currentPath();
            curpath=curpath+"/cache";  //将当前edit内容缓存
            if(QFile::remove(curpath)){
                /*string FILE=curpath.toStdString();

                const char* File=FILE.c_str();
                freopen(File,"w",stdout);

                QString text=ui->codeditor->toPlainText();
                if(text[text.size()-1]!='#') text=text+'#';
                //qDebug()<<text;
                cout<<text.toStdString();
                fclose(stdout);*/
                QFile File(curpath);
                File.open(QIODevice::WriteOnly);
                QString text=ui->codeditor->toPlainText();
                if(text[text.size()-1]!='#') text=text+'#';
                File.write(text.toUtf8());
                File.close();
                get_token(this);
                done_token=true;
            }
            else{
                QString dlgtitle="词法分析消息框";
                QString strinfo="发生了一些缓存错误";
                QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
                exit(0);
            }
        }
        else{
            QString dlgtitle="词法分析消息框";
            QString strinfo="词法分析的内容为空";
            QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
        }
    }

}

void MyWindow::on_action_dosbox_triggered()
{
    //程序路径
    QString path("C:/DOSBox-0.74-3/DOSBox.exe");
    //QString path("C:/Windows/System32/cmd.exe");
    qDebug()<<path;
    //命令或参数
    QStringList arguments;

    //arguments << "/c" << "shutdown" << "-s" << "-t" << "3000";

    //new一个进程对象
    QProcess *process = new QProcess(this);

    //启动并设置参数
    process->start(path);
 }

void MyWindow::on_actiongrammer_triggered()  //语法分析按钮
{
    if(done_grammer){
        QString dlgtitle="语法分析消息框";
        QString strinfo="已经进行过语法分析了";
        QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
    }else{
        if(done_token==false){
            QString dlgtitle="语法分析消息框";
            QString strinfo="文本内容发生改变,尚未进行词法分析,请先进行词法分析";
            QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
        }
        else{
            ll1(this);
            done_grammer=true;
            get_synbl(this);
        }
    }
}

void MyWindow::on_action_triggered()  //查看消除左递归后的文法
{
    QListWidget* grammerlist=new QListWidget();
    grammerlist->resize(800,600);
    extern production save_np[];
    extern int dnt;
    for(int i=0;i<dnt;i++){
        for(int j=0;j<save_np[i].size;j++){
            QString s = QString::fromStdString(save_np[i].li+" -> "+save_np[i].ri[j]);
            grammerlist->addItem(s);
        }
    }
    grammerlist->show();
}

void MyWindow::on_action_first_triggered()
{
    QListWidget* grammerlist=new QListWidget();
    grammerlist->resize(800,600);
    extern production save_np[];
    extern int dnt;
    for(int i=0;i<dnt;i++){
        for(int j=0;j<save_np[i].size;j++){
            QString s = QString::fromStdString("First of : "+save_np[i].ri[j]+" is :\n");
            for(auto it=save_np[i].first[j].begin();it!=save_np[i].first[j].end();it++){
                s+=QString::fromStdString(*it+"   ");
            }
            grammerlist->addItem(s);
        }
    }
    grammerlist->show();
}

void MyWindow::on_action_follow_triggered()
{
    QListWidget* grammerlist=new QListWidget();
    grammerlist->resize(800,600);
    extern production save_np[];
    extern int dnt;
    for(int i=0;i<dnt;i++){
            QString s = QString::fromStdString("Follow of : "+save_np[i].li+" is :\n");
            for(auto it=save_np[i].follow.begin();it!=save_np[i].follow.end();it++){
                s+=QString::fromStdString(*it+"   ");
            }
            grammerlist->addItem(s);
    }
    grammerlist->show();
}

void MyWindow::on_action_select_triggered()
{
    QListWidget* grammerlist=new QListWidget();
    grammerlist->resize(800,600);
    extern production save_np[];
    extern int dnt;
    for(int i=0;i<dnt;i++){
        for(int j=0;j<save_np[i].size;j++){
            QString s = QString::fromStdString("Select of : "+save_np[i].li+" -> "+save_np[i].ri[j]+" is :\n");
            for(auto it=save_np[i].select[j].begin();it!=save_np[i].select[j].end();it++){
                s+=QString::fromStdString(*it+"   ");
            }
            grammerlist->addItem(s);
        }
    }
    grammerlist->show();
}

void MyWindow::on_action_LL1_triggered()
{
        QTableWidget* grammerlist=new QTableWidget();
        grammerlist->resize(1000,800);
        extern int vn_size;
        extern string vn[];
        extern int vt_size;
        extern string vt[];
        grammerlist->setColumnCount(vt_size+1);
        QStringList sListHeader;
        sListHeader << " ";
        for(int i=0;i<vt_size;i++){  //设置第一行
            sListHeader.append(QString::fromStdString(vt[i]));
        }
        grammerlist->setHorizontalHeaderLabels(sListHeader);

        extern map<pair<string, string>, int> L_table;
        for(int i=0;i<vn_size;i++){ //每一行
            int iRow = grammerlist->rowCount();
            grammerlist->insertRow(iRow);
            QString ss=QString::fromStdString(vn[i]);
            grammerlist->setItem(iRow,0,new QTableWidgetItem(ss));
            for(int j=0;j<vt_size;j++){
               ss=QString::number(L_table[make_pair(vn[i],vt[j])]);
               grammerlist->setItem(iRow,j+1,new QTableWidgetItem(ss));
            }
        }
        grammerlist->show();
}

void MyWindow::on_codeditor_textChanged()  //文本内容改变,需要重新进行词法语法分析
{
    done_grammer=false;
    done_language=false;
    done_token=false;
    done_mean=false;
}

void MyWindow::on_action_trans_triggered()
{
    QListWidget* grammerlist=new QListWidget();
    grammerlist->resize(800,600);
    extern production np[];
    extern int dnt;
    for(int i=0;i<dnt;i++){
        for(int j=0;j<np[i].size;j++){
            QString s = QString::fromStdString(np[i].li+" -> "+np[i].ri[j]);
            grammerlist->addItem(s);
        }
    }
    grammerlist->show();
}

void MyWindow::on_action_ll1_triggered()
{
    if(done_grammer){
        QListWidget* grammerlist=new QListWidget();
        grammerlist->resize(800,600);
        QString curpath=QDir::currentPath();
        curpath=curpath+"/ll1_process";
        //qDebug()<<curpath;
        QFile file(curpath);
        file.open(QIODevice::ReadOnly);
        while(!file.atEnd()){
           // QString s=file.readLine();
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            QString line = codec->toUnicode(file.readLine());
            grammerlist->addItem(line);
        }
        grammerlist->show();
    }
    else{
        QString dlgtitle="查看ll1分析法过程消息框";
        QString strinfo="文本内容发生改变,尚未进行语法分析,请先进行语法分析";
        QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
    }
}

void MyWindow::on_actionmean_triggered()
{
    if(done_mean){
        QString dlgtitle="语义分析消息框";
        QString strinfo="已经进行过语义分析了";
        QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
    }else{
        if(done_grammer==false){
            QString dlgtitle="语义分析消息框";
            QString strinfo="文本内容发生改变,尚未进行语法分析,请先进行语法分析";
            QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
        }
        else{
            get_mean(this);
            done_mean=true;
        }
    }
}

void MyWindow::on_actionlanguage_triggered()
{
    if(done_language){
        QString dlgtitle="生成目标语句消息框";
        QString strinfo="已经进行过目标语句生成了";
        QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
    }else{
        if(done_mean==false){
            QString dlgtitle="生成目标语句消息框";
            QString strinfo="文本内容发生改变,尚未进行语义分析,请先进行语义分析";
            QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
        }
        else{
            translate(this);
            done_language=true;
        }
    }
}

void MyWindow::on_action_runall_triggered()
{
        on_actiontoken_triggered();
        on_actiongrammer_triggered();
        on_actionmean_triggered();
        on_actionlanguage_triggered();
}
