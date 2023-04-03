#include "frienddescription.h"
#include "ui_frienddescription.h"

FriendDescription::FriendDescription(QWidget *parent, std::string username, std::string friendship) :
    QWidget(parent),
    ui(new Ui::FriendDescription)
{
    ui->setupUi(this);
    ui->username->setText(QString::fromStdString(username));

    switch(atoi(friendship.c_str())){
        case 1: ui->friendship->setText("Friends"); break;
        case 2: ui->friendship->setText("Close friends");
    }
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::darkGray);
    setAutoFillBackground(true);
    setPalette(pal);
}

FriendDescription::~FriendDescription()
{
    delete ui;
}
