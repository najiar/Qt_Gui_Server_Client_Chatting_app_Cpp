#include "hometab.h"
#include "ui_hometab.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseclass.h"
#include "friendspage.h"

homeTab::homeTab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::homeTab)
{
    ui->setupUi(this);
    homeTab::setWindowTitle("Home Tab");//set tab name
    homeTab::setWindowIcon(QIcon(":/Images/windowIcon")); //set tab icon
    this->setFixedSize(QSize(680, 382));
    ui->welcomeName->setText(activeUsername); //set the welcome msg with the global username
    ui->welcomeName->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setItalic(true);
    ui->welcomeName->setFont(font);

}

homeTab::~homeTab()
{
    delete ui;
}
void homeTab::closeEvent(QCloseEvent *event) //close event is called when the user closes the app
{
    databaseClass db; //open db
    QSqlQuery query;
    query.prepare("UPDATE users SET isOnline = ? WHERE username = ?"); //set isOnline to false for the user becuse the app was closed
    query.addBindValue("False");
    query.addBindValue(activeUsername);
    query.exec();
}

void homeTab::on_logoutBtn_clicked() //when the logout button is clicked
{
    databaseClass db;
    QSqlQuery query;
    query.prepare("UPDATE users SET isOnline = ? WHERE username = ?"); //set isOnline to false for the user becuse the app was closed
    query.addBindValue("False");
    query.addBindValue(activeUsername);
    query.exec();
    homeTab::hide();

    MainWindow *loginTab= new MainWindow(); //show login screen to the user
    loginTab->show();
}


void homeTab::on_friendsBtn_clicked() //when the friend button is clicked
{
    homeTab::hide(); //hide the home page
    FriendsPage friendsTab; //show friends page
    friendsTab.setModal(true);
    friendsTab.exec();
}

