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

void nfs_window::collector()
{

}

void nfs_window::on_share_combobox_currentTextChanged(const QString &arg1)
{

}

void nfs_window::on_apply_clicked()
{
    QString name = ui->share_combobox->currentText();

}
