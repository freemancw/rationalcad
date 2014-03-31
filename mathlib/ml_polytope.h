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
 * @brief 3-dimensional polytope types.
 * @author Clinton Freeman <admin@freemancw.com>
 * @date 2013-10-23
 */

#ifndef RC_ML_POLYTOPE_H
#define RC_ML_POLYTOPE_H

#include "ml_common.h"
#include "ml_observer.h"
#include "ml_point.h"
#include "ml_line.h"
#include "ml_triangle.h"

namespace RCAD {

namespace QuadEdge {
class Cell;
}

//=============================================================================
// Interface: Polytope_3r
//=============================================================================

class Polytope_3r : public VisualGeometry {
public:
    Polytope_3r();

    void Initialize(const Point_3f& start, const Point_3f& cur);
    void Update(const Point_3f& cur);
    
private:
    QuadEdge::Cell* cell_;
    Point_3f start_;
};

namespace Construction {

Polytope_3r ConvexHull(const std::vector<SharedPoint_3r>& P);
Polytope_3r IntegerHull(const Polytope_3r& P);

}

//=============================================================================
// Interface: Tetrahedron_3r
//=============================================================================

/*
class Tetrahedron_3r : public VisualGeometry {
public:
    Tetrahedron_3r();

private:
    SharedPoint_3r a_;
    SharedPoint_3r b_;
    SharedPoint_3r c_;
    SharedPoint_3r d_;
};
*/

} // namespace RCAD

#endif // RC_ML_POLYTOPE_H
