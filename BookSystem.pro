QT       += sql widgets

#CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TEMPLATE = app
INCLUDEPATH += .
requires(qtConfig(tableview))

SOURCES += \
    addrecordinfo.cpp \
    bookinfo.cpp \
    find.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_user.cpp \
    recordinfo.cpp \
    stuinfo.cpp \
    updaterecordinfo.cpp

HEADERS += \
    addrecordinfo.h \
    bookinfo.h \
    find.h \
    initdb.h \
    login.h \
    mainwindow.h \
    mainwindow_user.h \
    recordinfo.h \
    stuinfo.h \
    updaterecordinfo.h

FORMS += \
    addrecordinfo.ui \
    bookinfo.ui \
    find.ui \
    login.ui \
    mainwindow.ui \
    mainwindow_user.ui \
    recordinfo.ui \
    stuinfo.ui \
    updaterecordinfo.ui

#TRANSLATIONS += \
#    BookSystem_zh_CN.ts

# Default rules for deployment.
target.path = /tmp/$${TARGET}/bin
INSTALLS += target
