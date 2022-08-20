#include "register.h"
#include "ui_register.h"
#include "friendspage.h"
#include "hometab.h"
#include "mainwindow.h"
#include "databaseclass.h"
#include "zingermail.h"
#include <iostream>
#include<QDebug>
#include <QRegularExpression>


using namespace std;

QString registrationCode = "";
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    this->setFixedSize(QSize(683, 383)); //set fixed size for the tab
    Register::setWindowTitle("Register Tab"); //set tab name
    Register::setWindowIcon(QIcon(":/Images/windowIcon")); //set tab icon

    //set text hints for the text inputs
    ui->firstNameInput->setPlaceholderText("First Name");
    ui->lastNameInput->setPlaceholderText("Last Name");
    ui->usernameInput->setPlaceholderText("Username");
    ui->passwordInput->setPlaceholderText("Password");
    ui->emailInput->setPlaceholderText("Email");
    ui->phoneNumberInput->setPlaceholderText("Phone Number");
    ui->fillAllTxt->setAlignment(Qt::AlignCenter); //align the fail text in the center of the tab


    QRegularExpression phoneValidator("^[0-9]*$"); //regex for phonenumber input / only numbers are allowed
    ui->phoneNumberInput->setValidator(new QRegularExpressionValidator(phoneValidator, this)); //set the validator for the phone number input
    ui->phoneNumberInput->setMaxLength(15); //max numbers can be inputed in the phone number input


    QStringList listPhoneCode=(QStringList()<<"+359"<<"+33"<<"+32"<<"+34"<<"+46"<<"+49"<<"+1"<<"+44"); //list of phone country codes allowed
    ui->countryCode->addItems(listPhoneCode); //add the list items to the comboBox
    ui->countryCode->setCurrentIndex(0); //set the default item

    ui->usernameInput->setMaxLength(13); //set the length for the username


    QStringList listGender=(QStringList()<<"Male"<<"Female"); //list of items for gender comboBox
    ui->genderInput->addItems(listGender); //set the list
    ui->genderInput->setCurrentIndex(1); //set the defualt item

    //since the user still didnt pass the register process dont show the verification button and code input fuild
    ui->verifyBtn->setVisible(false);
    ui->codeInput->setVisible(false);


}
Register::~Register()
{
    delete ui;
}
QString Register::getNames() //returns the user's firstname and lastname as a QString / used in the email msg
{
    if(ui->firstNameInput->text() != "" && ui->lastNameInput->text() != "") //if they are not empty
    {
        return ui->firstNameInput->text() + " " + ui->lastNameInput->text(); //return them
    }

    return "";

}
QString Register::randomMailCode(int len) //return the random digit of length n as a QString
{
    srand(time(NULL)); //srand seed

    QString code = "";

    for (int i = 0; i < len; i++)
    {

        int num = ((int)rand() % 10);

        code.append(QString::number(num)); //append the random digit to the result QString
    }
    return code; //return the code
}

