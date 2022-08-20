#include "friendspage.h"
#include "ui_friendspage.h"
#include "databaseclass.h"
#include "mainwindow.h"
#include "chatWindow.h"
#include "hometab.h"
#include <QMouseEvent>
QString activeChatUsername = "";


FriendsPage::FriendsPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendsPage)
{
    ui->setupUi(this);
    FriendsPage::setWindowTitle("Friends Tab"); //set tab name
    FriendsPage::setWindowIcon(QIcon(":/Images/windowIcon")); //set tab icon
    this->setFixedSize(QSize(698, 759));

    ui->friendsList->setSelectionBehavior( QAbstractItemView::SelectItems); //allow selection in friends list
    ui->friendsList->setSelectionMode( QAbstractItemView::SingleSelection ); //single selection only in the friends list
    ui->friendsList->setSpacing(5); //spacing between items

    ui->friendReqErrorTxt->setAlignment(Qt::AlignCenter);
    ui->failChatTxt->setAlignment(Qt::AlignCenter);

    loadFriendsRequests(); //call load Friends Requests function to refresh the friend requests list
    loadAcceptedFriends(); //call load Accepted friends function to refresh the friends list

    ui->addUserInput->setPlaceholderText("Add Friend by Username"); //set text hint

    connect(ui->sendReqBtn, &QPushButton::clicked, this, &FriendsPage::sendReq); //when send request button is clicked call sendReq function

}

FriendsPage::~FriendsPage()
{
    delete ui;
}

