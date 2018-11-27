#-------------------------------------------------
#
# Project created by QtCreator 2018-11-20T20:36:31
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hairdresser
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    loginwindow.cpp \
    databasetools.cpp \
    managementadd.cpp \
    paychangefare.cpp \
    spendaddspend.cpp \
    managementsearch.cpp \
    spendprintform.cpp \
    poweradduser.cpp \
    managementaddstaff.cpp \
    managementaddvip.cpp \
    managementsearchstaff.cpp \
    managementsearchvip.cpp

HEADERS += \
        mainwindow.h \
    loginwindow.h \
    databasetools.h \
    managementadd.h \
    paychangefare.h \
    spendaddspend.h \
    managementsearch.h \
    spendprintform.h \
    poweradduser.h \
    managementaddstaff.h \
    managementaddvip.h \
    managementsearchstaff.h \
    managementsearchvip.h

FORMS += \
        mainwindow.ui \
    loginwindow.ui \
    managementadd.ui \
    paychangefare.ui \
    spendaddspend.ui \
    managementsearch.ui \
    spendprintform.ui \
    poweradduser.ui \
    managementaddstaff.ui \
    managementaddvip.ui \
    managementsearchstaff.ui \
    managementsearchvip.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    db/db.qrc
