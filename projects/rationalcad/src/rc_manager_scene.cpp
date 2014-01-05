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
 * @date 06/11/2013
 */

// RationalCAD
#include "rc_common.h"
#include "rc_manager_scene.h"
#include "rc_manager_config.h"

// mathlib
#include "ml_common.h"
#include "ml_rational.h"
#include "ml_vector.h"
#include "ml_polygon.h"
#include "ml_matrix.h"
#include "ml_line.h"
#include "ml_intersection.h"

/*!
 * Specifies a lexicographical ordering on two vectors of nonnegative integers.
 *
 * A 123
 * B 1234
 */
bool operator<(const QVector<uint32_t>& a, const QVector<uint32_t>& b) {
    for(int i = 0; i < qMin(a.size(), b.size()); ++i) {
        if(a[i] < b[i]) {
            return true;
        } else if(a.at(i) > b.at(i)) {
            return false;
        }
    }

    // if we reach here they have to be equal
    if(a.size() == b.size()) {
        return false;
    } else if(a.size() < b.size()) {
        return true;
    } else {
        return false;
    }
}

BEGIN_NAMESPACE(RCAD)

Point_2f ToPoint_2f(const QVector2D& v, bool snap = false);

//=============================================================================
// SceneManager Constructors/Destructors
//=============================================================================

SceneManager::SceneManager() :
    cur_point_uid_(1) {}

SceneManager::~SceneManager() {}

//=============================================================================
// SceneManager Slots
//=============================================================================

void SceneManager::GenerateVboPoints() {
    QVector<GLVertex> points;
    for (auto i = viz_points_.begin(); i != viz_points_.end(); ++i) {
        GLVertex v(approx_points_.value(i.key())->approximation());
        v.set_mat_ambient(i->back().color());
        points.push_back(v);
    }
    emit UpdateVboPoints(points);
}

void SceneManager::GenerateVboLines() {
    QVector<GLVertex> lines;
    for (auto i = viz_segments_.begin(); i != viz_segments_.end(); ++i) {
        GLVertex p(approx_points_.value(i.key().first)->approximation());
        GLVertex q(approx_points_.value(i.key().second)->approximation());
        p.set_mat_ambient(i->back().color());
        q.set_mat_ambient(i->back().color());
        lines.push_back(p);
        lines.push_back(q);
    }
    emit UpdateVboLines(lines);
}

void SceneManager::GenerateVboTriangles() {
    QVector<GLVertex> triangles;
    for (auto i = viz_triangles_.begin(); i != viz_triangles_.end(); ++i) {
        auto current_vt = i.value().top();
        GLVertex a(approx_points_.value(i.key().at(0))->approximation());
        GLVertex b(approx_points_.value(i.key().at(1))->approximation());
        GLVertex c(approx_points_.value(i.key().at(2))->approximation());
        a.set_mat_ambient(current_vt.diffuse());
        b.set_mat_ambient(current_vt.diffuse());
        c.set_mat_ambient(current_vt.diffuse());
        triangles.push_back(a);
        triangles.push_back(b);
        triangles.push_back(c);
    }
    emit UpdateVboTriangles(triangles);
}

void SceneManager::SlotRegisterPoint_2r(Point_2r &p) {
    if(!p.unique_id()) {
        p.set_unique_id(cur_point_uid_++);
    }
    if(!approx_points_.contains(p.unique_id())) {
        auto approx = QSharedPointer<ApproximatePoint_3f>(new ApproximatePoint_3f(p));
        approx_points_.insert(p.unique_id(), approx);
        p.AddObserver(approx.data());
    }
}

