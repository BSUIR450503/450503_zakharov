#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T18:30:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sniffer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    toolbar.cpp \
    tcp_packet.cpp \
    sniffer.cpp \
    tcpsession.cpp \
    network_interface_chooser.cpp \
    session_model.cpp \
    packet_model.cpp \
    packet_window.cpp

HEADERS  += mainwindow.h \
    toolbar.h \
    structures.h \
    tcp_packet.h \
    sniffer.h \
    tcpsession.h \
    network_interface_chooser.h \
    session_model.h \
    packet_model.h \
    packet_window.h

RESOURCES += \
    icons.qrc
