#ifndef DOWNLOADERUI_H
#define DOWNLOADERUI_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class DownloaderUI;
}

class DownloaderUI : public QDialog
{
    Q_OBJECT

public:
    explicit DownloaderUI(QWidget *parent = 0);
    ~DownloaderUI();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::DownloaderUI *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *listmodel;
};

#endif // DOWNLOADERUI_H
