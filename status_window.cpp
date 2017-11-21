#include "status_window.h"
#include "ui_status_window.h"

status_window::status_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::status_window)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(check_status()));
}

status_window::~status_window()
{
    delete ui;
}

void status_window::status_prepare_window()
{
    last_disk_status = "";
    timer->start(3000);
    //przygotowanie interfejsu
}


void status_window::closeEvent(QCloseEvent *event)
{
    timer->stop();
    event->accept();
}

void status_window::check_status()
{
    QString check_cpu_command = "echo '[00052]'`awk -v a=\"$(awk '/cpu /{print $2+$4,$2+$4+$5}' /proc/stat; sleep .5)\" '/cpu /{split(a,b,\" \"); print 100*($2+$4-b[1])/($2+$4+$5-b[2])}'  /proc/stat`'[XXXXX]' \n";
   // QString check_cpu_command = "echo '[00052]'`grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage \"\"}'`'[XXXXX]' \n";
    bash_root->write(check_cpu_command.toStdString().c_str());

    QString check_memory_command = "echo '[00059]'`free --mega | sed -n 2p | awk '{ print $3\"/\"$2 }'`'[XXXXX]' \n";
    bash_root->write(check_memory_command.toStdString().c_str());

    //QString  check_disk_usage = "echo '[00060]'`df -h | grep -v ^tmpfs | grep -v ^udev | grep -v ^Filesystem | awk '{ print $1\"(\"$6\"):\"$3\"/\"$2\" (\"$5\")\" }' | sed ':a;N;$!ba;s/\\n/[delimiter]/g'`'[XXXXX]' \n";
    QString  check_disk_usage = "echo '[00060]'`df -h | grep -v ^tmpfs | grep -v ^udev | sed -n 2,99p | awk '{ print $1\"(\"$6\"):\"$3\"/\"$2\" (\"$5\")\" }' | sed ':a;N;$!ba;s/\\n/[delimiter]/g'`'[XXXXX]' \n";

    bash_root->write(check_disk_usage.toStdString().c_str());
}

void status_window::update_cpu_bar(int percent)
{
    ui->cpu_usage_bar->setValue(percent);
}

void status_window::update_memory_info(QString memory_info)
{
    ui->memory_usage_info->setText(memory_info+"MB");
}

void status_window::update_disk_info(QString disk_info)
{
    if(disk_info != last_disk_status)
    {
        last_disk_status = disk_info;
        //zrobic zmienna temporary i porównywac czy sie zmieniło i tylko wtedy update interfejsu
        ui->disks_combobox->clear();
        if(!disk_info.isEmpty() && disk_info.contains("[delimiter]"))
        {
            QStringList disks = disk_info.split("[delimiter]");
            foreach (QString disk, disks)
            {
                ui->disks_combobox->addItem(disk);
            }
        }
        else if (!disk_info.isEmpty())
        {
            ui->disks_combobox->addItem(disk_info);
        }
    }
}
//
//Command for memory:


//Command for disk usage
//check_disk_usage = "df -h | grep -v ^tmpfs | grep -v ^udev | grep -v ^Filesystem | awk '{ print $1\":\"$3\":\"$2\":\"$5 }'";//$6 instead of $1 for mount point
QString  check_disk_usage = "df -h | grep -v ^tmpfs | grep -v ^udev | grep -v ^Filesystem | awk '{ print $1\"(\"$6\"):\"$3\"/\"$2\" (\"$5\")\" }' | sed ':a;N;$!ba;s/\n/[delimiter]/g'";
/*Sample output
/partition:used:total:percentInUse
eg.
/dev/sda1(/):8,6G/28G (33%)


*/


//
/*
 Target functionality:
 monitor free ram
 monitor cpu usage
 disk usage
 raid health check




  */
//RAID:
//https://raid.wiki.kernel.org/index.php/Mdstat
//Create interpreter
/*
To interpret:
recovery = 12.6%

md1 : active raid1 sdb2[1] sda2[0]
      136448 blocks [2/2] [UU]

md2 : active raid1 sdb3[1] sda3[0]
      129596288 blocks [2/2] [UU]


md1 : active raid1 sde1[6](F) sdg1[1] sdb1[4] sdd1[3] sdc1[2]
      488383936 blocks [6/4] [_UUUU_]

      (F) - faulty
      [6/4] - should be 6, but currently 4 are active

*/
