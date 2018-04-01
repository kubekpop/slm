#include "docker_window.h"
#include "ui_docker_window.h"

docker_window::docker_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::docker_window)
{
    ui->setupUi(this);
}

docker_window::~docker_window()
{
    delete ui;
}

void docker_window::load_data(int operation, QString containers)
{
    if(operation == 1)
    {
        // load data here
        QStringList containers_splitted = containers.split("=");
        for (int i = 0; i < containers_splitted.size(); ++i)
        {
            ui->container_combobox->addItem(containers_splitted.at(i));
        }
    }
    else
    {
        QStringList images_splitted = containers.split("=");
        for (int i = 0; i < images_splitted.size(); ++i)
        {
            ui->image_combobox->addItem(images_splitted.at(i));
        }
    }

}

void docker_window::setPorts(QString ports)
{
    ui->portMappings->setText(ports);
}

void docker_window::setStatus(QString status)
{
    ui->state->setText(status);
}
void docker_window::check_status(QString container)
{
    QString commandPorts = "echo [00116]`docker port "+container+"`[XXXXX]";
    QString commandStatus = "sleep .5; echo [00119]`docker inspect -f '{{ .State.Status }}' "+container+"` `docker inspect -f '{{ .State.FinishedAt }}' "+container+"`[XXXXX]";
    bash_root->write(commandPorts.toStdString().c_str());
    //bash_root->write("echo DDSAWE");
    bash_root->write(commandStatus.toStdString().c_str());
}
void docker_window::on_container_combobox_currentTextChanged(const QString &arg1)//FinishedAt
{
    docker_window::check_status(arg1);

}

//docker inspect -f '{{ .Mounts }}' containerid

void docker_window::on_docker_stop_clicked()
{
    QString start_command = "echo [00124]"+ui->container_combobox->currentText()+"`docker stop $(docker ps -a -q --filter \"name="+ui->container_combobox->currentText()+"\")`[XXXXX]";

    bash_root->write(start_command.toStdString().c_str());
    docker_window::check_status(ui->container_combobox->currentText());
}

void docker_window::on_docker_start_clicked()
{
    //Not working/not tested due to the following issue: https://github.com/docker/for-linux/issues/211
    //Sometimes works :/
    QString start_command = "echo [00123]`docker start $(docker ps -a -q --filter \"name="+ui->container_combobox->currentText()+"\")`[XXXXX]";

    bash_root->write(start_command.toStdString().c_str());
    docker_window::check_status(ui->container_combobox->currentText());
}

void docker_window::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        QString command2 = "echo [00131]`docker image ls | sed -n 2,99p | awk '{ print $1 }' | sed ':a;N;$!ba;s/\\n/=/g'`[XXXXX]";
        //update_log(command2);
        bash_root->write(command2.toStdString().c_str());
    }
}

void docker_window::createContainer()
{



}

void docker_window::on_createButton_clicked()
{
    QString command = "docker run";
    if(ui->new_name->text() != "")
    {
        command += " --name "+ui->new_name->text();
    }

    if(ui->new_daemon->isChecked())
    {
        command += " -d";
    }

    QStringList ports_splitted = ui->new_portMappings->text().split(",");
    for (int i = 0; i < ports_splitted.size(); ++i)
    {
        if(ports_splitted[i] != "")
        {
            command += " -p " + ports_splitted[i];
        }
    }

    QStringList directories_splitted = ui->new_dir->text().split(",");
    for (int i = 0; i < directories_splitted.size(); ++i)
    {
        if(directories_splitted[i] != "")
        {
            command += " -v " + directories_splitted[i];
        }
    }
    command += " " + ui->image_combobox->currentText();
    //ui->command_placeholder->setText(command);
}
