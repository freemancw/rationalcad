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
    InitializeGlobalConfig();

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

    // create shader manager
    rInfo("Creating shader manager.");
    qDebug() << "Creating shader manager";
    shader_manager_ = QSharedPointer<ShaderManager>(new ShaderManager());

    // create perspective widget
    rInfo("Creating scene manager.");
    qDebug() << "Creating scene manager";
    scene_manager_ = QSharedPointer<SceneManager>(new SceneManager());

    // initialize widgets
    qDebug() << "Initializing ortho.";
    ortho_top->initialize(shader_manager_, scene_manager_);
    qDebug() << "Initializing perspective.";
    perspective->initialize(shader_manager_, scene_manager_);

    connect(ortho_top,
            SIGNAL(ChangeMessage(const QString&)),
            SLOT(UpdateStatusBarMsg(const QString&)));
    connect(ortho_top,
            SIGNAL(EndCreatePolytope(bool)),
            SLOT(on_button_polytope_toggled(bool)));
    connect(this,
            SIGNAL(BeginCreatePolytope(QString,QColor)),
            scene_manager_.data(),
            SLOT(BeginCreatePolytope(QString,QColor)));
    connect(this,
            SIGNAL(EndCreatePolytope()),
            scene_manager_.data(),
            SLOT(EndCreatePolytope()));
    connect(this,
            SIGNAL(Deselect()),
            scene_manager_.data(),
            SLOT(Deselect()));
    connect(this,
            SIGNAL(DeleteSelectedObject()),
            scene_manager_.data(),
            SLOT(DeleteSelectedObject()));

    auto object_color_style = QString("border:0;background-color: rgb(%1, %2, %3);").arg(
                g_config.tag_colors["face_default"].red()).arg(
                g_config.tag_colors["face_default"].green()).arg(
                g_config.tag_colors["face_default"].blue());
    ui->button_color->setStyleSheet(object_color_style);
    create_object_color_ = g_config.tag_colors["face_default"];

    ui->name_and_color->setDisabled(true);
    QPalette palette;
    palette.setColor(QPalette::Disabled, QPalette::WindowText,
                     QApplication::palette().color(QPalette::Disabled,
                                                   QPalette::WindowText));
    ui->name_and_color->setPalette(palette);

    QList<int> sizes;
    sizes.push_back(this->height());
    sizes.push_back(0);
    ui->vsplit_console->setSizes(sizes);
}

MainWindow::~MainWindow() {
    delete ui;
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

void MainWindow::on_action_toggle_snaps_toggled(bool checked) {
    g_config.snap_to_grid_ = checked;
}

void MainWindow::on_button_color_clicked() {
    create_object_color_ = QColorDialog::getColor(create_object_color_, this);
    QString color_style = QString("border:0;background-color: rgb(%1, %2, %3);").arg(
                create_object_color_.red()).arg(
                create_object_color_.green()).arg(
                create_object_color_.blue());
    ui->button_color->setStyleSheet(color_style);
    emit UpdateSelectedObjectColor(create_object_color_);
}

void MainWindow::on_button_polytope_toggled(bool checked) {
    if(checked) {
        ui->statusbar_main->showMessage("Polytope.");
        g_config.input_state_ = CREATE_POLYTOPE;

        ui->name_and_color->setDisabled(false);
        ui->line_edit_name->setText(QString("Object%1").arg(
                                        scene_manager_->NumObjects()));
        emit BeginCreatePolytope(ui->line_edit_name->text(),
                                 create_object_color_);
    } else {
        switch(g_config.input_state_) {
        case CREATE_POLYTOPE:
            ui->statusbar_main->clearMessage();
            g_config.input_state_ = SELECT;
            emit EndCreatePolytope();
            //ui->button_polygonal_cone->setChecked(false);
            break;
        default:
            break;
        }
    }
}
