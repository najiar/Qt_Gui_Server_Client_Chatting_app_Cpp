#include "chatWindow.h"
#include "qlistwidget.h"
#include "ui_chatWindow.h"
#include <QTcpSocket>
#include <QStringList>
#include <QSoundEffect>
#include "friendspage.h"
#include "mainwindow.h"
#include "databaseclass.h"

chatWindow::chatWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::chatWindow)
{
    ui->setupUi(this);
    chatWindow::setWindowTitle("Chat Tab"); //set tab name
    chatWindow::setWindowIcon(QIcon(":/Images/windowIcon")); //set tab icon
    this->setFixedSize(QSize(591, 500)); //set tab fixed size

    ui->msgList->setSpacing(5); //set spacing between messages in the messages List
    ui->msgList->setWordWrap(true); //this function will allow multiline in the list
    ui->msgInput->setPlaceholderText("Type something"); //set Message input text hint

    socket = new QTcpSocket(this); //initialize the tcp socket
    socket->connectToHost("83.148.86.226", 1999); //connect to zinger server "Host"


    connect(socket, &QTcpSocket::connected, this, &chatWindow::loggedIn); //if we successfully connected to the server call LoggedIn
    //emojis
    connect(ui->smileBtn, &QPushButton::clicked, this, [=]() //if an emoji button is clicked
    {
        QTextStream T(socket); //create Text stream and give the socket
        T << activeUsername << ": " << ui->smileBtn->text(); //stream the buttons text which is the emoji
        socket->flush();
    });
    connect(ui->sadBtn, &QPushButton::clicked, this, [=]() //same for all emojies
    {
        QTextStream T(socket);
        T << activeUsername << ": " << ui->sadBtn->text();
        socket->flush();
    });
    connect(ui->heartBtn, &QPushButton::clicked, this, [=]()
    {
        QTextStream T(socket);
        T << activeUsername << ": " << ui->heartBtn->text();
        socket->flush();
    });
    connect(ui->laughBtn, &QPushButton::clicked, this, [=]()
    {
        QTextStream T(socket);
        T << activeUsername << ": " << ui->laughBtn->text();
        socket->flush();
    });
    connect(ui->likeBtn, &QPushButton::clicked, this, [=]()
    {
        QTextStream T(socket);
        T << activeUsername << ": " << ui->likeBtn->text();
        socket->flush();
    });
    connect(socket, &QTcpSocket::readyRead, [&]() //if there is data to be read from the server (when users send)
    {

        QTextStream T(socket); //create text stream with the socket
        auto text = T.readAll(); //read the incoming stream

        QStringList splitt = text.split(":"); //since each message is combined with the sender username we split by :
        QString sender = splitt[0]; //Now the [0] index is the sender username


        databaseClass db;
        QString queryAvailability;
        QSqlQuery query;
        //check if the choosen user to chat with also wants to chat with current user
        //we use chatended to be sure its new chat, if its null it means the chat is new because no ending date yet
        query.prepare("SELECT * FROM chatting WHERE user1 = ? AND user2 = ? AND chatEnded = 'null'" );
        query.addBindValue(activeChatUsername);
        query.addBindValue(activeUsername);
        query.exec();
        query.next();
        queryAvailability = query.value(0).toString();
        db.closeDB();

        if(activeChatUsername == sender && queryAvailability.size() > 0) //if the choosen user is the sender and he choose the current user
        {
             //when a user joins server its username will be streamed using logged in function
            if(activeChatUsername == text) //check if its joining message (only username)
            {
                //create new item for the joining message and give it color, font and aligmnet and add it to the messages list
                QListWidgetItem * joined = new QListWidgetItem(activeChatUsername + " joined the chat!");
                ui->msgList->addItem(joined);
                joined->setTextAlignment(Qt::AlignCenter);
                joined->setForeground(Qt::blue);
                QFont font;
                font.setPointSize(18);
                font.setBold(2);
                joined->setFont(font);
            }

            if(activeChatUsername != text) //if the stream data is not only username, it means its message from the choosen user to chat with
            {
                //create new item for the message and give it color, font and aligmnet and add it to the messages list
                QListWidgetItem * chat = new QListWidgetItem(text);
                ui->msgList->addItem(chat);
                chat->setTextAlignment(Qt::AlignLeft);
                chat->setForeground(Qt::blue);
                QFont font;
                font.setPointSize(16);
                font.setBold(2);
                chat->setFont(font);

                //Since the message is sent to the current user from other user
                //The sound effect will be for recieving messages.
                QSoundEffect *sound = new QSoundEffect; //object of type QSoundEffect
                sound->setVolume(0.25f); //set sound volume
                sound->setSource(QUrl::fromLocalFile(":/Sounds/reciever.wav")); //path to the file
                sound->play(); //play the sound

            }
        }
        if(activeUsername == sender) //if the current user is the sender
        {
            if(activeUsername == text) //if the stream hold the current user's username
            {
                //create new item for the joining message and give it color, font and aligmnet and add it to the messages list
                QListWidgetItem * joined = new QListWidgetItem("You joined the chat");
                ui->msgList->addItem(joined);
                joined->setTextAlignment(Qt::AlignCenter);
                joined->setForeground(Qt::red);
                QFont font;
                font.setBold(2);
                font.setPointSize(16);
                joined->setFont(font);
            }

            if(activeUsername != text) //if the stream doesnt hold only username, its a message to current user (Myself)
            {

                QString msg = splitt[1]; //since its the current user's message, we remove the name from the message

                //create new item for the message and give it color, font and aligmnet and add it to the messages list
                QListWidgetItem * chat = new QListWidgetItem(msg);
                ui->msgList->addItem(chat);
                chat->setTextAlignment(Qt::AlignRight);
                chat->setForeground(Qt::red);
                QFont font;
                font.setPointSize(16);
                font.setBold(2);
                chat->setFont(font);

                //since the message is sent by the current user (Myself) to the current user (Myself)
                //The sound effect will be for sending messages
                QSoundEffect *sound = new QSoundEffect;//object of type QSoundEffect
                sound->setVolume(0.25f); //set sound volume
                sound->setSource(QUrl::fromLocalFile(":/Sounds/sender.wav")); //path to the file
                sound->play();
            }

        }

    });
}

