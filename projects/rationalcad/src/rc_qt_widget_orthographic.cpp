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
 */

// RationalCAD
#include "rc_common.h"
#include "rc_qt_window_main.h"
#include "rc_qt_widget_orthographic.h"
#include "rc_grid.h"
#include "rc_config.h"

// mathlib
#include "ml_point.h"
#include "ml_vector.h"

using namespace RCAD;

const int OrthographicWidget::kRedrawMsec = 16;
const int OrthographicWidget::kMinHintWidth = 64;
const int OrthographicWidget::kMinHintHeight = 64;
const int OrthographicWidget::kPrefHintWidth = 512;
const int OrthographicWidget::kPrefHintHeight = 384;

//=============================================================================
// Constructors / Destructors
//=============================================================================

OrthographicWidget::OrthographicWidget(OrthoOrientation orientation,
                                       QWidget *parent,
                                       const QGLWidget *shareWidget) :
    QGLWidget(parent, shareWidget),
    orientation_(orientation),
    num_frames_(0) {}

//=============================================================================
// Initialization
//=============================================================================

void OrthographicWidget::initialize(
    QSharedPointer<ShaderManager> shader_manager,
    QSharedPointer<SceneManager> scene_manager) {
    shader_manager_ = shader_manager;
    scene_manager_ = scene_manager;

    setAutoFillBackground(false);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(ShowContextMenu(const QPoint&)));
    connect(this,
            SIGNAL(SelectObject(QVector2D)),
            scene_manager_.data(),
            SLOT(SelectObject(QVector2D)));
}

void OrthographicWidget::initializeGL() {
    qDebug() << "Initializing orthographic OpenGL.";

    initializeOpenGLFunctions();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    shader_program_ = shader_manager_->getProgram("gl2_default");
    shader_program_->bind();

    modelview_.setToIdentity();
    shader_program_->setUniformValue("m_modelview", modelview_);

    attributes_.push_back(GL::Vertex::kPositionMeta);
    attributes_.push_back(GL::Vertex::kMatAmbientMeta);

    QVector<GL::Vertex> grid_verts;
    //! @todo magic numbers
    i_grid_.InitializeGrid(8, 8, 16, 16, grid_verts);
    i_grid_vbo_.UploadVertices(grid_verts);
    i_grid_vao_.create();
    i_grid_vao_.bind();
    i_grid_vbo_.buffer.bind();
    GL::EnableAttributes(shader_program_, attributes_);
    i_grid_vao_.release();
    i_grid_vbo_.buffer.release();

    vao_points_.create();
    vao_points_.bind();
    scene_manager_->points_vbo().buffer.bind();
    GL::EnableAttributes(shader_program_, attributes_);
    vao_points_.release();
    scene_manager_->points_vbo().buffer.release();

    vao_lines_.create();
    vao_lines_.bind();
    scene_manager_->lines_vbo().buffer.bind();
    GL::EnableAttributes(shader_program_, attributes_);
    vao_lines_.release();
    scene_manager_->lines_vbo().buffer.release();

    vao_triangles_.create();
    vao_triangles_.bind();
    scene_manager_->triangles_vbo().buffer.bind();
    GL::EnableAttributes(shader_program_, attributes_);
    vao_triangles_.release();
    scene_manager_->triangles_vbo().buffer.release();

    shader_program_->release();

    timer_.setTimerType(Qt::PreciseTimer);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_.start(kRedrawMsec);
}

//=============================================================================
// Rendering
//=============================================================================

void OrthographicWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    makeCurrent();

    shader_program_->bind();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawGrid();
    drawScene();

    shader_program_->release();

    draw2DOverlay();
}

void OrthographicWidget::drawGrid() {
    // setup modelview
    QMatrix4x4 mv;
    mv.scale(i_grid_.local_scale());
    mv.translate(-i_grid_.local_pos().x(), -i_grid_.local_pos().y());
    shader_program_->setUniformValue("m_modelview", mv);

    // issue render calls
    i_grid_vao_.bind();
    glDrawArrays(GL_LINES, 0, i_grid_vbo_.num_vertices);
    i_grid_vao_.release();
}

void OrthographicWidget::drawScene() {
    // setup modelview
    modelview_.setToIdentity();

    switch (orientation_) {
    case RIGHT:
        modelview_.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
        modelview_.rotate(-90.0f, 0.0f, 1.0f, 0.0f);
        break;
    case FRONT:
        modelview_.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
    default:
        break;
    }

    QVector3D position(-i_grid_.global_pos().x(),
                       -i_grid_.global_pos().y(), 0.0f);

    modelview_.scale(i_grid_.global_scale());
    modelview_.translate(position);

    shader_program_->setUniformValue("m_modelview", modelview_);

    // restore gl state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // issue render calls
    vao_points_.bind();
    glDrawArrays(GL_POINTS, 0, scene_manager_->points_vbo().num_vertices);
    vao_points_.release();
    vao_lines_.bind();
    glDrawArrays(GL_LINES, 0, scene_manager_->lines_vbo().num_vertices);
    vao_lines_.release();
    vao_triangles_.bind();
    glDrawArrays(GL_TRIANGLES, 0, scene_manager_->triangles_vbo().num_vertices);
    vao_triangles_.release();
}

