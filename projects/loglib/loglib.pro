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
TARGET = loglib
TEMPLATE = lib
CONFIG += staticlib

# Source files ================================================================

SOURCES += \
    src/testlog.cpp \
    src/test.cpp \
    src/StdioNode.cpp \
    src/RLogTime.cpp \
    src/RLogPublisher.cpp \
    src/RLogNode.cpp \
    src/rloglocation.cpp \
    src/rloginit.cpp \
    src/RLogChannel.cpp \
    src/rlog.cpp \
    src/Error.cpp

# Header files ================================================================

INCLUDEPATH += include/

HEADERS += \
    include/StdioNode.h \
    include/RLogTime.h \
    include/RLogPublisher.h \
    include/rlog-prec99.h \
    include/rlog-novariadic.h \
    include/RLogNode.h \
    include/rloglocation.h \
    include/rloginit.h \
    include/RLogChannel.h \
    include/rlog-c99.h \
    include/rlog.h \
    include/Mutex.h \
    include/Lock.h \
    include/Error.h \
    include/common.h
