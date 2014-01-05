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
 * @date 06/03/2013
 * @brief Manager type responsible for altering OpenGL states and data.
 */

#ifndef RC_MANAGER_GL_H
#define RC_MANAGER_GL_H

// Qt
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>

// RationalCAD
#include "rc_common.h"

// mathlib
#include "ml_point.h"
#include "ml_vector.h"

BEGIN_NAMESPACE(RCAD)

//=============================================================================
// Interfaces
//=============================================================================

struct GLAttributeMeta {
    GLAttributeMeta();
    GLAttributeMeta(const int count, const GLenum type, const int offset,
                    const char* name);

    int count;
    GLenum type;
    int offset;
    const char* name;
};

class GLVertex {
public:
    static const GLAttributeMeta kPositionMeta;
    static const GLAttributeMeta kTangentMeta;
    static const GLAttributeMeta kNormalMeta;
    static const GLAttributeMeta kBitangentMeta;
    static const GLAttributeMeta kUvcoordsMeta;
    static const GLAttributeMeta kMatAmbientMeta;
    static const GLsizei kStride;

    GLVertex();
    GLVertex(const Point_3f& position, const Vector_3f& tangent = Vector_3f(),
             const Vector_3f& normal = Vector_3f(),
             const Vector_3f& bitangent = Vector_3f(),
             const Point_2f& uvcoords = Point_2f(),
             const QColor& mat_ambient = Qt::black);
    GLVertex(const Point_3f& position, const QColor& mat_ambient);
    GLVertex(const Point_3f& position,const Vector_3f& normal);
    GLVertex(const Point_3f& position, const Vector_3f& normal,
             const QColor& mat_ambient);
    GLVertex(const Point_3f& position, const Point_2f& uvcoords);

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

typedef GLVertex GLPoint;

struct GLLine {
    GLLine() {}
    GLLine(const GLVertex& a, const GLVertex& b) :
        a(a),
        b(b) {}
    GLVertex a;
    GLVertex b;
};

struct GLTriangle {
    GLTriangle() {}
    GLTriangle(const GLVertex& a, const GLVertex& b, const GLVertex& c) :
        a(a),
        b(b),
        c(c) {}
    GLVertex a;
    GLVertex b;
    GLVertex c;
};

struct GLTriangleFan {
    QVector<GLVertex> vertices;
};

struct GLVertexBuffer {
    GLVertexBuffer();

    void UploadVertices(const QVector<GLVertex>& vertices);

    QOpenGLBuffer buffer;
    int num_vertices;
};

struct GLElementArray {
    GLElementArray();
    GLElementArray(const QString& tag, const GLenum mode, const GLsizei count,
                   const GLenum type, GLvoid* const indices);

    QString tag;
    GLenum mode;       // e.g. GL_TRIANGLES
    GLsizei count;     // number of indices
    GLenum type;       // e.g. GL_FLOAT
    GLvoid* indices;   // offset pointer from currently bound IBO
};

class GLManager : public QObject, public QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    GLManager();

    void Initialize();

    bool AddProgram(const QString& id, const QString& vert_path,
                    const QString& frag_path);
    void EnableAttributes(const QString& id,
                          const QList<GLAttributeMeta>& attributes);
    void DisableAttributes(const QString& id,
                           const QList<GLAttributeMeta>& attributes);

    void DrawPoints(const QString& id,
                    const QList<GLAttributeMeta>& attributes);
    void DrawLines(const QString& id,
                   const QList<GLAttributeMeta>& attributes);
    void DrawTriangles(const QString& id,
                       const QList<GLAttributeMeta>& attributes);

    void BindPointsVBO() { vbo_points_.buffer.bind(); }
    void BindLinesVBO() { vbo_lines_.buffer.bind(); }
    void BindTrianglesVBO() { vbo_triangles_.buffer.bind(); }
    void ReleasePointsVBO() { vbo_points_.buffer.release(); }
    void ReleaseLinesVBO() { vbo_lines_.buffer.release(); }
    void ReleaseTrianglesVBO() { vbo_triangles_.buffer.release(); }
    void DrawPointsVBO() { glDrawArrays(GL_POINTS, 0, vbo_points_.num_vertices); }
    void DrawLinesVBO() { glDrawArrays(GL_LINES, 0, vbo_lines_.num_vertices); }
    void DrawTrianglesVBO() { glDrawArrays(GL_TRIANGLES, 0, vbo_triangles_.num_vertices); }

    QSharedPointer<QOpenGLShaderProgram> GetProgram(const QString& id);

public slots:
    void UpdateVboPoints(QVector<GLVertex> verts);
    void UpdateVboLines(QVector<GLVertex> verts);
    void UpdateVboTriangles(QVector<GLVertex> verts);
    void LogDebugMessage(const QOpenGLDebugMessage& message);

private:
    QHash<QString, QSharedPointer<QOpenGLShaderProgram>> programs_;

    GLVertexBuffer vbo_points_;
    GLVertexBuffer vbo_lines_;
    GLVertexBuffer vbo_triangles_;

    QOpenGLDebugLogger gl_logger_;
};

 Q_DECLARE_METATYPE(QVector<GLVertex>)

END__NAMESPACE(RCAD)

#endif // RC_GL_ABSTRACTION_H
