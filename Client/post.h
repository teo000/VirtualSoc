#ifndef POST_H
#define POST_H

#include <QWidget>

namespace Ui {
class Post;
}

class Post : public QWidget
{
    Q_OBJECT

public:
    Post(QWidget *parent, std::string username, std::string time, std::string post, std::string privacy, bool isAdmin);
    ~Post();

private slots:
    void on_delete_post_button_clicked();

private:
    Ui::Post *ui;
};

#endif // POST_H
