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

#ifndef RC_SCENE_POINTSET_H
#define RC_SCENE_POINTSET_H

#include "scene.h"

namespace RCAD {

//=============================================================================
// Interface: ScenePointSet_3
//=============================================================================

class ScenePointSet_3 : public ISceneObject, public Visual::Geometry {
public:
    ScenePointSet_3() {
        model_point_set_.AddObserver(this);
    }

    void Initialize(const QVector2D& point) {
        model_point_set_.add(Point_3r(point.x(), point.y(), 0));
    }

    void Update(const QVector2D& point) {
        model_point_set_.add(Point_3r(point.x(), point.y(), 0));
    }

    void Initialize(const QVector<QVector3D>& data) {
        for (auto point : data) {
            model_point_set_.add(Point_3r(point.x(), point.y(), point.z()));
        }
    }

    void Select() override {
        Visual::Material selected_mat;
        selected_mat.set_ambient(Visual::Color::SKYBLUE);

        for (auto point : model_point_set_.points()) {
            SigPushVisualPoint_3r(*point, Visual::Point(selected_mat));
        }
    }

    void Deselect() override {
        for (auto point : model_point_set_.points()) {
            SigPopVisualPoint_3r(*point);
        }
    }

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) {
        Intersection::Toleranced::Ray_3rPointSet_3r isect(&ray, &model_point_set_);
        return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPointSet_3r::INTERSECTION_EMPTY, isect.time());
    }

    void UpdateColor(const QColor &color) override {}

    QString scene_object_type() const override {
        return "PointSet";
    }

    const QString& name() const override {
        return name_;
    }
    void set_name(const QString &name) override {
        name_ = name;
    }

    const PointSet_3r& model_point_set() const {
        return model_point_set_;
    }

private:
    PointSet_3r model_point_set_;
    QString name_;
};

}

#endif // RC_SCENE_POINTSET_H
