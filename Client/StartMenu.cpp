#include "StartMenu.h"
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
    connect(signup, SIGNAL(clicked()), this, SLOT(OpenSignupMenu()));

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

   // loginMenu->open();
    loginMenu->exec();
    delete loginMenu;
}

void StartMenu::OpenSignupMenu()
{

    signupMenu = new SignupMenu(this);

   // loginMenu->open();
    signupMenu->exec();
    delete signupMenu;
}


