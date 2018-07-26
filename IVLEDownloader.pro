#-------------------------------------------------
#
# Project created by QtCreator 2012-08-24T21:28:43
#
#-------------------------------------------------

QT       += core gui sql\
            widgets\
            webkitwidgets\
            network
CONFIG +=c++11 app_bundle
macx{
    QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu++0x -stdlib=libc++
}

TARGET = IVLEDownloader
TEMPLATE = app


DEFINES +=  QT_DEPRECATED_WARNINGS\
            MAXRECENT=5

SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    ivlefetcher.cpp \
    settings.cpp \
    downloader.cpp \
    recentfileaction.cpp \
    announcementsmenu.cpp \
    announcementaction.cpp \
    advanceddialog.cpp \
    lapi.cpp \
    promise.cpp \
    externalpageparser.cpp \
    globalvar.cpp \
    downloaderui.cpp \
    qtjson.cpp \
    form.cpp \
    dialog.cpp \
    additem.cpp \
    item.cpp \
    parsing.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    ivlefetcher.h \
    settings.h \
    downloader.h \
    recentfileaction.h \
    announcementsmenu.h \
    announcementaction.h \
    advanceddialog.h \
    lapi.h \
    promise.h \
    externalpageparser.h \
    globalvar.h \
    downloaderui.h \
    qtjson.h \
    form.h \
    dialog.h \
    additem.h \
    item.h \
    parsing.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    advanceddialog.ui \
    downloaderui.ui \
    form.ui \
    dialog.ui \
    additem.ui \
    item.ui

RESOURCES += \
    res.qrc
