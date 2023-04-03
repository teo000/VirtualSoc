#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>

namespace Ui {
class AddFriend;
}

class AddFriend : public QDialog
{
    Q_OBJECT

public:
    AddFriend(QWidget *parent = nullptr);
    ~AddFriend();

private slots:
    void on_enter_clicked();

private:
    Ui::AddFriend *ui;
};

#endif // ADDFRIEND_H
