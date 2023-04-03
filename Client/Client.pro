QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HelperFunctions.cpp \
    addfriend.cpp \
    deleteuser.cpp \
    frienddescription.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    messenger.cpp \
    post.cpp

HEADERS += \
    HelperFunctions.h \
    addfriend.h \
    deleteuser.h \
    frienddescription.h \
    mainwindow.h \
    message.h \
    messenger.h \
    post.h

FORMS += \
    addfriend.ui \
    deleteuser.ui \
    frienddescription.ui \
    mainwindow.ui \
    message.ui \
    post.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
