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
#include "rc_integer_grid.h"
#include "rc_manager_config.h"

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

OrthographicWidget::OrthographicWidget(QSharedPointer<GLManager> gl_manager,
                                       QSharedPointer<SceneManager> scene_manager,
                                       QWidget *parent,
                                       const QGLWidget *shareWidget,
                                       OrthoOrientation orientation) :
    QGLWidget(parent, shareWidget),
    gl_manager_(gl_manager),
    scene_manager_(scene_manager),
    orientation_(orientation),
    num_frames_(0) {
    setAutoFillBackground(false);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(ShowContextMenu(const QPoint&)));
    connect(this,
            SIGNAL(AddVertexToNewPolygon(QVector2D)),
            scene_manager_.data(),
            SLOT(AddVertexToNewPolygon(QVector2D)));
    connect(this,
            SIGNAL(SetVertexForNew2Cone(QVector2D)),
            scene_manager_.data(),
            SLOT(SetVertexForNew2Cone(QVector2D)));
    connect(this,
            SIGNAL(AddConstraintToNew2Cone(QVector2D)),
            scene_manager_.data(),
            SLOT(AddConstraintToNew2Cone(QVector2D)));
    connect(this,
            SIGNAL(Update2ConeA(QVector2D)),
            scene_manager_.data(),
            SLOT(Update2ConeA(QVector2D)));
    connect(this,
            SIGNAL(Update2ConeB(QVector2D)),
            scene_manager_.data(),
            SLOT(Update2ConeB(QVector2D)));
    connect(this,
            SIGNAL(SelectObject(QVector2D)),
            scene_manager_.data(),
            SLOT(SelectObject(QVector2D)));
}

//=============================================================================
// OpenGL init / draw / resize
//=============================================================================

void OrthographicWidget::initializeGL() {
    rInfo("Initializing OpenGL.");

    initializeOpenGLFunctions();

    shader_program_ = gl_manager_->GetProgram("gl2_default");
    shader_program_->bind();

    gl_manager_->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    gl_manager_->glEnable(GL_CULL_FACE);
    gl_manager_->glEnable(GL_PROGRAM_POINT_SIZE);
    //gl_manager_->glEnable(GL_BLEND);
    //gl_manager_->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    modelview_.setToIdentity();
    shader_program_->setUniformValue("m_modelview", modelview_);

    QList<GLAttributeMeta> ortho_attributes;
    ortho_attributes.push_back(GLVertex::kPositionMeta);
    ortho_attributes.push_back(GLVertex::kMatAmbientMeta);

    QVector<GLVertex> grid_verts;
    QVector<GLushort> grid_idxs;
    i_grid_.InitializeGrid(8, 8, 16, 16, grid_verts, grid_idxs);


    // upload vert data
    glGenBuffers(1, &vbo_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, grid_verts.size() * sizeof(GLVertex),
                 grid_verts.data(), GL_STATIC_DRAW);

    // upload index data
    glGenBuffers(1, &ibo_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid_idxs.size() * sizeof(GLushort),
                 grid_idxs.data(), GL_STATIC_DRAW);

    // clean up (important for QPainter to work correctly)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vao_points_.create();
    vao_points_.bind();
    scene_manager_->BindPointsVBO();
    gl_manager_->EnableAttributes("gl2_default", ortho_attributes);
    vao_points_.release();
    scene_manager_->ReleasePointsVBO();

    vao_lines_.create();
    vao_lines_.bind();
    scene_manager_->BindLinesVBO();
    gl_manager_->EnableAttributes("gl2_default", ortho_attributes);
    vao_lines_.release();
    scene_manager_->ReleaseLinesVBO();

    vao_triangles_.create();
    vao_triangles_.bind();
    scene_manager_->BindTrianglesVBO();
    gl_manager_->EnableAttributes("gl2_default", ortho_attributes);
    vao_triangles_.release();
    scene_manager_->ReleaseTrianglesVBO();

    shader_program_->release();

    timer_.setTimerType(Qt::PreciseTimer);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_.start(kRedrawMsec);
}

void OrthographicWidget::paintEvent(QPaintEvent *event) {
    static QTime frametime = QTime::currentTime();

    Q_UNUSED(event);

    makeCurrent();

    shader_program_->bind();

    gl_manager_->glEnable(GL_DEPTH_TEST);

    gl_manager_->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    i_grid_.Draw();
    setupModelview();
    drawScene();

    shader_program_->release();

    draw2DOverlay();

    ++num_frames_;
}