void Register::on_registerBtn_clicked() //when the register button is clicked
{
    //password regex: At least: One letter, On uppercase letter, one digit and length between 4 and 15
    QRegularExpression passwordRegex("^(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z]).{4,15}$");
    //username regex: only digits string are not allowed, small letters, upprcase letters, digits, '_' and '-' are allowed
    QRegularExpression usernameRegex("^(?![0-9]+$)[A-Za-z0-9_-]{3,13}$");

    bool passEmptyFields = true; //bool to check if all fields are not empty
    bool passUsernameRegex = true; // bool to check if username regex is passed
    bool passPasswordRegex = true; // bool to check if password regex is passed


    if(ui->firstNameInput->text().isEmpty()) //if firstname filed is empty
    {
         ui->fillAllTxt->setText("Firstname is empty!"); //print to user
         passEmptyFields = false; //wont be true until the user fills the input
    }
    else if(ui->lastNameInput->text().isEmpty()) //same for the rest
    {
         ui->fillAllTxt->setText("Lastname is empty!");
         passEmptyFields = false;
    }
    else if(ui->usernameInput->text().isEmpty())
    {
         ui->fillAllTxt->setText("Username is empty!");
         passEmptyFields = false;
    }
    else if(ui->passwordInput->text().isEmpty())
    {
         ui->fillAllTxt->setText("Password is empty!");
         passEmptyFields = false;
    }
    else if(ui->ageSpin->text() == "0")
    {
         ui->fillAllTxt->setText("Please select age!");
         passEmptyFields = false;
    }
    else if(ui->emailInput->text().isEmpty())
    {
         ui->fillAllTxt->setText("Email is empty!");
         passEmptyFields = false;
    }
    else if(ui->phoneNumberInput->text().isEmpty())
    {
         ui->fillAllTxt->setText("Phonenumber is empty!");
         passEmptyFields = false;
    }
    else if(!usernameRegex.match(ui->usernameInput->text()).hasMatch()) //if input does not pass the regex of the username
    {
            ui->fillAllTxt->setText("Username is not valid!"); //print to user
            passUsernameRegex = false; //wont be true until the user inputs information that passes the regeg
    }
    else if(!passwordRegex.match(ui->passwordInput->text()).hasMatch())//same for the password
    {
            ui->fillAllTxt->setText("Password should contain: One upper, one lower, one digit and length >= 4");
            passPasswordRegex = false;
    }

    if(passEmptyFields) //if we pass empty fields
    {
        if(passUsernameRegex) //if we username and password regex
        {
            if(passPasswordRegex)
            {
                databaseClass db; //open db
                QString queryResult;
                QSqlQuery query;
                query.prepare("SELECT username FROM users WHERE username = ?"); //check if username already taken
                query.addBindValue(ui->usernameInput->text());
                query.exec();
                query.next();
                queryResult = query.value(0).toString();

                if(!queryResult.isEmpty())
                {
                    ui->fillAllTxt->setText("Username already registred"); //print to user
                }
                else //if username is not taken
                {
                    QString names = this->getNames(); //get the user firstname and lastname

                    //Hide registration inputs
                    ui->ageSpin->setVisible(false);
                    ui->backBtn->setVisible(false);
                    ui->emailInput->setVisible(false);
                    ui->firstNameInput->setVisible(false);
                    ui->lastNameInput->setVisible(false);
                    ui->genderInput->setVisible(false);
                    ui->passwordInput->setVisible(false);
                    ui->registerBtn->setVisible(false);
                    ui->phoneNumberInput->setVisible(false);
                    ui->usernameInput->setVisible(false);
                    ui->countryCode->setVisible(false);


                    registrationCode = randomMailCode(4);//generate a random code of length 4
                    //create object of type zingerMail and send the code to user's email
                    mail = new zingerMail(ui->emailInput->text(), registrationCode, names);
                    ui->verifyBtn->setVisible(true); //show verification button
                    ui->codeInput->setVisible(true); //show verification input
                    ui->fillAllTxt->setText("Please enter the verification code from your email!");
                    //when the user inputs the code on_verifyBtn_clicked is called
                }
            }
         }
     }
}


void Register::on_backBtn_clicked()
{
    Register::hide(); //hider register tab
    MainWindow *loginTab= new MainWindow(); //show login tab
    loginTab->show();
}


void Register::on_verifyBtn_clicked() //when the user inputs the code on_verifyBtn_clicked is called
{
    if(ui->codeInput->text() == registrationCode) //if user's inputed code equals the code we sent to the user via email
    {
        //hash the user's password and insert the users informations into the users table in the db
        QString hashedPass = QCryptographicHash::hash(ui->passwordInput->text().toLatin1().toHex(), QCryptographicHash::Sha256).toHex();
        QSqlQuery query;
        query.prepare("insert into users (fname, lname, username, password, age, email, phoneNum, gender, isOnline, isChatting) values (?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(ui->firstNameInput->text());
        query.addBindValue(ui->lastNameInput->text());
        query.addBindValue(ui->usernameInput->text());
        query.addBindValue(hashedPass);
        query.addBindValue(ui->ageSpin->text());
        query.addBindValue(ui->emailInput->text());
        query.addBindValue(ui->countryCode->currentText() + ui->phoneNumberInput->text());
        query.addBindValue(ui->genderInput->currentText());
        query.addBindValue("True"); //set the user to be currently online since he logs in after register
        query.addBindValue("False"); //set isChatting to false since he just joined
        if(!query.exec())
        {
            qDebug("Error inserting to table");
        }
        activeUsername = ui->usernameInput->text();
        Register::hide();
        homeTab homePage;
        homePage.setModal(true);
        homePage.exec();
    }
    else //if the user inputs wrong code
    {
        ui->fillAllTxt->setText("Verification code is wrong, Try again!");
    }
}

