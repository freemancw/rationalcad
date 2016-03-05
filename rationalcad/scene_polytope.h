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

#ifndef RC_SCENE_POLYTOPE_H
#define RC_SCENE_POLYTOPE_H

#include "scene.h"

namespace RCAD {

//=============================================================================
// Interface: ScenePolytope_3
//=============================================================================

class ScenePolytope_3 : public ISceneObject, public Visual::Geometry {
public:
    ScenePolytope_3() {
        model_polytope_.AddObserver(this);
    }

    void Initialize(const QVector2D& start, const QVector2D& cur) {
        model_polytope_.Initialize(Point_3f(start.x(), start.y(), 0),
                                   Point_3f(cur.x(), cur.y(), 8));
    }

    void Update(const QVector2D& cur) {
        model_polytope_.Update(Point_3f(cur.x(), cur.y(), 8));
    }

    void Select() override {
        Visual::Material selected_mat;
        selected_mat.set_ambient(Visual::Color::SKYBLUE);

        QuadEdge::CellVertexIterator cellVerts(model_polytope_.cell());
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            SigPushVisualPoint_3r(*v->pos, Visual::Point(selected_mat));
        }

        QuadEdge::CellFaceIterator cellFaces(model_polytope_.cell());
        QuadEdge::Face *f;
        while ((f = cellFaces.next()) != 0) {
            QuadEdge::FaceEdgeIterator faceEdges(f);
            QuadEdge::Edge *e;

            while ((e = faceEdges.next()) != 0) {
                if (e->Org() < e->Dest()) {
                    SigPushVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos),
                                            Visual::Segment(selected_mat));
                }
            }
        }
    }

    void Deselect() override {
        QuadEdge::CellVertexIterator cellVerts(model_polytope_.cell());
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            SigPopVisualPoint_3r(*v->pos);
        }

        QuadEdge::CellFaceIterator cellFaces(model_polytope_.cell());
        QuadEdge::Face *f;
        while ((f = cellFaces.next()) != 0) {
            QuadEdge::FaceEdgeIterator faceEdges(f);
            QuadEdge::Edge *e;

            while ((e = faceEdges.next()) != 0) {
                if (e->Org() < e->Dest()) {
                    SigPopVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos));
                }
            }
        }
    }

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) {
        Intersection::Toleranced::Ray_3rPolytope_3r isect(&ray, &model_polytope_);
        return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPolytope_3r::INTERSECTION_EMPTY, isect.time());
    }

    void UpdateColor(const QColor &color) override {}

    QString scene_object_type() const override {
        return "Polytope";
    }

    const QString& name() const override {
        return name_;
    }
    void set_name(const QString &name) override {
        name_ = name;
    }

private:
    Polytope_3r model_polytope_;
    QString name_;
};

}

#endif // RC_SCENE_POLYTOPE_H