void FriendsPage::sendReq() //to send friend requests to a user
{
    ui->friendReqErrorTxt->setText(""); //set error msg to empty

    databaseClass db; //open db

    //check if such username exists using the user's input
    QString checkUser;
    QSqlQuery queryCheckUser;
    queryCheckUser.prepare("SELECT username FROM users WHERE username = ?");
    queryCheckUser.addBindValue(ui->addUserInput->text());
    queryCheckUser.exec();
    queryCheckUser.next();
    checkUser = queryCheckUser.value(0).toString();

    //check if you already sent request to this user
    QString pendingFriends;
    QSqlQuery queryPendingFriends;
    queryPendingFriends.prepare("SELECT * FROM friends WHERE usernameSender = ? AND usernameReceiver = ? AND isFriends = 'Pending'");
    queryPendingFriends.addBindValue(activeUsername);
    queryPendingFriends.addBindValue(ui->addUserInput->text());
    queryPendingFriends.exec();
    queryPendingFriends.next();
    pendingFriends = queryPendingFriends.value(0).toString();

    //check if you are already friends with the user
    QString alreadyFriendsSideOne;
    QSqlQuery queryAlreadyFriendsSideOne;
    queryAlreadyFriendsSideOne.prepare("SELECT * FROM friends WHERE usernameSender = ? AND usernameReceiver = ? AND isFriends = 'True'");
    queryAlreadyFriendsSideOne.addBindValue(activeUsername);
    queryAlreadyFriendsSideOne.addBindValue(ui->addUserInput->text());
    queryAlreadyFriendsSideOne.exec();
    queryAlreadyFriendsSideOne.next();
    alreadyFriendsSideOne = queryAlreadyFriendsSideOne.value(0).toString();

    //check if the user is already friends you
    QString alreadyFriendsSideTwo;
    QSqlQuery queryAlreadyFriendsSideTwo;
    queryAlreadyFriendsSideTwo.prepare("SELECT * FROM friends WHERE usernameSender = ? AND usernameReceiver = ? AND isFriends = 'True'");
    queryAlreadyFriendsSideTwo.addBindValue(ui->addUserInput->text());
    queryAlreadyFriendsSideTwo.addBindValue(activeUsername);
    queryAlreadyFriendsSideTwo.exec();
    queryAlreadyFriendsSideTwo.next();
    alreadyFriendsSideTwo = queryAlreadyFriendsSideTwo.value(0).toString();


    //check if the user tries to add somone who added him
    QList<QListWidgetItem *> sameFriendFromReqList = ui->friendsReqList->findItems(ui->addUserInput->text(), Qt::MatchExactly);


    if(checkUser.isEmpty()) //if no user with such username
    {
        ui->friendReqErrorTxt->setText("User not found!"); //print to user
        checkUser = "";
        ui->addUserInput->setText("");
        db.closeDB();

    }
    else if(pendingFriends.size() > 0) //if the user already sent a friend request
    {
        ui->friendReqErrorTxt->setText("You have already sent request to this user!");
        pendingFriends = "";
        ui->addUserInput->setText("");
        db.closeDB();


    }
    else if(alreadyFriendsSideOne.size() > 0 || alreadyFriendsSideTwo.size() > 0) //if the user is a friend already
    {
        ui->friendReqErrorTxt->setText("You are already Friends!");
        ui->addUserInput->setText("");
        db.closeDB();


    }
    else if(activeUsername == ui->addUserInput->text()) //if the user tries to add him/her self
    {
        ui->friendReqErrorTxt->setText("You cant add yourself! :D");
        ui->addUserInput->setText("");
        db.closeDB();

    }
    else if(sameFriendFromReqList.size() > 0) //if the user already has a friend request from this user
    {
        ui->friendReqErrorTxt->setText("You already have request from this user!");
        ui->addUserInput->setText("");
        db.closeDB();
    }
    else //if all the steps are passed
    {
        //Add to the friends table a pending request with the username sender and username receiver
        QSqlQuery querySent;
        querySent.prepare("insert into friends (usernameSender, usernameReceiver, isFriends) values (?,?,?)");
        querySent.addBindValue(activeUsername);
        querySent.addBindValue(ui->addUserInput->text());
        querySent.addBindValue("Pending"); //put the friendship state as pending until the user accepts the request

       if(!querySent.exec())
       {
            qDebug("Error Sending friend request");
       }

       ui->friendReqErrorTxt->setText("Sent!"); //show the user that the request is sent
       ui->addUserInput->setText(""); //clear the add user input
       db.closeDB(); //close db connection
    }


}
void FriendsPage::loadFriendsRequests() //load friend requests sent from other users and put them in the friend requests list
{
    QStringList friendsReqListTemp; //to hold the friends from the db
    databaseClass db;
    QSqlQuery query;

    //get all of the items from the db where username receiver is the current user and friendship state is pending
    query.prepare("SELECT usernameSender FROM friends WHERE usernameReceiver = ? AND isFriends = ?");
    query.addBindValue(activeUsername);
    query.addBindValue("Pending");
    query.exec();
    for(int i = 0; query.next(); i++) //while there is still items in the query
    {
        friendsReqListTemp << (query.value(0).toString()); //put them in the temp result list
    }

    for(int i=0 ; i < friendsReqListTemp.length() ; i++)
    {
        ui->friendsReqList->addItem(friendsReqListTemp.at(i)); //add all items found to the friend request list
    }

}

