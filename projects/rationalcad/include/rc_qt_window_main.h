/*
 * This file is part of the Degree-Driven Algorithm Design Project (DDAD).
 *
 * DDAD is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \brief Main window responsible for forwarding widget input.
 * \author {Clinton Freeman <freeman@cs.unc.edu>}
 * \date 05/23/2012
 */

#ifndef DDAD_RC_QT_WINDOW_MAIN_H
#define DDAD_RC_QT_WINDOW_MAIN_H

// Qt
#include <QMainWindow>

// RationalCAD
#include "rc_common.h"
#include "rc_manager_scene.h"
#include "rc_logger.h"

namespace DDAD {

class WorkerThread : public QThread { void run() { exec(); } };

}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    // polygon
    void BeginCreatePolygon(const QString& name, const QColor& face_color);
    void EndCreatePolygon();
    void ComputeIntegerHull();

    // cone_2
    void BeginCreate2Cone(const QString& name, const QColor& face_color);
    void EndCreate2Cone();
    void ComputeCIGP();


    void BeginCreatePolytope(const QString& name, const QColor& face_color);
    void EndCreatePolytope();

    // all objects
    void UpdateSelectedObjectName(const QString& name);
    void UpdateSelectedObjectColor(const QColor& color);
    void DeleteSelectedObject();
    void Deselect();
    void QuitManager();

private slots:
    void on_action_about_triggered();
    void on_action_preferences_triggered();
    void on_action_toggle_snaps_toggled(bool arg1);
    void on_button_color_clicked();
    void on_compute_integer_hull_clicked();

    void UpdateStatusBarMsg(const QString& status);

    void on_compute_interior_grid_pt_clicked();
    void on_button_polygon_toggled(bool checked);
    void on_button_polytope_toggled(bool checked);
    void on_button_polygonal_cone_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    QSharedPointer<DDAD::SceneManager> scene_manager_;
    QSharedPointer<DDAD::GLManager> gl_manager_;
    QSharedPointer<QThread> scene_manager_thread_;

    DDAD::RCLogger logger_;

    QColor create_object_color_;
};

#endif // DDAD_RC_QT_WINDOW_MAIN_H
