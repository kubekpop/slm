#include "nfs_window.h"
#include "ui_nfs_window.h"

nfs_window::nfs_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nfs_window)
{
    ui->setupUi(this);
}

nfs_window::~nfs_window()
{
    delete ui;
}
QString nfs_window::from_begin_to_char(QString input, QString given_char, bool if_false_returns_rest)
{
    // todo: fix zeby nie sypala programu  jak niema given chara
    QString separated_begin;
    if(given_char.length() == 1 && input.contains(given_char))
    {
        for(int i = 0; i <= input.length(); i++)
        {
            if(input.at(i) == given_char)
            {
                input.remove(0,(i+1));
                break;
            }
            else
            {
                separated_begin.append(input.at(i));
            }
        }
    }
    if(if_false_returns_rest == true)
    {
      return separated_begin ;
    }
    else
    {
      return input;
    }
}

QStringList nfs_window::sort_options(QString share_options)
{
    //QString share_path = "1:/data *";//grep -n -v "^\#" /etc/exports | cut -d"(" -f1 | sed zamienianko nowa linia na delimiter
    //QString share_options = "uid=600,lol,no_root_squash,ro,ns=frd";//grep -v "^\#" /etc/exports | cut -d"(" -f2 | cut -d")" -f1 | sed ':a;N;$!ba;s/\n/=/g' zamienianko nowa linia na delimiter
    QStringList options = share_options.split(",");
    QStringList options_ordered;

    options_ordered.clear();
    for (int i = 0; i < options.size(); ++i)//find permissions parameter and, if exists, insert it at the beginning
    {

        if(options[i] == "ro" || options[i] == "rw")
        {
            options_ordered.append(options[i]);
        }
    }

    for (int i = 0; i < options.size(); ++i)//find squash parameter and, if exists, insert it at the second place (or first if no permissions parameter has been set before)
    {
        if(options[i] == "root_squash" || options[i] == "no_root_squash" || options[i] == "all_squash")
        {
            options_ordered.append(options[i]);
        }
    }

    for (int i = 0; i < options.size(); ++i)//find any other parameters and append them at the end
    {
        if(options[i] == "ro" || options[i] == "rw" || options[i] == "root_squash" || options[i] == "no_root_squash" || options[i] == "all_squash")
        {
            //emit data_to_log("nie dopisuje: "+options[i]);
            //options_ordered.insert(0,options[i]);
        }
        else
        {
            options_ordered.append(options[i]);
        }
    }


    for (int i = 0; i < options_ordered.size(); ++i)//list params
    {
        QString s = QString::number(i);
    }
    return options_ordered;
}

void nfs_window::nfs_prepare_window(QString paths, QString params)
{
    ui->delete_share->setChecked(false);
    paths_splitted.clear();
    params_splitted.clear();
    ui->share_combobox->clear();
    ui->share_combobox->addItem("new");
    if(paths.length() > 0 && params.length() > 0)
    {
        paths_splitted = paths.split("libQt5Xml5");
        params_splitted = params.split("libQt5Xml5");
        for (int i = 0; i < paths_splitted.size(); ++i)
        {
            ui->share_combobox->addItem(from_begin_to_char(paths_splitted.at(i)," ",true));
        }
    }
}

void nfs_window::on_share_combobox_currentTextChanged(const QString &arg1)
{
    ui->delete_share->setChecked(false);
    if(arg1 == "new")
    {
        ui->path->clear();
        ui->access->clear();
        ui->ro_radio->setChecked(true);
        ui->all_squash_radio->setChecked(true);
        ui->additional_options->clear();
    }
    else
    {
        if(paths_splitted.size() > 0 && params_splitted.size() > 0)
        {
            for (int i = 0; i < paths_splitted.size(); ++i)
            {
                if(arg1 == from_begin_to_char(paths_splitted.at(i)," ",true))
                {
                    ui->path->setText(from_begin_to_char(from_begin_to_char(paths_splitted.at(i)," ",true),":",false));
                    ui->access->setText(from_begin_to_char(paths_splitted.at(i)," ",false));
                    QStringList sorted_options = sort_options(params_splitted.at(i));
                    if(sorted_options.at(0) == "ro")
                    {
                        ui->ro_radio->setChecked(true);
                    }
                    else
                    {
                        ui->rw_radio->setChecked(true);
                    }

                    if(sorted_options.at(1) == "all_squash")
                    {
                        ui->all_squash_radio->setChecked(true);
                    }
                    else if(sorted_options.at(1) == "root_squash")
                    {
                        ui->root_squash_radio->setChecked(true);
                    }
                    else
                    {
                        ui->no_root_squash_radio->setChecked(true);
                    }
                    sorted_options.removeFirst();
                    sorted_options.removeFirst();
                    QString additional_options = sorted_options.join(",");
                    ui->additional_options->setText(additional_options);
                }
            }
        }
    }
}

void nfs_window::on_apply_clicked()
{
        QString access = ui->access->text();
        QString path = ui->path->text();
        if(access.length() > 0 && path.length() > 0 &&  path.startsWith("/"))
        {
            QString name = ui->share_combobox->currentText();
            QString perm;
            QString additional;
            QString squash;
            if(ui->all_squash_radio->isChecked())
            {
                squash = "all_squash";
            }
            else if(ui->root_squash_radio->isChecked())
            {
                squash = "root_squash";
            }
            else
            {
                squash = "no_root_squash";
            }

            if(ui->ro_radio->isChecked())
            {
                perm = "ro";
            }
            else
            {
                perm = "rw";
            }
            if(ui->additional_options->text() != "")
            {
                additional = ","+ui->additional_options->text();
            }
            QString share;
            if(ui->delete_share->isChecked() == true && ui->share_combobox->currentText() != "new")
            {
                share = "";
            }
            else
            {
                share = path+" "+access+"("+perm+","+squash+additional+")";
            }
            QString add_share_command;
            if(ui->share_combobox->currentText() == "new")
            {
                add_share_command = "echo '[00048]'`echo '"+share+"' >> /etc/exports`'[XXXXX]' \n";
            }
            else
            {
                add_share_command = "echo '[00051]'`sed -i '"+from_begin_to_char(ui->share_combobox->currentText(),":",true)+"s!.*!"+share+"!' /etc/exports`'[XXXXX]' \n";
            }
            bash_root->write(add_share_command.toStdString().c_str());
        }
        else
        {
            QMessageBox::information(this,tr("Error"),tr("Wrong values!"));

        }
}


void nfs_window::get_params(QString paths)
{
    QString command_params = "echo '[00050]"+paths+"[paths_and_params]'`grep -v \"^\\#\" /etc/exports | cut -d\"(\" -f2 | cut -d\")\" -f1 | sed ':a;N;$!ba;s%\\n%libQt5Xml5%g'`'[XXXXX]' \n";
    bash_root->write(command_params.toStdString().c_str());
}

//PATHS: 6:/lolec *libQt5Xml57:/bolec * PARAMS: ro,all_squash,test,uid=500libQt5Xml5ro,no_root_squash,test,uid=600
//QStringList pathz = kupaslonia!
//
/*
tablica1: /sciezka
tablica2: parametry_scieszki
tablica1[0]="6:/lolec 192.168.0.1/24,192.168.0.0/24"
QString::indexOf(" ");
substring od początku do indexu
tablica2[0]="ro,all_squash,test,uid=500"
*/
//sed -i 'nr_liniis!.*!share!' /etc/exports
