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

#ifndef RC_SCENE_TERRAINMESH_H
#define RC_SCENE_TERRAINMESH_H

#include "scene.h"

namespace RCAD {

//=============================================================================
// Interface: SceneTerrainMesh_3
//=============================================================================

class SceneTerrainMesh_3 : public ISceneObject, public Visual::Geometry {
public:
    SceneTerrainMesh_3() {
        //model_terrain_mesh_.AddObserver(this);
    }

    void Initialize(const std::vector<Point_3f>& data) {
        //model_terrain_mesh_.Initialize(data);
    }

    void Select() override {}
    void Deselect() override {}

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) {
        return Intersection::Ray_3rSceneObject();
    }

    void UpdateColor(const QColor &color) override {}

    QString scene_object_type() const override {
        return "TerrainMesh";
    }

    const QString& name() const override {
        return name_;
    }
    void set_name(const QString &name) override {
        name_ = name;
    }

private:
    //TerrainMesh_3r model_terrain_mesh_;
    QString name_;
};

}

#endif // RC_SCENE_TERRAINMESH_H
