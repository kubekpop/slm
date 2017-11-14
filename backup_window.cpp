#include "backup_window.h"
#include "ui_backup_window.h"

backup_window::backup_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::backup_window)
{
    ui->setupUi(this);
}

backup_window::~backup_window()
{
    delete ui;
}
/*
Co i jak czytać z configa:
export BM_REPOSITORY_ROOT="/var/archives"
export BM_ARCHIVE_FREQUENCY="daily"
export BM_ARCHIVE_PREFIX="$HOSTNAME"
export BM_TARBALL_NAMEFORMAT="long"
export BM_TARBALL_FILETYPE="tar.gz"

w taki sposób:
grep '^[[:blank:]]*export[[:blank:]]*PARAMETR=' /etc/backup-manager.conf | cut -d= -f2
tak jak to jest w case 5coś

i jeszcze
grep '^[[:blank:]]*BM_TARBALL_TARGETS' /etc/backup-manager.conf | cut -d= -f2 i tutaj output może być różny:
('/dir1')//tu może być " zamiast '
('/important')
('/data')
naszą metodą i:
"/etc"
"/boot"
metodą z dokumentacji na normalnej numeracji tablicy

*/

QString backup_window::remove_char(QString input, QString char_to_remove)
{
    // removes given char from begginig and end many times
    while(input.at(0) == char_to_remove.at(0))
    {
        input.remove(0,1);
    }
    while(input.at(input.size()-1) == char_to_remove.at(0))
    {
        input.remove(input.size()-1,1);
    }
    return input;
}

QString backup_window::remove_one_char(QString input, QString char_to_remove)
{
    // removes given char from begginig and end only once
    if(input.at(0) == char_to_remove.at(0))
    {
        input.remove(0,1);
    }
    if(input.at(input.size()-1) == char_to_remove.at(0))
    {
        input.remove(input.size()-1,1);
    }
    return input;
}

