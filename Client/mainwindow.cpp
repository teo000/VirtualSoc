#include "mainwindow.h"
#include "deleteuser.h"
#include "frienddescription.h"
#include "messenger.h"
#include "ui_mainwindow.h"
#include "HelperFunctions.h"
#include "addfriend.h"
#include "post.h"
#include "unistd.h"
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_enterLogin_clicked()
{
    QString username = ui->writeLoginUser->text();
    QString password = ui->writeLoginPass->text();
    if(username.length()==0 || password.length()==0){
        QDialog* gresit = new QDialog(this);
        QLabel* labelGresit = new QLabel(gresit);
        gresit->resize(150,50);
        labelGresit->setText("Please complete both fields with your account information.");
        gresit->open();
    }
    else{
        char lgn[] ="login\0";
        SendRequest(sd, lgn);

        SendRequest(sd, username.toLocal8Bit().data());
        SendRequest(sd, password.toLocal8Bit().data());
        SendInt(sd, 0);
        char* rsp = ReceiveResponse(sd);


        printf("%s",rsp);

        if(strcmp(rsp, "Login successful!")==0){
            this->loggedin = true;
            this->username = username.toStdString();
            ui->stackedWidget->setCurrentIndex(1);
            ui->stackedWidget_MainPage->setCurrentIndex(0);
            setWindowTitle(QString::fromStdString("VirtualSoc ~ " + this->username ));

            LoadFeed(ui->scrollArea);

        }
        else{
            QDialog* response = new QDialog(this);
            QLabel* labelResponse = new QLabel(response);
            response->resize(200, 50);
            labelResponse->setText(rsp);
            response->open();
        }

    }
}

void MainWindow::on_enterSignup_clicked()
{
    QString name = ui->writeSignupName->text();
    QString username = ui->writeSignupUser->text();
    QString password = ui->writeSignupPass->text();
    int privacy = ui->signup_privacy->currentIndex();
    if(name.length() == 0 ||username.length()==0 || password.length()==0 ||
            !isAlphanumeric(name.toStdString()) || !isAlphanumeric(username.toStdString())){
        QDialog* gresit = new QDialog(this);
        QLabel* labelGresit = new QLabel(gresit);
        labelGresit->setText("The name and username should only contain alphanumerical characters");
        gresit->open();
    }
    else{
        char sgp[] ="signup\0";
        SendRequest(sd, sgp);
        SendRequest(sd, name.toLocal8Bit().data());
        SendRequest(sd, username.toLocal8Bit().data());
        SendRequest(sd, password.toLocal8Bit().data());
        SendInt(sd, 0);
        SendInt(sd, privacy);
        char* rsp = ReceiveResponse(sd);

       // printf(rsp)
        QDialog* response = new QDialog(this);
        response->resize(200, 50);
        QLabel* labelResponse = new QLabel(response);
        labelResponse->setText(rsp);
        response->open();
    }
}


void MainWindow::on_enterPost_clicked()
{
    QString post = ui->writePost->text();
    if(post.length()>0){
        SendRequest(sd, "post");
        SendRequest(sd, post.toLocal8Bit().data());
        SendInt(sd, ui->shareWith->currentIndex());
        char* rsp = ReceiveResponse(sd);
        //adauga cazul de eroare
        free(rsp);
        LoadFeed(ui->scrollArea);
    }
}

void MainWindow::LoadFeed(QScrollArea* qscrl){

    feed = new QWidget();
    QVBoxLayout* vbox = new QVBoxLayout;
    QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    feed->setLayout(vbox);

    SendRequest(sd, "load_feed");
    while(1){
        char* username = ReceiveResponse(sd);
        if((strcmp(username, "!@#"))==0) break;
        char* time = ReceiveResponse(sd);
        char* post = ReceiveResponse(sd);
        char* privacy = ReceiveResponse(sd);

        Post* newPost = new Post(nullptr, username, time, post, privacy, isAdmin);

        vbox->addWidget(newPost);
    }
    vbox->addItem(verticalSpacer);
    feed->setLayout(vbox);
    qscrl->setWidget(feed);
    char* rsp =ReceiveResponse(sd);
    free(rsp);
}


void MainWindow::on_refreshFeed_clicked()
{
    delete feed;
    LoadFeed(ui->scrollArea);
}


void MainWindow::on_addFriend_clicked()
{
    AddFriend* addfriend = new AddFriend(this);
    addfriend ->show();
}


void MainWindow::on_Exit_clicked()
{
    SendRequest(sd, "exit");
    ::close(sd);
    close();
}


void MainWindow::on_logoutButton_clicked()
{
    SendRequest(sd, "logout");
    if(mess != NULL) {delete mess;  mess=nullptr;}
    this->loggedin = false;
    this->username.erase();
    ui->stackedWidget->setCurrentIndex(0);
    setWindowTitle(QString::fromStdString("VirtualSoc"));
}


