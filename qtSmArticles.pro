#-------------------------------------------------
#
# Project created by QtCreator 2016-04-29T16:12:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtSmArticles
TEMPLATE = app


SOURCES += main.cpp\
        qtsmarticles.cpp \
    qdownloader.cpp

HEADERS  += qtsmarticles.h \
    qdownloader.h \
    keypresseater.h

FORMS    += qtsmarticles.ui

DISTFILES += \
    resources/r_e_s_icon.png \
    resources/r_e_s_icon.ico

RC_ICONS = resources/r_e_s_icon.ico