void backup_window::backup_prepare_window(QString config_values)
{
    // replace unnecessary chars
    config_values.replace("\"","");
    config_values.replace("'","");
    ui->backup_directories->clear();
    QStringList config_values_list = config_values.split("[backup-manager-config-split]");
    if(config_values_list.size() == 6)
    {
        // BM_TARBALL_TARGETS or BM_TARBALL_DIRECTORIES
        QString backup_dirs = config_values_list.at(0);
        backup_dirs = remove_char(backup_dirs," ");
        if(!backup_dirs.isEmpty())
        {
            if(backup_dirs.contains("\n"))
            {
                QStringList backup_dirs_list = backup_dirs.split("\n");
                foreach (QString dir, backup_dirs_list)
                {
                    dir = remove_one_char(dir,"(");
                    dir = remove_one_char(dir,")");
                    if(!dir.isEmpty())
                    {
                        ui->backup_directories->append(dir);
                    }
                }
            }
            else
            {
                QStringList backup_dirs_list = backup_dirs.split(" ");
                foreach (QString dir, backup_dirs_list)
                {
                    dir = remove_one_char(dir,"(");
                    dir = remove_one_char(dir,")");
                    if(!dir.isEmpty())
                    {
                        ui->backup_directories->append(dir);
                    }
                }
            }
        }
        else
        {
             emit data_to_log("BM_TARBALL_TARGETS or BM_TARBALL_DIRECTORIES not found in /etc/backup-manager.conf");
        }


        // BM_REPOSITORY_ROOT
        QString BM_REPOSITORY_ROOT = config_values_list.at(1);
        BM_REPOSITORY_ROOT = remove_char(BM_REPOSITORY_ROOT, " ");
        if(!BM_REPOSITORY_ROOT.isEmpty())
        {
             ui->destination_directory->setText(BM_REPOSITORY_ROOT);
        }
        else
        {
             emit data_to_log("BM_REPOSITORY_ROOT not found in /etc/backup-manager.conf");
        }

        // BM_ARCHIVE_FREQUENCY
        QString BM_ARCHIVE_FREQUENCY = config_values_list.at(2);
        // w nastempnej linijce wali buga bo BM_ARCHIVE_FREQUENCY to spacja bo niema jej w configu
        if(BM_ARCHIVE_FREQUENCY != " ")
        {
            BM_ARCHIVE_FREQUENCY = remove_char(BM_ARCHIVE_FREQUENCY, " ");
        }
        else
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::information(this,tr(""),tr("BM_ARCHIVE_FREQUENCY not found in /etc/backup-manager.conf. Do you want to add it?"),QMessageBox::No | QMessageBox::Yes);
            if(reply == QMessageBox::Yes)
            {
                QString add_BM_ARCHIVE_FREQUENCY = "echo '[00055]'`echo 'export BM_ARCHIVE_FREQUENCY=\"daily\"' >> /etc/backup-manager.conf`'[XXXXX]' \n";
                bash_root->write(add_BM_ARCHIVE_FREQUENCY.toStdString().c_str());
            }
        }
        if(BM_ARCHIVE_FREQUENCY == "hourly")
        {
            ui->frequency_hourly->setChecked(true);
        }
        else if(BM_ARCHIVE_FREQUENCY == "daily")
        {
            ui->frequency_daily->setChecked(true);
        }
        else
        {
            emit data_to_log("BM_ARCHIVE_FREQUENCY not found in /etc/backup-manager.conf");
        }

        // BM_ARCHIVE_PREFIX
        QString BM_ARCHIVE_PREFIX = config_values_list.at(3);
        BM_ARCHIVE_PREFIX = remove_char(BM_ARCHIVE_PREFIX, " ");
        if(!BM_ARCHIVE_PREFIX.isEmpty())
        {
            ui->file_name_prefix->setText(BM_ARCHIVE_PREFIX);
        }
        else
        {
             emit data_to_log("BM_ARCHIVE_PREFIX not found in /etc/backup-manager.conf");
        }

        // BM_TARBALL_NAMEFORMAT
        QString BM_TARBALL_NAMEFORMAT = config_values_list.at(4);
        BM_TARBALL_NAMEFORMAT = remove_char(BM_TARBALL_NAMEFORMAT, " ");
        if(!BM_TARBALL_NAMEFORMAT.isEmpty())
        {
             // ico ja mam z tym wgle zrobic?
        }
        else
        {
             emit data_to_log("BM_TARBALL_NAMEFORMAT not found in /etc/backup-manager.conf");
        }

        // BM_TARBALL_FILETYPE
        QString BM_TARBALL_FILETYPE = config_values_list.at(5);
        BM_TARBALL_FILETYPE = remove_char(BM_TARBALL_FILETYPE, " ");
        if(BM_TARBALL_FILETYPE == "tar")
        {
            ui->format_tar->setChecked(true);
            // format tar
        }
        else if(BM_TARBALL_FILETYPE == "tar.gz")
        {
            ui->format_tar_gz->setChecked(true);
            // format tar.gz
        }
        else if(BM_TARBALL_FILETYPE == "tar.bz2")
        {
            ui->format_tar_bz2->setChecked(true);
            // format tar.bz2
        }
        else if(BM_TARBALL_FILETYPE == "tar.xz")
        {
            ui->format_tar_xz->setChecked(true);
            // format tar.xz
        }
        else if(BM_TARBALL_FILETYPE == "tar.lzma")
        {
            ui->format_tar_lzma->setChecked(true);
            // format tar.lzma
        }
        else if(BM_TARBALL_FILETYPE == "dar")
        {
            ui->format_dar->setChecked(true);
            // format dar
        }
        else if(BM_TARBALL_FILETYPE == "zip")
        {
            ui->format_zip->setChecked(true);
            // format zip
        }
        else
        {
            emit data_to_log("BM_TARBALL_FILETYPE not found in /etc/backup-manager.conf");
        }

    }
    else
    {
        emit data_to_log("Error while parsing /etc/backup-manager.conf");
    }
    //tutaj jest to export BM_TARBALL_DIRECTORIES="/etc /boot" i sprawdziuc czy jest i jest niezahaszowane
    //jestli tak juest to wyjebu komunikat i przerabianko na tabkices
    QString command = "grep '^[[:space:]]*BM_TARBALL_DIRECTORIES' && echo true";//jezeli niezahaszowane to true

}

