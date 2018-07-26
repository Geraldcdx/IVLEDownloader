//Created by Gerald Chua Deng Xiang


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
#include <QPixmap>
#include "dialog.h"
#include "item.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include "additem.h"
#include <QtSql>
#include <QPushButton>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTreeView>
#include <QHeaderView>

DownloaderUI::DownloaderUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloaderUI)
{
    ui->setupUi(this);
    setupFiles(DIRECTORY);//FILES TAB //change directory to make it your own path
    initDB();//Datebase creation & to-do-list implementation
    initParser();//ANNOUNCEMENT UI && Exams parsing UI
    ui->webView->load(QUrl("https://www.nuswhispers.com/home/")); //Add NUSWhispers
    setTableHeaders();//Adding CAP calculator
    startOutlook();

}
void DownloaderUI::initParser()
{
    setExamTable();//Sets exams tabs information
    connect(innerPage, SIGNAL(loadFinished(bool)), SLOT(parse(bool)));//connects with modules code and ID parsing
    connect(innerPage2, SIGNAL(loadFinished(bool)), SLOT(parse2(bool)));//connects with announcement parsing
    connect(innerPage3, SIGNAL(loadFinished(bool)), SLOT(parse3(bool)));//connects with exam details parsing
    removeTabs();//removes all the default tabs
    ModulesPageLoader();//Load announcement modules pages
    poll(); //Timer to poll every 1 hour
}

//Preprocessing that clears default tabs
void DownloaderUI::removeTabs()
{
    //qDebug()<<ui->tabWidget_2->count();
    ui->tabWidget_2->setCurrentIndex(0);
    ui->tabWidget_2->removeTab(2);
    ui->tabWidget_2->removeTab(1);
    ui->tabWidget_2->removeTab(0);
    //qDebug()<<"Test here";
    //qDebug()<<ui->tabWidget_2->count();
}

//FILES UI
void DownloaderUI::setupFiles(QString sPath)
{
    //Creates the FileSystemModel for the TreeView which is the right widget
    dirmodel = new QFileSystemModel(this);
    dirmodel ->setFilter(QDir::NoDotAndDotDot|QDir::AllDirs);//Change this such that you are able to set your own filters
    ui->treeView->setModel(this->dirmodel);
    dirmodel->setRootPath(sPath);
    ui->treeView->setRootIndex(dirmodel->index(sPath));
    //Creates the FileSystemModel for the listview, the right file widget
    listmodel=new QFileSystemModel(this);
    listmodel->setRootPath(sPath);
    ui->listView->setModel(listmodel);
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

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

//ANNOUNCEMENT UI

void DownloaderUI::poll()
{
QTimer *timer = new QTimer(this);
QObject::connect(timer, SIGNAL(timeout()), this, SLOT(ModulesPageLoader()));
timer->start(3600000); //time specified in ms to poll modulesPageLoader every 1 hour first
}

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
    examsParsing();//Parse exam info
}

