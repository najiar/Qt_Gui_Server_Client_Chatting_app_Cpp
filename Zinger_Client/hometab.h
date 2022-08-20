#ifndef HOMETAB_H
#define HOMETAB_H

#include <QDialog>

namespace Ui {
class homeTab;
}

class homeTab : public QDialog
{
    Q_OBJECT

public:
    explicit homeTab(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event); //close event for the tab
    ~homeTab();

private slots:
    void on_logoutBtn_clicked();

    void on_friendsBtn_clicked();

private:
    Ui::homeTab *ui;
};

#endif // HOMETAB_H
