#ifndef STARTMENU_H
#define STARTMENU_H
#include "LoginMenu.h"
#include "signupmenu.h"
#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    StartMenu(QWidget *parent = nullptr);
    ~StartMenu();
private:
    QPushButton *login;
    QPushButton *signup;
    LoginMenu* loginMenu;
    SignupMenu* signupMenu;
public slots:
    void OpenLoginMenu();
    void OpenSignupMenu();
};
#endif // STARTMENU_H
