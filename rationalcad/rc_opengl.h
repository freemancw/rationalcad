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
 * @brief Manager type responsible for altering OpenGL states and data.
 */

#ifndef RC_OPENGL_H
#define RC_OPENGL_H

// Qt
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>

// RationalCAD
#include "rc_common.h"

// mathlib
#include "ml_point.h"
#include "ml_vector.h"

namespace RCAD {

namespace GL {

//=============================================================================
// Interface: AttributeMeta
//=============================================================================

struct AttributeMeta {
    AttributeMeta();
    AttributeMeta(const int count, const GLenum type, const int offset,
                  const char* name);

    int count;
    GLenum type;
    int offset;
    const char* name;
};

//=============================================================================
// Interface: Vertex
//=============================================================================

class Vertex {
public:
    static const AttributeMeta kPositionMeta;
    static const AttributeMeta kTangentMeta;
    static const AttributeMeta kNormalMeta;
    static const AttributeMeta kBitangentMeta;
    static const AttributeMeta kUvcoordsMeta;
    static const AttributeMeta kMatAmbientMeta;
    static const GLsizei kStride;

    Vertex();
    Vertex(const Point_3f& position, const Vector_3f& tangent = Vector_3f(),
           const Vector_3f& normal = Vector_3f(),
           const Vector_3f& bitangent = Vector_3f(),
           const Point_2f& uvcoords = Point_2f(),
           const QColor& mat_ambient = Qt::black);
    Vertex(const Point_3f& position, const QColor& mat_ambient);
    Vertex(const Point_3f& position,const Vector_3f& normal);
    Vertex(const Point_3f& position, const Vector_3f& normal,
           const QColor& mat_ambient);
    Vertex(const Point_3f& position, const Point_2f& uvcoords);

    void set_position(const Point_3f& position);
    void set_tangent(const Vector_3f& tangent);
    void set_normal(const Vector_3f& normal);
    void set_bitangent(const Vector_3f& bitangent);
    void set_uvcoords(const Point_2f& uvcoords);
    void set_mat_ambient(const QColor& mat_ambient);
    void set_mat_ambient(const Visual::Color& mat_ambient);

private:
    std::array<GLfloat, 3> position_;
    std::array<GLfloat, 3> tangent_;
    std::array<GLfloat, 3> normal_;
    std::array<GLfloat, 3> bitangent_;
    std::array<GLfloat, 2> uvcoords_;
    std::array<GLfloat, 4> mat_ambient_;
};

typedef Vertex Point;

struct Line {
    Line() {}
    Line(const Vertex& a, const Vertex& b) :
        a(a),
        b(b) {}
    Vertex a;
    Vertex b;
};

struct Triangle {
    Triangle() {}
    Triangle(const Vertex& a, const Vertex& b, const Vertex& c) :
        a(a),
        b(b),
        c(c) {}
    Vertex a;
    Vertex b;
    Vertex c;
};

struct TriangleFan {
    QVector<Vertex> vertices;
};

struct VertexBuffer {
    VertexBuffer();

    void UploadVertices(const QVector<Vertex>& vertices);

    QOpenGLBuffer buffer;
    int num_vertices;
};

struct ElementArray {
    ElementArray();
    ElementArray(const QString& tag, const GLenum mode, const GLsizei count,
                 const GLenum type, GLvoid* const indices);

    QString tag;
    GLenum mode;       // e.g. GL_TRIANGLES
    GLsizei count;     // number of indices
    GLenum type;       // e.g. GL_FLOAT
    GLvoid* indices;   // offset pointer from currently bound IBO
};

void EnableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                      const QList<AttributeMeta>& attributes);

void DisableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                       const QList<AttributeMeta>& attributes);

} // namespace GL

Q_DECLARE_METATYPE(QVector<GL::Vertex>)

//=============================================================================
// Interface: ShaderManager
//=============================================================================

class ShaderManager : public QOpenGLFunctions_3_3_Core {
public:
    ShaderManager();

    bool addProgram(const QString& id,
                    const QString& vert_path,
                    const QString& frag_path);

    QSharedPointer<QOpenGLShaderProgram> getProgram(const QString& id);

private:
    QHash<QString, QSharedPointer<QOpenGLShaderProgram>> programs_;
};

} // namespace RCAD

#endif // RC_OPENGL_H