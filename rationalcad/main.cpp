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

// Qt
#include <QApplication>
#include <QSplashScreen>
#include <QtDebug>

// RCAD
#include "common.h"
#include "qt_window_main.h"

#define _ELPP_THREAD_SAFE
_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {

    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);

    LOG(INFO) << "===========================================================";

    QApplication a(argc, argv);

    // Qt5 provides a nice default GUI stylesheet called Fusion
    //QApplication::setStyle(QStyleFactory::create("Fusion"));

    // register any types that will pass through Qt's signal/slot mechanism
    LOG(INFO) << "registering qt meta types";
    qRegisterMetaType<QVector<RCAD::GL::Vertex>>("QVector<RCAD::GL::Vertex>");
    qRegisterMetaType<QVector<QVector3D>>("QVector<QVector3D>");
    qRegisterMetaType<QVector<qint32>>("QVector<qint32>");

    LOG(INFO) << "displaying splash screen";
    QPixmap splash_image(":/splash3.png");
    QSplashScreen splash(splash_image);
    splash.show();
    //splash.showMessage("Initializing...");
    a.processEvents();

    LOG(INFO) << "displaying main window";
    MainWindow w;
    w.showMaximized();
    splash.finish(&w);

    LOG(INFO) << "beginning main event loop";
    return a.exec();
}