//Stores all QUrls into modulelist and CourseNAme
void DownloaderUI::storeintolist()
{
    QMapIterator<QString, QString> iter(modulesmap);
    while(iter.hasNext()){
        iter.next();
        //qDebug()<<iter.key()<<iter.value();
        QString str=QString("https://ivle.nus.edu.sg/API/Lapi.svc/Announcements?APIKey=%1&AuthToken=%2&CourseID=%3&Duration=0&TitleOnly=false&output=json").arg(APIKEY).arg(TOKEN).arg(iter.value());
        QString str2=QString("https://ivle.nus.edu.sg/api/Lapi.svc/Timetable_ModuleExam?APIKey=%1&AuthToken=%2&CourseID=%3").arg(APIKEY).arg(TOKEN).arg(iter.value());
        modulelist.append(QUrl(str));//For URL with courseID, e.g. wfjefkawrgbawruh-42y8qwr2q- == gibberish string
        examslist.append(QUrl(str2));//For URL with course ID to parse exam information
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
         QTextDocument doc,doc2,doc3,doc4;
         doc.setHtml(nested["Title"].toString());//Strips the HTML from the given information
         doc2.setHtml(nested["Description"].toString());
         doc3.setHtml(nested["CreatedDate_js"].toString());
         QString str=doc3.toPlainText();
         QString subString = str.mid(0,10);//extraction of the date
         QString subString2=str.mid(11,8);//extraction of the time
         str="";
         str.append("Date: "+subString+"        Time: "+subString2);
         QtJson::JsonObject nested2=nested["Creator"].toMap();
         doc4.setHtml(nested2["Name"].toString());
         strann.append("------------------------------------------------------------------------------------------------------------------------------ANNOUNCEMENT-------------------------------------------------------------------------------------------------------------\n");
         strann.append(str+"\n\n");
         strann.append("Written by: "+doc4.toPlainText()+"\n\n");
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

//Exam Details UI

void DownloaderUI::setExamTable()
{
    //setting the headers of the Exams tab
    ui->tableWidget_2->setColumnCount(2);
    QStringList headers;
    headers<<"Module"<<"Date of Exam";
    ui->tableWidget_2->setHorizontalHeaderLabels(headers);
    ui->tableWidget_2->resizeColumnsToContents();
}
void DownloaderUI::examsParsing()
{
    cnt2=0;
    continued2();
}
void DownloaderUI::continued2(){
     if(cnt2 < modulelist.size()){
         innerPage3->setUrl(examslist.at(cnt2));//connects to parse3(bool)
     }
     else
     {
         ui->tableWidget_2->resizeColumnsToContents();
         return;
     }
}
//Parses Exam information
void DownloaderUI::parse3(bool)
{
    QWebFrame *frameInner3 = innerPage3->page()->mainFrame();
    QWebElement doc = frameInner3->documentElement();
    QString json=doc.toPlainText();// json2 is a QString containing the JSON data
    //qDebug()<<json;

    //same as in parse(bool)
    bool ok;
    QtJson::JsonObject result = QtJson::parse(json, ok).toMap();
    if(!ok) {
      qFatal("An error occurred during parsing");
    }
    //qDebug()<<"Testing the printing of Json:   "<<result["Comments"].toString();
    QString strann,ExamInfo,ModuleCode;
    foreach(QVariant plugin, result["Results"].toList()) {//for[]
         QtJson::JsonObject nested = plugin.toMap();
         QTextDocument doc,doc2;
         doc.setHtml(nested["ExamInfo"].toString());//Strips the HTML from the given information
         doc2.setHtml(nested["ModuleCode"].toString());
         //sets the additional exam dates
         ExamInfo=doc.toPlainText();
         ModuleCode=doc2.toPlainText();
         qDebug()<<ExamInfo<<ModuleCode;
    }
    //qDebug()<<strann;
    if(ExamInfo!=""){
        int num=ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
        ui->tableWidget_2->setItem(num,f,new QTableWidgetItem(ModuleCode));
        ui->tableWidget_2->setItem(num,s,new QTableWidgetItem(ExamInfo));
    }
        cnt2++;//iterates to continue the loop
        continued2();

}

//CAP CALCULATOR UI
//set Table headers and loads the mapping of grade to score
void DownloaderUI::setTableHeaders()
{
    ui->tableWidget->setColumnCount(4);
    QStringList headers;
    headers<<"Module"<<"Credit Units"<<"Grade"<<"Current CAP";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->resizeColumnsToContents();
    loadCAPscores();//loads the cap scores
    ui->tableWidget_3->setAlternatingRowColors(true);
    ui->tableWidget_3->resizeColumnsToContents();
    ui->tableWidget_4->setAlternatingRowColors(true);
    ui->tableWidget_4->resizeColumnsToContents();
    for(int i=0;i<11;i++)
        for(int j=0;j<2;j++){
            ui->tableWidget_3->item(i,j)->setTextAlignment(Qt::AlignCenter);

        }
     ui->tableWidget_3->resizeRowsToContents();
     ui->tableWidget_4->resizeRowsToContents();
}
//Press to add module and calculate CAP
void DownloaderUI::on_pushButton_2_clicked()
{
    Dialog add;
    int outcome;
    add.setWindowTitle("Add a module!");
    outcome=add.exec();
    if(outcome==QDialog::Rejected){
        return;
    }
    else{
        QString mod=add.Module();
        QString grade=add.Grade();
        float units=add.Creditunits();

        //qDebug()<<mod<<grade<<units;
        previousCAP=totalcreditunits*CAP;//takes the previous cap
        totalcreditunits+=units;//total units
        gradescore=convert[grade];
        CAP=(gradescore*units+previousCAP)/totalcreditunits;
        //qDebug()<<gradescore<<units<<previousCAP<<totalcreditunits;

        //sets the tables
        int num=ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(num,first,new QTableWidgetItem(mod));
        ui->tableWidget->setItem(num,second,new QTableWidgetItem(QString::number(units)));
        ui->tableWidget->setItem(num,third,new QTableWidgetItem(grade));
        ui->tableWidget->setItem(num,fourth,new QTableWidgetItem(QString::number(CAP)));
        //qDebug()<<mod<<grade<<units;
    }
}

//Clears the rows of CAP workings
void DownloaderUI::loadCAPscores()
{
    convert["A+"]=5.0;
    convert["A"]=5.0;
    convert["A-"]=4.5;
    convert["B+"]=4.0;
    convert["B"]=3.5;
    convert["B-"]=3.0;
    convert["C+"]=2.5;
    convert["C"]=2.0;
    convert["D+"]=1.5;
    convert["D"]=1.0;
    convert["F"]=0;
}
void DownloaderUI::on_pushButton_3_clicked()
{
    previousCAP=0;
    totalcreditunits=0;
    ui->tableWidget->setRowCount(0);
}
void DownloaderUI::on_pushButton_5_clicked()
{
    //Write function to determine 1st class honours assuming 160MCS
    float current=CAP*totalcreditunits;
    float unitsleft=160-totalcreditunits;
    //1st class
    float firstclass=(4.5*160-current)/unitsleft;
    //2nd upper
    float secondclass=(4.0*160-current)/unitsleft;
    //2nd lower
    float secondlower=(3.5*160-current)/unitsleft;
    float thirdclass=(3.0*160-current)/unitsleft;
    float pass=(2.0*160-current)/unitsleft;
    QString string;
    string.append("The numbers reflected are the minimum average grade you need for the remaining modular credits units towards 160mcs:\n\n");
    string.append("To get 1st Class Honours(CAP=4.5) you need "+QString::number(firstclass)+"\n\n");
    string.append("To get 2nd Upper Class Honours(CAP=4.0) you need "+QString::number(secondclass)+"\n\n");
    string.append("To get 2nd Lower Class Honours(CAP=3.5) you need "+QString::number(secondlower)+"\n\n");
    string.append("To get 3rd Class Honours(CAP=3.0) you need "+QString::number(thirdclass)+"\n\n");
    string.append("To get pass(CAP=2.0) you need "+QString::number(pass)+"\n\n");
    string.append("JiaYou and all the Best");
    QMessageBox::information(this,"CAP Goals",string);
}
//For the To-Do-List UI

//function to intialise the to-do-list
void DownloaderUI::initDB()
{
    QString DB=DIRECTORY;
    DB.append("/db.sqlite");
    makeDB(DB);//makes the Database based on path DB
    pull();//draws out all information for to-do-list

}
//Makes new items
void DownloaderUI::on_pushButton_6_clicked()
{
    AddItem add;
    int outcome;
    outcome=add.exec();
    if(outcome==QDialog::Rejected){
        return;
    }
    QString item=add.GetText();
    Item *i=new Item;
    connect(i,SIGNAL(itemdeleted(QString)),this,SLOT(deleted(QString)));
    i->setText(item);
    i->setProgress(0);
    connect(i,SIGNAL(changeVal(QString,int)),this,SLOT(update(QString,int)));
    ui->verticalLayout_6->addWidget(i);
    addvalues(item,0);
}
//Add values to the DB
void DownloaderUI::addvalues(QString Item,int Progress)
{
    if(!db.open()) qDebug()<<"FAILED";
    QSqlQuery qry;
    qry.prepare("INSERT INTO todoDB ("
                "Item,"
                "Progress)"
                "VALUES (?,?);");
    qry.addBindValue(Item);
    qry.addBindValue(Progress);
    if(!qry.exec())qDebug()<<"qruy failed";
    db.close();
}
//Update Values in the DB
void DownloaderUI::update(QString Item,int Progress)
{
    if(!db.open()) qDebug()<<"FAILED";
    QSqlQuery qry;
    qry.prepare("update todoDB set Item=?,Progress=? where Item=?" );
    qry.addBindValue(Item);
    qry.addBindValue(Progress);
    qry.addBindValue(Item);
    if(!qry.exec())qDebug()<<"Doesn't work";
    db.close();
}
//Delete Values in DB
void DownloaderUI::deleted(QString Item)
{
    if(!db.open()) qDebug()<<"FAILED";
    QSqlQuery qry;
    qry.prepare("delete from todoDB where Item=?");
    qry.addBindValue(Item);
    if(!qry.exec())qDebug()<<"GG";
    db.close();
}

//Sets up the UI based on saved information
void DownloaderUI::pull()
{
    if(!db.open()) qDebug()<<"FAILED";
    QSqlQuery qry;
    qry.prepare("SELECT Item,Progress FROM todoDB");
    qry.exec();
    //int num=1;
    //QString str="i";
    while (qry.next()){
            QSqlRecord record = qry.record();
            //qDebug() <<record.value("Item").toString()<<record.value("Progress").toInt();
            Item *i=new Item;
            connect(i,SIGNAL(itemdeleted(QString)),this,SLOT(deleted(QString)));
            i->setText(record.value("Item").toString());
            i->setProgress(record.value("Progress").toInt());
            connect(i,SIGNAL(changeVal(QString,int)),this,SLOT(update(QString,int)));
            ui->verticalLayout_6->addWidget(i);
        }
    db.close();
}
//Intialises a DB
void DownloaderUI::makeDB(QString path)
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);//"C:/Users/Gerald/Desktop/orbitaltest/db.sqlite"
    if(!db.open()) qDebug()<<"FAILED";
    QString query="CREATE TABLE todoDB ("
                  "Item VARCHAR(150),"
                   "Progress integer);";
    QSqlQuery qry;
    if(!qry.exec(query)) qDebug()<<"Query failed or Database already created";
    db.close();
}


