#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>
#include "globalvar.h"
#include "downloaderui.h"
#include "mainwindow.h"
#include <QWebView>
#include <QWebElement>
#include <QWebFrame>
#include <QUrl>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose,false);
    this->setWindowFlags((Qt::WindowFlags) (Qt::Window | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint  & (~Qt::WindowFullscreenButtonHint)) );
    webviewDialog = new QDialog(this);
    webviewDialog->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
    webviewDialog->setAttribute(Qt::WA_QuitOnClose,false);
    webView = new QWebView(webviewDialog);
    webviewDialog->layout()->addWidget(webView);
    webviewDialog->layout()->setMargin(0);
    connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(onWebviewLoaded()));

    adialog = new AdvancedDialog(this);
    connect(adialog, &AdvancedDialog::configSaved, this, &SettingsDialog::configSaved);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onWebviewLoaded(){
    QUrl url = webView->url();
    if(url.host() == QString("ivle.nus.edu.sg")&&
            url.path() == QString("/api/login/login_result.ashx")&&
            QUrlQuery(url).queryItemValue("r") == QString("0")){
        qDebug()<<"OK!";
        webviewDialog->close();
        emit gottenToken(webView->page()->mainFrame()->toPlainText());
    }
}

void SettingsDialog::on_pushButton_clicked()
{
    webView->setUrl(QString("https://ivle.nus.edu.sg/api/login/?apikey=%1").arg(APIKEY));
    //webviewDialog->setWindowModality(Qt::ApplicationModal);
    //setting modality cause the cursor to disappear in textbox...
    webviewDialog->show();
}

void SettingsDialog::setLabelText(const QString &t){
    ui->label->setText(t);
}

void SettingsDialog::setDlText(const QString &t){
    ui->label_2->setText(t);
}

void SettingsDialog::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Choose a download destination");
    if(!path.isEmpty()){
        ui->label_2->setText(QString("Download to: %1").arg(path));
        ui->verticalLayout->update();
        emit updateDirectory(path);
    }
}

void SettingsDialog::setDisplayedSettings(QVariantMap m)
{
    ui->spinBox->setValue( (int)(m["maxFileSize"].toDouble() / 1024 / 1024) );
    ui->notifyCheckBox->setCheckState( m["notify"].toBool() ? Qt::Checked : Qt::Unchecked );
    ui->igUpCheckBox->setCheckState( m["ignoreUploadable"].toBool() ? Qt::Checked : Qt::Unchecked );
    ui->notifyAnmCheckBox->setCheckState( m["notifyAnm"].toBool() ? Qt::Checked : Qt::Unchecked );
    adialog->setConfigContent(m["jsonConfig"].toString());
}

bool SettingsDialog::event(QEvent *e)
{
    if(e->type() == QEvent::WindowActivate){
        this->setWindowFlags(this->windowFlags() & (~Qt::WindowStaysOnTopHint));
        this->show();
    }
    return QDialog::event(e);
}

void SettingsDialog::closeEvent(QCloseEvent *e){
    QVariantMap m;
    m["maxFileSize"] = ui->spinBox->value();
    m["ignoreUploadable"] = ui->igUpCheckBox->checkState() == Qt::Checked;
    m["notify"] = ui->notifyCheckBox->checkState() == Qt::Checked;
    m["notifyAnm"] = ui->notifyAnmCheckBox->checkState() == Qt::Checked;
    emit closedWithSettings(m);
    QDialog::closeEvent(e);
}

void SettingsDialog::on_pushButton_3_clicked()
{
    adialog->show();
}

void SettingsDialog::on_pushButton_4_clicked()
{
    QSettings* regSett;
    regSett = new QSettings("Organization-name","Project-name");
    if(regSett->value("KEY","failed").toString().length()==21)
    {
    DownloaderUI UI;
    this->close();
    UI.setWindowFlags(Qt::Window);
    UI.setModal(true);
    UI.exec();
    this->show();
    }
    else QMessageBox::warning(this,"Failed","APIKEY is invalid, do parse a correct APIKEY before using the downloader!");

}

void SettingsDialog::on_pushButton_5_clicked()
{
    webviewDialog2 = new QDialog(this);// an object that is shown in the UI, code below will set the settings of the Dialog box seen
    webviewDialog2->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
    webviewDialog2->setAttribute(Qt::WA_QuitOnClose,false);
    webView2 = new QWebView(webviewDialog2);
    webviewDialog2->layout()->addWidget(webView2);
    webviewDialog2->layout()->setMargin(0);
    webView2->setUrl(QString("https://ivle.nus.edu.sg/LAPI/default.aspx"));
    webviewDialog2->show();
    QSettings settings ("IVLE", "App");
    settings.beginGroup("userinfo");
    settings.setValue("tempUsername", "null");
    settings.setValue("tempPassword", "null");
    settings.endGroup();
    getAPIkey();
}
void SettingsDialog::getAPIkey()
{

    QUrl url("https://ivle.nus.edu.sg/LAPI/default.aspx");//url to obtain the APIKEY
    webView2->setUrl(url);
    //Signals and slot function that ensures that the webpage is loaded finish before executing the parse function
    connect(webView2, SIGNAL(loadFinished(bool)), SLOT(parse(bool)));
}

//Parsing function that uses QSettings,QWebView,QWebFrame and QWebElement to extract the APIKEY from parsing HTML code on the webpage that generates the APIKEY
void SettingsDialog::parse(bool){
    QWebFrame *frameInner = webView2->page()->mainFrame();
    QWebElement doc = frameInner->documentElement();
    QWebElement key = doc.findFirst("b");//function to find the first element in the HTML tag <b>
    APIKEY = key.toPlainText();
    //qDebug()<<APIKEY;
    QSettings* regSett;
    regSett = new QSettings("Organization-name","Project-name");
    regSett->setValue("KEY",APIKEY); //Store value of key defined by user

    qDebug()<<regSett->value("KEY","does not exist");
}