void MainWindow::on_messengerButton_clicked()
{
    if(mess!=NULL) {delete mess; mess=nullptr;}
    ui->stackedWidget_MainPage->setCurrentIndex(1);
    mess = new Messenger(this, ui->scrollArea_messenger);
}


void MainWindow::on_feedButton_clicked()
{
    if(mess != nullptr) { delete mess; mess=nullptr; }
    ui->stackedWidget_MainPage->setCurrentIndex(0);
    LoadFeed(ui->scrollArea);
}


void MainWindow::on_messageEnter_clicked()
{
    QString receiver = ui->messageUser->text();
    QString text = ui->messageText->text();
    if(text.length()>0 && receiver.length()>0){
        switch(PermissionToMessage(this->username.c_str(), receiver.toLocal8Bit().data())){
            case 0:{
                QDialog* response = new QDialog(this);
                response->resize(200, 50);
                QLabel* labelResponse = new QLabel(response);
                labelResponse->setText("This user is private and does not have you in their friend list");
                response->open();
            } break;
            case 1:{
                if(mess != NULL) {delete mess; mess=nullptr;}
                SendRequest(sd, "send_message");
                SendRequest(sd, receiver.toLocal8Bit().data());
                SendRequest(sd, text.toLocal8Bit().data());

                char* rsp = ReceiveResponse(sd);
                //adauga cazul de eroare
                mess = new Messenger(this, ui->scrollArea_messenger);
                QDialog* response = new QDialog(this);
                response->resize(200, 50);
                QLabel* labelResponse = new QLabel(response);
                labelResponse->setText(rsp);
                response->open();
                free(rsp);
            } break;
            case -1:{
                QDialog* response = new QDialog(this);
                response->resize(200, 50);
                QLabel* labelResponse = new QLabel(response);
                labelResponse->setText("There was an error. Please try again.");
                response->open();
            } break;
            case 2:{
                QDialog* response = new QDialog(this);
                response->resize(200, 50);
                QLabel* labelResponse = new QLabel(response);
                labelResponse->setText("User does not exist");
                response->open();
            }
        }
    }



}

void MainWindow::LoadMessenger(QScrollArea* qscrl){
    /*
    messenger = new QWidget();
    QVBoxLayout* vbox = new QVBoxLayout;
    QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    messenger->setLayout(vbox);

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
    messenger->setLayout(vbox);
    qscrl->setWidget(messenger);
    char* rsp =ReceiveResponse(sd);
    free(rsp);
    */
    printf("acum nu mai stergem\n");
   // if(mess != NULL) delete mess;
    printf("acum nu am sters\n");
     mess = new Messenger(this, qscrl);

}



void MainWindow::on_reloadMessenger_clicked()
{
    ui->stackedWidget_MainPage->setCurrentIndex(1);
    LoadMessenger(ui->scrollArea_messenger);
}


void MainWindow::on_noLogButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    LoadFeed(ui->scrollArea_noLog);

}


void MainWindow::on_exit_noLog_clicked()
{
    SendRequest(sd, "exit");
    ::close(sd);
    close();
}


void MainWindow::on_adminButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_enterSignupAdmin_clicked()
{
    QString name = ui->writeSignupNameAdmin->text();
    QString username = ui->writeSignupUserAdmin->text();
    QString password = ui->writeSignupPassAdmin->text();
    QString admin_pass = ui->writeAdminPass->text();

    if(name.length() == 0 ||username.length()==0 || password.length()==0 || admin_pass.length() ==0 ||
            !isAlphanumeric(name.toStdString()) || !isAlphanumeric(username.toStdString())){
        QDialog* gresit = new QDialog(this);
        QLabel* labelGresit = new QLabel(gresit);
        labelGresit->setText("The name and username should only contain alphanumerical characters");
        gresit->open();
    }
    else{
        SendRequest(sd, "signup");

        SendRequest(sd, name.toLocal8Bit().data());
        SendRequest(sd, username.toLocal8Bit().data());
        SendRequest(sd, password.toLocal8Bit().data());
        SendInt(sd, 1);
        SendRequest(sd, admin_pass.toLocal8Bit().data());
        char* rsp = ReceiveResponse(sd);

       // printf(rsp)
        QDialog* response = new QDialog(this);
        response->resize(200, 50);
        QLabel* labelResponse = new QLabel(response);
        labelResponse->setText(rsp);
        response->open();
    }
}


void MainWindow::on_exitAdminLogin_clicked()
{
    SendRequest(sd, "exit");
    ::close(sd);
    close();
}


