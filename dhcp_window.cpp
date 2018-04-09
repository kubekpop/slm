#include "dhcp_window.h"
#include "ui_dhcp_window.h"

dhcp_window::dhcp_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dhcp_window)
{
    ui->setupUi(this);
}

dhcp_window::~dhcp_window()
{
    delete ui;
}

void dhcp_window::bash_output_interpreter(QString output)
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
