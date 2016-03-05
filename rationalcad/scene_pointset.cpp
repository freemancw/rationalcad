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

#include "scene_pointset.h"

namespace RCAD {

ScenePointSet_3::ScenePointSet_3() {
    model_point_set_.AddObserver(this);
}

void ScenePointSet_3::Initialize(const QVector2D& point) {
    model_point_set_.add(Point_3r(point.x(), point.y(), 0));
}

void ScenePointSet_3::Initialize(const QVector<QVector3D>& data) {
    for (auto point : data) {
        model_point_set_.add(Point_3r(point.x(), point.y(), point.z()));
    }
}

void ScenePointSet_3::Update(const QVector2D& point) {
    model_point_set_.add(Point_3r(point.x(), point.y(), 0));
}

void ScenePointSet_3::Select() {
    Visual::Material selected_mat;
    selected_mat.set_ambient(Visual::Color::SKYBLUE);

    for (auto point : model_point_set_.points()) {
        SigPushVisualPoint_3r(*point, Visual::Point(selected_mat));
    }
}

void ScenePointSet_3::Deselect() {
    for (auto point : model_point_set_.points()) {
        SigPopVisualPoint_3r(*point);
    }
}

Intersection::Ray_3rSceneObject ScenePointSet_3::intersect(const Ray_3r &ray) {
    Intersection::Toleranced::Ray_3rPointSet_3r isect(&ray, &model_point_set_);
    return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPointSet_3r::INTERSECTION_EMPTY, isect.time());
}

void ScenePointSet_3::UpdateColor(const QColor &color) {}

QString ScenePointSet_3::scene_object_type() const {
    return "PointSet";
}

const QString& ScenePointSet_3::name() const {
    return name_;
}

void ScenePointSet_3::set_name(const QString &name) {
    name_ = name;
}

const PointSet_3r& ScenePointSet_3::model_point_set() const {
    return model_point_set_;
}

}
