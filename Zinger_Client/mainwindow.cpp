#include "mainwindow.h"
#include "qevent.h"
#include "qsoundeffect.h"
#include "ui_mainwindow.h"
#include "register.h"
#include "hometab.h"
#include "databaseclass.h"
#include <iostream>

QString activeUsername = "";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setWindowTitle("Login Tab"); //set tab name
    MainWindow::setWindowIcon(QIcon(":/Images/windowIcon")); //set tab icon
    this->setFixedSize(QSize(680, 407)); //set tab size
    ui->passwordInput->setPlaceholderText("Password"); //set text hints for the password and username inputs
    ui->usernameInput->setPlaceholderText("Username");
    ui->failTxt->setAlignment(Qt::AlignCenter); //A text that will only show if auth is not correct
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{

    databaseClass db;
    if(db.checkAuth(ui->usernameInput->text(), ui->passwordInput->text())) //check if login info is correct
    {
            activeUsername = ui->usernameInput->text(); //Global var of the user's username

            databaseClass db; //open database
            QSqlQuery query;
            query.prepare("UPDATE users SET isOnline = ? WHERE username = ?"); //set the user to be online in the database after login
            query.addBindValue("True");
            query.addBindValue(activeUsername);
            query.exec();

            MainWindow::hide(); //hide login tab
            homeTab homePage; //show home tab
            homePage.setModal(true);
            homePage.exec();
    }
    else //if not show fail text
    {
        ui->failTxt->setText("Wrong login info.");
    }

}

void MainWindow::on_registerBtn_clicked()
{
    MainWindow::hide(); //hide login tab
    Register registerPage; //show the register page
    registerPage.setModal(true);
    registerPage.exec();


}

