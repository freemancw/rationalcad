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
 * @date 2013-05-23
 * @brief Main window responsible for forwarding widget input.
 */

#ifndef RCAD_RC_QT_WINDOW_MAIN_H
#define RCAD_RC_QT_WINDOW_MAIN_H

// Qt
#include <QMainWindow>

// RationalCAD
#include "rc_common.h"
#include "rc_scene.h"
#include "rc_logger.h"

BEGIN_NAMESPACE(Ui)

class MainWindow;

END_NAMESPACE(Ui)

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    // polytope
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
    void UpdateStatusBarMsg(const QString& status);

private:
    void initializeLogging();
    void initializeConnections();

    Ui::MainWindow *ui;

    QSharedPointer<RCAD::SceneManager> scene_manager_;
    QSharedPointer<RCAD::ShaderManager> shader_manager_;

    RCAD::RCLogger logger_;

    QColor create_object_color_;
};

#endif // RCAD_RC_QT_WINDOW_MAIN_H
