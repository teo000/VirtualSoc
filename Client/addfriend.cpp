#include "addfriend.h"
#include "HelperFunctions.h"
#include "ui_addfriend.h"

AddFriend::AddFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);
}

AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::on_enter_clicked()
{
    QString name =  ui->name->text();
    if (name.length()>0){
        SendRequest(sd, "addfriend");
        SendRequest(sd,name.toLocal8Bit().data());
        SendInt(sd, ui->privacy->currentIndex()+1);

        char* rsp = ReceiveResponse(sd);
        QDialog* succes = new QDialog(this);
        QLabel* label = new QLabel(succes);
        label->setText(rsp);
        succes->resize(150,50);
        succes->open();
    }
}

