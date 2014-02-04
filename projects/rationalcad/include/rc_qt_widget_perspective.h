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
 * @date 2013-01-29
 * @brief OpenGL widget that allows for navigation with 3D perspective camera.
 */

#ifndef RCAD_RC_QT_WIDGET_PERSPECTIVE_H
#define RCAD_RC_QT_WIDGET_PERSPECTIVE_H

// Qt
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_3_3_Core>

// RationalCAD
#include "rc_common.h"
#include "rc_opengl.h"
#include "rc_scene.h"

class QBasicTimer;
class QOpenGLShaderProgram;

class PerspectiveWidget : public QGLWidget,
    protected QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    PerspectiveWidget(QWidget* parent, const QGLWidget* shareWidget);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void initialize(QSharedPointer<RCAD::ShaderManager> shader_manager,
                    QSharedPointer<RCAD::SceneManager> scene_manager);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void handleInput();
    void drawScene();
    void draw2DOverlay();

    static const int kRedrawMsec;
    static const int kMinHintWidth;
    static const int kMinHintHeight;
    static const int kPrefHintWidth;
    static const int kPrefHintHeight;

    quint32 num_frames_;
    QTimer timer_;
    QHash<int, bool> key_states_;
    QMatrix4x4 modelview_;
    QMatrix4x4 projection_;
    QVector3D camera_pos_;
    QVector3D camera_rot_;
    bool camera_active_;

    QSharedPointer<RCAD::ShaderManager> shader_manager_;
    QSharedPointer<RCAD::SceneManager> scene_manager_;
    QSharedPointer<QOpenGLShaderProgram> shader_program_;

    QList<RCAD::GLAttributeMeta> attributes_;

    QOpenGLVertexArrayObject vao_points_;
    QOpenGLVertexArrayObject vao_lines_;
    QOpenGLVertexArrayObject vao_triangles_;
};

#endif // RCAD_RC_QT_WIDGET_PERSPECTIVE_H
