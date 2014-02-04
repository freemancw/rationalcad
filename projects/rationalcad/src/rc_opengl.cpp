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
 * @date 2013-06-03
 */

// RationalCAD
#include "rc_common.h"
#include "rc_opengl.h"

namespace RCAD {

//=============================================================================
// ShaderManager
//=============================================================================

ShaderManager::ShaderManager() {
    initializeOpenGLFunctions();

    // override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    bool shaders_valid =
               addProgram("gl2_default",
                          ":/shaders/gl2_default.vsh",
                          ":/shaders/gl2_default.fsh")
            && addProgram("gl3_default",
                          ":/shaders/gl3_default.vsh",
                          ":/shaders/gl3_default.fsh");

    if (!shaders_valid) {
        qDebug() << "Shaders did not compile.";
    }

    // restore system locale
    setlocale(LC_ALL, "");
}

bool ShaderManager::addProgram(const QString& id, const QString& vert_path,
                               const QString &frag_path) {
    QSharedPointer<QOpenGLShaderProgram> sp(new QOpenGLShaderProgram());
    bool v = sp->addShaderFromSourceFile(QOpenGLShader::Vertex, vert_path);
    bool f = sp->addShaderFromSourceFile(QOpenGLShader::Fragment, frag_path);
    bool l = sp->link();

    if (v && f && l) {
        programs_.insert(id, sp);
        return true;
    } else {
        qDebug() << id;
        return false;
    }
}

QSharedPointer<QOpenGLShaderProgram> ShaderManager::getProgram(
    const QString& id) {
    return programs_.value(id);
}

//=============================================================================
// OpenGL abstraction layer
//=============================================================================

namespace GL {

void EnableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                      const QList<GLAttributeMeta>& attributes) {
    foreach (GLAttributeMeta attr_meta, attributes) {
        program->enableAttributeArray(attr_meta.name);
        program->setAttributeBuffer(attr_meta.name,
                                    attr_meta.type, attr_meta.offset,
                                    attr_meta.count, GLVertex::kStride);
    }
}

void DisableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                       const QList<GLAttributeMeta>& attributes) {
    foreach (GLAttributeMeta attr_meta, attributes) {
        program->disableAttributeArray(attr_meta.name);
    }
}

} // namespace GL

//=============================================================================
// GLAttributeMeta
//=============================================================================

GLAttributeMeta::GLAttributeMeta() {}

GLAttributeMeta::GLAttributeMeta(const int count, const GLenum type,
                                 const int offset, const char* name) :
    count(count),
    type(type),
    offset(offset),
    name(name) {}

//=============================================================================
// GLVertex
//=============================================================================

// static vertex attribute definitions

const GLAttributeMeta GLVertex::kPositionMeta(3, GL_FLOAT,
                        offsetof(GLVertex, position_), "v_position");
const GLAttributeMeta GLVertex::kTangentMeta(3, GL_FLOAT,
                        offsetof(GLVertex, tangent_), "v_tangent");
const GLAttributeMeta GLVertex::kNormalMeta(3, GL_FLOAT,
                        offsetof(GLVertex, normal_), "v_normal");
const GLAttributeMeta GLVertex::kBitangentMeta(3, GL_FLOAT,
                        offsetof(GLVertex, bitangent_), "v_bitangent");
const GLAttributeMeta GLVertex::kUvcoordsMeta(2, GL_FLOAT,
                        offsetof(GLVertex, uvcoords_), "v_multi_tex_coord_0");
const GLAttributeMeta GLVertex::kMatAmbientMeta(4, GL_FLOAT,
                        offsetof(GLVertex, mat_ambient_), "v_mat_ambient");
const GLsizei GLVertex::kStride = static_cast<GLsizei>(sizeof(GLVertex));

// constructors

GLVertex::GLVertex() {}

GLVertex::GLVertex(const Point_3f &position, const Point_2f &uvcoords) {
    set_position(position);
    set_uvcoords(uvcoords);
}

GLVertex::GLVertex(const Point_3f &position, const QColor &mat_ambient) {
    set_position(position);
    set_mat_ambient(mat_ambient);
}

GLVertex::GLVertex(const Point_3f &position, const Vector_3f &normal) {
    set_position(position);
    set_normal(normal);
}

GLVertex::GLVertex(const Point_3f &position, const Vector_3f &normal,
                   const QColor &mat_ambient) {
    set_position(position);
    set_normal(normal);
    set_mat_ambient(mat_ambient);
}

GLVertex::GLVertex(const Point_3f &position, const Vector_3f &tangent,
                   const Vector_3f &normal, const Vector_3f &bitangent,
                   const Point_2f &uvcoords, const QColor &mat_ambient) {
    set_position(position);
    set_tangent(tangent);
    set_normal(normal);
    set_bitangent(bitangent);
    set_uvcoords(uvcoords);
    set_mat_ambient(mat_ambient);
}

// setters

void GLVertex::set_position(const Point_3f &position) {
    position_ = position.elements();
}
void GLVertex::set_tangent(const Vector_3f &tangent) {
    tangent_ = tangent.elements();
}
void GLVertex::set_normal(const Vector_3f &normal) {
    normal_ = normal.elements();
}
void GLVertex::set_bitangent(const Vector_3f &bitangent) {
    bitangent_ = bitangent.elements();
}
void GLVertex::set_uvcoords(const Point_2f &uvcoords) {
    uvcoords_ = uvcoords.elements();
}
void GLVertex::set_mat_ambient(const QColor &mat_ambient) {
    mat_ambient_[0] = mat_ambient.redF();
    mat_ambient_[1] = mat_ambient.greenF();
    mat_ambient_[2] = mat_ambient.blueF();
    mat_ambient_[3] = mat_ambient.alphaF();
}
void GLVertex::set_mat_ambient(const Visual::Color &mat_ambient) {
    mat_ambient_[0] = static_cast<GLfloat>(mat_ambient.r())/255.0f;
    mat_ambient_[1] = static_cast<GLfloat>(mat_ambient.g())/255.0f;
    mat_ambient_[2] = static_cast<GLfloat>(mat_ambient.b())/255.0f;
    mat_ambient_[3] = static_cast<GLfloat>(mat_ambient.a())/255.0f;
}

//=============================================================================
// GLVertexBuffer
//=============================================================================

GLVertexBuffer::GLVertexBuffer() :
    num_vertices(0) {}

void GLVertexBuffer::UploadVertices(const QVector<GLVertex> &vertices) {
    if(!buffer.isCreated()) {
        buffer.create();
        buffer.bind();
        buffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    }
    buffer.bind();
    buffer.allocate(vertices.data(), vertices.size()*sizeof(GLVertex));
    buffer.release();

    num_vertices = vertices.size();
}

//=============================================================================
// GLElementArray
//=============================================================================

GLElementArray::GLElementArray() {}

GLElementArray::GLElementArray(const QString &tag, const GLenum mode,
                               const GLsizei count, const GLenum type,
                               GLvoid * const indices) :
    tag(tag),
    mode(mode),
    count(count),
    type(type),
    indices(indices) {}

} // namespace RCAD