void chatWindow::loggedIn() //when a user connects to the host
{
    if(socket->state() == QAbstractSocket::ConnectedState) //if the socket is in connected state
    {
        QTextStream T(socket); //We create new text steam with the socket
        T << activeUsername; //we stream current user's username, used for the joining messages
        socket->flush();
    }
}

chatWindow::~chatWindow()
{
    delete ui;
}


void chatWindow::on_sendBtn_clicked() //when a user clicks on the send button
{
    QTextStream T(socket);//We create new text steam with the socket
    T << activeUsername << ": " << ui->msgInput->text(); //we stream what the user inputed in the message input
    socket->flush();
    ui->msgInput->clear(); //clear the message input
}

void chatWindow::closeEvent(QCloseEvent *event) //this even is called when the user closes the app
{
    QDateTime date = QDateTime::currentDateTime(); //current date and time
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss"); //formating date and time
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();

    //since the user closed the app, set isChatting to false and isOnline to false in the db
    databaseClass db;
    QSqlQuery query;
    query.prepare("UPDATE users SET isChatting = ?, isOnline = ? WHERE username = ?");
    query.addBindValue("False");
    query.addBindValue("False");
    query.addBindValue(activeUsername);
    query.exec();

    //Since the user closed the app, update the table chatting and set isChatting to false, and chat ended date and time
    //this applies for the current user only because the choosen user to chat with may still be in chat

    query.prepare("UPDATE chatting SET isChatting = ?, chatEnded = ? WHERE (user1 = ? AND user2 = ?)");
    query.addBindValue("False");
    query.addBindValue(formattedTimeMsg);
    query.addBindValue(activeUsername);
    query.addBindValue(activeChatUsername);
    query.exec();
    socket->close();
}

void chatWindow::on_closeBtn_clicked() //when the user clicks on the close button
{
    //same like the close event only diffrence is user stays online and friends page will be shown
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
    databaseClass db;
    QSqlQuery query;
    query.prepare("UPDATE users SET isChatting = ? WHERE username = ?");
    query.addBindValue("False");
    query.addBindValue(activeUsername);
    query.exec();

    query.prepare("UPDATE chatting SET isChatting = ?, chatEnded = ? WHERE (user1 = ? AND user2 = ?)");
    query.addBindValue("False");
    query.addBindValue(formattedTimeMsg);
    query.addBindValue(activeUsername);
    query.addBindValue(activeChatUsername);
    query.exec();

    socket->close();
    chatWindow::hide();
    FriendsPage friendsTab;
    friendsTab.setModal(true);
    friendsTab.exec();
}

