#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtSql>
#include <QSpinBox>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setquery(QSqlDatabase getdb, QStringList getcourselist,QStringList getnamelist);

private slots:



    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_classeButton_clicked();



private:
    QSqlDatabase newdb;
    QStringList allcourselist2;
    QStringList allnamelist;
    QStringList addgradepoint;
    QStringList addcourse;
    QStringList addcoursenum;
    void achievedata();
    void idtextchanged();
    void nametextchanged();
    void gradeedited();
    void gradetextchanged();
    QLineEdit *lineedit;
    QSpinBox *spinBox;
    QComboBox *combo1;
    QComboBox *combo;
    int count=0;

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