//Outlook
//Opens QSettings and check if username and password exists
void DownloaderUI::startOutlook(){

    //Opens QSettings, default username and password set to null
    QSettings settings ("IVLE", "App");
    settings.beginGroup("userinfo");
    QString tempUsername= settings.value("tempUsername","null").toString();
    QString tempPassword = settings.value("tempPassword", "null").toString();
    settings.endGroup();

    //If Qsettings is null, group box will show for user to key in details
    if(tempUsername=="null"){
        ui->webView_2->load(QUrl("https://outlook.office.com/owa/?realm=u.nus.edu"));
        ui->webView_2->hide();

        connect(this,SIGNAL(login()),this,SLOT(loadOutlook()));
    //If Qsettings was saved before, outlook will load user details and login
    } else {
        ui->groupBox->hide();
        ui->webView_2->load(QUrl("https://outlook.office.com/owa/?realm=u.nus.edu"));
        connect(ui->webView_2, SIGNAL(loadFinished(bool)), this, SLOT(loadOutlook()));
    }

}

//Saves user details into Qsettings and load outlook
void DownloaderUI::on_pushButton_login_clicked()
{
    //Saves line edit username and password into Qsettings and sends out login signal to load Outlook
    QSettings settings ("IVLE", "App");
    settings.beginGroup("userinfo");
    settings.setValue("tempUsername", ui->lineEdit_username->text());
    settings.setValue("tempPassword", ui->lineEdit_password->text());
    settings.endGroup();
    //qDebug()<< "temp details saved";
    startOutlook();
    emit login();

}

