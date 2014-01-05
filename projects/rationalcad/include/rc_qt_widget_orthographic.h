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
 * \brief OpenGL widget that displays an orthogonal view of the current scene.
 * \author {Clinton Freeman <freeman@cs.unc.edu>}
 * \date 01/29/2013
 */

#ifndef DDAD_RC_QT_WIDGET_ORTHOGRAPHIC_H
#define DDAD_RC_QT_WIDGET_ORTHOGRAPHIC_H

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

class OrthographicWidget :
        public QGLWidget,
        protected QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    //! \todo eventually need additional constructor that does not attempt
    //!       to create a new GL context for subsequent orthowidgets
    OrthographicWidget(QSharedPointer<DDAD::GLManager> gl_manager,
                       QSharedPointer<DDAD::SceneManager> scene,
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
    // polygon
    void EndCreatePolygon(bool checked);
    void AddVertexToNewPolygon(const QVector2D& coords);
    // polytope
    void EndCreatePolytope(bool checked);
    // cone_2
    void EndCreate2Cone(bool checked);
    void SetVertexForNew2Cone(const QVector2D& coords);
    void AddConstraintToNew2Cone(const QVector2D& coords);
    void Update2ConeA(const QVector2D& coords);
    void Update2ConeB(const QVector2D& coords);
    // tetrahedron
    //void EndCreateTetrahedron(bool checked);
    //void AddVertexToNewTetrahedron(const QVector2D& coords);

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

    QSharedPointer<DDAD::GLManager> gl_manager_;
    QSharedPointer<DDAD::SceneManager> scene_manager_;
    QSharedPointer<QOpenGLShaderProgram> shader_program_;

    QOpenGLVertexArrayObject vao_points_;
    QOpenGLVertexArrayObject vao_lines_;
    QOpenGLVertexArrayObject vao_triangles_;

    quint32 num_frames_;
    DDAD::IntegerGrid i_grid_;
    QTimer timer_;
    QMatrix4x4 modelview_;
    QMatrix4x4 projection_;
    QHash<int, bool> key_states_;
    OrthoOrientation orientation_;
};

#endif // DDAD_RC_QT_WIDGET_ORTHOGRAPHIC_H
