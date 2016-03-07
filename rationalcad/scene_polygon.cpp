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

#include "scene_polygon.h"

namespace RCAD {

//=============================================================================
// Implementation: ScenePolygon_2
//=============================================================================

ScenePolygon_2::ScenePolygon_2() {
    model_polygon_.AddObserver(this);
}

ScenePolygon_2::~ScenePolygon_2() {
    LOG(DEBUG) << "destroying scene polygon...";
}

void ScenePolygon_2::Initialize(const QVector2D& start) {
    LOG(DEBUG) << "initializing polygon.";

    model_polygon_.push_back(Point_2r(start.x(), start.y()));
}

void ScenePolygon_2::Update(const QVector2D& cur) {
    LOG(DEBUG) << "updating polygon.";

    model_polygon_.push_back(Point_2r(cur.x(), cur.y()));
}

void ScenePolygon_2::Finalize() {
    LOG(DEBUG) << "finalizing polygon.";

    model_polygon_.CloseBoundary();
}

void ScenePolygon_2::Select() {
    LOG(DEBUG) << "selecting polygon.";

    Visual::Material selected_mat;
    selected_mat.set_ambient(Visual::Color::SKYBLUE);

    SharedPoint_2r last_vertex;
    for (auto vertex : model_polygon_.boundary().vertices()) {
        SigPushVisualPoint_2r(*vertex, Visual::Point(selected_mat,
            model_polygon_.z_order()));

        if (last_vertex) {
            SigPushVisualSegment_2r(Segment_2r(last_vertex, vertex),
                                    Visual::Segment(selected_mat));
        }

        last_vertex = vertex;
    }
}

void ScenePolygon_2::Deselect() {
    LOG(DEBUG) << "deselecting polygon.";

    SharedPoint_2r last_vertex;
    for (auto vertex : model_polygon_.boundary().vertices()) {
        SigPopVisualPoint_2r(*vertex);

        if (last_vertex) {
            SigPopVisualSegment_2r(Segment_2r(last_vertex, vertex));
        }

        last_vertex = vertex;
    }
}

Intersection::Ray_3rSceneObject ScenePolygon_2::intersect(const Ray_3r &ray) {
    Intersection::Toleranced::Ray_3rPolygon_2r isect(&ray, &model_polygon_);
    return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPolygon_2r::INTERSECTION_EMPTY, isect.time());
}

void ScenePolygon_2::UpdateColor(const QColor &color) {}

QString ScenePolygon_2::scene_object_type() const {
    return "Polygon";
}

const QString& ScenePolygon_2::name() const {
    return name_;
}

void ScenePolygon_2::set_name(const QString &name) {
    name_ = name;
}

const Polygon_2r& ScenePolygon_2::model_polygon() const {
    return model_polygon_;
}

}
