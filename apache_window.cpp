#include "apache_window.h"
#include "ui_apache_window.h"

apache_window::apache_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::apache_window)
{
    ui->setupUi(this);
}

apache_window::~apache_window()
{
    delete ui;
}

void apache_window::bash_output_interpreter(QString output)
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
