#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "messenger.h"
#include "qscrollarea.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::string username;
    bool loggedin = false;
    bool isAdmin = false;
    Ui::MainWindow *ui;
    QWidget* feed;
    QWidget* friendList;
    Messenger* mess;

    void LoadFeed(QScrollArea* qscrl);
    void LoadMessenger(QScrollArea* qscrl);
    int PermissionToMessage(const char* user1, const char* user2);

private slots:
    void on_enterLogin_clicked();
    void on_enterSignup_clicked();
    void on_enterPost_clicked();
    void on_refreshFeed_clicked();
    void on_addFriend_clicked();
    void on_Exit_clicked();
    void on_logoutButton_clicked();
    void on_messengerButton_clicked();
    void on_feedButton_clicked();
    void on_messageEnter_clicked();
    void on_reloadMessenger_clicked();
    void on_noLogButton_clicked();
    void on_exit_noLog_clicked();
    void on_adminButton_clicked();
    void on_enterSignupAdmin_clicked();
    void on_exitAdminLogin_clicked();
    void on_backButton_clicked();
    void on_enterLoginAdmin_clicked();
    void on_exitAdmin_clicked();
    void on_logoutButtonAdmin_clicked();
    void on_refreshFeedAdmin_clicked();
    void on_enterPostAdmin_clicked();
    void on_messengerButtonAdmin_clicked();
    void on_reloadMessenger_admin_clicked();
    void on_messageEnter_admin_clicked();
    void on_exit_login_clicked();
    void on_removeUser_clicked();
    void on_friend_list_clicked();
    void on_login_noLog_clicked();
};
#endif // MAINWINDOW_H
