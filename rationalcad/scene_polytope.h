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
    ScenePolytope_3();

    void Initialize(const QVector2D& start, const QVector2D& cur);

    void Update(const QVector2D& cur);

    void Select() override;

    void Deselect() override;

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) override;

    void UpdateColor(const QColor &color) override;

    QString scene_object_type() const override;

    const QString& name() const override;

    void set_name(const QString &name) override;

private:
    Polytope_3r model_polytope_;
    QString name_;
};

}

#endif // RC_SCENE_POLYTOPE_H
