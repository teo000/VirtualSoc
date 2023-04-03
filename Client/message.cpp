#include "message.h"
#include "ui_message.h"

Message::Message(QWidget *parent, std::string sender, std::string message, std::string time) :
    QWidget(parent),
    ui(new Ui::Message)
{
    ui->setupUi(this);
    ui->user->setText(QString::fromStdString(sender));
    ui->time->setText(QString::fromStdString(time));
    ui->message->setText(QString::fromStdString(message));

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::darkGray);
    setAutoFillBackground(true);
    setPalette(pal);
}

Message::~Message()
{
    delete ui;
}
