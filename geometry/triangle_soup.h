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

#ifndef GE_TRIANGLE_SOUP_H
#define GE_TRIANGLE_SOUP_H

#include "common.h"
#include "visual.h"
#include "triangle.h"

namespace RCAD {

class TriangleSoup_3r : public Visual::Geometry {
public:
    TriangleSoup_3r();
    ~TriangleSoup_3r();

    void AddTriangle(const Triangle_3r& triangle);

private:
    std::vector<Triangle_3r> triangle_soup_;

    Visual::Material mat_vertex_;
    Visual::Material mat_edge_;
    Visual::Material mat_face_;
};

} // namespace RCAD

#endif // GE_TRIANGLE_SOUP_H
