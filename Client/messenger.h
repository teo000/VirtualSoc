#ifndef MESSENGER_H
#define MESSENGER_H

#include "qboxlayout.h"
#include "qscrollarea.h"
#include <QWidget>

class Messenger : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* vbox;
    QScrollArea* qscrl;
public:
    explicit Messenger(QWidget *parent, QScrollArea* qscrl);

private slots:
    void ReloadMessenger();

};

#endif // MESSENGER_H