/*
GUI for backup-manager https://github.com/sukria/Backup-Manager
-load backup directories from /etc/backup-manager.conf
-interpret backup directories, change all to array if space exists in at least one directory
-load destination dir from /etc/backup-manager.conf





# Where to store the archives
export BM_REPOSITORY_ROOT="/var/archives"

# At which frequency will you build your archives?
# You can choose either "daily" or "hourly".
# This should match your CRON configuration.
export BM_ARCHIVE_FREQUENCY="daily"

# Prefix of every archive on that box (default is HOSTNAME)
export BM_ARCHIVE_PREFIX="$HOSTNAME"

# Archive filename format
# 	long  : host-full-path-to-folder.tar.gz
# 	short : parentfolder.tar.gz
export BM_TARBALL_NAMEFORMAT="long"


# Type of archives
# Available types are:
#     tar, tar.gz, tar.bz2, tar.xz, tar.lzma, dar, zip.
# Make sure to satisfy the appropriate dependencies
# (bzip2, dar, xz, lzma, ...).
export BM_TARBALL_FILETYPE="tar.gz"

# Paths without spaces in their name:
# export BM_TARBALL_DIRECTORIES="/etc /boot"

# If one or more of the targets contain a space, use the array:
declare -a BM_TARBALL_TARGETS

BM_TARBALL_TARGETS[0]="/etc"
BM_TARBALL_TARGETS[1]="/boot"

export BM_TARBALL_TARGETS

# Files to exclude when generating tarballs, you can put absolute
# or relative paths, Bash wildcards are possible.
export BM_TARBALL_BLACKLIST="/dev /sys /proc /tmp"

*/

