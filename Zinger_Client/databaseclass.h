#ifndef DATABASECLASS_H
#define DATABASECLASS_H
#include <QString>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>


class databaseClass
{
    public:
        QSqlDatabase usersDB; //db
        databaseClass(); //default cons
        bool checkAuth(QString username, QString password); //check if user info are correct
        void closeDB(); //close the connection to the database
};

#endif // DATABASECLASS_H