//Inputs Qsettings username and password into outlook url placeholders and login
void DownloaderUI:: loadOutlook(){

    ui->webView_2->show();
    ui->groupBox->hide();
    //calls qsettings
    QSettings settings ("IVLE", "App");
    settings.beginGroup("userinfo");
    QString tempUsername= settings.value("tempUsername","null").toString();
    QString tempPassword = settings.value("tempPassword", "null").toString();
    settings.endGroup();

    //inserts username and password into webview
    ui->webView_2->page()->mainFrame()->evaluateJavaScript(QString("document.getElementById('ContentPlaceHolder1_UsernameTextBox').value = '%1'").arg(tempUsername));
    ui->webView_2->page()->mainFrame()->evaluateJavaScript(QString("document.getElementById('ContentPlaceHolder1_PasswordTextBox').value = '%1'").arg(tempPassword));
    ui->webView_2->page()->mainFrame()->evaluateJavaScript(QString("document.getElementById('ContentPlaceHolder1_SubmitButton').click()"));

    disconnect(ui->webView_2, SIGNAL(loadFinished(bool)), this, SLOT(loadOutlook()));
    disconnect(ui->webView_2, SIGNAL(login()), this, SLOT(loadOutlook()));
}

//Function resets username and password to null, allowing the user to log in from fresh
void DownloaderUI::on_pushButton_forgetMe_clicked()
{
    QSettings settings ("IVLE", "App");
    settings.beginGroup("userinfo");
    settings.setValue("tempUsername", "null");
    settings.setValue("tempPassword", "null");
    //settings.endGroup();
    //qDebug()<<settings.value("tempUsername","exist");

    ui->webView_2->hide();
//delete and recreate object
    ui->groupBox->show();

}





