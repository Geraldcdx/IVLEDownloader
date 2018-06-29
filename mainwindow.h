/*
 * Just the main interface
 * By Yao Yujian
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include "settingsdialog.h"
#include "settings.h"
#include "ivlefetcher.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDesktopServices>
#include <QDateTime>
#include "recentfileaction.h"
#include "announcementsmenu.h"
#include "externalpageparser.h"
#include "downloaderui.h"
#include <QtWebKit>
#include <QtWebKitWidgets>
#include <QFileDialog>
#include <QCheckBox>
#include <QtWidgets>
#include <QString>
#include <QVariant>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void processToken(const QString&);
    void updateStatus(fetchingState);
    void updateDirectory(const QString&);
    void logDownload(const QString& filename);
    void updateFiles();
    void createFetcher();
    void updateRecent(const QString& filename);
    void processSettingsDialog(QVariantMap);
    
private slots:
    void on_pushButton_clicked();
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void menuToShow();
    void openFolder();
    void processAnnouncements(QVariantList);
    void ivleLoginPage();
    void getAPIkey();
    void parse(bool);

    QSettings* InitRegSettings();
    void SetMyValue(QString key, QVariant value);
    QVariant GetMyValue(QString key, QVariant defaultValue);

private:
    Ui::MainWindow *ui;
    SettingsDialog* settingsDialog;
    Settings* settings;
    IVLEFetcher* ivlefetcher;
    QSystemTrayIcon* icon;
    QMenu* trayMenu;
    QAction* statusAction;
    QAction* updateAction;
    QFile* logger;
    QIcon normalIcon;
    QIcon attnIcon;
    bool needStart;
    QMenu* recent;
    AnnouncementsMenu* announcements;
    recentFileAction* recents[MAXRECENT];
    ExternalPageParser* parser;
    QWebView* webView;
    QDialog* webviewDialog;
    DownloaderUI* UI;
};

#endif // MAINWINDOW_H
