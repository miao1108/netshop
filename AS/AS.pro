QT       += core gui
QT       +=network
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DES.cpp \
    MD5.cpp \
    RSA.cpp \
    main.cpp \
    mainwindow.cpp \
    mysocket.cpp \
    mytcpserver.cpp \
    network.cpp \
    serverthread.cpp

HEADERS += \
    DES.h \
    MD5.h \
    RSA.h \
    mainwindow.h \
    mysocket.h \
    mytcpserver.h \
    network.h \
    serverthread.h

FORMS += \
    mainwindow.ui

LIBS += -llibgmp-10
win32: LIBS += -L$$PWD/ -llibgmp-10
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
