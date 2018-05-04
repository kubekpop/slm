#ifndef NFS_WINDOW_H
#define NFS_WINDOW_H

#include <QWidget>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class nfs_window;
}

class nfs_window : public QWidget
{
    Q_OBJECT

public:
    explicit nfs_window(QWidget *parent = 0);
    ~nfs_window();
    QProcess *bash_root;
    void load_shares();
    void nfs_prepare_window(QString paths, QString params);
    void get_params(QString paths);
    QString from_begin_to_char(QString input, QString given_char, bool if_false_returns_rest);
    QStringList sort_options(QString share_options);
    void bash_output_interpreter(QString output);

signals:
    void data_to_log(QString new_content);

private slots:

    void on_share_combobox_currentTextChanged(const QString &arg1);

    void on_apply_clicked();

private:
    Ui::nfs_window *ui;
    QStringList paths_splitted;
    QStringList params_splitted;
};

#endif // NFS_WINDOW_H
