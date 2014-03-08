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
 * @date 2013-01-13
 * @brief Manager type responsible for storing and manipulating scene objects.
 */

#ifndef RC_SCENE_H
#define RC_SCENE_H

// RationalCAD
#include "rc_common.h"
#include "rc_config.h"
#include "rc_opengl.h"

// mathlib
#include "ml_polygon.h"
#include "ml_polytope.h"
#include "ml_rational.h"
#include "ml_line.h"
#include "ml_observer.h"
#include "ml_triangle.h"
#include "ml_cone.h"

bool operator<(const QVector<uint32_t>& a, const QVector<uint32_t>& b);

namespace RCAD {

//=============================================================================
// Interface: ApproximatePoint_3f
//=============================================================================

class ApproxPoint_3f : public IPointObserver {
public:
    ApproxPoint_3f();
    ApproxPoint_3f(const Point_2r& p);
    ApproxPoint_3f(const Point_3r& p);

    void SlotPositionChanged_2r(const Point_2r& p) override;
    void SlotPositionChanged_3r(const Point_3r& p) override;

    const Point_3f& approx() const;

private:
    Point_3f approx_;
};

//=============================================================================
// Interface: ISceneObject
//=============================================================================

struct ISceneObject {
    virtual void Select() = 0;
    virtual void Deselect() = 0;
    virtual void UpdateColor(const QColor& color) = 0;
};

//=============================================================================
// Interface: ScenePolytope_3
//=============================================================================

class ScenePolytope_3 : public ISceneObject, public VisualGeometry {
public:
    ScenePolytope_3() {
        model_polytope_.AddObserver(this);
    }

    void Update() {
        model_polytope_.Update();
    }

    void Select() override {}
    void Deselect() override {}
    void UpdateColor(const QColor &color) override {}

private:
    Polytope_3r model_polytope_;
};

//=============================================================================
// Interface: SceneObserver
//=============================================================================

class SceneObserver : public QObject, public IGeometryObserver {

    Q_OBJECT

public:
    SceneObserver();
    ~SceneObserver();

    void SlotRegisterPoint_2r(Point_2r& p) override;

    void SlotPushVisualPoint_2r(const Point_2r& p, const Visual::Point& vp,
        const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_2r(const Segment_2r& s,
        const Visual::Segment& vs, const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_2r(const Triangle_2r& t,
        const Visual::Triangle& vt, const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_2r(const Point_2r& p,
        const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_2r(const Segment_2r& s,
        const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_2r(const Triangle_2r& t,
        const uint32_t msec_delay = 0) override;

    void SlotRegisterPoint_3r(Point_3r& p) override;

    void SlotPushVisualPoint_3r(const Point_3r& p, const Visual::Point& vp,
        const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_3r(const Segment_3r& s,
        const Visual::Segment& vs, const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_3r(const Triangle_3r& t,
        const Visual::Triangle& vt, const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_3r(const Point_3r& p,
        const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_3r(const Segment_3r& s,
        const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_3r(const Triangle_3r& t,
        const uint32_t msec_delay = 0) override;

    void SlotUpdate() override;

signals:
    void UpdateVboPoints(QVector<GL::Vertex> verts);
    void UpdateVboLines(QVector<GL::Vertex> verts);
    void UpdateVboTriangles(QVector<GL::Vertex> verts);

private:
    void GenerateVboPoints();
    void GenerateVboLines();
    void GenerateVboTriangles();

    QHash<QString, QSharedPointer<ISceneObject>> scene_objects_;
    quint32 cur_point_uid_;
    QHash<uint32_t, QSharedPointer<ApproxPoint_3f>> approx_points_;
    QHash<uint32_t, QStack<Visual::Point>> viz_points_;
    QMap<QPair<uint32_t, uint32_t>, QStack<Visual::Segment>> viz_segments_;
    QMap<QVector<uint32_t>, QStack<Visual::Triangle>> viz_triangles_;
};

//=============================================================================
// Interface: SceneManager
//=============================================================================

class SceneManager : public QObject {

    Q_OBJECT

public:
    SceneManager();
    ~SceneManager();

    int NumObjects() const;
    const QString& selected_name() const;
    GL::VertexBuffer& points_vbo();
    GL::VertexBuffer& lines_vbo();
    GL::VertexBuffer& triangles_vbo();

public slots:

    void UpdateVboPoints(QVector<GL::Vertex> verts);
    void UpdateVboLines(QVector<GL::Vertex> verts);
    void UpdateVboTriangles(QVector<GL::Vertex> verts);

    void BeginCreatePolytope(const QVector2D& coords);
    void UpdateNewPolytope(const QVector2D& coords);
    void EndCreatePolytope();

    void UpdateSelectedObjectName(const QString& name);
    void UpdateSelectedObjectColor(const QColor& color);
    void DeleteSelectedObject();
    void SelectObject(const QVector2D& coords);
    void Deselect();

private:
    bool ObjectIsSelected() const;
    ISceneObject* SelectedObject();
    ScenePolytope_3* SelectedPolytope_3();

    GL::VertexBuffer points_vbo_;
    GL::VertexBuffer lines_vbo_;
    GL::VertexBuffer triangles_vbo_;

    SceneObserver scene_observer_;
    QString selected_name_;
    QSharedPointer<QThread> animation_thread_;
};

} // namespace RCAD

#endif // RC_SCENE_H