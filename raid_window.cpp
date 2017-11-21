#include "raid_window.h"
#include "ui_raid_window.h"

raid_window::raid_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::raid_window)
{
    ui->setupUi(this);
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
