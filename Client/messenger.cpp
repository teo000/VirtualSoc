#include "messenger.h"
#include "HelperFunctions.h"
#include "message.h"
#include "qboxlayout.h"
#include "qtimer.h"

Messenger::Messenger(QWidget *parent, QScrollArea* qscrl)
    : QWidget{parent}
{
    this->qscrl = qscrl;
    vbox = new QVBoxLayout;
    QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    printf("messenger/n");
    SendRequest(sd, "load_messenger");
    while(1){
        char* username = ReceiveResponse(sd);
        if((strcmp(username, "!@#"))==0) break;
        char* text = ReceiveResponse(sd);
        char* time = ReceiveResponse(sd);

        Message* newMessage = new Message(nullptr, username, text, time);

        vbox->addWidget(newMessage);
    }
    vbox->addItem(verticalSpacer);
    setLayout(vbox);
    qscrl->setWidget(this);
    char* rsp =ReceiveResponse(sd);
    free(rsp);


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ReloadMessenger()));
    timer->start(1000);

}

void Messenger::ReloadMessenger(){
    delete vbox;
    vbox = new QVBoxLayout;
    QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    SendRequest(sd, "load_messenger");
    while(1){
        char* username = ReceiveResponse(sd);
        if((strcmp(username, "!@#"))==0) break;
        char* text = ReceiveResponse(sd);
        char* time = ReceiveResponse(sd);

        Message* newMessage = new Message(nullptr, username, text, time);

        vbox->addWidget(newMessage);
    }
    vbox->addItem(verticalSpacer);
    setLayout(vbox);

    qscrl->setWidget(this);
    char* rsp =ReceiveResponse(sd);
    free(rsp);
}


