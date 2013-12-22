#==============================================================================
# @author Clinton Freeman <admin@freemancw.com>
# @date 12/17/2013
#==============================================================================

QT       -= core gui

TARGET = mathlib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += mathlib.cpp

HEADERS += mathlib.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
