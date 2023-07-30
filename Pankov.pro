QT       += core gui sql printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    autosystem.cpp \
    ci.cpp \
    core.cpp \
    initialform.cpp \
    main.cpp \
    ntime.cpp \
    workdatabase.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    analyserci.cpp \
    drilldownchart.cpp \
    drilldownslice.cpp

HEADERS += \
    autosystem.h \
    cellsY.h \
    ci.h \
    core.h \
    initialform.h \
    ntime.h \
    workdatabase.h \
    mainwindow.h \
    qcustomplot.h \
    analyserci.h \
    drilldownchart.h \
    drilldownslice.h

FORMS += \
    autosystem.ui \
    initialform.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