void backup_window::save_config()
{
    //BEGIN flushing BM_TARBALL_DIRECTORIES
    QString command = "echo '[00055]'`sed -i '/^[[:space:]]*export[[:space:]]BM_TARBALL_DIRECTORIES.*/c\\\\#export BM_TARBALL_DIRECTORIES=\"\"' /etc/backup-manager.conf`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
    //END flushing BM_TARBALL_DIRECTORIES
    //BEGIN flushing old BM_TARBALL_TARGETS
    command = "echo '[00055]'`sed -i '/^[[:space:]]*export[[:space:]]BM_TARBALL_TARGETS.*/c\\\\' /etc/backup-manager.conf`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
    command = "echo '[00055]'`sed -i '/^[[:space:]]*BM_TARBALL_TARGETS/c\\\\' /etc/backup-manager.conf`'[XXXXX]' \n";//to check
    bash_root->write(command.toStdString().c_str());
    command = "echo '[00055]'`sed -i '/^[[:space:]]*declare -a BM_TARBALL_TARGETS/c\\\\' /etc/backup-manager.conf`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
    //END building new BM_TARBALL_TARGETS

    if(ui->backup_directories->toPlainText() != "")
    {
        //BUILDING NEW BM_TARBALL_TARGETS
        QString command2 = "echo '[00055]'`echo \"declare -a BM_TARBALL_TARGETS\" >> /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command2.toStdString().c_str());
        QStringList directories = ui->backup_directories->toPlainText().split("\n");
        foreach(QString dir, directories)
        {
            QString command3 = "echo '[00055]'`echo \"BM_TARBALL_TARGETS+=('"+dir+"')\" >> /etc/backup-manager.conf`'[XXXXX]' \n";
            bash_root->write(command3.toStdString().c_str());
        }
        QString command4 = "echo '[00055]'`echo \"export BM_TARBALL_TARGETS\" >> /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command4.toStdString().c_str());
    }
    else
    {
        // log error
       //return;
    }

    if(ui->destination_directory->text() != "")
    {
        QString destination_directory = ui->destination_directory->text();
        QString command_destination_directory = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_REPOSITORY_ROOT/c\\export BM_REPOSITORY_ROOT=\""+destination_directory+"\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_destination_directory.toStdString().c_str());
    } else
    {
        // log error
       // return;
    }

    if(ui->file_name_prefix->text() != "")
    {
        QString file_name_prefix = ui->file_name_prefix->text();
        QString command_prefix = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_ARCHIVE_PREFIX/c\\export BM_ARCHIVE_PREFIX=\""+file_name_prefix+"\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_prefix.toStdString().c_str());
    }
    else
    {
        // log error
        //return;
    }

    if(ui->format_tar->isChecked())
    {
        QString command_format_tar = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_TARBALL_FILETYPE/c\\export BM_TARBALL_FILETYPE=\"tar\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_format_tar.toStdString().c_str());
        // format tar
    }
    else if(ui->format_tar_gz->isChecked())
    {
        QString command_format_tar_gz = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_TARBALL_FILETYPE/c\\export BM_TARBALL_FILETYPE=\"tar.gz\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_format_tar_gz.toStdString().c_str());
        // format tar.gz
    }
    else if(ui->format_tar_bz2->isChecked())
    {
        QString command_format_tar_bz2 = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_TARBALL_FILETYPE/c\\export BM_TARBALL_FILETYPE=\"tar.bz2\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_format_tar_bz2.toStdString().c_str());
        // format tar.bz2
    }
    else if(ui->format_tar_xz->isChecked())
    {
        QString command_format_tar_xz = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_TARBALL_FILETYPE/c\\export BM_TARBALL_FILETYPE=\"tar.xz\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_format_tar_xz.toStdString().c_str());
        // format tar.xz
    }
    else if(ui->format_tar_lzma->isChecked())
    {
        QString command_format_tar_lzma = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_TARBALL_FILETYPE/c\\export BM_TARBALL_FILETYPE=\"tar.lzma\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_format_tar_lzma.toStdString().c_str());
        // format tar.lzma
    }
    else if(ui->format_dar->isChecked())
    {
        QString command_format_dar = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_TARBALL_FILETYPE/c\\export BM_TARBALL_FILETYPE=\"dar\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_format_dar.toStdString().c_str());
        // format dar
    }
    else
    {
        QString command_format_zip = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_TARBALL_FILETYPE/c\\export BM_TARBALL_FILETYPE=\"zip\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_format_zip.toStdString().c_str());
        // format zip
    }

    if(ui->frequency_daily->isChecked())
    {
        QString command_frequency_daily = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_ARCHIVE_FREQUENCY/c\\export BM_ARCHIVE_FREQUENCY=\"daily\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_frequency_daily.toStdString().c_str());
        // frequency dialy
    }
    else
    {
        QString command_frequency_hourly = "echo '[00055]'`sed -i '/^[[:space:]]*export BM_ARCHIVE_FREQUENCY/c\\export BM_ARCHIVE_FREQUENCY=\"hourly\"\\' /etc/backup-manager.conf`'[XXXXX]' \n";
        bash_root->write(command_frequency_hourly.toStdString().c_str());
        // frequency hourly
    }
/*
export BM_REPOSITORY_ROOT="/var/archives"
export BM_ARCHIVE_FREQUENCY="daily"
export BM_ARCHIVE_PREFIX="$HOSTNAME"
export BM_TARBALL_NAMEFORMAT="long"
export BM_TARBALL_FILETYPE="tar.gz"
*/
}

void backup_window::on_run_now_clicked()
{
    QString backup_directories = ui->backup_directories->toPlainText();
    QString destination_directory = ui->destination_directory->text();
    QString file_name_prefix = ui->file_name_prefix->text();
    QStringList backup_directories_list = backup_directories.split("\n");
    backup_directories_list.removeDuplicates();
    /*
    foreach (QString dir, backup_directories_list)
    {
        QMessageBox::information(this,tr("Dir"),dir);
    }
    //QMessageBox::information(this,tr("Information"),tr(""));
    */
    save_config();
}

