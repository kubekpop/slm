#ifndef APACHE_H
#define APACHE_H

#include <QWidget>

namespace Ui {
class apache;
}

class apache : public QWidget
{
    Q_OBJECT

public:
    explicit apache(QWidget *parent = 0);
    ~apache();

private:
    Ui::apache *ui;
};

#endif // APACHE_H
