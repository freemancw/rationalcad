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
 * @date 08/27/2013
 */

#include "ml_common.h"
#include "ml_line.h"
#include "ml_intersection.h"
#include "ml_quadrilateral.h"

namespace RCAD {

//=============================================================================
// Implementation: Parallelogram_2r
//=============================================================================

Parallelogram_2r::Parallelogram_2r() {}

Parallelogram_2r::Parallelogram_2r(const Vector_2r &north,
                                   const Vector_2r &east,
                                   std::shared_ptr<Point_2r> a,
                                   IGeometryObserver *obs) :
    a_(a) {
    b_ = std::make_shared<Point_2r>(*a_+east);
    c_ = std::make_shared<Point_2r>(*b_+north);
    d_ = std::make_shared<Point_2r>(*a_+north);
    seg_ab_ = Segment_2r(a_, b_);
    seg_bc_ = Segment_2r(b_, c_);
    seg_cd_ = Segment_2r(c_, d_);
    seg_da_ = Segment_2r(d_, a_);
    tri_abc_ = Triangle_2r(a_, b_, c_);
    tri_acd_ = Triangle_2r(a_, c_, d_);

    if(obs != nullptr) {
        AddObserver(obs);
        SigRegisterPoint_2r(*a_);
        SigRegisterPoint_2r(*b_);
        SigRegisterPoint_2r(*c_);
        SigRegisterPoint_2r(*d_);
        //SigPushVisualSegment(seg_ab_, RSF_NORMAL);
        //SigPushVisualSegment(seg_bc_, RSF_NORMAL);
        //SigPushVisualSegment(seg_cd_, RSF_NORMAL);
        //SigPushVisualSegment(seg_da_, RSF_NORMAL);
        face_diffuse_ = Visual::Color(0, 0, 0, 100);
        //SigPushVisualTriangle_2r(tri_abc_, Visual::Triangle(face_diffuse_));
        //SigPushVisualTriangle_2r(tri_acd_, Visual::Triangle(face_diffuse_), 1000);
    }
}

Parallelogram_2r::Parallelogram_2r(std::shared_ptr<Point_2r> a,
                                   std::shared_ptr<Point_2r> b,
                                   std::shared_ptr<Point_2r> c,
                                   std::shared_ptr<Point_2r> d,
                                   IGeometryObserver *obs) :
    a_(a),
    b_(b),
    c_(c),
    d_(d) {
    seg_ab_ = Segment_2r(a_, b_);
    seg_bc_ = Segment_2r(b_, c_);
    seg_cd_ = Segment_2r(c_, d_);
    seg_da_ = Segment_2r(d_, a_);
    tri_abc_ = Triangle_2r(a_, b_, c_);
    tri_acd_ = Triangle_2r(a_, c_, d_);

    if(obs != nullptr) {
        AddObserver(obs);
        SigRegisterPoint_2r(*a_);
        SigRegisterPoint_2r(*b_);
        SigRegisterPoint_2r(*c_);
        SigRegisterPoint_2r(*d_);
        //SigPushVisualSegment(seg_ab_, RSF_NORMAL);
        //SigPushVisualSegment(seg_bc_, RSF_NORMAL);
        //SigPushVisualSegment(seg_cd_, RSF_NORMAL);
        //SigPushVisualSegment(seg_da_, RSF_NORMAL);
        face_diffuse_ = Visual::Color(0, 0, 0, 100);
        //SigPushVisualTriangle_2r(tri_abc_, Visual::Triangle(face_diffuse_));
        //SigPushVisualTriangle_2r(tri_acd_, Visual::Triangle(face_diffuse_), 4000);
    }
}

Parallelogram_2r::~Parallelogram_2r() {
    /*
    SignalPopSegmentFlags(seg_ab_);
    SignalPopSegmentFlags(seg_bc_);
    SignalPopSegmentFlags(seg_cd_);
    SignalPopSegmentFlags(seg_da_);
    SignalPopTriangleState(tri_abc_);
    SignalPopTriangleState(tri_acd_);
    */
}

const Segment_2r& Parallelogram_2r::GetSide(Direction side) const {
    switch(side) {
    default:
    case DIRECTION_NORTH: return seg_cd_;
    case DIRECTION_SOUTH: return seg_ab_;
    case DIRECTION_EAST: return seg_bc_;
    case DIRECTION_WEST: return seg_da_;
    }
}

Vector_2r Parallelogram_2r::GetVector(Direction dir) const {
    switch(dir) {
    default:
    case DIRECTION_NORTH: return -seg_da_.support().V();
    case DIRECTION_SOUTH: return seg_da_.support().V();
    case DIRECTION_EAST: return seg_ab_.support().V();
    case DIRECTION_WEST: return -seg_ab_.support().V();
    }
}

// Accessors/Mutators =========================================================

const Point_2r& Parallelogram_2r::a() const {
    return *a_;
}
const Point_2r& Parallelogram_2r::b() const {
    return *b_;
}
const Point_2r& Parallelogram_2r::c() const {
    return *c_;
}
const Point_2r& Parallelogram_2r::d() const {
    return *d_;
}
std::shared_ptr<Point_2r> Parallelogram_2r::a_sptr() {
    return a_;
}
std::shared_ptr<Point_2r> Parallelogram_2r::b_sptr() {
    return b_;
}
std::shared_ptr<Point_2r> Parallelogram_2r::c_sptr() {
    return c_;
}
std::shared_ptr<Point_2r> Parallelogram_2r::d_sptr() {
    return d_;
}

// Constructions ==============================================================

namespace Construction {

std::shared_ptr<Parallelogram_2r> Shear(const Parallelogram_2r& basis,
                                        Direction dir, IGeometryObserver* obs) {
    auto a = std::make_shared<Point_2r>(basis.a());
    auto b = std::make_shared<Point_2r>(basis.b());
    auto c = std::make_shared<Point_2r>(basis.c());
    auto d = std::make_shared<Point_2r>(basis.d());
    auto t = basis.GetVector(dir);

    switch(dir) {
    case DIRECTION_NORTH:
    case DIRECTION_SOUTH:
        (*b) += t;
        (*c) += t;
        break;
    case DIRECTION_EAST:
    case DIRECTION_WEST:
        (*c) += t;
        (*d) += t;
        break;
    }

    return std::make_shared<Parallelogram_2r>(a, b, c, d, obs);
}

std::shared_ptr<Parallelogram_2r> Translate(const Parallelogram_2r& basis,
                                            Direction dir, uint32_t len,
                                            IGeometryObserver* obs) {
    auto a = std::make_shared<Point_2r>(basis.a());
    auto b = std::make_shared<Point_2r>(basis.b());
    auto c = std::make_shared<Point_2r>(basis.c());
    auto d = std::make_shared<Point_2r>(basis.d());
    auto t = basis.GetVector(dir);
    (*a) += t*len;
    (*b) += t*len;
    (*c) += t*len;
    (*d) += t*len;
    return std::make_shared<Parallelogram_2r>(a, b, c, d, obs);
}

std::shared_ptr<Parallelogram_2r> Z2Neighbors(const Point_2r &p,
                                              IGeometryObserver *obs) {
    auto a = std::make_shared<Point_2r>(Z2Neighbor(p, QUADRANT_SOUTHWEST));
    auto b = std::make_shared<Point_2r>(Z2Neighbor(p, QUADRANT_SOUTHEAST));
    auto c = std::make_shared<Point_2r>(Z2Neighbor(p, QUADRANT_NORTHEAST));
    auto d = std::make_shared<Point_2r>(Z2Neighbor(p, QUADRANT_NORTHWEST));
    return std::make_shared<Parallelogram_2r>(a, b, c, d, obs);
}

} // namespace Construction

} // namespace RCAD