void SceneManager::SlotPushVisualPoint_2r(const Point_2r& p,
                                          const Visual::Point& vp,
                                          const uint32_t msec_delay) {
    viz_points_[p.unique_id()].push_back(vp);
    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPopVisualPoint_2r(const Point_2r& p,
                                      const uint32_t msec_delay) {
    rAssert(!viz_points_.value(p.unique_id()).isEmpty());
    viz_points_[p.unique_id()].pop_back();

    if(viz_points_.value(p.unique_id()).isEmpty()) {
        viz_points_.remove(p.unique_id());
    }

    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPushVisualSegment_2r(const Segment_2r& s,
                                         const Visual::Segment& vs,
                                         const uint32_t msec_delay) {
    QPair<uint32_t, uint32_t> key(s.p().unique_id(), s.q().unique_id());
    viz_segments_[key].push_back(vs);
    GenerateVboLines();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPopVisualSegment_2r(const Segment_2r& s,
                                        const uint32_t msec_delay) {
    QPair<uint32_t, uint32_t> key(s.p().unique_id(), s.q().unique_id());
    rAssert(!viz_segments_.value(key).isEmpty());
    viz_segments_[key].pop_back();

    if(viz_segments_.value(key).isEmpty()) {
        viz_segments_.remove(key);
    }

    GenerateVboLines();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPushVisualTriangle_2r(const Triangle_2r &t,
                                          const Visual::Triangle& vt,
                                          const uint32_t msec_delay) {
    QVector<uint32_t> key;
    key.push_back(t.a().unique_id());
    key.push_back(t.b().unique_id());
    key.push_back(t.c().unique_id());

    viz_triangles_[key].push_back(vt);

    GenerateVboTriangles();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPopVisualTriangle_2r(const Triangle_2r &t,
                                         const uint32_t msec_delay) {
    QVector<uint32_t> key;
    key.push_back(t.a().unique_id());
    key.push_back(t.b().unique_id());
    key.push_back(t.c().unique_id());

    viz_triangles_[key].pop_back();

    if(viz_triangles_.value(key).isEmpty()) {
        viz_triangles_.remove(key);
    }

    GenerateVboTriangles();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotRegisterPoint_3r(Point_3r &p) {
    if(!p.unique_id()) {
        p.set_unique_id(cur_point_uid_++);
    }
    if(!approx_points_.contains(p.unique_id())) {
        auto approx = QSharedPointer<ApproximatePoint_3f>(new ApproximatePoint_3f(p));
        approx_points_.insert(p.unique_id(), approx);
        p.AddObserver(approx.data());
    }
}

void SceneManager::SlotPushVisualPoint_3r(const Point_3r& p,
                                          const Visual::Point& vp,
                                          const uint32_t msec_delay) {
    viz_points_[p.unique_id()].push_back(vp);
    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPopVisualPoint_3r(const Point_3r& p,
                                      const uint32_t msec_delay) {
    rAssert(!viz_points_.value(p.unique_id()).isEmpty());
    viz_points_[p.unique_id()].pop_back();

    if(viz_points_.value(p.unique_id()).isEmpty()) {
        viz_points_.remove(p.unique_id());
    }

    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPushVisualSegment_3r(const Segment_3r& s,
                                         const Visual::Segment& vs,
                                         const uint32_t msec_delay) {
    QPair<uint32_t, uint32_t> key(s.p().unique_id(), s.q().unique_id());
    viz_segments_[key].push_back(vs);
    GenerateVboLines();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPopVisualSegment_3r(const Segment_3r& s,
                                        const uint32_t msec_delay) {
    QPair<uint32_t, uint32_t> key(s.p().unique_id(), s.q().unique_id());
    rAssert(!viz_segments_.value(key).isEmpty());
    viz_segments_[key].pop_back();

    if(viz_segments_.value(key).isEmpty()) {
        viz_segments_.remove(key);
    }

    GenerateVboLines();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPushVisualTriangle_3r(const Triangle_3r &t,
                                             const Visual::Triangle& vt,
                                             const uint32_t msec_delay) {
    QVector<uint32_t> key;
    key.push_back(t.a().unique_id());
    key.push_back(t.b().unique_id());
    key.push_back(t.c().unique_id());

    viz_triangles_[key].push_back(vt);

    GenerateVboTriangles();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotPopVisualTriangle_3r(const Triangle_3r &t,
                                            const uint32_t msec_delay) {
    QVector<uint32_t> key;
    key.push_back(t.a().unique_id());
    key.push_back(t.b().unique_id());
    key.push_back(t.c().unique_id());

    viz_triangles_[key].pop_back();

    if(viz_triangles_.value(key).isEmpty()) {
        viz_triangles_.remove(key);
    }

    GenerateVboTriangles();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneManager::SlotUpdate() {
    GenerateVboPoints();
    GenerateVboLines();
    GenerateVboTriangles();
}

//=============================================================================
// SceneManager Polygon Management
//=============================================================================

void SceneManager::BeginCreatePolygon(const QString& name,
                                      const QColor& face_color) {
    Deselect();
    scene_objects_.insert(name, QSharedPointer<ISceneObject>(
                              new ScenePolygon_2(face_color)));
    selected_name_ = name;
    SelectedPolygon_2()->AddObserver(this);
}

void SceneManager::EndCreatePolygon() {
    if(SelectedPolygon_2()->NumVertices() < 3) {
        scene_objects_.remove(selected_name_);
        selected_name_ = "";
    } else {
        SelectedPolygon_2()->CloseBoundary();
    }
}

void SceneManager::AddVertexToNewPolygon(const QVector2D& coords) {
    SelectedPolygon_2()->AppendVertexToBoundary(ToPoint_2f(coords,
                                                g_config.snap_to_grid_));
}

void SceneManager::ComputeIntegerHull() {
    if(!ObjectIsSelected()) return;

    SelectedPolygon_2()->ComputeIntegerHull();
}

//=============================================================================
// SceneManager Cone_2 Management
//=============================================================================

void SceneManager::BeginCreate2Cone(const QString &name,
                                    const QColor &face_color) {
    Deselect();
    scene_objects_.insert(name, QSharedPointer<ISceneObject>(
                              new SceneCone_2()));
    selected_name_ = name;
    SelectedCone_2()->AddObserver(this);
}

void SceneManager::EndCreate2Cone() {
    //rDebug("update on end");
    SelectedCone_2()->Update();
}

static std::shared_ptr<Point_2r> cone_a;
static std::shared_ptr<Point_2r> cone_b;

void SceneManager::Update2ConeA(const QVector2D& coords) {
    Point_2r p(ToPoint_2f(coords, g_config.snap_to_grid_));
    cone_a->set_elements(p.elements());
    cone_a->SigPositionChanged();
    SlotUpdate();
}

void SceneManager::Update2ConeB(const QVector2D& coords) {
    Point_2r p(ToPoint_2f(coords, g_config.snap_to_grid_));
    cone_b->set_elements(p.elements());
    cone_b->SigPositionChanged();
    SlotUpdate();
}

void SceneManager::SetVertexForNew2Cone(const QVector2D &coords) {
    SelectedCone_2()->SetVertex(ToPoint_2f(coords,
                                           g_config.snap_to_grid_));
    g_config.input_state_ = CREATE_2CONE_RAY_A;
    cone_a = std::make_shared<Point_2r>(ToPoint_2f(coords, g_config.snap_to_grid_));
    SelectedCone_2()->AddConstraint(cone_a);
}

void SceneManager::AddConstraintToNew2Cone(const QVector2D &coords) {
    g_config.input_state_ = CREATE_2CONE_RAY_B;
    cone_b = std::make_shared<Point_2r>(ToPoint_2f(coords, g_config.snap_to_grid_));
    SelectedCone_2()->AddConstraint(cone_b);
}

void SceneManager::ComputeCIGP() {
    if(!ObjectIsSelected()) return;

    SelectedCone_2()->ComputeCIGP();
}

//=============================================================================
// SceneManager Polytope_3 Management
//=============================================================================

void SceneManager::BeginCreatePolytope(const QString &name,
                                       const QColor &face_color) {
    Deselect();
    scene_objects_.insert(name, QSharedPointer<ISceneObject>(
                          new ScenePolytope_3()));
    selected_name_ = name;
    SelectedPolytope_3()->AddObserver(this);
    SelectedPolytope_3()->Update();
}

void SceneManager::EndCreatePolytope() {
    SelectedPolytope_3()->Update();
}

//=============================================================================
// Scene Object Management
//=============================================================================

void SceneManager::UpdateSelectedObjectName(const QString &name) {
    if(!ObjectIsSelected()) return;

    QSharedPointer<ISceneObject> obj_ptr(SelectedObject());
    scene_objects_.remove(selected_name_);
    selected_name_ = name;
    scene_objects_.insert(selected_name_, obj_ptr);
}

void SceneManager::UpdateSelectedObjectColor(const QColor &color) {
    if(!ObjectIsSelected()) return;

    SelectedObject()->UpdateColor(color);
}

void SceneManager::DeleteSelectedObject() {
    if(!ObjectIsSelected()) return;

    scene_objects_.remove(selected_name_);
    selected_name_ = "";
}

void SceneManager::SelectObject(const QVector2D& coords) {
    Deselect();
}

void SceneManager::Deselect() {
    if(!ObjectIsSelected()) return;

    SelectedObject()->Deselect();
    selected_name_ = "";
}

int SceneManager::NumObjects() const {
    return scene_objects_.size();
}

ISceneObject* SceneManager::SelectedObject() {
    return scene_objects_.value(selected_name_).data();
}
ScenePolygon_2* SceneManager::SelectedPolygon_2() {
    return dynamic_cast<ScenePolygon_2*>(SelectedObject());
}
ScenePolytope_3* SceneManager::SelectedPolytope_3() {
    return dynamic_cast<ScenePolytope_3*>(SelectedObject());
}
SceneCone_2* SceneManager::SelectedCone_2() {
    return dynamic_cast<SceneCone_2*>(SelectedObject());
}
bool SceneManager::ObjectIsSelected() const {
    return selected_name_ != "";
}

const QString& SceneManager::selected_name() const {
    return selected_name_;
}

static Point_2f ToPoint_2f(const QVector2D &v, bool snap) {
    return snap ? Point_2f(qRound(v.x()), qRound(v.y())) :
                  Point_2f(v.x(), v.y());
}

//=============================================================================
// ScenePolygon_2
//=============================================================================

ScenePolygon_2::ScenePolygon_2() {
    model_polygon_.AddObserver(this);
    test_.AddObserver(this);
}

ScenePolygon_2::ScenePolygon_2(const QColor &face_color) {
    model_polygon_.AddObserver(this);
}

void ScenePolygon_2::AppendVertexToBoundary(const Point_2r& v) {
    model_polygon_.AppendVertexToBoundary(v);
}

const size_t ScenePolygon_2::NumVertices() const {
    return model_polygon_.NumVertices();
}

void ScenePolygon_2::CloseBoundary() {
    model_polygon_.CloseBoundary();
}

void ScenePolygon_2::ComputeIntegerHull() {
    model_polygon_.ComputeIntegerHull();
}

void ScenePolygon_2::UpdateColor(const QColor &color) {

}

void ScenePolygon_2::Select() {

}

void ScenePolygon_2::Deselect() {

}

//=============================================================================
// ApproximatePoint_3f
//=============================================================================

ApproximatePoint_3f::ApproximatePoint_3f() {}

ApproximatePoint_3f::ApproximatePoint_3f(const Point_2r &p) {
    approximation_.set_x(p.x().get_d());
    approximation_.set_y(p.y().get_d());
    approximation_.set_z(0);
}

ApproximatePoint_3f::ApproximatePoint_3f(const Point_3r &p) {
    approximation_.set_x(p.x().get_d());
    approximation_.set_y(p.y().get_d());
    approximation_.set_z(p.z().get_d());
}

void ApproximatePoint_3f::SlotPositionChanged_2r(const Point_2r &p) {
    approximation_.set_x(p.x().get_d());
    approximation_.set_y(p.y().get_d());
}

void ApproximatePoint_3f::SlotPositionChanged_3r(const Point_3r& p) {
    approximation_.set_x(p.x().get_d());
    approximation_.set_y(p.y().get_d());
    approximation_.set_z(p.z().get_d());
}

const Point_3f& ApproximatePoint_3f::approximation() const {
    return approximation_;
}

END__NAMESPACE(RCAD)
