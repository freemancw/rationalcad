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

// RationalCAD
#include "rc_common.h"
#include "rc_qt_dialog_about.h"
#include "rc_qt_dialog_preferences.h"
#include "rc_qt_window_main.h"
#include "ui_rc_window_main.h"
#include "rc_qt_widget_orthographic.h"
#include "rc_qt_widget_perspective.h"
#include "rc_scene.h"

using namespace RCAD;

//=============================================================================
// Constructors/Destructors
//=============================================================================

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    initializeLogging();

    ConfigManager::get().Initialize();

    // create toolbar buttons
    QActionGroup *input_state_buttons = new QActionGroup(ui->toolBar->layout());

    // select objects button
    QAction* select_objects = new QAction("Select Objects",
                                          input_state_buttons);
    select_objects->setIcon(QIcon("://icons/select_object.png"));
    select_objects->setCheckable(true);

    connect(select_objects,
            SIGNAL(triggered()),
            SLOT(onSelectObjectsTriggered()));

    // create polytope button
    QAction* create_polytope = new QAction("Create Polytope",
                                           input_state_buttons);
    create_polytope->setIcon(QIcon("://icons/create_polytope.png"));
    create_polytope->setCheckable(true);
    create_polytope->setChecked(true);

    connect(create_polytope,
            SIGNAL(triggered()),
            SLOT(onCreatePolytopeTriggered()));

    // snap to grid button
    QAction* snap_to_grid = new QAction("Snap to Grid", ui->toolBar->layout());
    snap_to_grid->setIcon(QIcon("://icons/snap_to_grid.png"));
    snap_to_grid->setCheckable(true);
    snap_to_grid->setChecked(true);

    connect(snap_to_grid,
            SIGNAL(toggled(bool)),
            SLOT(onSnapToGridToggled(bool)));

    // add buttons to toolbar
    ui->toolBar->addAction(select_objects);
    ui->toolBar->addAction(create_polytope);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(snap_to_grid);

    // create orthographic widget
    rInfo("Creating orthographic view.");
    qDebug() << "Creating orthographic view";
    auto ortho_top = new OrthographicWidget(TOP, ui->group_top);
    ui->group_top->layout()->addWidget(ortho_top);
    ortho_top->installEventFilter(this);
    //qDebug() << ortho_top->format();

    // create perspective widget
    rInfo("Creating perspective view.");
    qDebug() << "Creating perspective view";
    auto perspective = new PerspectiveWidget(ui->group_perspective, ortho_top);
    ui->group_perspective->layout()->addWidget(perspective);
    perspective->installEventFilter(this);
    perspective->context()->makeCurrent();
    //qDebug() << perspective->format();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize();
    connect(logger,
            &QOpenGLDebugLogger::messageLogged,
            this,
            &MainWindow::onLogMessage);
    logger->startLogging();

    // create renderer
    rInfo("Creating renderer.");
    qDebug() << "Creating renderer.";
    renderer_ = QSharedPointer<Renderer>(new Renderer());

    // create scene manager
    rInfo("Creating scene manager.");
    qDebug() << "Creating scene manager.";
    scene_manager_ = QSharedPointer<SceneManager>(new SceneManager(renderer_));

    // initialize widgets
    qDebug() << "Initializing ortho.";
    ortho_top->initialize(renderer_, scene_manager_);
    qDebug() << "Initializing perspective.";
    perspective->initialize(renderer_, scene_manager_);

    connect(ortho_top,
            SIGNAL(ChangeMessage(const QString&)),
            SLOT(UpdateStatusBarMsg(const QString&)));
    connect(this,
            SIGNAL(Deselect()),
            &scene_manager_->scene_observer_,
            SLOT(onDeselect()));
    connect(this,
            SIGNAL(DeleteSelectedObject()),
            &scene_manager_->scene_observer_,
            SLOT(onDeleteSelectedObject()));

    QList<int> sizes;
    sizes.push_back(this->height());
    sizes.push_back(0);
    ui->vsplit_console->setSizes(sizes);
}

void MainWindow::onLogMessage(const QOpenGLDebugMessage &message) {
    qDebug() << message.message();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSelectObjectsTriggered() {
    ConfigManager::get().set_input_state(SELECT);
}

void MainWindow::onCreatePolytopeTriggered() {
    ConfigManager::get().set_input_state(CREATE_POLYTOPE);
}

void MainWindow::onSnapToGridToggled(bool state) {
    ConfigManager::get().set_snap_to_grid(state);
}

void MainWindow::initializeLogging() {
    logger_.set_console(ui->console);
    logger_.subscribeTo(rlog::GetGlobalChannel("info"));
    logger_.subscribeTo(rlog::GetGlobalChannel("debug"));
    logger_.subscribeTo(rlog::GetGlobalChannel("error"));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        switch(keyEvent->key()) {
        case Qt::Key_Escape:
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            keyPressEvent(keyEvent);
            return true;
        default:
            break;
        }
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        emit Deselect();
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        emit DeleteSelectedObject();
        break;
    default:
        break;
    }
}

//=============================================================================
// Top Menubar Items
//=============================================================================

void MainWindow::on_action_about_triggered() {
    AboutDialog *a = new AboutDialog(this);
    a->addBuildDate();
    a->show();
}

void MainWindow::on_action_preferences_triggered() {
    PreferencesDialog *p = new PreferencesDialog(this);
    p->show();
}

void MainWindow::UpdateStatusBarMsg(const QString &status) {
    ui->statusbar_main->showMessage(status);
}

void MainWindow::on_actionUser_Manual_triggered() {
    QDesktopServices::openUrl(QUrl("file:///C:/RationalCADUserManual.pdf"));
}
