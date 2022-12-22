#ifndef SIGNUPMENU_H
#define SIGNUPMENU_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class SignupMenu : public QDialog
{    Q_OBJECT
 public:
     SignupMenu(QWidget *parent/* = nullptr*/);
     ~SignupMenu();
 private:
     QLabel* labelUser = new QLabel("username: ", this);
     QLabel* labelPass = new QLabel("password: ", this);
     QLineEdit* writeUser;
     QLineEdit* writePass;
     QPushButton* enterPass = new QPushButton("enter", this);
 private slots:
     void SendSignupData();
};

#endif // SIGNUPMENU_H
