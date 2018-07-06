//Created by Gerald Chua Deng Xiang
//Potential improvements
//1)Can maybe just parse modulecode and ID once only and not always?? QSettings or external text file
//2)Find out a method to compare previous files and changes in them? But, doesn't sound feasible but does save computing power
//but not on polling rate

#include "downloaderui.h"
#include "ui_downloaderui.h"
#include "globalvar.h"
#include <QUrl>
#include <QDesktopServices>
#include <QFileSystemModel>
#include <QDir>
#include <QDebug>
#include "qtjson.h"
#include "form.h"
#include <QFile>
#include <QJsonObject>
#include <QWebView>
#include <QTimer>

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

    //Adding announcement tabs
    connect(innerPage, SIGNAL(loadFinished(bool)), SLOT(parse(bool)));//connects with modules code and ID parsing
    connect(innerPage2, SIGNAL(loadFinished(bool)), SLOT(parse2(bool)));//connects with announcement parsing
    //removes all the default tabs
    ui->tabWidget_2->setCurrentIndex(0);
    ui->tabWidget_2->removeTab(2);
    ui->tabWidget_2->removeTab(1);
    ui->tabWidget_2->removeTab(0);
    ModulesPageLoader();
    //Timer Setting
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(ModulesPageLoader()));
    timer->start(3600000); //time specified in ms to poll modulesPageLoader every 1 hour first
}

//ANNOUNCEMENT UI

//Loads the "modules" api to extract module information
void DownloaderUI::ModulesPageLoader()
{
    QString modules=QString("https://ivle.nus.edu.sg/api/Lapi.svc/Modules?APIKey=%1&AuthToken=%2&Duration=0&IncludeAllInfo=false").arg(APIKEY).arg(TOKEN);
    QUrl url2(modules);
    innerPage->setUrl(url2);

}
//Parsing of modulesName and CourseIC connected to ModulesPageloader()
void DownloaderUI::parse(bool)
{
    //loads webpage info into a QString
    QWebFrame *frameInner = innerPage->page()->mainFrame();
    QWebElement doc = frameInner->documentElement();
    QString json=doc.toPlainText(); // json is a QString containing the JSON data
    //qDebug()<<json;

    //Uses a library for json parsing found here----https://github.com/qt-json/qt-json , qtjason.cpp and qtjason.h

    //---------SYNTAX FOUND ON GITHUB STARTS HERE-----------
    bool ok;
    QtJson::JsonObject result = QtJson::parse(json, ok).toMap();
    if(!ok) {
      qFatal("An error occurred during parsing");
    }
    //qDebug()<<"Testing the printing of Json:   "<<result["Comments"].toString();

    foreach(QVariant plugin, result["Results"].toList()) {//for[]
         QtJson::JsonObject nested = plugin.toMap();
         //qDebug()<<nested["ID"].toString();
         if(nested["ID"].toString()!="00000000-0000-0000-0000-000000000000")//removes closed modules
            modulesmap[nested["CourseCode"].toString()]=nested["ID"].toString();//"ID" is the courseID, "CourseCode" is CourseName
    }
    //---------------ENDS HERE-----------------------------------

    storeintolist();
    announcementParsing();//this starts the announcement parsing

}

//Stores all QUrls into modulelist and CourseNAme
void DownloaderUI::storeintolist()
{
    QMapIterator<QString, QString> iter(modulesmap);
    while(iter.hasNext()){
        iter.next();
        //qDebug()<<iter.key();
        QString str=QString("https://ivle.nus.edu.sg/API/Lapi.svc/Announcements?APIKey=OLRtFGIWu0X2ce3rEfAzE&AuthToken=1973134E762926E27F69FD82D61628D9BF638F547331C85F50E81AEC4D2FD35D926B0963555F4A8D3A62379E1B046AE329A834EA42E5287F32DE17C4155E9F59F425407BCC40D9FA786F1591419E496309CF997843BF951780B6015D1C0BDADEE2EAA18D0C97C3BF90C944A389480F231BA35B3D819E8063B9403B42FC2453D57EE2F62367F27D8DDD352E53B81D846C39AA0E07747598C336792D8167E1FBD024603E83B166E21697FBEE19376667B44E87492DA6B98BF8EE7FC070A29DF7C3C7B3DFFAC872474635DB8D60EDD8B2BB27AAE39D7DBC21C93AEB9CB9E93D3FEEF8FEB7F68AA72E9679818DAFE1C89FD9&CourseID=%1&Duration=0&TitleOnly=false&output=json").arg(iter.value());
        modulelist.append(QUrl(str));//For courseID, e.g. wfjefkawrgbawruh-42y8qwr2q- == gibberish string
        courselist.append(iter.key());//For courseName e.g.2040
    }
}

