#include "nfs.h"
#include "ui_nfs.h"

nfs::nfs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nfs)
{
    ui->setupUi(this);
}

nfs::~nfs()
{
    delete ui;
}

void nfs::collector()
{

}

void nfs::on_share_combobox_currentTextChanged(const QString &arg1)
{

}

void nfs::on_apply_clicked()
{
    QString name = ui->share_combobox->currentText();

}
