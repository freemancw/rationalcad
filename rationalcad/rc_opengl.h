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
    /* QOpenGLVertexArrayObject vao */
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

namespace Primitive {
    enum Type {
        POINTS,
        LINES,
        TRIANGLES,
        MAX
    };
}

enum PrimitiveType {
    PRIM_POINTS,
    PRIM_LINES,
    PRIM_TRIANGLES,
    NUM_PRIMITIVES
};

void EnableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                      const QList<AttributeMeta>& attributes);

void DisableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                       const QList<AttributeMeta>& attributes);

class VertexCache {
public:
    VertexCache(){}

    void Initialize(QOpenGLShaderProgram& program,
                    const QVector<AttributeMeta>& attributes) {
        UploadVertices(QVector<Vertex>());
        vao_.create();
        vao_.bind();
        vbo_.bind();
        foreach (AttributeMeta attr_meta, attributes) {
            program.enableAttributeArray(attr_meta.name);
            program.setAttributeBuffer(
                attr_meta.name,
                attr_meta.type,
                attr_meta.offset,
                attr_meta.count,
                Vertex::kStride
            );
        }
        vbo_.release();
        vao_.release();
    }

    void UploadVertices(const QVector<Vertex>& vertices) {
        if (!vbo_.isCreated()) {
            vbo_.create();
            vbo_.bind();
            vbo_.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        }
        vbo_.bind();
        vbo_.allocate(vertices.data(), vertices.size()*sizeof(Vertex));
        vbo_.release();

        num_vertices_ = vertices.size();
    }

    void Bind() {
        vao_.bind();
    }

    void Release() {
        vao_.release();
    }

    quint32 num_vertices() const {
        return num_vertices_;
    }

private:
    QOpenGLBuffer vbo_;
    QOpenGLVertexArrayObject vao_;
    quint32 num_vertices_;
};

class RenderGroup {
public:
    RenderGroup(){}

    void Initialize(const QString& vpath, const QString& fpath,
                    const QVector<AttributeMeta>& attributes) {
        bool v = program_.addShaderFromSourceFile(
            QOpenGLShader::Vertex, vpath
        );
        bool f = program_.addShaderFromSourceFile(
            QOpenGLShader::Fragment, fpath
        );
        bool l = program_.link();

        if (v && f && l) {
            qDebug() << "rendergroup successfully compiled.";
        } else {
            qDebug() << "rendergroup failed";
        }

        program_.bind();
        for (int i = 0; i < NUM_PRIMITIVES; ++i) {
            vertex_cache_[i].Initialize(program_, attributes);
        }
        program_.release();
    }

    void Bind(const quint32 ptype) {
        program_.bind();
        vertex_cache_[ptype].Bind();
    }

    void Release(const quint32 ptype) {
        vertex_cache_[ptype].Release();
        program_.release();
    }

    void UploadVertices(const quint32 ptype, const QVector<Vertex>& vertices) {
        program_.bind();
        vertex_cache_[ptype].UploadVertices(vertices);
        program_.release();
    }

    quint32 NumVertices(const quint32 ptype) {
        return vertex_cache_[ptype].num_vertices();
    }

//private:
    QOpenGLShaderProgram program_;
    QVector<AttributeMeta> vertex_attributes_;
    VertexCache vertex_cache_[NUM_PRIMITIVES];
};

} // namespace GL

Q_DECLARE_METATYPE(QVector<GL::Vertex>)

//=============================================================================
// Interface: ShaderManager
//=============================================================================

class Renderer : public QOpenGLFunctions_3_3_Core {
public:
    Renderer() {
        initializeOpenGLFunctions();

        QVector<GL::AttributeMeta> attributes;
        attributes.push_back(GL::Vertex::kPositionMeta);
        attributes.push_back(GL::Vertex::kMatAmbientMeta);
        render_groups_[Visual::MC_OPAQUE][Visual::ML_UNLIT].Initialize(
            ":shaders/mat_unlit_opaque.vsh",
            ":shaders/mat_unlit_opaque.fsh",
            attributes
        );
        /*
        render_groups_[Visual::MC_TRANSPARENT][Visual::ML_UNLIT].Initialize(
            ":shaders/mat_unlit_transparent.vsh",
            ":shaders/mat_unlit_transparent.fsh",
            attributes
        );
        attributes.push_back(GL::Vertex::kNormalMeta);
        render_groups_[Visual::MC_OPAQUE][Visual::ML_FLAT].Initialize(
            ":shaders/mat_flat_opaque.vsh",
            ":shaders/mat_flat_opaque.fsh",
            attributes
        );
        render_groups_[Visual::MC_TRANSPARENT][Visual::ML_FLAT].Initialize(
            ":shaders/mat_flat_transparent.vsh",
            ":shaders/mat_flat_transparent.fsh",
            attributes
        );
        */
    }

    void UpdateRenderGroup(const quint32 coverage_idx,
                           const quint32 lighting_idx,
                           const quint32 primtype_idx,
                           const QVector<GL::Vertex>& vertices) {
        render_groups_[coverage_idx][lighting_idx].UploadVertices(
            primtype_idx,
            vertices
        );
    }

    /*
    GL::RenderGroup& GetRenderGroup(const quint32 coverage,
                                    const quint32 lighting);
                                    */

//private:
    GL::RenderGroup render_groups_[Visual::MC_MAX][Visual::ML_MAX];
};

} // namespace RCAD

#endif // RC_OPENGL_H
