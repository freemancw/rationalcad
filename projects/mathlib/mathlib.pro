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

QT -= core gui
TARGET = mathlib
TEMPLATE = lib
CONFIG += staticlib
DEFINES += "RLOG_COMPONENT=mathlib"

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

# Source files ================================================================

SOURCES += \
    src/ml_observer.cpp \
    src/ml_line.cpp \
    src/ml_point.cpp \
    src/ml_intersection.cpp \
    src/ml_wedge.cpp \
    src/ml_polygon.cpp \
    src/ml_triangle.cpp \
    src/ml_cone.cpp \
    src/ml_quadrilateral.cpp \
    src/ml_common.cpp \
    src/ml_matrix.cpp \
    src/ml_polytope.cpp

# Header files ================================================================

INCLUDEPATH += include/

HEADERS += \
    include/ml_integer.h \
    include/ml_rational.h \
    include/ml_common.h \
    include/ml_point.h \
    include/ml_vector.h \
    include/ml_matrix.h \
    include/ml_polygon.h \
    include/ml_line.h \
    include/ml_intersection.h \
    include/ml_observer.h \
    include/ml_wedge.h \
    include/ml_triangle.h \
    include/ml_cone.h \
    include/ml_quadrilateral.h \
    include/ml_polytope.h
