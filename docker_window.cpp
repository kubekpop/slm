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

void docker_window::load_data(QString containers)
{
    // load data here
    QStringList containers_splitted = containers.split("=");
    for (int i = 0; i < containers_splitted.size(); ++i)
    {
        ui->container_combobox->addItem(containers_splitted.at(i));
    }



    //ui->container->addItem(containers);

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
