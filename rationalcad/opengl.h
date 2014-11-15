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

#ifndef RC_OPENGL_H
#define RC_OPENGL_H

// Qt
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>

// RationalCAD
#include "common.h"

// kernel
#include "../geometry/point.h"
#include "../geometry/vector.h"
#include "../geometry/visual.h"

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
        E_POINTS,
        E_LINES,
        E_TRIANGLES,
        E_MAX
    };
}

namespace Context {
    enum Name {
        E_ORTHOGRAPHIC,
        E_PERSPECTIVE,
        E_MAX
    };
}

void EnableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                      const QList<AttributeMeta>& attributes);

void DisableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                       const QList<AttributeMeta>& attributes);

//=============================================================================
// Interface: VertexCache
//=============================================================================

class VertexCache {
public:
    VertexCache() :
        num_vertices_(0) {}

    void Initialize(QOpenGLShaderProgram& program,
                    const QVector<AttributeMeta>& attributes) {
        vao_.create();
        vao_.bind();
        vbo_ptr_->bind();
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
        vbo_ptr_->release();
        vao_.release();
    }

    void BindSettings() {
        vao_.bind();
    }

    void ReleaseSettings() {
        vao_.release();
    }

    void UploadVertices(const QVector<Vertex>& vertices) {
        if (!vbo_ptr_->isCreated()) {
            vbo_ptr_->create();
            vbo_ptr_->bind();
            vbo_ptr_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
        }
        vbo_ptr_->bind();
        vbo_ptr_->allocate(vertices.data(), vertices.size()*sizeof(Vertex));
        vbo_ptr_->release();

        num_vertices_ = vertices.size();
    }

    quint32 num_vertices() const {
        return num_vertices_;
    }

    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer* vbo_ptr_;
    quint32 num_vertices_;
};

//=============================================================================
// Interface: RenderGroup
//=============================================================================

class RenderGroup {
public:
    RenderGroup(){}

    void Initialize(const QString& vpath, const QString& fpath,
                    const QVector<AttributeMeta>& attributes) {
        vertex_attributes_ = attributes;
        vpath_ = vpath;
        fpath_ = fpath;
        for (int i = 0; i < Primitive::E_MAX; ++i) {
            vertex_cache_[i].UploadVertices(QVector<Vertex>());
        }
        bool v = program_.addShaderFromSourceFile(
            QOpenGLShader::Vertex, vpath_
        );
        bool f = program_.addShaderFromSourceFile(
            QOpenGLShader::Fragment, fpath_
        );
        bool l = program_.link();

        if (v && f && l) {
            qDebug() << "rendergroup successfully compiled.";
        } else {
            qDebug() << "rendergroup failed";
        }

        program_.bind();
        for (int i = 0; i < Primitive::E_MAX; ++i) {
            vertex_cache_[i].Initialize(program_, vertex_attributes_);
        }
        program_.release();
    }

    void BindPrimitive(GL::Primitive::Type ptype) {
        program_.bind();
        vertex_cache_[ptype].BindSettings();
    }

    void ReleasePrimitive(GL::Primitive::Type ptype) {
        vertex_cache_[ptype].ReleaseSettings();
        program_.release();
    }

    /*
    void UploadVertices(GL::Primitive::Type ptype,
                        const QVector<Vertex>& vertices) {
        vertex_cache_[ptype].UploadVertices(vertices);
    }
    */

    quint32 NumVertices(GL::Primitive::Type ptype) {
        return vertex_cache_[ptype].num_vertices();
    }

    QString vpath_;
    QString fpath_;
    QOpenGLShaderProgram program_;
    QVector<AttributeMeta> vertex_attributes_;
    VertexCache vertex_cache_[Primitive::E_MAX];
};

} // namespace GL

Q_DECLARE_METATYPE(QVector<GL::Vertex>)

//=============================================================================
// Interface: Renderer
//=============================================================================

class Renderer : public QOpenGLFunctions_3_3_Core {
public:
    Renderer() {
        // why isn't this per-context?
        initializeOpenGLFunctions();
    }

    ~Renderer() {
        qDebug() << "renderer delete!";
    }

    void InitContext(GL::Context::Name cname) {
        switch (cname) {
        case GL::Context::E_ORTHOGRAPHIC:
            qDebug() << "Renderer: initializing orthographic settings.";
            break;
        case GL::Context::E_PERSPECTIVE:
            qDebug() << "Renderer: initializing perspective settings.";
            break;
        }

        QVector<GL::AttributeMeta> attributes;
        attributes.push_back(GL::Vertex::kPositionMeta);
        attributes.push_back(GL::Vertex::kMatAmbientMeta);

        render_groups_[Visual::Coverage::E_OPAQUE]
                      [Visual::Lighting::E_UNLIT]
                      [cname].Initialize(":shaders/mat_unlit_opaque.vsh",
                                         ":shaders/mat_unlit_opaque.fsh",
                                         attributes);

        render_groups_[Visual::Coverage::E_TRANSPARENT]
                      [Visual::Lighting::E_UNLIT]
                      [cname].Inititialize(":shaders/mat_unlit_transparent.vsh",
                                           ":shaders/mat_unlit_transparent.fsh",
                                           attributes);

        attributes.push_back(GL::Vertex::kNormalMeta);

        render_groups_[Visual::Coverage::E_OPAQUE]
                      [Visual::Lighting::E_FLAT]
                      [cname].Initialize(":shaders/mat_flat_opaque.vsh",
                                         ":shaders/mat_flat_opaque.fsh",
                                         attributes);

        render_groups_[Visual::Coverage::E_TRANSPARENT]
                      [Visual::Lighting::E_FLAT]
                      [cname].Initialize(":shaders/mat_flat_transparent.vsh",
                                         ":shaders/mat_flat_transparent.fsh",
                                         attributes);
    }

    void BindContext(GL::Context::Name cname) {

    }

    void UpdateRenderGroup(Visual::Coverage::Type ctype,
                           Visual::Lighting::Type ltype,
                           GL::Primitive::Type ptype,
                           const QVector<GL::Vertex>& vertices) {
        render_groups_[ctype][ltype].UploadVertices(ptype, vertices);
    }

    void UpdateVertexBuffer(Visual::Coverage::Type ctype,
                            Visual::Lighting::Type ltype,
                            GL::Primitive::Type ptype,
                            const QVector<GL::Vertex>& vertices) {
        vertex_buffers_[ctype][ltype][ptype].UploadVertices(vertices);
    }

    /*
    GL::RenderGroup& RenderGroup(Visual::Coverage::Type ctype, Visual::Lighting::Type ltype) {
        return render_groups_[ctype][ltype];
    }
    */

    /*
    void DrawRenderGroup(Visual::Coverage::Type ctype,
                         Visual::Lighting::Type ltype,
                         const std::vector<GL::Primitive::Type>& ptypes) {

    }
    */

    GL::Context::Name current_gl_context_;
    GL::RenderGroup render_groups_[Visual::Coverage::E_MAX]
                                  [Visual::Lighting::E_MAX]
                                  [GL::Context::E_MAX];
    GL::VertexBuffer vertex_buffers_[Visual::Coverage::E_MAX]
                                    [Visual::Lighting::E_MAX]
                                    [GL::Primitive::Type];
};

} // namespace RCAD

#endif // RC_OPENGL_H
