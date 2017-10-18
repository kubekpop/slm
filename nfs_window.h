#ifndef NFS_WINDOW_H
#define NFS_WINDOW_H

#include <QWidget>

namespace Ui {
class nfs_window;
}

class nfs_window : public QWidget
{
    Q_OBJECT

public:
    explicit nfs_window(QWidget *parent = 0);
    ~nfs_window();

private slots:
    void collector();

    void on_share_combobox_currentTextChanged(const QString &arg1);

    void on_apply_clicked();

private:
    Ui::nfs_window *ui;
};

#endif // NFS_WINDOW_H
