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
#include"QMessageBox"
#include"complier.h"

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    filename=new QLineEdit(tr("请打开文件"));
    ui->statusBar->addWidget(filename);
    ui->centralWidget->setFixedSize(1500,700);

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

void MyWindow::on_actionlook_triggered()
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

void MyWindow::on_actiontoken_triggered()
{
    if(ui->codeditor->toPlainText()!=""){
        QString curpath=QDir::currentPath();
        curpath=curpath+"/cache.txt";  //将当前edit内容缓存
        string FILE=curpath.toStdString();
        const char* File=FILE.c_str();
        freopen(File,"w",stdout);
        QString text=ui->codeditor->toPlainText();
        cout<<text.toStdString();
        fclose(stdout);
        get_token();
    }
    else{
        QString dlgtitle="词法分析消息框";
        QString strinfo="词法分析的内容为空";
        QMessageBox::information(this,dlgtitle,strinfo,QMessageBox::Ok);
    }
}
