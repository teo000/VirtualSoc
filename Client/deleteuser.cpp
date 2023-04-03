#include "deleteuser.h"
#include "ui_deleteuser.h"
#include "HelperFunctions.h"

DeleteUser::DeleteUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteUser)
{
    ui->setupUi(this);
}

DeleteUser::~DeleteUser()
{
    delete ui;
}

void DeleteUser::on_enter_clicked()
{
    QString name =  ui->name->text();
    if (name.length()>0){
        SendRequest(sd, "delete_user");
        SendRequest(sd,name.toLocal8Bit().data());

        char* rsp = ReceiveResponse(sd);
        QDialog* succes = new QDialog(this);
        QLabel* label = new QLabel(succes);
        label->setText(rsp);
        succes->resize(150,50);
        succes->open();
    }
}

