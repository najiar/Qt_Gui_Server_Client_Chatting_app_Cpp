#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class chatWindow; }
QT_END_NAMESPACE

class QTcpSocket;
class chatWindow : public QDialog
{
    Q_OBJECT

public:
    chatWindow(QWidget *parent = nullptr);
    ~chatWindow();
    void closeEvent(QCloseEvent *event); //event that is called when user exits the app

public slots:
    void loggedIn(); //when the user logges in

private slots:
    void on_sendBtn_clicked(); //when the user send message

    void on_closeBtn_clicked(); //when the user presses on the close button

private:
    Ui::chatWindow *ui;
    QTcpSocket *socket; //transmission control protocol socket
};
#endif
