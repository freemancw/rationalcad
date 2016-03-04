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
 * @brief KD-Tree types
 */

#ifndef GE_KD_TREE_H
#define GE_KD_TREE_H

#include "common.h"
#include "visual.h"
#include "point.h"
#include "line.h"
#include "triangle.h"

namespace RCAD {

//=============================================================================
// Interface: KdTree_3r
//=============================================================================

struct KdNode_3r {
    KdNode_3r *children[2];
    int split_axis;
    rational split_dist;
    int tri_index;
};

class KdTree_3r {
public:
    KdTree_3r();

    //Ray_3rTriangle_3r IntersectRay(const Ray_3r& r) const;
    rational SampleDistanceFieldAt(const Point_3r& p) const;


private:
    KdNode_3r *root_;
};

//=============================================================================
// Algorithms
//=============================================================================


} // namespace RCAD

#endif // GE_KD_TREE_H
