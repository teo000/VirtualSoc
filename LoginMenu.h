#ifndef LOGINMENU_H
#define LOGINMENU_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


class LoginMenu : public QDialog
{
    Q_OBJECT
public:
    LoginMenu(QWidget *parent/* = nullptr*/);
    ~LoginMenu();
private:
    QLabel* labelUser = new QLabel("username: ", this);
    QLabel* labelPass = new QLabel("password: ", this);
    QLineEdit* writeUser;
    QLineEdit* writePass;
    QPushButton* enterPass = new QPushButton("enter", this);
private slots:
    void SendLoginData();
};

#endif // LOGINMENU_H
