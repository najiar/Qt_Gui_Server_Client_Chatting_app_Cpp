#ifndef FRIENDSPAGE_H
#define FRIENDSPAGE_H

#include <QDialog>
extern QString activeChatUsername; //global var that holds the choosen user to chat with


namespace Ui {
class FriendsPage;
}

class FriendsPage : public QDialog
{
    Q_OBJECT

public:
    explicit FriendsPage(QWidget *parent = nullptr);
    ~FriendsPage();
    void loadFriendsRequests(); //loads friend requests for the user
    void loadAcceptedFriends(); //loads Accepted friends for the user
    void sendReq(); //sends friend request to a user

private slots:

    void on_acceptReqBtn_clicked();

    void on_rejectReqBtn_clicked();

    void on_removeFriendBtn_clicked();

    void on_chatBtn_clicked();

    void on_homeBtn_clicked();
    void closeEvent(QCloseEvent *event); //close event
    void mouseMoveEvent(QMouseEvent *e); //mouse move event / used to refresh friends list to show which user is online and offline


private:
    Ui::FriendsPage *ui;
};

#endif // FRIENDSPAGE_H
