#include <QCoreApplication>
#include "server.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    server Server;
    if (!Server.startZingerServer(0000)) //start the server over the port 1999
    {
        qDebug("Error starting server");
        return 1;
    };
    qDebug("Zinger server started");
    return a.exec();
}
