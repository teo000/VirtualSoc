#ifndef FRIENDDESCRIPTION_H
#define FRIENDDESCRIPTION_H

#include <QWidget>

namespace Ui {
class FriendDescription;
}

class FriendDescription : public QWidget
{
    Q_OBJECT

public:
    FriendDescription(QWidget *parent, std::string username, std::string friendship);
    ~FriendDescription();

private:
    Ui::FriendDescription *ui;
};

#endif // FRIENDDESCRIPTION_H
