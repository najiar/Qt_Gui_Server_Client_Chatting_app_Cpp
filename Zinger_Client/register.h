#ifndef REGISTER_H
#define REGISTER_H

#include "zingermail.h"
#include <QDialog>
#include <stdio.h>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

extern QString registrationCode;

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:

    explicit Register(QWidget *parent = nullptr);
    ~Register();
    QString getNames(); //get the registered user's name and lastname for the email confirmation code


private slots:

    QString randomMailCode(int len); //generates random numbers of length n .. used for  email confirmation code
    void on_registerBtn_clicked();

    void on_backBtn_clicked();


    void on_verifyBtn_clicked(); //when the email confirmation is inserted this function is called for verification

private:
    Ui::Register *ui;
    zingerMail *mail = nullptr; //pointer of type zingerMail
};

#endif // REGISTER_H
