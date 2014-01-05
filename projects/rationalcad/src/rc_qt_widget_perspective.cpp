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
#include "rc_qt_widget_perspective.h"
#include "rc_manager_scene.h"

using namespace DDAD;

const int PerspectiveWidget::kRedrawMsec = 16;
const int PerspectiveWidget::kMinHintWidth = 64;
const int PerspectiveWidget::kMinHintHeight = 64;
const int PerspectiveWidget::kPrefHintWidth = 256;
const int PerspectiveWidget::kPrefHintHeight = 256;

//=============================================================================
// Constructors / Destructors
//=============================================================================

PerspectiveWidget::PerspectiveWidget(QSharedPointer<GLManager> gl_manager,
                                     QSharedPointer<SceneManager> scene_manager,
                                     QWidget *parent,
                                     const QGLWidget *shareWidget) :
    QGLWidget(parent, shareWidget),
    gl_manager_(gl_manager),
    scene_manager_(scene_manager),
    camera_active_(false),
    num_frames_(0) {
    setAutoFillBackground(false);
}

//=============================================================================
// OpenGL init / draw / resize
//=============================================================================

void PerspectiveWidget::initializeGL() {
    rDebug("Initializing OpenGL.");

    initializeOpenGLFunctions();

    shader_program_ = gl_manager_->GetProgram("gl3_default");

    gl_manager_->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    gl_manager_->glDisable(GL_CULL_FACE);
    gl_manager_->glEnable(GL_MULTISAMPLE);
    gl_manager_->glEnable(GL_PROGRAM_POINT_SIZE);
    gl_manager_->glEnable(GL_DEPTH_BUFFER_BIT);
    gl_manager_->glEnable(GL_DEPTH_TEST);

    shader_program_->bind();

    modelview_.setToIdentity();
    shader_program_->setUniformValue("m_modelview", modelview_);
    camera_pos_ = QVector3D(6, -2, 2);
    camera_rot_.setZ(-45.0f);
    camera_rot_.setX(22.5f);

    QList<GLAttributeMeta> persp_attributes;
    persp_attributes.push_back(GLVertex::kPositionMeta);
    persp_attributes.push_back(GLVertex::kNormalMeta);
    persp_attributes.push_back(GLVertex::kMatAmbientMeta);

    vao_points_.create();
    vao_points_.bind();
    gl_manager_->BindPointsVBO();
    gl_manager_->EnableAttributes("gl3_default", persp_attributes);
    vao_points_.release();
    gl_manager_->ReleasePointsVBO();

    vao_lines_.create();
    vao_lines_.bind();
    gl_manager_->BindLinesVBO();
    gl_manager_->EnableAttributes("gl3_default", persp_attributes);
    vao_lines_.release();
    gl_manager_->ReleaseLinesVBO();

    vao_triangles_.create();
    vao_triangles_.bind();
    gl_manager_->BindTrianglesVBO();
    gl_manager_->EnableAttributes("gl3_default", persp_attributes);
    vao_triangles_.release();
    gl_manager_->ReleaseTrianglesVBO();

    shader_program_->release();

    timer_.setTimerType(Qt::PreciseTimer);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_.start(kRedrawMsec);
}

void PerspectiveWidget::paintEvent(QPaintEvent *event) {
    static QTime frametime = QTime::currentTime();

    Q_UNUSED(event);

    makeCurrent();

    shader_program_->bind();

    gl_manager_->glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handleInput();

    modelview_.setToIdentity();
    modelview_.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
    modelview_.rotate(camera_rot_.x(), 1.0f, 0.0f, 0.0f);
    modelview_.rotate(camera_rot_.y(), 0.0f, 1.0f, 0.0f);
    modelview_.rotate(camera_rot_.z(), 0.0f, 0.0f, 1.0f);
    modelview_.translate(-camera_pos_);
    shader_program_->setUniformValue("m_modelview", modelview_);

    drawScene();

    shader_program_->release();


    // setup the QPainter for drawing the overlay (e.g. 2D text)
    QPainter painter;
    painter.begin(this);
    painter.setPen(Qt::white);

    if(camera_active_) {
        painter.setFont(QFont("Segoe UI", 16, QFont::Normal));
        painter.drawText(width()/2-8, height()/2+4, "+");
    }

    int fps = qRound(static_cast<float>(num_frames_)/frametime.elapsed()*1000);

    painter.setFont(QFont("Segoe UI", 10, QFont::Normal));
    painter.drawText(width()-20, 16, QString("%1").arg(fps));

    // clean up
    painter.end();

    ++num_frames_;
}

