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
 * @date 01/29/2013
 * @brief OpenGL widget that displays an orthogonal view of the current scene.
 */

#ifndef RCAD_RC_QT_WIDGET_ORTHOGRAPHIC_H
#define RCAD_RC_QT_WIDGET_ORTHOGRAPHIC_H

// Qt
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_3_3_Core>

// RationalCAD
#include "rc_common.h"
#include "rc_integer_grid.h"
#include "rc_manager_scene.h"
#include "rc_manager_gl.h"

class QGLShaderProgram;
class MainWindow;

enum OrthoOrientation {
    BOTTOM,
    TOP,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

class OrthographicWidget : public QGLWidget,
    protected QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    OrthographicWidget(QSharedPointer<RCAD::GLManager> gl_manager,
                       QSharedPointer<RCAD::SceneManager> scene,
                       QWidget* parent = nullptr,
                       const QGLWidget* shareWidget = nullptr,
                       OrthoOrientation orientation = TOP);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void ShowContextMenu(const QPoint& p);

signals:
    void ChangeMessage(const QString& msg);
    void SelectObject(const QVector2D& coords);

    // polytope
    void EndCreatePolytope(bool checked);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    QVector2D mousePressToWorld(QMouseEvent* event) const;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void setupModelview();
    void drawScene();
    void draw2DOverlay();

    static const int kRedrawMsec;
    static const int kMinHintWidth;
    static const int kMinHintHeight;
    static const int kPrefHintWidth;
    static const int kPrefHintHeight;

    QSharedPointer<RCAD::GLManager> gl_manager_;
    QSharedPointer<RCAD::SceneManager> scene_manager_;
    QSharedPointer<QOpenGLShaderProgram> shader_program_;

    QOpenGLVertexArrayObject vao_points_;
    QOpenGLVertexArrayObject vao_lines_;
    QOpenGLVertexArrayObject vao_triangles_;

    QOpenGLBuffer vbo_grid_;
    QOpenGLBuffer ibo_grid_;
    QOpenGLVertexArrayObject vao_grid_;

    quint32 num_frames_;
    RCAD::IntegerGrid i_grid_;
    QTimer timer_;
    QMatrix4x4 modelview_;
    QMatrix4x4 projection_;
    QHash<int, bool> key_states_;
    OrthoOrientation orientation_;
};

#endif // RCAD_RC_QT_WIDGET_ORTHOGRAPHIC_H
