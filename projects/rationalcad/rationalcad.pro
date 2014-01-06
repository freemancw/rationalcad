#==============================================================================
#
#  This file is part of RationalCAD.
#
#  RationalCAD is free software: you can redistribute it and/or modify it under
#  the terms of the GNU General Public License as published by the Free
#  Software Foundation, either version 3 of the License, or (at your option)
#  any later version.
#
#  RationalCAD is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
#  more details. You should have received a copy of the GNU General Public
#  License along with RationalCAD. If not, see <http://www.gnu.org/licenses/>.
#
#==============================================================================

# @author Clinton Freeman <admin@freemancw.com>
# @date 01/04/2014

# Project setup ===============================================================

QT += core gui opengl widgets
TARGET = rationalcad
TEMPLATE = app

# Dependency: loglib ==========================================================

INCLUDEPATH += $$PWD/../loglib/include

win32 {
    Debug {
        LIBS += -L$$OUT_PWD/../loglib/debug/ -lloglib
        DEPENDPATH += $$OUT_PWD/../loglib/debug/
        PRE_TARGETDEPS += $$OUT_PWD/../loglib/debug/loglib.lib
    }
    Release {
        LIBS += -L$$OUT_PWD/../loglib/release/ -lloglib
        DEPENDPATH += $$OUT_PWD/../loglib/release/
        PRE_TARGETDEPS += $$OUT_PWD/../loglib/release/loglib.lib
    }
}

# Dependency: mpir ============================================================

win32 {
    Debug {
        INCLUDEPATH += $$PWD/../depend/mpir/lib/Win32/Debug
        DEPENDPATH += $$PWD/../depend/mpir/lib/Win32/Debug
        LIBS += -L$$PWD/../depend/mpir/lib/Win32/Debug/ -lmpir
        LIBS += -L$$PWD/../depend/mpir/lib/Win32/Debug/ -lmpirxx
        PRE_TARGETDEPS += $$PWD/../depend/mpir/lib/Win32/Debug/mpir.lib
        PRE_TARGETDEPS += $$PWD/../depend/mpir/lib/Win32/Debug/mpirxx.lib
    }
    Release {
        INCLUDEPATH += $$PWD/../depend/mpir/lib/Win32/Release
        DEPENDPATH += $$PWD/../depend/mpir/lib/Win32/Release
        LIBS += -L$$PWD/../depend/mpir/lib/Win32/Release/ -lmpir
        LIBS += -L$$PWD/../depend/mpir/lib/Win32/Release/ -lmpirxx
        PRE_TARGETDEPS += $$PWD/../depend/mpir/lib/Win32/Release/mpir.lib
        PRE_TARGETDEPS += $$PWD/../depend/mpir/lib/Win32/Release/mpirxx.lib
    }
}

# Dependency: mathlib =========================================================

INCLUDEPATH += $$PWD/../mathlib/include

win32 {
    Debug {
        LIBS += -L$$OUT_PWD/../mathlib/debug/ -lmathlib
        DEPENDPATH += $$OUT_PWD/../mathlib/debug/
        PRE_TARGETDEPS += $$OUT_PWD/../mathlib/debug/mathlib.lib
    }
    Release {
        LIBS += -L$$OUT_PWD/../mathlib/release/ -lmathlib
        DEPENDPATH += $$OUT_PWD/../mathlib/release/
        PRE_TARGETDEPS += $$OUT_PWD/../mathlib/release/mathlib.lib
    }
}

# Source files ================================================================

SOURCES += \
    src/rc_main.cpp \
    src/rc_integer_grid.cpp \
    src/rc_logger.cpp \
    src/rc_manager_config.cpp \
    src/rc_manager_gl.cpp \
    src/rc_manager_scene.cpp \
    src/rc_qt_dialog_about.cpp \
    src/rc_qt_dialog_preferences.cpp \
    src/rc_qt_widget_orthographic.cpp \
    src/rc_qt_widget_perspective.cpp \
    src/rc_qt_window_main.cpp

# Header files ================================================================

INCLUDEPATH += include/

HEADERS += \
    include/rc_common.h \
    include/rc_array.h \
    include/rc_integer_grid.h \
    include/rc_logger.h \
    include/rc_manager_config.h \
    include/rc_manager_gl.h \
    include/rc_manager_scene.h \
    include/rc_qt_dialog_about.h \
    include/rc_qt_dialog_preferences.h \
    include/rc_qt_widget_perspective.h \
    include/rc_qt_widget_orthographic.h \
    include/rc_qt_window_main.h

# Misc files ==================================================================

FORMS += \
    forms/rc_window_main.ui \
    forms/rc_dialog_about.ui \
    forms/rc_dialog_preferences.ui

RESOURCES += \
    resources/resources.qrc

win32 {
    RC_FILE = resources/icon.rc
}
