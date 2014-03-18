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
 * @date 08/23/2013
 */

#ifndef RC_ML_CONE_H
#define RC_ML_CONE_H

#include "ml_common.h"
#include "ml_rational.h"
#include "ml_vector.h"
#include "ml_line.h"
#include "ml_observer.h"
#include "ml_triangle.h"

namespace RCAD {

class Parallelogram_2r;

//=============================================================================
// Interface: Cone_2r
//=============================================================================

class Cone_2r : public VisualGeometry {
public:
    Cone_2r() {}
    ~Cone_2r() {}

    void ComputeCIGP();

    void Update() {
        //rDebug("updating cone");
        ray_a_.Update();
        ray_b_.Update();
    }

    void set_endpoint_a(SharedPoint_2r a) {
        ray_a_ = Ray_2r(vertex_, a);
        seg_a_ = Segment_2r(vertex_, a);
        interior_.set_b(a);
        SigRegisterPoint_2r(*a);
        SigPushVisualSegment_2r(seg_a_, Visual::Segment());
    }
    void set_endpoint_b(SharedPoint_2r b) {
        ray_b_ = Ray_2r(vertex_, b);
        seg_b_ = Segment_2r(vertex_, b);
        interior_.set_c(b);
        SigRegisterPoint_2r(*b);
        SigPushVisualSegment_2r(seg_b_, Visual::Segment());
        Visual::Triangle vt;
        vt.set_diffuse(Visual::Color(0, 0, 0, 100));
        SigPushVisualTriangle_2r(interior_, vt);
    }
    void set_vertex(SharedPoint_2r vertex) {
        vertex_ = vertex;
        interior_.set_a(vertex);
        SigRegisterPoint_2r(*vertex_);
        SigPushVisualPoint_2r(*vertex_, Visual::Point());
    }
    const Point_2r& vertex() const { return *vertex_; }
    SharedPoint_2r vertex_sptr() { return vertex_; }
    const Ray_2r& ray_a() const { return ray_a_; }
    const Ray_2r& ray_b() const { return ray_b_; }

private:
    SharedPoint_2r vertex_;
    Ray_2r ray_a_;
    Ray_2r ray_b_;
    Segment_2r seg_a_;
    Segment_2r seg_b_;
    Triangle_2r interior_;
};

namespace Predicate {
bool AContainsB(const Cone_2r& a, const Point_2r& b);
bool AExitsOneSideOfB(const Cone_2r& a, const Parallelogram_2r& b);
bool AContainsVertexOfB(const Cone_2r& a, const Parallelogram_2r& b);
}

//=============================================================================
// Interface: Cone_3r
//=============================================================================

/*
class Cone_3r : public VisualGeometry {
public:

private:

};
*/

} // namespace RCAD

#endif // RC_ML_CONE_H
