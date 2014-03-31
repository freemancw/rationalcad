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

/*!
 * @author Clinton Freeman <admin@freemancw.com>
 * @date 2014-03-30
 */

#ifndef RC_ML_TRIANGULATION_H
#define RC_ML_TRIANGULATION_H

#include "ml_common.h"
#include "ml_observer.h"
#include "ml_quadedge.h"

namespace RCAD {

class TerrainMesh_3r : public VisualGeometry {
public:
    TerrainMesh_3r() {}

    void Initialize(const std::vector<Point_3f>& samples) {
         // compute a bounding box of the input points xy coordinates

    }

private:
    QuadEdge::Cell* cell_;
};

} // namespace RCAD

#endif // RC_ML_TRIANGULATION_H
