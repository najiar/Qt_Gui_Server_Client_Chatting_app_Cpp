QT       += core gui network sql widgets multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


QTPLUGIN += QSQLMYSQL

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    chatwindow.cpp \
    databaseclass.cpp \
    friendspage.cpp \
    hometab.cpp \
    main.cpp \
    mainwindow.cpp \
    register.cpp \
    chatWindow.cpp \
    zingermail.cpp



HEADERS +=  \
#    chatwindow.h \
    databaseclass.h \
    friendspage.h \
    hometab.h \
    mainwindow.h \
    register.h \
    chatWindow.h \
    zingermail.h


FORMS += \
    friendspage.ui \
    hometab.ui \
    mainwindow.ui \
    register.ui \
    chatWindow.ui

INCLUDEPATH += \Images

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
