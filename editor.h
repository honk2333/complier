#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include"QLineEdit"

namespace Ui {
class MyWindow;
}
class MyWindow : public QMainWindow
{
    Q_OBJECT

public:

    void setInsertTextColor(QColor col);

    explicit MyWindow(QWidget *parent = nullptr);

    ~MyWindow();

private slots:
    void on_boldcheck_clicked(bool checked);

    void on_italiccheck_clicked(bool checked);

    void on_undercheck_clicked(bool checked);

    void setTextFontColor();


    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_groundcomboBox_activated(const QString &arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_codeditor_customContextMenuRequested(const QPoint &pos);

    void show_line();

    void on_actionopenfile_triggered();

    void on_actionsavefile_triggered();

    void on_actionlook_triggered();

    void on_actiontoken_triggered();

private:
    Ui::MyWindow *ui;
    QLineEdit *filename;
};

#endif // MYWINDOW_H
