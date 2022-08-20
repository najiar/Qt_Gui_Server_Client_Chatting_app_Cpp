#include "zingermail.h"
#include "mainwindow.h"
#include"register.h"

zingerMail::zingerMail(const QString &to, const QString &code, const QString &names)
{
    connect(&socket, &QSslSocket::readyRead, this, &zingerMail::onReadyRead); //when there is data to be read call onReadyRead function
    connect(&socket, &QSslSocket::connected, this, &zingerMail::connectedToServer); //when we are successfully connected to the host call connectedToServer

    //message that will be sent to user's email
    message = "To: " + to + "\n";
    message.append("From: Your_email\n");
    message.append("Subject: Zinger code\n");
    message.append("Welcome ").append(names).append(" to Zinger Chat!").append("\n");
    message.append("Your email confirmation code is: (").append(code).append(")\n");
    message.append("We wish you an amazing experience!").append("\n");
    message.append("Zinger chat team").append("\n");
    this->to = to; //set the reciever

    step = initialState; //put the state to initial

    socket.connectToHostEncrypted("smtp.gmail.com", 465); //connect to host

    if (!socket.waitForConnected(40000)) //if we pass the timeout due
    {
         qDebug() << socket.errorString();
    }
    strm.setDevice(&socket); //we set the stream device to be our ssl socket
}
zingerMail::~zingerMail()
{
    deleteLater(); //signal is emitted
}
void zingerMail::connectedToServer()
{
    qDebug() << "Connected successfully to the server!.";
}

void zingerMail::onReadyRead()
{
    QString serverFeedback; //to hold the host feedback

    do
    {
        serverFeedback = socket.readLine(); //get the host feedback
    }
    while (socket.canReadLine() && serverFeedback[3] != ' '); //while there is feedback from the host
    serverFeedback.truncate(3); //get first 3 chars only from the feedback

    if (step == initialState && serverFeedback == "220") //joining state
    {
        strm << "HELO Zinger" <<"\r\n"; //stream HELO handshake
        strm.flush();
        step = login; //next step
    }
    else if (step == login && serverFeedback == "250") //Auth state
    {
        strm << "AUTH LOGIN" << "\r\n";//stream AUTH LOGIN to start the authentication with the host
        strm.flush();
        step = Username; //next step
    }
    else if (step == Username && serverFeedback == "334")
    {
        strm <<  QByteArray().append("Your_email").toBase64()<< "\r\n"; //stream the username converted to Base64
        strm.flush();
        step = Password; //next step
    }
    else if (step == Password && serverFeedback == "334")
    {
        strm << QByteArray().append("Your_Password (Email token)").toBase64() << "\r\n"; //stream the password converted to Base64
        strm.flush();
        step = mailContent; //next step
    }
    else if (step == mailContent && serverFeedback == "235")
    {
        strm << "MAIL FROM:<" << "Your_email" << ">\r\n"; //Stream the sender email
        strm.flush();
        step = To; //next step
    }
    else if (step == To && serverFeedback == "250")
    {
        strm << "RCPT TO:<" << to << ">\r\n"; //r //Stream the reciever email
        strm.flush();
        step = dataReady; //next step
    }
    else if (step == dataReady && serverFeedback == "250")
    {
        strm << "DATA\r\n"; //we stream DATA to tell the host we are ready to stream the message
        strm.flush();
        step = msgBody; //next step
    }
    else if (step == msgBody && serverFeedback == "354")
    {
        strm << message << "\r\n.\r\n"; //stream the message
        strm.flush();
        step = finish;// go to finish state

    }
    else if (step == finish && serverFeedback == "250")
    {
        strm << "QUIT\r\n"; //we tell the host we are quiting
        strm.flush();

    }
    else //last
    {
        qDebug() << "No feedback found from the server!";
    }
}
