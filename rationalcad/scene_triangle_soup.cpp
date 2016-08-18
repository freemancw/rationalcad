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
    //assert(indices.size() % 3 == 0);

    QVector<SharedPoint_3r> sharedVertices;

    for (auto vertex : vertices) {
        sharedVertices.push_back(std::make_shared<Point_3r>(vertex.x(), vertex.y(), vertex.z()));
    }

    for (auto i = 0; i < indices.size(); i += 3) {
        Triangle_3r tri(sharedVertices[indices[i]],
                        sharedVertices[indices[i+1]],
                        sharedVertices[indices[i+2]]);
        model_triangle_soup_.AddTriangle(tri);
    }
}

void SceneTriangleSoup_3::Select() {

}

void SceneTriangleSoup_3::Deselect() {

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
