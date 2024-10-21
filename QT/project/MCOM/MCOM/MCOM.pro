QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += serialport
QT += printsupport
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    debugwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    ./mc_protocol/mc_frame.cpp \
    ./mc_protocol/mc_protocol.cpp \
    serialwindow.cpp \
    comment.cpp \
    qcustomplot.cpp \
    processthread.cpp
HEADERS += \
    commands.h \
    debugwindow.h \
    mainwindow.h \
    serialwindow.h \
    comment.h \
    ./mc_protocol/mc_frame.h \
    ./mc_protocol/mc_protocol.h \
    ./mc_protocol/commands.h \
    qcustomplot.h \
    processthread.h

FORMS += \
    debugwindow.ui \
    mainwindow.ui \
    serialwindow.ui
# QMAKE_CFLAGS += -Wa,-mbig-obj
# QMAKE_CXXFLAGS += -Wa,-mbig-obj
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
