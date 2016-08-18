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

#include "scene_triangle_soup.h"

namespace RCAD {

//=============================================================================
// Implementation: SceneTriangleSoup_3
//=============================================================================

SceneTriangleSoup_3::SceneTriangleSoup_3() {
    model_triangle_soup_.AddObserver(this);
}

void SceneTriangleSoup_3::Initialize(const QVector<QVector3D>& vertices,
                                     const QVector<qint32>& indices) {
    QVector<SharedPoint_3r> sharedVertices;

    for (auto vertex : vertices) {
        sharedVertices.push_back(std::make_shared<Point_3r>(vertex.x(),
                                                            vertex.y(),
                                                            vertex.z()));
    }

    std::vector<Triangle_3r> triangles;
    for (auto i = 0; i < indices.size(); i += 3) {
        Triangle_3r tri(sharedVertices[indices[i]],
                        sharedVertices[indices[i+1]],
                        sharedVertices[indices[i+2]]);
        triangles.push_back(tri);
    }

    model_triangle_soup_.AddTriangles(triangles);
}

void SceneTriangleSoup_3::Select() {
    Visual::Material selected_mat;
    selected_mat.set_ambient(Visual::Color::SKYBLUE);

    SigBeginBatch();

    auto triangle_soup = model_triangle_soup_.triangle_soup();
    for (auto triangle : triangle_soup) {
        SigPushVisualPoint_3r(triangle.a(), Visual::Point(selected_mat));
        SigPushVisualPoint_3r(triangle.b(), Visual::Point(selected_mat));
        SigPushVisualPoint_3r(triangle.c(), Visual::Point(selected_mat));
        SigPushVisualSegment_3r(Segment_3r(triangle.a_sptr(), triangle.b_sptr()),
                                Visual::Segment(selected_mat));
        SigPushVisualSegment_3r(Segment_3r(triangle.b_sptr(), triangle.c_sptr()),
                                Visual::Segment(selected_mat));
        SigPushVisualSegment_3r(Segment_3r(triangle.c_sptr(), triangle.a_sptr()),
                                Visual::Segment(selected_mat));
    }

    SigEndBatch();
    SigUpdate();
}

void SceneTriangleSoup_3::Deselect() {
    SigBeginBatch();

    auto triangle_soup = model_triangle_soup_.triangle_soup();
    for (auto triangle : triangle_soup) {
        SigPopVisualPoint_3r(triangle.a());
        SigPopVisualPoint_3r(triangle.b());
        SigPopVisualPoint_3r(triangle.c());
        SigPopVisualSegment_3r(Segment_3r(triangle.a_sptr(), triangle.b_sptr()));
        SigPopVisualSegment_3r(Segment_3r(triangle.b_sptr(), triangle.c_sptr()));
        SigPopVisualSegment_3r(Segment_3r(triangle.c_sptr(), triangle.a_sptr()));
    }

    SigEndBatch();
    SigUpdate();
}

Intersection::Ray_3rSceneObject SceneTriangleSoup_3::intersect(const Ray_3r &ray) {
    return Intersection::Ray_3rSceneObject(false, 0);
}

void SceneTriangleSoup_3::UpdateColor(const QColor &color) {}

QString SceneTriangleSoup_3::scene_object_type() const {
    return "TriangleSoup";
}

const QString& SceneTriangleSoup_3::name() const {
    return name_;
}

void SceneTriangleSoup_3::set_name(const QString &name) {
    name_ = name;
}

}