//Iterates through the list to parse announcements and load webpages into innerPage2
void DownloaderUI::continued(){
     if(cnt < modulelist.size()){
         mod=courselist.at(cnt);
         innerPage2->setUrl(modulelist.at(cnt));//connects to parse2(bool)
         //qDebug()<<cnt;
     }
     else addModulesTabs();
}

//Starts the loading of webpages by calling continued()
void DownloaderUI::announcementParsing()
{
    cnt=0;
    continued();

}

//Parses announcement title and descption and strips HTML
void DownloaderUI::parse2(bool)
{

    QWebFrame *frameInner2 = innerPage2->page()->mainFrame();
    QWebElement doc = frameInner2->documentElement();
    QString json2=doc.toPlainText();// json2 is a QString containing the JSON data
    //qDebug()<<json2;

    //same as in parse(bool)
    bool ok;
    QtJson::JsonObject result = QtJson::parse(json2, ok).toMap();
    if(!ok) {
      qFatal("An error occurred during parsing");
    }
    //qDebug()<<"Testing the printing of Json:   "<<result["Comments"].toString();


    QString strann;
    foreach(QVariant plugin, result["Results"].toList()) {//for[]
         QtJson::JsonObject nested = plugin.toMap();
         QTextDocument doc,doc2;
         doc.setHtml(nested["Title"].toString());//Strips the HTML from the given information
         doc2.setHtml(nested["Description"].toString());
         strann.append("------------------------------------------------------------------------------------------------------------------------------ANNOUNCEMENT-------------------------------------------------------------------------------------------------------------\n");
         strann.append(doc.toPlainText());
         strann.append("\n");
         strann.append(doc2.toPlainText());
         strann.append("\n\n\n\n");
         //qDebug()<< doc.toPlainText();
         //qDebug()<< doc2.toPlainText();
    }
    //qDebug()<<strann;

    //Creating a Announcement.txt to store all announcement in modules so as to extract out for display
    //Storing of Announcements
    QString filename=DIRECTORY;//"C:/Users/Gerald/Desktop/orbitaltest" ;//use your directory for testing
    filename.append("/"+mod);//mod stores the current module name
    filename.append("/Announcements.txt");
    QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
                    stream << strann;
        }
        file.close();
        cnt++;//iterates to continue the loop
     //ui->label->setText(strann);
        continued();

}

//Creates ModulesTabs and add relevent announments to them
void DownloaderUI::addModulesTabs()
{
    for(int i=0;i<courselist.size();i++){
        Form* text= new Form();
        ui->tabWidget_2->addTab(text,QString("%1").arg(courselist.at(i)));
        QString filename=DIRECTORY;//"C:/Users/Gerald/Desktop/orbitaltest" ;//use your directory for testing
        filename.append("/"+(QString("%1").arg(courselist.at(i))));//mod stores the current module name
        filename.append("/Announcements.txt");
        QFile file(filename);
        QString line;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                    QTextStream stream(&file);
                    while (!stream.atEnd()){
                        line.append(stream.readLine()+"\n");
                    }
                }
                file.close();
        text->settingofText(line);
    }
}

//DOWNLOADED FILES UI

//For the folder view on the left
void DownloaderUI::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath=dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(listmodel->setRootPath(sPath));
}
//For the documents view on the right
void DownloaderUI::on_listView_clicked(const QModelIndex &index)
{
    QString sPath=listmodel->fileInfo(index).absoluteFilePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(sPath));
}

DownloaderUI::~DownloaderUI()
{
    delete ui;
}
