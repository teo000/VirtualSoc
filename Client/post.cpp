//#include "stdlib.h"
#include "HelperFunctions.h"
#include "post.h"
#include "qdialog.h"
#include "ui_post.h"

Post::Post(QWidget *parent, std::string username, std::string time, std::string post, std::string privacy, bool isAdmin) :
    QWidget(parent),
    ui(new Ui::Post)
{
    ui->setupUi(this);
    ui->username->setText(QString::fromStdString(username));
    ui->time->setText(QString::fromStdString(time));
    ui->postText->setText(QString::fromStdString(post));

    switch(atoi(privacy.c_str())){
        case 0: ui->sharedWith->setText("shared with: everyone"); break;
        case 1: ui->sharedWith->setText("shared with: friends"); break;
        case 2: ui->sharedWith->setText("shared with: close friends");
    }
    if(!isAdmin) ui->delete_post_button->hide();
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::darkGray);
    setAutoFillBackground(true);
    setPalette(pal);
}

Post::~Post()
{
    delete ui;
}

void Post::on_delete_post_button_clicked()
{
    SendRequest(sd, "delete_post");
    SendRequest(sd, ui->username->text().toLocal8Bit().data());
    SendRequest(sd, ui->time->text().toLocal8Bit().data());
    char* rsp = ReceiveResponse(sd);

   // printf(rsp)
    QDialog* response = new QDialog(this);
    response->resize(200, 50);
    QLabel* labelResponse = new QLabel(response);
    labelResponse->setText(rsp);
    response->open();
}

