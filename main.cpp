#include <QtWidgets>
#include "mainwindow.h"
#include "lapi.h"
#include <QSplashScreen>
#include <QTimer>
#include <QNetworkAccessManager>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Vincent");
    a.setApplicationName("Offline IVLE");
    //CREATES A SPLASH SCREEN
    QSplashScreen *splashed=new QSplashScreen;
    splashed->setPixmap(QPixmap(":/icons/transparentlogo.png"));
    splashed->show();
    QTimer::singleShot(2000,splashed,SLOT(close()));
    MainWindow w;

    // w.show();
    return a.exec();
}
