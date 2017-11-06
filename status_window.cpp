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
    QString check_cpu_command = "echo '[00052]'`awk -v a=\"$(awk '/cpu /{print $2+$4,$2+$4+$5}' /proc/stat; sleep 1)\" '/cpu /{split(a,b,\" \"); print 100*($2+$4-b[1])/($2+$4+$5-b[2])}'  /proc/stat`'[XXXXX]' \n";
   // QString check_cpu_command = "echo '[00052]'`grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage \"\"}'`'[XXXXX]' \n";

    bash_root->write(check_cpu_command.toStdString().c_str());
}

void status_window::update_cpu_bar(int percent)
{
    ui->cpu_usage_bar->setValue(percent);
}
//
//Command dla memory:
QString check_memory_command = "free --mega | sed -n 2p | awk '{ print $3\"/\"$2 }'";
//
/*
 Target functionality:
 monitor free ram
 monitor cpu usage
 disk usage
 raid health check




  */
