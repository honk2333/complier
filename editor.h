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
    void print_token(QString s);
    void clear_token();
    void print_wordlist(QString s);
    void clear_wordlist();
    void print_fout(QString s);
    void clear_fout();
    void print_language(QString s);
    void clear_language();
    void print_synbl(QString s);
    void clear_synbl();

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

    void on_action_dosbox_triggered();

    void on_actiongrammer_triggered();

    void on_action_triggered();

    void on_action_first_triggered();

    void on_action_follow_triggered();

    void on_action_select_triggered();

    void on_action_LL1_triggered();

    void on_codeditor_textChanged();

    void on_action_trans_triggered();

    void on_action_ll1_triggered();

    void on_actionmean_triggered();

    void on_actionlanguage_triggered();

    void on_action_runall_triggered();

private:
    Ui::MyWindow *ui;
    QLineEdit *filename;
    bool done_token,done_grammer,done_language,done_mean;
};

#endif // MYWINDOW_H
