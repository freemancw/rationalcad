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

#include "common.h"
#include "triangle_soup.h"

using namespace RCAD::Visual;

namespace RCAD {

//=============================================================================
// Implementation: TriangleSoup_3r
//=============================================================================

TriangleSoup_3r::TriangleSoup_3r() {
    mat_face_.set_diffuse(Color::WHITE);
    mat_face_.set_ambient(Color::WHITE);
}

TriangleSoup_3r::~TriangleSoup_3r() {
    for (auto triangle : triangle_soup_) {
        SigPopVisualPoint_3r(triangle.a());
        SigPopVisualPoint_3r(triangle.b());
        SigPopVisualPoint_3r(triangle.c());
        SigPopVisualSegment_3r(Segment_3r(triangle.a_sptr(), triangle.b_sptr()));
        SigPopVisualSegment_3r(Segment_3r(triangle.b_sptr(), triangle.c_sptr()));
        SigPopVisualSegment_3r(Segment_3r(triangle.c_sptr(), triangle.a_sptr()));
        SigPopVisualTriangle_3r(triangle);
    }
}

void TriangleSoup_3r::AddTriangle(Triangle_3r &triangle) {
    triangle_soup_.push_back(triangle);

    SigRegisterPoint_3r(triangle.a());
    SigPushVisualPoint_3r(triangle.a(), Visual::Point(mat_vertex_));

    SigRegisterPoint_3r(triangle.b());
    SigPushVisualPoint_3r(triangle.b(), Visual::Point(mat_vertex_));

    SigRegisterPoint_3r(triangle.c());
    SigPushVisualPoint_3r(triangle.c(), Visual::Point(mat_vertex_));

    SigPushVisualSegment_3r(Segment_3r(triangle.a_sptr(), triangle.b_sptr()),
                            Visual::Segment(mat_edge_));

    SigPushVisualSegment_3r(Segment_3r(triangle.b_sptr(), triangle.c_sptr()),
                            Visual::Segment(mat_edge_));

    SigPushVisualSegment_3r(Segment_3r(triangle.c_sptr(), triangle.a_sptr()),
                            Visual::Segment(mat_edge_));

    SigPushVisualTriangle_3r(triangle, Visual::Triangle(mat_face_));
}

}
