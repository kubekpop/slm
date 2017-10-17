#include "apache.h"
#include "ui_apache.h"

apache::apache(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::apache)
{
    ui->setupUi(this);
}

apache::~apache()
{
    delete ui;
}
