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

#include "scene_polyline.h"

namespace RCAD {

//=============================================================================
// Implementation: ScenePolyline_2
//=============================================================================

ScenePolyline_2::ScenePolyline_2() {
    model_polyline_.AddObserver(this);
}

ScenePolyline_2::~ScenePolyline_2() {
    LOG(DEBUG) << "destroying scene polyline...";
}

void ScenePolyline_2::Initialize(const QVector2D& start) {
    LOG(DEBUG) << "initializing polyline.";

    model_polyline_.push_back(Point_2r(start.x(), start.y()));
}

void ScenePolyline_2::Update(const QVector2D& cur) {
    LOG(DEBUG) << "updating polyline.";

    model_polyline_.push_back(Point_2r(cur.x(), cur.y()));
}

void ScenePolyline_2::Select() {
    LOG(DEBUG) << "selecting polyline.";

    Visual::Material selected_mat;
    selected_mat.set_ambient(Visual::Color::SKYBLUE);

    SharedPoint_2r last_vertex;
    for (auto vertex : model_polyline_.vertices()) {
        SigPushVisualPoint_2r(*vertex, Visual::Point(selected_mat,
            model_polyline_.z_order()));

        if (last_vertex) {
            SigPushVisualSegment_2r(Segment_2r(last_vertex, vertex),
                                    Visual::Segment(selected_mat));
        }

        last_vertex = vertex;
    }
}

void ScenePolyline_2::Deselect() {
    LOG(DEBUG) << "deselecting polyline.";

    SharedPoint_2r last_vertex;
    for (auto vertex : model_polyline_.vertices()) {
        SigPopVisualPoint_2r(*vertex);

        if (last_vertex) {
            SigPopVisualSegment_2r(Segment_2r(last_vertex, vertex));
        }

        last_vertex = vertex;
    }
}

Intersection::Ray_3rSceneObject ScenePolyline_2::intersect(const Ray_3r &ray) {
    Intersection::Toleranced::Ray_3rPolyline_2r isect(&ray, &model_polyline_);
    return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPolyline_2r::INTERSECTION_EMPTY, isect.time());
}

void ScenePolyline_2::UpdateColor(const QColor &color) {}

QString ScenePolyline_2::scene_object_type() const {
    return "Polyline";
}

const QString& ScenePolyline_2::name() const {
    return name_;
}

void ScenePolyline_2::set_name(const QString &name) {
    name_ = name;
}

const Polyline_2r& ScenePolyline_2::model_polyline() const {
    return model_polyline_;
}

}
