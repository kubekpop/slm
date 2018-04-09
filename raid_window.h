#ifndef RAID_WINDOW_H
#define RAID_WINDOW_H

#include <QWidget>

namespace Ui {
class raid_window;
}

class raid_window : public QWidget
{
    Q_OBJECT

public:
    explicit raid_window(QWidget *parent = 0);
    ~raid_window();
    void bash_output_interpreter(QString output);

signals:
    void data_to_log(QString new_content);


private:
    Ui::raid_window *ui;
};

#endif // RAID_WINDOW_H
