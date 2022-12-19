#ifndef STARTMENU_H
#define STARTMENU_H
#include "LoginMenu.h"
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
public slots:
    void OpenLoginMenu();

};
#endif // STARTMENU_H
