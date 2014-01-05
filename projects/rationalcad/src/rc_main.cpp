/*
 * This file is part of RationalCAD.
 *
 * RationalCAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * RationalCAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with RationalCAD. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @author Clinton Freeman <admin@freemancw.com>
 * @date 05/23/2012
 */

// Qt
#include <QApplication>
#include <QSplashScreen>
#include <QtDebug>

// RationalCAD
#include "rc_common.h"
#include "rc_qt_window_main.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    rlog::RLogInit(argc, argv);

    // Qt5 provides a nice default GUI stylesheet called Fusion
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // register any types that will pass through Qt's signal/slot mechanism
    qRegisterMetaType<QVector<DDAD::GLVertex>>("QVector<DDAD::GLVertex>");

    QPixmap splash_image(":/splash3.png");
    QSplashScreen splash(splash_image);
    splash.show();
    //splash.showMessage("Initializing...");
    a.processEvents();

    while(true);
    MainWindow w;
    w.showMaximized();
    splash.finish(&w);

    return a.exec();
}