void FriendsPage::loadAcceptedFriends() //load already accpted friends and put them in the friends list
{
    ui->friendsList->clear(); //we clear all list items

    //Get users which added the current user from the friends table where username is the current one and friendship state is True
    //Get friends that other users added the current user
    QStringList friendsListTemp;
    databaseClass db;
    QSqlQuery query;
    query.prepare("SELECT usernameSender FROM friends WHERE isFriends = ? AND usernameReceiver = ?");
    query.addBindValue("True");
    query.addBindValue(activeUsername);
    query.exec();

    for(int i = 0; query.next(); i++)
    {
        friendsListTemp << (query.value(0).toString()); //add the users that added the current user to the temp list
    }

    //Get users that current user added from the friends table where username is the current one and friendship state is True
    //Get friends that the user added
    query.prepare("SELECT usernameReceiver FROM friends WHERE isFriends = ? AND usernameSender = ?");
    query.addBindValue("True");
    query.addBindValue(activeUsername);
    query.exec();

    for(int i = 0; query.next(); i++)
    {
        friendsListTemp << (query.value(0).toString());//add the users that the current user added to the temp list
    }

    friendsListTemp.removeDuplicates(); //if somehow an error occured

    QListWidgetItem *newItem = nullptr; //create an item

    for(int i=0 ; i < friendsListTemp.length() ; i++) //for each found user
    {
        newItem = new QListWidgetItem; //initialize the item

        //we get online state for each user found from the table users in the db
        databaseClass db; //open db
        QString queryResult;
        QSqlQuery query;
        query.prepare("SELECT isOnline FROM users WHERE username = ?");
        query.addBindValue(friendsListTemp.at(i));
        query.exec();
        query.next();
        queryResult = query.value(0).toString();
        db.closeDB();
        newItem->setText(friendsListTemp.at(i));
        if(queryResult == "True")//if the  User is online
        {
            newItem->setBackground(Qt::green); //we set its background color to be green

        }
        else //if the User is offline
        {
            newItem->setBackground(Qt::red); //we set its background color to be red
        }


        ui->friendsList->addItem(newItem); //we add the item to the Friends list
        newItem = nullptr;
    }
}
void FriendsPage::mouseMoveEvent(QMouseEvent *e) //event called when mouse is moved
{
    if(e->type() == QEvent::MouseMove) //if the mouse is moved
    {
        loadAcceptedFriends(); //load Accepted friends
        ui->friendsList->repaint(); //repaint the friends list
        qApp->processEvents();
    }
}

void FriendsPage::on_acceptReqBtn_clicked() //when accept friend request button is clicked
{
    if(ui->friendsReqList->currentItem() == nullptr) //if the user didnt select an item from friend requests list
    {
        return; //do nothing
    }
    ui->friendsList->clear(); //clear the list

    QString selectedItemText = ui->friendsReqList->currentItem()->text(); //get the selected item from the friend requests list
    //set the user to be friends with the selected username from the list
    //update query to change friendship status from Pending to True
    databaseClass db;
    QSqlQuery query;
    //Since the user is the reciever now, the sender is the selected item from the list
    query.prepare( "UPDATE friends SET isFriends = ? WHERE usernameSender = ? AND usernameReceiver = ?" );
    query.addBindValue("True");
    query.addBindValue(selectedItemText);
    query.addBindValue(activeUsername);
    if(!query.exec())
    {
        qDebug("Error Accepting friend request");
        return;
    }


    QListWidgetItem * item = ui->friendsReqList->currentItem(); //get the current item

    ui->friendsReqList->removeItemWidget(item); //since request is accepted, remove the item from the requests list
    delete item;

    loadAcceptedFriends(); //load accpted friend to show the newly accepted request
}


void FriendsPage::on_rejectReqBtn_clicked() //When reject button is clicked
{
    if(ui->friendsReqList->currentItem() == nullptr) // if the user didnt select an item from the friend request list
    {
        return; //do nothting
    }
    QString selectedItemText = ui->friendsReqList->currentItem()->text(); //get the item that the user selected from the list

    //update the friends table where the sender is the selected user from the list and reviever is the current loged user
    //set the friendship state to rejected
    databaseClass db;
    QString queryResult;
    QSqlQuery query;
    query.prepare( "UPDATE friends SET isFriends = ? WHERE usernameSender = ? AND usernameReceiver = ?" );
    query.addBindValue("Rejected");
    query.addBindValue(selectedItemText);
    query.addBindValue(activeUsername);


    if(!query.exec())
    {
        qDebug("Error Rejecting friend request");
    }

    QListWidgetItem* item = ui->friendsReqList->currentItem(); //get the selected item

    ui->friendsReqList->removeItemWidget(item); //remove it from the friend requests
    delete item;

}


