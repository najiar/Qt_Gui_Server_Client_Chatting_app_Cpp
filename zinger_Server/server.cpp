#include "server.h"
#include "serversocket.h"
#include <QDebug>
#include <QTextStream>

server::server(QObject *parent)
    :QTcpServer(parent)
{
}

bool server::startZingerServer(qint16 port)
{
    return listen(QHostAddress("yourIp"), port); //start the server

}

void server::incomingConnection(qintptr handle)
{
    auto socket = new serverSocket(handle, this); //initialize the socket
    sockets << socket; //add the socket to the socket list //users available
    qDebug() << "User with socket ID: "<< handle << "got connected to the server!";

    connect(socket, &serverSocket::onReadyRead, this, [=](serverSocket *s) //if a socket is sending data
    {
        qDebug() << "User with ID: "<< handle << " is sending data!";
        QTextStream T(s); //create a text stream with the data
        auto text = T.readAll(); //read the data

        for(auto i : sockets) //send the data to all of the sockets //available users
        {
            QTextStream K(i);
            K << text;
            i->flush();
        }
    });

    connect(socket, &serverSocket::onStateChanged, this, [=](serverSocket *s) //if the socket gets disconnected
    {
        if(socket->state() == QTcpSocket::UnconnectedState)
        {
            qDebug() << "User with socket ID: "<< handle << "got diconected from the server!";
            sockets.removeOne(s); //remove it from the socket list // available users
            s->deleteLater(); //delete it
        }
    });
}
