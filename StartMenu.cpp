#include "StartMenu.h"
#include "HelperFunctions.h"
#include <QVBoxLayout>
StartMenu::StartMenu(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Social Network");
    resize(100,100);
    login=new QPushButton(this);
    login->setText("Login");
    connect(login, SIGNAL(clicked()), this, SLOT(OpenLoginMenu()));

    signup=new QPushButton(this);
    signup->setText("Sign-up");

    QVBoxLayout *loginBox = new QVBoxLayout(this);
    loginBox->addWidget(login);
    loginBox->addWidget(signup);
}

StartMenu::~StartMenu()
{
}

void StartMenu::OpenLoginMenu()
{
    //trimitem catre server comanda login
    loginMenu = new LoginMenu(this);
    char lgn[] ="login\0";
    SendRequest(sd, lgn);
   // loginMenu->open();
    loginMenu->exec();
    delete loginMenu;
}

