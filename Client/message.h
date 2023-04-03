#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>

namespace Ui {
class Message;
}

class Message : public QWidget
{
    Q_OBJECT

public:
    Message(QWidget *parent, std::string sender, std::string message, std::string time);
    ~Message();

private:
    Ui::Message *ui;
};

#endif // MESSAGE_H
