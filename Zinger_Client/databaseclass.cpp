#include "databaseclass.h"
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QString>
#include <string>
#include <iostream>
#include "ui_register.h"
#include "register.h"
using namespace std;

databaseClass::databaseClass()
{
    usersDB = QSqlDatabase::addDatabase("QMYSQL"); //set the db type to be MySql
    usersDB.setHostName("YourIP"); //set the host ip
    usersDB.setPort(0000); //set the host port
    usersDB.setUserName("root"); //join like root
    usersDB.setPassword(""); //empty password
    usersDB.setDatabaseName("dbname"); //database name
    if(usersDB.open()) //if connection achived
    {
         qDebug("MYSQL OPENED successfully" );
    }
    else    //if connection failed
    {
        qDebug("DIDNT WORK MYSQL" );
    }

    QSqlQuery databaseQuery_tables(usersDB); //create query with the QSqlDatabase

    //create table users in the database
    databaseQuery_tables.prepare("CREATE TABLE IF NOT EXISTS users (ID INTEGER NOT NULL AUTO_INCREMENT, fname TEXT , lname TEXT , username TEXT  , password TEXT , age TEXT , email TEXT , phoneNum TEXT , gender TEXT, isOnline TEXT, isChatting TEXT, PRIMARY KEY (ID));");
    if (!databaseQuery_tables.exec())
    {
            qDebug("Error creating table users");
            return;
    }
    else
    {
        qDebug("Users Tabel created successfully" );
    }

    //create table friends in the database
    databaseQuery_tables.prepare("CREATE TABLE IF NOT EXISTS friends (ID INTEGER NOT NULL AUTO_INCREMENT, usernameSender TEXT , usernameReceiver TEXT , isFriends TEXT , PRIMARY KEY (ID));");
    if (!databaseQuery_tables.exec())
    {
            qDebug("Error creating table Friends");
            return;
    }
    else
    {
        qDebug("Friends Tabel created successfully" );
    }
    //create table chatting in the database
    databaseQuery_tables.prepare("CREATE TABLE IF NOT EXISTS chatting (ID INTEGER NOT NULL AUTO_INCREMENT, user1 TEXT , user2 TEXT , isChatting TEXT , chatStarted TEXT , chatEnded TEXT , PRIMARY KEY (ID));");
    if (!databaseQuery_tables.exec())
    {
            qDebug("Error creating table chatting");
            return;
    }
    else
    {
        qDebug("chatting Tabel created successfully" );
    }
}


bool databaseClass::checkAuth(QString username, QString password) //check if auth info are correct
{

     QString queryResult; //for the result of the query
     QSqlQuery query; //query
     //look for results in the db for this username and password
     query.prepare("SELECT username, password FROM users WHERE username = ? AND password = ?");
     query.addBindValue(username);
     QString hashedPass = QCryptographicHash::hash(password.toLatin1().toHex(), QCryptographicHash::Sha256).toHex(); //hash password to compare
     query.addBindValue(hashedPass); //bind with hashed password
     query.exec();
     query.next();
     queryResult = query.value(0).toString();

     if(queryResult.isEmpty()) //if its empty no user found of wrong auth
     {
         return false;
     }
     else
     {
         return true;
     }
}
void databaseClass::closeDB()
{

    QSqlDatabase db = QSqlDatabase::database();
    db.close(); //close the db

    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );//Removes the database connection
    return;
}