static const float pi180 = M_PI/180.0f;
void PerspectiveWidget::handleInput() {
    if(camera_active_) {
        float sinx = sin(camera_rot_.x()*pi180);
        float cosx = cos(camera_rot_.x()*pi180);
        float sinz = sin(-camera_rot_.z()*pi180);
        float cosz = cos(-camera_rot_.z()*pi180);

        float zplus90 = -camera_rot_.z()+90.0f;
        float sinz90 = sin(zplus90*pi180);
        float cosz90 = cos(zplus90*pi180);

        QVector3D forward(sinz*cosx, -cosz*cosx, sinx);
        forward.normalize();
        QVector3D right(sinz90*cosx, -cosz90*cosx, 0);
        right.normalize();

        if(key_states_[Qt::Key_W])
            camera_pos_ -= forward;
        if(key_states_[Qt::Key_A])
            camera_pos_ -= right;
        if(key_states_[Qt::Key_S])
            camera_pos_ += forward;
        if(key_states_[Qt::Key_D])
            camera_pos_ += right;
        if(key_states_[Qt::Key_Space])
            camera_pos_ += QVector3D(0.0f, 0.0f, 0.5f);
        if(key_states_[Qt::Key_C])
            camera_pos_ -= QVector3D(0.0f, 0.0f, 0.5f);
    }
}

void PerspectiveWidget::drawScene() {
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

void PerspectiveWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    shader_program_->bind();

    projection_.setToIdentity();
    projection_.perspective(80.0f, (float)width/height, 0.125f, 1024.0f);
    shader_program_->setUniformValue("m_projection", projection_);

    shader_program_->release();
}

//=============================================================================
// Input Handlers: Mouse, Keyboard, and Timer Events
//=============================================================================

static QPoint last_click_pos;

void PerspectiveWidget::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    update();
}

void PerspectiveWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() & Qt::RightButton)
        camera_active_ = !camera_active_;

    if(camera_active_) {
        setCursor(Qt::BlankCursor);
        setMouseTracking(true);
        grabKeyboard();
        last_click_pos = mapToGlobal(event->pos());
    } else {
        setCursor(Qt::ArrowCursor);
        setMouseTracking(false);
        releaseKeyboard();
    }
}

void PerspectiveWidget::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
}

void PerspectiveWidget::mouseMoveEvent(QMouseEvent *event) {
    if(camera_active_) {
        QPoint delta = last_click_pos - mapToGlobal(event->pos());
        QCursor::setPos(last_click_pos);

        camera_rot_.setZ(camera_rot_.z()-static_cast<float>(delta.x()) / 2.0f);
        camera_rot_.setX(camera_rot_.x()-static_cast<float>(delta.y()) / 2.0f);

        // clamp up/down view angle
        camera_rot_.setX(qMax(qMin(camera_rot_.x(), 89.99f), -89.99f));

        // if we get a stream of mousemove events they will block repainting,
        // so make sure other GUI events get processed
        QCoreApplication::processEvents();
    }
}

void PerspectiveWidget::wheelEvent(QWheelEvent *event) {
    Q_UNUSED(event);
}

void PerspectiveWidget::keyPressEvent(QKeyEvent *event) {
    key_states_[event->key()] = true;
}

void PerspectiveWidget::keyReleaseEvent(QKeyEvent *event) {
    key_states_[event->key()] = false;
}

//=============================================================================
// Misc overrides / OpenGL support routines
//=============================================================================

QSize PerspectiveWidget::minimumSizeHint() const {
    return QSize(kMinHintWidth, kMinHintHeight);
}

QSize PerspectiveWidget::sizeHint() const {
    return QSize(kPrefHintWidth, kPrefHintHeight);
}
