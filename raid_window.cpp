#include "raid_window.h"
#include "ui_raid_window.h"

raid_window::raid_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::raid_window)
{
    ui->setupUi(this);

    QString array = "/dev/md0";
    int level = 1;
    QStringList members;
    members.append("/dev/sda1");
    members.append("/dev/sdb1");
    QString status = "cos";
    //ui->raid_status->




}

raid_window::~raid_window()
{
    delete ui;
}
/*
 Target functionality:
 create array
 array health check
 remove broken disk from array
 add new disk to array


*/
//https://raid.wiki.kernel.org/index.php/Mdstat
