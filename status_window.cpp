#include "status_window.h"
#include "ui_status_window.h"

status_window::status_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::status_window)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(check_cpu()));
}

status_window::~status_window()
{
    delete ui;
}

void status_window::status_prepare_window()
{

    timer->start(3000);
    //przygotowanie interfejsu
}

void status_window::check_cpu()
{
    QString check_cpu_command = "echo '[00052]'`awk -v a=\"$(awk '/cpu /{print $2+$4,$2+$4+$5}' /proc/stat; sleep .5)\" '/cpu /{split(a,b,\" \"); print 100*($2+$4-b[1])/($2+$4+$5-b[2])}'  /proc/stat`'[XXXXX]' \n";
   // QString check_cpu_command = "echo '[00052]'`grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage \"\"}'`'[XXXXX]' \n";

    bash_root->write(check_cpu_command.toStdString().c_str());
}

void status_window::update_cpu_bar(int percent)
{
    ui->cpu_usage_bar->setValue(percent);
}
//
//Command for memory:
QString check_memory_command = "free --mega | sed -n 2p | awk '{ print $3\"/\"$2 }'";

//Command for disk usage
QString check_disk_usage = "df -h | grep -v ^tmpfs | grep -v ^udev | grep -v ^Filesystem | awk '{ print $1\":\"$3\":\"$2\":\"$5 }'";//$6 instead of $1 for mount point
/*Sample output
/partition:used:total:percentInUse
eg.
/dev/sda1:8,6G:28G:33%
/dev/sda2:0:3,9G:0%

*/


//
/*
 Target functionality:
 monitor free ram
 monitor cpu usage
 disk usage
 raid health check




  */
