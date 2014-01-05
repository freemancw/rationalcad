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
 * \brief OpenGL widget that allows for navigation with 3D perspective camera.
 * \author {Clinton Freeman <freeman@cs.unc.edu>}
 * \date 01/29/2013
 */

#ifndef DDAD_RC_QT_WIDGET_PERSPECTIVE_H
#define DDAD_RC_QT_WIDGET_PERSPECTIVE_H

// Qt
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_3_3_Core>

// RationalCAD
#include "rc_common.h"
#include "rc_manager_gl.h"
#include "rc_manager_scene.h"

class QBasicTimer;
class QOpenGLShaderProgram;

class PerspectiveWidget : public QGLWidget,
        protected QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    PerspectiveWidget(QSharedPointer<DDAD::GLManager> gl_manager,
                      QSharedPointer<DDAD::SceneManager> scene,
                      QWidget* parent = nullptr,
                      const QGLWidget* shareWidget = nullptr);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

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

    QSharedPointer<DDAD::GLManager> gl_manager_;
    QSharedPointer<DDAD::SceneManager> scene_manager_;
    QSharedPointer<QOpenGLShaderProgram> shader_program_;

    QOpenGLVertexArrayObject vao_points_;
    QOpenGLVertexArrayObject vao_lines_;
    QOpenGLVertexArrayObject vao_triangles_;
};

#endif // DDAD_VISUALIZER_QT_WIDGET_OPENGL_3_H
