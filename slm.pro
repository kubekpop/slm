#-------------------------------------------------
#
# Project created by QtCreator 2017-10-07T00:05:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slm
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


SOURCES += \
        main.cpp \
        main_widget.cpp \
    dhcp_window.cpp \
    ftp_window.cpp \
    settings_window.cpp \
    firewall_window.cpp \
    apache_window.cpp \
    nfs_window.cpp \
    raid_window.cpp \
    status_window.cpp \
    backup_window.cpp \
    ssh_connect_window.cpp \
    samba_window.cpp \
    dns_window.cpp \
    qemu_window.cpp

HEADERS += \
        main_widget.h \
    dhcp_window.h \
    ftp_window.h \
    settings_window.h \
    firewall_window.h \
    apache_window.h \
    nfs_window.h \
    raid_window.h \
    status_window.h \
    backup_window.h \
    ssh_connect_window.h \
    samba_window.h \
    dns_window.h \
    qemu_window.h

FORMS += \
        main_widget.ui \
    dhcp_window.ui \
    ftp_window.ui \
    settings_window.ui \
    firewall_window.ui \
    apache_window.ui \
    nfs_window.ui \
    raid_window.ui \
    status_window.ui \
    backup_window.ui \
    ssh_connect_window.ui \
    samba_window.ui \
    dns_window.ui \
    qemu_window.ui

RESOURCES += \
    resources.qrc
