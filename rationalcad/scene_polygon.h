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

#ifndef RC_SCENE_POLYGON_H
#define RC_SCENE_POLYGON_H

#include "scene.h"

namespace RCAD {

//=============================================================================
// Interface: ScenePolygon_2
//=============================================================================

class ScenePolygon_2 : public ISceneObject, public Visual::Geometry {
public:
    ScenePolygon_2();
    ~ScenePolygon_2();

    void Initialize(const QVector2D& start);

    void Update(const QVector2D& cur);

    void Select() override;

    void Deselect() override;

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) override;

    void UpdateColor(const QColor &color) override;

    QString scene_object_type() const override;

    const QString& name() const override;

    void set_name(const QString &name) override;

    const Polygon_2r& model_polygon() const;

private:
    Polygon_2r model_polygon_;
    QString name_;
};

}

#endif // RC_SCENE_POLYGON_H
