#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QTcpSocket>

class serverSocket : public QTcpSocket
{
    Q_OBJECT

public:
    serverSocket(qintptr handle, QObject *parent);

signals:
    void onReadyRead(serverSocket *); //when socket is sending data

    void onStateChanged(serverSocket *); //when socket is changes state to disconnected
};
#endif // SERVERSOCKET_H
