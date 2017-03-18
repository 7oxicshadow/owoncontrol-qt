#-------------------------------------------------
#
# Project created by QtCreator 2017-02-21T20:08:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = owoncontrol-qt
TEMPLATE = app


SOURCES += main.cpp\
        owoncontrol.cpp \
    usb_interface.cpp \
    owon_commands.cpp \
    warning.cpp

HEADERS  += owoncontrol.h \
    usb_interface.h \
    main.h \
    owon_commands.h \
    warning.h

FORMS    += owoncontrol.ui \
    warning.ui

LIBS     += -lusb-1.0
