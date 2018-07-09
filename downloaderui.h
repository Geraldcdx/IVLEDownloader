#ifndef DOWNLOADERUI_H
#define DOWNLOADERUI_H

#include <QDialog>
#include <QFileSystemModel>
#include <QJsonObject>
#include <QUrl>
#include <QWebView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebElement>
#include <QWebFrame>
#include <QTextDocument>
#include <QSqlDatabase>

namespace Ui {
class DownloaderUI;
}

class DownloaderUI : public QDialog
{
    Q_OBJECT

public:
    explicit DownloaderUI(QWidget *parent = 0);
    void storeintolist();
    void continued();
    void announcementParsing();
    void addModulesTabs();
    void makeDB(QString path);
    void addvalues(QString,int);
    void pull();
       ~DownloaderUI();

public slots:
    void deleted(QString);
    void update(QString,int);

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void on_listView_clicked(const QModelIndex &index);
    void parse(bool);
    void parse2(bool);
    void ModulesPageLoader();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::DownloaderUI *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *listmodel;
    QWebView *innerPage = new QWebView();
    QWebView *innerPage2 = new QWebView();
    QMap<QString,QString> modulesmap;
    QList<QUrl> modulelist;
    QList<QString>courselist;
    QString mod;
    int cnt;

    float Units=0,gradescore,CAP=0,totalcreditunits=0,previousCAP;
    QMap<QString,float> convert;
    enum columns{
        first,second,third,fourth
    };
    QSqlDatabase db;

};

#endif // DOWNLOADERUI_H
