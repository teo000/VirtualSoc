#include "LoginMenu.h"
#include "HelperFunctions.h"
#include <QLineEdit>
#include <QGridLayout>



LoginMenu::LoginMenu(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("Login");
    writeUser = new QLineEdit(this);
    writePass = new QLineEdit(this);
    writePass->setEchoMode(QLineEdit::Password);
    connect(enterPass, SIGNAL(clicked()), this, SLOT(SendLoginData()));
    QGridLayout* grid = new QGridLayout(this);
    grid->addWidget(labelUser, 0,0);
    grid->addWidget(writeUser, 0,1);
    grid->addWidget(labelPass, 1,0);
    grid->addWidget(writePass, 1,1);
    grid->addWidget(enterPass, 2,0);
}

LoginMenu::~LoginMenu()
{

}

void LoginMenu::SendLoginData()
{
    QString username = writeUser->text();
    QString password = writePass->text();
    if(username.length()==0 || password.length()==0){
        enterPass->setText("scrie ba ceva");
        QDialog* gresit = new QDialog(this);
        QLabel* labelGresit = new QLabel(gresit);
        labelGresit->setText("gresit");
        gresit->open();
    }
    else enterPass->setText("am trimis");
    SendRequest(sd, username.toLocal8Bit().data());
    SendRequest(sd, password.toLocal8Bit().data());
    char rsp[100];
    ReceiveResponse(sd, rsp);
   // printf(rsp)
    QDialog* response = new QDialog(this);
    QLabel* labelResponse = new QLabel(response);
    labelResponse->setText(rsp);
    response->open();
}
