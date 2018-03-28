#include "ftp_window.h"
#include "ui_ftp_window.h"

std::array<std::string, 7> akcje = {"anonymous_enable", "write_enable", "anon_upload_enable", "anon_mkdir_write_enable", "local_enable", "chown_uploads", "chroot_local_user" };

ftp_window::ftp_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ftp_window)
{
    ui->setupUi(this);
}

ftp_window::~ftp_window()
{
    delete ui;
}



void ftp_window::ftpChange(QString param, bool logic)
{

    QString new_val;
    if(logic == true)
    {
        new_val = "YES";
    }
    else
    {
        new_val = "NO";
    }

    QString fileFtp = "/home/kuba/vsftpd.conf";
    //sprawdza, i jak jest wpis w configu to echuje "jest"
    QString command = "grep \"^[[:blank:]]*"+param+".*=.*$\" "+fileFtp+" >/dev/null && echo jest";
//=================================PROCES
    QProcess processFtp2;
    processFtp2.start("/bin/bash", QStringList()<< "-c" << command);
    processFtp2.waitForFinished();
    QString added = processFtp2.readAllStandardOutput();
    added.replace("\n","");

    processFtp2.close();

    bool is_present=true;

    if(added=="")
    {
        //to sie ma robić jak nie ma wpisu w configu
        command = "echo "+param+"="+new_val+">>"+fileFtp;
        //=================================PROCES
        QProcess processFtp3;
        processFtp3.start("/bin/bash", QStringList()<< "-c" << command);
        processFtp3.waitForFinished();
        processFtp3.close();
        is_present=true;
    }
    else
    {
        is_present=false;//wpis jest, i ma robic to co wczesniej bylo czyli sedem zamieniac
    }

    if(is_present==false)
    {
    command = "sed -i '/^[[:space:]]*"+param+"=.*/c\\"+param+"="+new_val+"' "+fileFtp;
    //=================================PROCES
    QProcess processFtp;
    processFtp.start("/bin/bash", QStringList()<< "-c" << command);
    processFtp.waitForFinished();
    processFtp.close();
    }
 }

void ftp_window::ftpChange(QString param, QString logic)
{
    QString fileFtp = "/home/kuba/vsftpd.conf";

    //sprawdza, i jak jest wpis w configu to echuje "jest"
    QString command = "grep \"^[[:blank:]]*"+param+".*=.*$\" "+fileFtp+" >/dev/null && echo jest";
    //=================================PROCES
    QProcess processFtp2;
    processFtp2.start("/bin/bash", QStringList()<< "-c" << command);
    processFtp2.waitForFinished();
    QString added = processFtp2.readAllStandardOutput();
    added.replace("\n","");

    processFtp2.close();

    bool is_present=true;

    if(added=="")
    {
        //to sie ma robić jak nie ma wpisu w configu
        command = "echo "+param+"="+logic+">>"+fileFtp;
        //=================================PROCES
        QProcess processFtp3;
        processFtp3.start("/bin/bash", QStringList()<< "-c" << command);
        processFtp3.waitForFinished();
        processFtp3.close();
        is_present=true;
    }
    else
    {
        is_present=false;//wpis jest, i ma robic to co wczesniej bylo czyli sedem zamieniac
    }

    if(is_present==false)
    {
    command = "sed -i '/^[[:space:]]*"+param+"=.*/c\\"+param+"="+logic+"' "+fileFtp;
    //=================================PROCES
    QProcess processFtp;
    processFtp.start("/bin/bash", QStringList()<< "-c" << command);
    processFtp.waitForFinished();
    processFtp.close();
    }

    /*
    QString command = "sed -i '/"+param+"/c\\"+param+"="+logic+"' "+fileFtp;
    QProcess processFtp;
    processFtp.start("/bin/bash", QStringList()<< "-c" << command);
    processFtp.waitForFinished();
    processFtp.close();
    */
}

