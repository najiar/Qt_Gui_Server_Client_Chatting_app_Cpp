#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
extern QString activeUsername;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_loginBtn_clicked(); //When login button is clicked

    void on_registerBtn_clicked(); //When register button is clicked

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
