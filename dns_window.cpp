#include "dns_window.h"
#include "ui_dns_window.h"

dns_window::dns_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dns_window)
{
    ui->setupUi(this);
}

dns_window::~dns_window()
{
    delete ui;
}

void dns_window::bash_output_interpreter(QString output)
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
        default:
            break;
        }
    }
}
