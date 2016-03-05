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

#ifndef RC_SCENE_POLYLINE_H
#define RC_SCENE_POLYLINE_H

#include "scene.h"

namespace RCAD {

//=============================================================================
// Interface: ScenePolyline_2
//=============================================================================

class ScenePolyline_2 : public ISceneObject, public Visual::Geometry {
public:
    ScenePolyline_2() {
        model_polyline_.AddObserver(this);
    }

    ~ScenePolyline_2() {
        LOG(DEBUG) << "destroying scene polyline...";
    }

    void Initialize(const QVector2D& start) {
        LOG(DEBUG) << "initializing polyline.";

        model_polyline_.push_back(Point_2r(start.x(), start.y()));
    }

    void Update(const QVector2D& cur) {
        LOG(DEBUG) << "updating polyline.";

        model_polyline_.push_back(Point_2r(cur.x(), cur.y()));
    }

    void Select() override {
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

    void Deselect() override {
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

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) {
        Intersection::Toleranced::Ray_3rPolyline_2r isect(&ray, &model_polyline_);
        return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPolyline_2r::INTERSECTION_EMPTY, isect.time());
    }

    void UpdateColor(const QColor &color) override {}

    QString scene_object_type() const override {
        return "Polyline";
    }

    const QString& name() const override {
        return name_;
    }

    void set_name(const QString &name) override {
        name_ = name;
    }

    const Polyline_2r& model_polyline() const {
        return model_polyline_;
    }

private:
    Polyline_2r model_polyline_;
    QString name_;
};

}

#endif // RC_SCENE_POLYLINE_H
