#include "signupmenu.h"
#include "HelperFunctions.h"
#include <QGridLayout>

SignupMenu::SignupMenu(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle("Sign-up");
    writeUser = new QLineEdit(this);
    writePass = new QLineEdit(this);
    writePass->setEchoMode(QLineEdit::Password);
    connect(enterPass, SIGNAL(clicked()), this, SLOT(SendSignupData()));
    QGridLayout* grid = new QGridLayout(this);
    grid->addWidget(labelUser, 0,0);
    grid->addWidget(writeUser, 0,1);
    grid->addWidget(labelPass, 1,0);
    grid->addWidget(writePass, 1,1);
    grid->addWidget(enterPass, 2,0);
}

SignupMenu::~SignupMenu()
{

}

void SignupMenu::SendSignupData()
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
    else{ enterPass->setText("am trimis");

        char sgp[] ="signup\0";
        SendRequest(sd, sgp);

        SendRequest(sd, username.toLocal8Bit().data());
        SendRequest(sd, password.toLocal8Bit().data());
        char rsp[100];
        ReceiveResponse(sd, rsp);

       // printf(rsp)
        QDialog* response = new QDialog(this);
        response->resize(150, 50);
        QLabel* labelResponse = new QLabel(response);
        labelResponse->setText(rsp);
        response->open();
    }
}
