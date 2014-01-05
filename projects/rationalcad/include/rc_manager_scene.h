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
 * @date 01/13/2013
 * @brief Manager type responsible for storing and manipulating scene objects.
 */

#ifndef RC_MANAGER_SCENE_H
#define RC_MANAGER_SCENE_H

// RationalCAD
#include "rc_common.h"
#include "rc_manager_config.h"
#include "rc_manager_gl.h"

// mathlib
#include "ml_polygon.h"
#include "ml_polytope.h"
#include "ml_rational.h"
#include "ml_line.h"
#include "ml_observer.h"
#include "ml_triangle.h"
#include "ml_cone.h"

bool operator<(const QVector<uint32_t>& a, const QVector<uint32_t>& b);

BEGIN_NAMESPACE(RCAD)

class ApproximatePoint_3f : public IPointObserver {
public:
    ApproximatePoint_3f();
    ApproximatePoint_3f(const Point_2r& p);
    ApproximatePoint_3f(const Point_3r& p);

    void SlotPositionChanged_2r(const Point_2r& p) override;
    void SlotPositionChanged_3r(const Point_3r& p) override;

    const Point_3f& approximation() const;

private:
    Point_3f approximation_;
};

//=============================================================================
// ScenePolygon_2
//=============================================================================

struct ISceneObject {
    virtual void Select() = 0;
    virtual void Deselect() = 0;
    virtual void UpdateColor(const QColor& color) = 0;
};

class ScenePolygon_2 : public ISceneObject, public VisualGeometry {
public:
    ScenePolygon_2();
    explicit ScenePolygon_2(const QColor& color);

    void AppendVertexToBoundary(const Point_2r& v);
    void CloseBoundary();
    void ComputeIntegerHull();
    bool ContainsPoint(const Point_2r& p) const;
    const size_t NumVertices() const;

    void Select() override;
    void Deselect() override;
    void UpdateColor(const QColor& color) override;

private:
    Polygon_2r model_polygon_;
    Polytope_3r test_;
};

class SceneCone_2 : public ISceneObject, public VisualGeometry {
public:
    SceneCone_2() {
        model_cone_.AddObserver(this);
    }

    void SetVertex(const Point_2r& v) {
        model_cone_.set_vertex(std::make_shared<Point_2r>(v));
    }
    void AddConstraint(const Point_2r& c) {
        if(g_config.input_state_ == CREATE_2CONE_RAY_A) {
            model_cone_.set_endpoint_a(std::make_shared<Point_2r>(c));
            rDebug("set endpoint a");
        } else {
            model_cone_.set_endpoint_b(std::make_shared<Point_2r>(c));
            rDebug("set endpoint b");
        }
    }
    void AddConstraint(std::shared_ptr<Point_2r> c) {
        if(g_config.input_state_ == CREATE_2CONE_RAY_A) {
            model_cone_.set_endpoint_a(c);
            rDebug("set endpoint a");
        } else {
            model_cone_.set_endpoint_b(c);
            rDebug("set endpoint b");
        }
    }
    void ComputeCIGP() {
        rDebug("hey there");
        model_cone_.ComputeCIGP();
    }

    void Update() {
        model_cone_.Update();
    }

    void Select() override {}
    void Deselect() override {}
    void UpdateColor(const QColor &color) override {}

private:
    Cone_2r model_cone_;
};

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
// SceneManager
//=============================================================================

/*!
 *  \brief Manager type responsible for storing and manipulating geometric
 *         objects the user creates.
 */
class SceneManager : public QObject, public IGeometryObserver {

    Q_OBJECT

public:
    SceneManager();
    ~SceneManager();

    void SlotRegisterPoint_2r(Point_2r& p) override;

    void SlotPushVisualPoint_2r(const Point_2r& p, const Visual::Point& vp,
                             const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_2r(const Segment_2r& s, const Visual::Segment& vs,
                               const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_2r(const Triangle_2r& t,
                                const Visual::Triangle& vt,
                                const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_2r(const Point_2r& p,
                            const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_2r(const Segment_2r& s,
                              const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_2r(const Triangle_2r& t,
                               const uint32_t msec_delay = 0) override;

    void SlotRegisterPoint_3r(Point_3r& p) override;

    void SlotPushVisualPoint_3r(const Point_3r& p, const Visual::Point& vp,
                             const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_3r(const Segment_3r& s, const Visual::Segment& vs,
                               const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_3r(const Triangle_3r& t,
                                const Visual::Triangle& vt,
                                const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_3r(const Point_3r& p,
                            const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_3r(const Segment_3r& s,
                              const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_3r(const Triangle_3r& t,
                               const uint32_t msec_delay = 0) override;

    void SlotUpdate() override;

    int NumObjects() const;
    const QString& selected_name() const;

public slots:
    void BeginCreatePolygon(const QString& name, const QColor& face_color);
    void EndCreatePolygon();
    void AddVertexToNewPolygon(const QVector2D& coords);
    void ComputeIntegerHull();

    void BeginCreatePolytope(const QString& name, const QColor& face_color);
    void EndCreatePolytope();

    void BeginCreate2Cone(const QString& name, const QColor& face_color);
    void EndCreate2Cone();
    void Update2ConeA(const QVector2D& coords);
    void Update2ConeB(const QVector2D& coords);
    void SetVertexForNew2Cone(const QVector2D& coords);
    void AddConstraintToNew2Cone(const QVector2D& coords);
    void ComputeCIGP();

    void UpdateSelectedObjectName(const QString& name);
    void UpdateSelectedObjectColor(const QColor& color);
    void DeleteSelectedObject();
    void SelectObject(const QVector2D& coords);
    void Deselect();

signals:
    void UpdateVboPoints(QVector<GLVertex> verts);
    void UpdateVboLines(QVector<GLVertex> verts);
    void UpdateVboTriangles(QVector<GLVertex> verts);

private:
    bool ObjectIsSelected() const;
    ISceneObject* SelectedObject();
    ScenePolygon_2* SelectedPolygon_2();
    ScenePolytope_3* SelectedPolytope_3();
    SceneCone_2* SelectedCone_2();

    void GenerateVboPoints();
    void GenerateVboLines();
    void GenerateVboTriangles();

    QString selected_name_;
    QHash<QString, QSharedPointer<ISceneObject>> scene_objects_;

    quint32 cur_point_uid_;
    QHash<uint32_t, QSharedPointer<ApproximatePoint_3f>> approx_points_;
    QHash<uint32_t, QStack<Visual::Point>> viz_points_;
    QMap<QPair<uint32_t, uint32_t>, QStack<Visual::Segment>> viz_segments_;
    QMap<QVector<uint32_t>, QStack<Visual::Triangle>> viz_triangles_;
};

END__NAMESPACE(RCAD)

#endif // RC_MANAGER_SCENE_H