QString ftp_window::ftpCheck(QString param)
{
    QString fileFtp = "/home/kuba/vsftpd.conf";
    QString command = "grep \"^[[:blank:]]*"+param+".*=.*$\" "+fileFtp+" | cut -d= -f2";
    //=================================PROCES
    QProcess statussFtp;
    statussFtp.start("/bin/bash", QStringList()<< "-c" << command);
    statussFtp.waitForFinished();
    QString stanFtpParam = statussFtp.readAllStandardOutput();
    stanFtpParam.replace("\n","");
    statussFtp.close();
    if(stanFtpParam=="")
    {
        stanFtpParam="NO";
    }
    return stanFtpParam;
}

void ftp_window::ftpChange(QString param)
{
    QString command = "mkdir "+param+"; chown root:ftp "+param+"; chmod 555 "+param+"; usermod -d "+param+" ftp";
    //=================================PROCES
    QProcess process;
    process.start("/bin/bash", QStringList()<< "-c" << command);
    process.waitForFinished();
    process.close();
}


void ftp_window::ftpOptionCheck()
{
    for(int i = 0; i<7;i++)
    {
        //QString ftpParam = akcje[i];
        QString result = ftpCheck(QString::fromStdString(akcje[i]));
        bool state = false;
        if(result == "YES" || result == "yes")
        {
            state = true;
        }
        else
        {
            state = false;
        }

        switch (i) {
        case 0:
            ui->anonymous_enable->setChecked(state);
            break;
        case 1:
            ui->write_enable->setChecked(state);
            break;
        case 2:
            ui->anon_upload_enable->setChecked(state);
            break;
        case 3:
            ui->anon_mkdir_write_enable->setChecked(state);
            break;
        case 4:
            ui->local_enable->setChecked(state);
            break;
        case 5:
            ui->chown_uploads->setChecked(state);
            break;
        case 6:
            ui->chroot_local_user->setChecked(state);
            break;
        default:
            break;
        }
    }
}


void ftp_window::ftpOptionCheckText()
{
    QString username = ftpCheck("chown_username");
    if(username == "NO")
    {
        username = "";
    }
    ui->chown_username->setText(username);
    QString banner = ftpCheck("ftpd_banner");
    if(banner == "NO")
    {
        banner="";
    }
    ui->ftpd_banner->setText(banner);
}

/*
 * AKCJE DLA FTP:
anonymous_enable=NO
write_enable=YES
anon_upload_enable=YES
anon_mkdir_write_enable=YES
local_enable=YES
chown_uploads=YES
chown_username=whoever
ftpd_banner=Welcome to blah FTP service.
chroot_local_user=YES

ZMIANA CHROOTA DLA ANONUW:
sudo mkdir /srv/ftp
sudo usermod -d /srv/ftp ftp

 */



void ftp_window::on_anonymous_enable_toggled(bool checked)
{
    ftpChange("anonymous_enable", checked);
}

void ftp_window::on_local_enable_toggled(bool checked)
{
    ftpChange("local_enable", checked);
}

void ftp_window::on_chown_username_editingFinished()
{
    ftpChange("chown_username",ui->chown_username->text());
}

void ftp_window::on_ftpd_banner_editingFinished()
{
    ftpChange("ftpd_banner",ui->ftpd_banner->text());
}


void ftp_window::on_write_enable_toggled(bool checked)
{
    ftpChange("write_enable", checked);
}

void ftp_window::on_anon_upload_enable_toggled(bool checked)
{
    ftpChange("anon_upload_enable", checked);
}

void ftp_window::on_anon_mkdir_write_enable_toggled(bool checked)
{
    ftpChange("anon_mkdir_write_enable", checked);
}

void ftp_window::on_chroot_local_user_toggled(bool checked)
{
    ftpChange("chroot_local_user", checked);
}

void ftp_window::on_chown_uploads_toggled(bool checked)
{
    ftpChange("chown_uploads", checked);
}

void ftp_window::on_anonymous_root_editingFinished()
{
    //ftpChange(ui->anonymous_root->text());
}

