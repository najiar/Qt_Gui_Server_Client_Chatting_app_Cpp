#ifndef SERVER_H
#define SERVER_H

#include<QTcpServer>

class serverSocket;
class server : public QTcpServer
{
public:
    server(QObject *parent = nullptr);

    bool startZingerServer(qint16 port); //to start the tcp server

protected:

    void incomingConnection(qintptr handle); //when a new connection joins to the server, readyRead and stateChanged are connected there

private:
    QList<serverSocket *> sockets; //list of tcp sockets //available users
};

#endif // SERVER_H
