#include "docker_window.h"
#include "ui_docker_window.h"
//#define DEV
QString containerg;

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

void docker_window::bash_output_interpreter(QString output)
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
        case 1://load docker containers
            this->load_data(1, output);
            #if defined(DEV)
            emit data_to_log("Loading docker window with following containers available: "+output);
            #endif
            this->show();
            break;
        case 2://get docker ports
            this->setPorts(output);
            break;
        case 3://get docker status
            this->setStatus(output);
            #if defined(DEV)
            emit data_to_log("Stauts of docker: "+output);
            #endif
            break;
        case 4:
            #if defined(DEV)
            emit data_to_log("Loading new container window with following images available: "+output);
            #endif
            this->load_data(2, output);

            break;
        case 5:
            emit data_to_log("Creating container "+output);
            break;
        case 6://123//start container

        case 7://124//stop container
            emit data_to_log("Stopping container: "+output);
            break;

        default:
            break;
        }
    }
}

void docker_window::load_data(int operation, QString containers)
{
    if(operation == 1)//load existing containers
    {
        //load data here
        QStringList containers_splitted = containers.split("=");
        ui->container_combobox->clear();
        for (int i = 0; i < containers_splitted.size(); ++i)
        {
            ui->container_combobox->addItem(containers_splitted.at(i));

        }
    }
    else//load existing images
    {
        QStringList images_splitted = containers.split("=");
        ui->image_combobox->clear();
        for (int i = 0; i < images_splitted.size(); ++i)
        {
            ui->image_combobox->addItem(images_splitted.at(i));
        }
    }
}

void docker_window::setPorts(QString ports)
{
    if(ports == "")
    {
        QString commandPorts = "echo [dckr][00002]`docker inspect -f '{{ .HostConfig.PortBindings }}' "+containerg+"`[XXXXX]";
        bash_root->write(commandPorts.toStdString().c_str());
    }
    ui->portMappings->setText(ports);
}

void docker_window::setStatus(QString status)
{
    ui->state->setText(status);
}
void docker_window::check_status(QString container)
{
    //sleep .5 just because it works more often :)
    QString commandPorts = "sleep .5; echo [dckr][00002]`docker port "+container+"`[XXXXX]";
    QString commandStatus = "sleep .5; echo [dckr][00003]`docker inspect -f '{{ .State.Status }}' "+container+"` `docker inspect -f '{{ .State.FinishedAt }}' "+container+"`[XXXXX]";

    bash_root->write(commandPorts.toStdString().c_str());
    bash_root->write(commandStatus.toStdString().c_str());
    containerg = container;//setting for setPorts()
}
void docker_window::on_container_combobox_currentTextChanged(const QString &arg1)//FinishedAt
{
    docker_window::check_status(arg1);
}

//docker inspect -f '{{ .Mounts }}' containerid

void docker_window::on_docker_stop_clicked()
{
    QString start_command = "echo [dckr][00007]"+ui->container_combobox->currentText()+"`docker stop $(docker ps -a -q --filter \"name="+ui->container_combobox->currentText()+"\")`[XXXXX]";
    bash_root->write(start_command.toStdString().c_str());
    docker_window::check_status(ui->container_combobox->currentText());
}

void docker_window::on_docker_start_clicked()
{
    QString start_command = "echo [dckr][00006]`docker start $(docker ps -a -q --filter \"name="+ui->container_combobox->currentText()+"\")`[XXXXX]";
    bash_root->write(start_command.toStdString().c_str());
    docker_window::check_status(ui->container_combobox->currentText());
}

void docker_window::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        QString command2 = "echo [dckr][00004]`docker image ls | sed -n 2,99p | awk '{ print $1 }' | sed ':a;N;$!ba;s/\\n/=/g'`[XXXXX]";
        bash_root->write(command2.toStdString().c_str());
    }
    else if(index == 0)
    {
        QString command = "echo [dckr][00001]`docker ps -a --format '{{.Names}}' | sed ':a;N;$!ba;s/\\n/=/g'`[XXXXX]";
        bash_root->write(command.toStdString().c_str());
        docker_window::check_status(ui->container_combobox->currentText());
    }
}


void docker_window::on_createButton_clicked()
{
    QString command = "echo '[dckr][00005]`docker run";
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
    command += "` "+ui->new_name->text()+"'[XXXXX]'";
    bash_root->write(command.toStdString().c_str());
}