void FriendsPage::on_removeFriendBtn_clicked() //when remove friend button clicked
{
    if(ui->friendsList->currentItem() == nullptr)//if the user didnt select an item from the friends list
    {
        return; //do nothing
    }
    QString selectedItemText = ui->friendsList->currentItem()->text(); //get the selected item

    //if the current logged in user is the Receiver and the selected text is the sender
    //set friend ship state to Removed
    databaseClass db;
    QString queryResult;
    QSqlQuery query;
    query.prepare( "UPDATE friends SET isFriends = ? WHERE usernameSender = ? AND usernameReceiver = ?");
    query.addBindValue("Removed");
    query.addBindValue(selectedItemText);
    query.addBindValue(activeUsername);

    if(!query.exec())
    {
        qDebug("Error Removing friend");
    }
    //if the selected text is the Receiver and the current logged in user is the sender
    //set friend ship state to Removed
    query.prepare( "UPDATE friends SET isFriends = ? WHERE usernameReceiver = ? AND usernameSender = ?");
    query.addBindValue("Removed");
    query.addBindValue(selectedItemText);
    query.addBindValue(activeUsername);

    if(!query.exec())
    {
        qDebug("Error Removing friend");
    }

    QListWidgetItem* item = ui->friendsList->currentItem(); //get the selected item
    ui->friendsList->removeItemWidget(item); //remove it from the friend list
    delete item;
    loadAcceptedFriends();
}
void FriendsPage::closeEvent(QCloseEvent *event) //The event is called when the app is closed
{
    //set the current user's online state to False in the users table since the app is closed
    databaseClass db;
    QSqlQuery query;
    query.prepare("UPDATE users SET isOnline = ? WHERE username = ?");
    query.addBindValue("False");
    query.addBindValue(activeUsername);
    query.exec();
}


void FriendsPage::on_chatBtn_clicked() //when chat button is clicked
{
    if(ui->friendsList->currentItem() == nullptr) // if the user didnt select an user from the friend list
    {
        return; //do nothing
    }
    activeChatUsername = ui->friendsList->currentItem()->text(); //we set the global to the choosen user from the friends list
    //we check if the user that we choose to chat with is online from the db using the global var
    databaseClass db;
    QString queryResult;
    QString queryResultChatting;
    QSqlQuery query;
    query.prepare("SELECT isOnline FROM users WHERE username = ?");
    query.addBindValue(activeChatUsername);
    query.exec();
    query.next();
    queryResult = query.value(0).toString();

    if(queryResult == "True") //if the user is online
    {
        QDateTime date = QDateTime::currentDateTime(); //get the current date and time
        QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss"); //date and time formating
        QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();

        //insert into chatting table the user1 the currnet user, user2 the user that is choosen to chat with
        //we set isChatting to true and chat statrted date and time to be the current date and time.
        //chat ended is null since we dont know yet when the users will exit the chat
        query.prepare("insert into chatting (user1, user2, isChatting, chatStarted, chatEnded) values (?,?,?,?,?)");
        query.addBindValue(activeUsername);
        query.addBindValue(activeChatUsername);
        query.addBindValue("True");
        query.addBindValue(formattedTimeMsg);
        query.addBindValue("null");
        if(!query.exec())
        {
            qDebug("Error adding chat settings to database");
        }

        //set the isChatting to true for the current user since the user started chatting
        query.prepare("UPDATE users SET isChatting = ? WHERE username = ?");
        query.addBindValue("True");
        query.addBindValue(activeUsername);
        query.exec();

        FriendsPage::hide(); //hide friends page
        chatWindow newChat; //show chat page
        newChat.setModal(true);
        newChat.exec();
    }
    else //if the choosen user is offline
    {
        ui->failChatTxt->setText(activeChatUsername + " Is offline!"); //print to user
    }
}
void FriendsPage::on_homeBtn_clicked()
{
    FriendsPage::hide();
    homeTab homePage;
    homePage.setModal(true);
    homePage.exec();
}

