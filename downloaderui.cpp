#include "downloaderui.h"
#include "ui_downloaderui.h"
#include "globalvar.h"
#include <QUrl>
#include <QDesktopServices>
#include <QFileSystemModel>
#include <QDir>
#include <QDebug>

DownloaderUI::DownloaderUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloaderUI)
{
    ui->setupUi(this);

    //Creates the FileSystemModel for the TreeView which is the right widget
    //QString sPath="C:/Users";  //Change this such that you can issue your own relative path
    QString sPath= DIRECTORY;
    dirmodel = new QFileSystemModel(this);
    dirmodel ->setFilter(QDir::NoDotAndDotDot|QDir::AllDirs);//Change this such that you are able to set your own filters
    ui->treeView->setModel(this->dirmodel);
    dirmodel->setRootPath(sPath);
    ui->treeView->setRootIndex(dirmodel->index(sPath));
    //qDebug()<<QDir::currentPath();

    //Creates the FileSystemModel for the listview, the right file widget
    listmodel=new QFileSystemModel(this);
    listmodel->setRootPath(sPath);
    ui->listView->setModel(listmodel);
}

DownloaderUI::~DownloaderUI()
{
    delete ui;
}

void DownloaderUI::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath=dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(listmodel->setRootPath(sPath));
}

void DownloaderUI::on_listView_clicked(const QModelIndex &index)
{
    QString sPath=listmodel->fileInfo(index).absoluteFilePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(sPath));
}
