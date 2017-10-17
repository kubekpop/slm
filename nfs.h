#ifndef NFS_H
#define NFS_H

#include <QWidget>

namespace Ui {
class nfs;
}

class nfs : public QWidget
{
    Q_OBJECT

public:
    explicit nfs(QWidget *parent = 0);
    ~nfs();

private slots:
    void collector();

    void on_share_combobox_currentTextChanged(const QString &arg1);

    void on_apply_clicked();

private:
    Ui::nfs *ui;
};

#endif // NFS_H
