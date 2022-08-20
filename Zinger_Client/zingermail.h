#ifndef ZINGERMAIL_H
#define ZINGERMAIL_H

#include <QtNetwork/QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class zingerMail; }
QT_END_NAMESPACE

class zingerMail : public QObject
{
    Q_OBJECT

public:

    //costructor with parameters: Mail to, Mail confimation code, User's firstname and lastname
    zingerMail(const QString &to, const QString &code, const QString &names);
    ~zingerMail();

private slots:
    void connectedToServer(); //if we are successfully connected to the host
    void onReadyRead(); //if there is data to be read

private:
    QString message; //holds the email message
    QTextStream strm; //Text stream to answer on the host feedbacks
    QSslSocket socket; //Secure socket layer
    QString to;//reciever
    enum steps {initialState, login, Username, Password, mailContent, To, dataReady, msgBody, finish}; //Steps for communicating with the host
    int step;
};
#endif // ZINGERMAIL_H
