#==============================================================================
# @author Clinton Freeman <admin@freemancw.com>
# @date 12/17/2013
#==============================================================================

# Project setup ===============================================================

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = rationalcad
TEMPLATE = app

# Source files ================================================================

SOURCES += \
    src/rc_mainwindow.cpp \
    src/rc_main.cpp \
    src/rc_reverse.cpp \
    src/rc_boggle.cpp \
    src/rc_duplicate.cpp \
    src/rc_segments.cpp

# Header files ================================================================

INCLUDEPATH += include/

HEADERS += \
    include/rc_mainwindow.h \
    include/rc_common.h \
    include/rc_reverse.h \
    include/rc_boggle.h \
    include/rc_duplicate.h \
    include/rc_segments.h \
    include/rc_array.h

# Form files ==================================================================

FORMS += \
    forms/rc_mainwindow.ui
