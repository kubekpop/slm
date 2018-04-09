#include "qemu_window.h"
#include "ui_qemu_window.h"
#include <QDebug>

qemu_window::qemu_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qemu_window)
{
    ui->setupUi(this);
}

qemu_window::~qemu_window()
{
    delete ui;
}

void qemu_window::bash_output_interpreter(QString output)
{
    if(output.startsWith("["))
    {
        QString tag = output.left(7);
        tag.replace("[","");
        tag.replace("]","");
        int tag_number = tag.toInt();
        output.remove(0, 7);
        switch(tag_number)
        {

        case 1://68
            this->on_load(output);
            break;
        case 2://69
            this->set_status(output);
            break;
        case 3://70
            this->set_xml(output);
            break;
        case 4://71
            //emit data_to_log("Restarting guest");
            this->check_status();
            break;
        case 5://72
            //emit data_to_log("Stopping guest");
            this->check_status();
            break;
        case 6://73
            //emit data_to_log("Starting guest");
            this->check_status();
            break;
        case 7://xml
            this->set_xml(output);
            break;

        default:
            break;
        }
    }
}

void qemu_window::on_load(QString machines)
{
    ui->qemu_combobox->clear();
    if(!machines.isEmpty())
    {
        //https://www.centos.org/docs/5/html/5.2/Virtualization/chap-Virtualization-Managing_guests_with_virsh.html
    //https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/virtualization_deployment_and_administration_guide/sect-managing_guest_virtual_machines_with_virsh-shutting_down_rebooting_and_force_shutdown_of_a_guest_virtual_machine
    QString listMachines = "virsh --connect qemu:///system list | awk '{ print $2 }' | sed -n 3,999p | sed ':a;N;$!ba;s/\n/=/g'";

    QString forceOff = "virsh --connect qemu:///system managedsave guest1 --running";

    //machines = "openmeetings-prod=wordpress-prod=ilias-prod=opensuse42_exam_nfs_new=demo-serwer=demo-client-15=demo-client-14=demo-client-13=demo-client-12=demo-client-11=demo-client-10=demo-client-9=demo-client-8=demo-client-7=demo-client6=demo-client-5=demo-client4=demo-client3=demo-client2=demo-client1-new";

    QStringList machines_list = machines.split("=");
    foreach (QString machine, machines_list)
    {
        if(!machine.isEmpty())
        {
            ui->qemu_combobox->addItem(machine);
        }
    }
    this->show();
    }
    else
    {
        emit data_to_log("No vms on host");
        //tu okienko rze nima maszinu
    }
}
/*
 * case 68:
            qemu_win->on_load(output);
            break;
        case 69:
            qemu_win->set_status(output);
            break;
        case 70:
            qemu_win->set_xml(output);
            break;
            */

void qemu_window::on_qemu_combobox_currentIndexChanged(const QString &arg1)
{
    check_status();
    QString command = "echo [qemu][00007]`virsh --connect qemu:///system dumpxml "+arg1+"`[XXXXX]";
    //emit data_to_log(command);
    bash_root->write(command.toStdString().c_str());
}
void qemu_window::check_status()
{
    QString statusCommand = "echo [qemu][00002]`export LC_MESSAGES=en_US.utf8; virsh --connect qemu:///system list --all | grep "+ui->qemu_combobox->currentText()+" | awk '{ print $3 }'`[XXXXX]";
    bash_root->write(statusCommand.toStdString().c_str());
    //set_xml("lol");
    qDebug() << statusCommand;
}

void qemu_window::set_status(QString status)
{
    //status = "running";
    ui->qemu_status->setText(status);
}

void qemu_window::set_xml(QString xml)
{
    //xml = "<lol>\n<test>testowanko</test>\n</lol>";
    ui->qemu_xml_content->setPlainText(xml);
}

void qemu_window::on_qemu_restart_clicked()
{
    QString restart = "echo [qemu][00004]`virsh --connect qemu:///system reset "+ui->qemu_combobox->currentText()+"`[XXXXX]";
    bash_root->write(restart.toStdString().c_str());
}

void qemu_window::on_qemu_stop_clicked()
{
    QString forceOff = "echo [qemu][00005]`virsh --connect qemu:///system managedsave "+ui->qemu_combobox->currentText()+" --running`[XXXXX]";
    bash_root->write(forceOff.toStdString().c_str());
}

void qemu_window::on_qemu_start_clicked()
{
    QString start = "echo [qemu][00006]`virsh --connect qemu:///system start "+ui->qemu_combobox->currentText()+"`[XXXXX]";
    bash_root->write(start.toStdString().c_str());
}
