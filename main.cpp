#include <QtWidgets>
#include "mainwindow.h"
#include "lapi.h"
#include <QSplashScreen>
#include <QTimer>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setOrganizationName("YYJHAO");
    a.setApplicationName("IVLE Downloader");

    QSplashScreen *splashed=new QSplashScreen;
    splashed->setPixmap(QPixmap(":/icons/transparentlogo.png"));
    splashed->show();
    QTimer::singleShot(2000,splashed,SLOT(close()));
    MainWindow w;

    // w.show();
    return a.exec();
}