void OrthographicWidget::draw2DOverlay() {
    // setup the QPainter for drawing the overlay (e.g. 2D text)
    QPainter painter;
    painter.begin(this);
    painter.setFont(QFont("Segoe UI", 8, QFont::Bold));

    // label x coordinates
    QVector<QPair<int, int>> x_coords = i_grid_.GetMajorXCoords(width());
    for (int i = 0; i < x_coords.size(); ++i) {
        painter.drawText(x_coords.at(i).first+2, 12,
                         QString::number(x_coords.at(i).second));
    }

    // label y coordinates
    QVector<QPair<int, int>> y_coords = i_grid_.GetMajorYCoords(height());
    for (int i = 0; i < y_coords.size(); ++i) {
        int convertedY = height()-y_coords.at(i).first;
        painter.drawText(6, convertedY-2,
                         QString::number(y_coords.at(i).second));
    }

    // clean up
    painter.end();
}

void OrthographicWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    shader_program_->bind();

    float halfWidth  = static_cast<float>(width)/2.0f;
    float halfHeight = static_cast<float>(height)/2.0f;

    //! @todo magic numbers
    projection_.setToIdentity();
    projection_.ortho(-halfWidth, halfWidth, -halfHeight, halfHeight,
                      -8192.0f*8, 8192.0f*8);
    shader_program_->setUniformValue("m_projection", projection_);

    shader_program_->release();
}

//=============================================================================
// Input handlers: mouse, keyboard, and timer events
//=============================================================================

void OrthographicWidget::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    update();
}

static QPoint last_click_pos;

QVector2D OrthographicWidget::mousePressToWorld(QMouseEvent* event) const {
    int converted_y = height()-event->y();
    qreal halfwidth = static_cast<qreal>(width())/2;
    qreal halfheight = static_cast<qreal>(height())/2;
    QVector2D screen_coords(event->x()-halfwidth, converted_y-halfheight);
    return i_grid_.GetCoordsForPixel(screen_coords);
}

void OrthographicWidget::mousePressEvent(QMouseEvent *event) {
    int convertedY = height()-event->y();
    QVector2D world_coords = mousePressToWorld(event);

    if (event->buttons() & Qt::LeftButton) {
        switch (g_config.input_state_) {
        case SELECT:
            if(event->modifiers() & Qt::ShiftModifier) {
                emit SelectObject(world_coords);
            }
            break;
        case CREATE_POLYTOPE:
            emit BeginCreatePolytope();
            break;
        default:
            break;
        }
    }

    if (event->buttons() & Qt::MiddleButton) {
        setCursor(Qt::ClosedHandCursor);
        last_click_pos.setX(event->x());
        last_click_pos.setY(convertedY);
    }

    if (event->buttons() & Qt::RightButton) {
        switch (g_config.input_state_) {
        case SELECT:
        default:
            break;
        }
    }
}

void OrthographicWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::MiddleButton)) {
        setCursor(Qt::ArrowCursor);
    }
}

void OrthographicWidget::mouseMoveEvent(QMouseEvent *event) {
    int convertedY = height()-event->y();

    QVector2D delta(last_click_pos.x()-event->x(),
                    last_click_pos.y()-convertedY);

    switch (g_config.input_state_) {
    default:
        break;
    }

    if (event->buttons() & Qt::LeftButton) {

    }

    if (event->buttons() & Qt::MiddleButton) {
        i_grid_.Translate(delta);
    }

    last_click_pos.setX(event->x());
    last_click_pos.setY(convertedY);
    QCoreApplication::processEvents();
}

void OrthographicWidget::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0) {
        i_grid_.IncreaseMagnification();   // forward, away from user, zoom in
    } else if (event->delta() < 0) {
        i_grid_.DecreaseMagnification();   // backward, toward user, zoom out
    }
}

void OrthographicWidget::ShowContextMenu(const QPoint &p) {
    /*
    if(g_config.input_state_ != SELECT)  {
        return;
    }

    QPoint gp = mapToGlobal(p);

    QMenu ctxt_menu;
    ctxt_menu.addAction("Import mesh here...");

    QAction* selected_item = ctxt_menu.exec(gp);
    if(selected_item) {
        QString selected_file = QFileDialog::getOpenFileName(
                    this, "Import Mesh", "./",
                    "3ds Max 3DS (*.3ds);;"
                    "ASCII Scene Export (*.ase);;"
                    "Collada (*.dae);;"
                    "Wavefront Object (*.obj)");
        //Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(
                    selected_file.toStdString(),
                    aiProcess_Triangulate            |
                    aiProcess_JoinIdenticalVertices);

        if(!scene) {
            rDebug("error");
        }
    } else {

    }
    */
}

void OrthographicWidget::keyPressEvent(QKeyEvent *event) {
    key_states_[event->key()] = true;
}

void OrthographicWidget::keyReleaseEvent(QKeyEvent *event) {
    key_states_[event->key()] = false;
}

//=============================================================================
// Misc overrides / OpenGL support routines
//=============================================================================

QSize OrthographicWidget::minimumSizeHint() const {
    return QSize(kMinHintWidth, kMinHintHeight);
}

QSize OrthographicWidget::sizeHint() const {
    return QSize(kPrefHintWidth, kPrefHintHeight);
}