void OrthographicWidget::setupModelview() {
    modelview_.setToIdentity();

    switch(orientation_) {
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
}

void OrthographicWidget::drawScene() {
    //gl_manager_->glEnable(GL_BLEND);
    //gl_manager_->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vao_points_.bind();
    gl_manager_->DrawPointsVBO();
    vao_points_.release();
    vao_lines_.bind();
    gl_manager_->DrawLinesVBO();
    vao_lines_.release();
    vao_triangles_.bind();
    gl_manager_->DrawTrianglesVBO();
    vao_triangles_.release();
}

void OrthographicWidget::draw2DOverlay() {
    // setup the QPainter for drawing the overlay (e.g. 2D text)
    QPainter painter;
    painter.begin(this);
    painter.setFont(QFont("Segoe UI", 8, QFont::Bold));

    // label x coordinates
    QVector<QPair<int, int>> x_coords = i_grid_.GetMajorXCoords(width());
    for(int i = 0; i < x_coords.size(); ++i) {
        painter.drawText(x_coords.at(i).first+2, 12,
                         QString::number(x_coords.at(i).second));
    }

    // label y coordinates
    QVector<QPair<int, int>> y_coords = i_grid_.GetMajorYCoords(height());
    for(int i = 0; i < y_coords.size(); ++i) {
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

    projection_.setToIdentity();
    projection_.ortho(-halfWidth, halfWidth, -halfHeight, halfHeight,
                      -8192.0f*8, 8192.0f*8);
    shader_program_->setUniformValue("m_projection", projection_);

    shader_program_->release();
}

//=============================================================================
// Input Handlers: Mouse, Keyboard, and Timer Events
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

    if(event->buttons() & Qt::LeftButton) {
        switch(g_config.input_state_) {
        case CREATE_POLYGON:
            emit AddVertexToNewPolygon(world_coords);
            emit ChangeMessage("Right-click to finish polygon creation.");
            return;
        case CREATE_2CONE_VERTEX:
            emit SetVertexForNew2Cone(world_coords);
            emit ChangeMessage("Left-click to place first constraint for polygonal cone.");
            setMouseTracking(true);
            grabKeyboard();
            //emit AddConstraintToNew2Cone(world_coords);
            return;
        case CREATE_2CONE_RAY_A:
            emit AddConstraintToNew2Cone(world_coords);
            emit ChangeMessage("Left-click to place second constraint for polygonal cone.");
            return;
        case CREATE_2CONE_RAY_B:
            emit EndCreate2Cone(false);
            releaseKeyboard();
            setMouseTracking(false);
            return;
        case SELECT:
            if(event->modifiers() & Qt::ShiftModifier) {
                emit SelectObject(world_coords);
            }
        default:
            break;
        }
    }

    if(event->buttons() & Qt::MiddleButton) {
        setCursor(Qt::ClosedHandCursor);
        last_click_pos.setX(event->x());
        last_click_pos.setY(convertedY);
    }

    if(event->buttons() & Qt::RightButton) {
        switch(g_config.input_state_) {
        case CREATE_POLYGON:
            emit EndCreatePolygon(false);
            return;
        case CREATE_2CONE_VERTEX:
        case CREATE_2CONE_RAY_A:
        case CREATE_2CONE_RAY_B:
            emit EndCreate2Cone(false);
            return;
        case SELECT:
        default:
            break;
        }
    }
}

void OrthographicWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(!(event->buttons() & Qt::MiddleButton)) {
        setCursor(Qt::ArrowCursor);
    }
}

void OrthographicWidget::mouseMoveEvent(QMouseEvent *event) {
    int convertedY = height()-event->y();

    QVector2D delta(last_click_pos.x()-event->x(),
                    last_click_pos.y()-convertedY);

    switch(g_config.input_state_) {
    case CREATE_POLYGON:
        break;
    case CREATE_2CONE_RAY_A:
        emit Update2ConeA(mousePressToWorld(event));
        break;
    case CREATE_2CONE_RAY_B:
        emit Update2ConeB(mousePressToWorld(event));
        break;
    default:
        break;
    }

    if(event->buttons() & Qt::LeftButton) {

    }

    if(event->buttons() & Qt::MiddleButton) {
        i_grid_.Translate(delta);
    }

    last_click_pos.setX(event->x());
    last_click_pos.setY(convertedY);
    QCoreApplication::processEvents();
}

void OrthographicWidget::wheelEvent(QWheelEvent *event) {
    if(event->delta() > 0) {
        i_grid_.IncreaseMagnification();   // forward, away from user, zoom in
    } else if(event->delta() < 0) {
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
