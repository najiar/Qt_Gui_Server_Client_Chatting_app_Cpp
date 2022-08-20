#include "serversocket.h"

serverSocket::serverSocket(qintptr handle, QObject *parent)
    :QTcpSocket(parent)
{
    setSocketDescriptor(handle);

    connect(this, &serverSocket::readyRead, [&]() //if the socket is sending data
    {
        emit onReadyRead(this); //emit onReadyRead function
    });

    connect(this, &serverSocket::stateChanged, [&]() //if the socket state changes to disconected
    {
        emit onStateChanged(this); //emit onStateChanged function
    });
}