void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_enterLoginAdmin_clicked()
{
    QString username = ui->writeLoginUserAdmin->text();
    QString password = ui->writeLoginPassAdmin->text();
    if(username.length()==0 || password.length()==0){
        QDialog* gresit = new QDialog(this);
        QLabel* labelGresit = new QLabel(gresit);
        gresit->resize(200,50);
        labelGresit->setText("Please complete both fields with your account information.");
        gresit->open();
    }
    else{
        char lgn[] ="login\0";
        SendRequest(sd, lgn);

        SendRequest(sd, username.toLocal8Bit().data());
        SendRequest(sd, password.toLocal8Bit().data());
        SendInt(sd, 1);
        char* rsp = ReceiveResponse(sd);


        printf("%s",rsp);

        if(strcmp(rsp, "Login successful!")==0){
            this->loggedin = true;
            this->username = username.toStdString();
            this->isAdmin = true;
            ui->stackedWidget->setCurrentIndex(4);
            ui->stackedWidget_admin->setCurrentIndex(0);
            setWindowTitle(QString::fromStdString("VirtualSoc ~ " + this->username ));
            LoadFeed(ui->scrollArea_admin);

        }
        else{
            QDialog* response = new QDialog(this);
            QLabel* labelResponse = new QLabel(response);
            response->resize(200, 50);
            labelResponse->setText(rsp);
            response->open();
        }

    }
}


void MainWindow::on_exitAdmin_clicked()
{
    SendRequest(sd, "exit");
    ::close(sd);
    close();
}


void MainWindow::on_logoutButtonAdmin_clicked()
{
    SendRequest(sd, "logout");
    if(mess != NULL) { delete mess;  mess=nullptr;}
    this->isAdmin = false;
    this->loggedin = false;
    this->username.erase();
    ui->stackedWidget->setCurrentIndex(0);
    setWindowTitle(QString::fromStdString("VirtualSoc"));
}


void MainWindow::on_refreshFeedAdmin_clicked()
{
    delete feed;
    LoadFeed(ui->scrollArea_admin);
}

void MainWindow::on_enterPostAdmin_clicked()
{
    printf("am ajuns in functie\n");
    QString post = ui->writePostAdmin->text();
    if(post.length()>0){
        SendRequest(sd, "post");
        SendRequest(sd, post.toLocal8Bit().data());
        //SendInt(sd, ui->shareWith->currentIndex());
        char* rsp = ReceiveResponse(sd);
        //adauga cazul de eroare
        free(rsp);
        LoadFeed(ui->scrollArea_admin);
    }
}


void MainWindow::on_messengerButtonAdmin_clicked()
{
    ui->stackedWidget_admin->setCurrentIndex(1);
    if(mess != NULL) {delete mess;  mess=nullptr;}
    mess = new Messenger(this, ui->scrollArea_messenger_admin);
}



void MainWindow::on_reloadMessenger_admin_clicked()
{
    LoadMessenger(ui->scrollArea_messenger_admin);
}


void MainWindow::on_messageEnter_admin_clicked()
{

    QString receiver = ui->messageUser_admin->text();
    QString text = ui->messageText_admin->text();
    if(text.length()>0 && text.length()>0){
        if(mess != NULL) {delete mess;  mess=nullptr;}
        SendRequest(sd, "send_message");
        SendRequest(sd, receiver.toLocal8Bit().data());
        SendRequest(sd, text.toLocal8Bit().data());

        char* rsp = ReceiveResponse(sd);
        mess = new Messenger(this, ui->scrollArea_messenger_admin);
        QDialog* response = new QDialog(this);
        response->resize(200, 50);
        QLabel* labelResponse = new QLabel(response);
        labelResponse->setText(rsp);
        response->open();
        free(rsp);
    }
}

void MainWindow::on_exit_login_clicked()
{
    SendRequest(sd, "exit");
    ::close(sd);
    close();
}


void MainWindow::on_removeUser_clicked()
{
    DeleteUser* deleteUser = new DeleteUser(this);
    deleteUser ->show();
}

int MainWindow::PermissionToMessage(const char* user1, const char* user2){
    SendRequest(sd, "permission_to_message");
    SendRequest(sd, user1);
    SendRequest(sd, user2);
    return GetInt(sd);
}

void MainWindow::on_friend_list_clicked()
{
   if(mess!=nullptr) {delete mess; mess=nullptr;}
   ui->stackedWidget_MainPage->setCurrentIndex(2);

   friendList = new QWidget();
   QVBoxLayout* vbox = new QVBoxLayout;
   QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

   SendRequest(sd, "friend_list");
   while(1){
       char* username = ReceiveResponse(sd);
       if((strcmp(username, "!@#"))==0) break;
       char* friendship = ReceiveResponse(sd);

       FriendDescription* newFriend = new FriendDescription(nullptr, username, friendship);

       vbox->addWidget(newFriend);
   }
   vbox->addItem(verticalSpacer);
   friendList->setLayout(vbox);
   ui->scrollArea_friendList->setWidget(friendList);
   char* rsp =ReceiveResponse(sd);
   free(rsp);
}


void MainWindow::on_login_noLog_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

