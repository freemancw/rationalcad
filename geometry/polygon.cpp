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
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-07-21
 */

#include "common.h"
#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "polygon.h"
#include "wedge.h"

using namespace RCAD::Predicate;

namespace RCAD {

//=============================================================================
// Algorithms
//=============================================================================

Polygon_2r Melkman(const Polyline_2r& P, Visual::IGeometryObserver* observer) {
    Polygon_2r hull;
    hull.AddObserver(observer);

    // initialize hull
    hull.push_back(P[0]);
    hull.push_back(P[1]);
    hull.push_back(P[0]);

    for (size_t i = 2; i < P.size(); ++i) {
        if (!RIsLeftOrInsidePQ(*hull.back(1), *hull.back(0), *P[i]) ||
            !RIsLeftOrInsidePQ(*P[i], *hull.front(1), *hull.front(0))) {
            while (!RIsLeftOrInsidePQ(*hull.back(1), *hull.back(0), *P[i])) {
                hull.pop_back();
            }
            while (!RIsLeftOrInsidePQ(*P[i], *hull.front(0), *hull.front(1))) {
                hull.pop_front();
            }
            hull.push_back(P[i]);
            hull.push_front(P[i]);
        }
    }

    return hull;
}

Polygon_2r IntegerHull(const Polygon_2r& P, Visual::IGeometryObserver *observer) {
    Polygon_2r ihull;
    ihull.AddObserver(observer);

    /*
    // canonicalize the boundary chain
    boundary_.RotateToMaxX();

    // grab the rightmost vertex
    auto vert_itr_max_x = begin(boundary_.vertices());

    // find the bottommost vertex
    bool (*cmp)(const PolyChainVertex_2r&,
                const PolyChainVertex_2r&) = Predicate::AIsBelowB;
    auto vert_itr_min_y = std::min_element(begin(boundary_.vertices()),
                                           end(boundary_.vertices()), cmp);

    // construct an initial z and use it to init the tentative integer hull
    Point_2r z(Floor(vert_itr_max_x->vertex().x()),
               Floor(vert_itr_min_y->vertex().y()));
    PolyChain_2r tentative_hull;
    tentative_hull.AddObserver(this);
    tentative_hull.AppendVertex(z);

    // initialize wedge stack
    WedgeStack_2r wstack;
    wstack.AddObserver(this);
    wstack.AttachToOrigin(tentative_hull.back().vertex_sptr());
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 0, 1), Vector_2r(-1, 0)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 1, 0), Vector_2r( 0, 1)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 0,-1), Vector_2r( 1, 0)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r(-1, 0), Vector_2r( 0,-1)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 0, 1), Vector_2r(-1, 0)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 1, 0), Vector_2r( 0, 1)));

    auto vert_itr_P = vert_itr_max_x;
    auto L = Line_2r(SLOPE_PINFINITY, vert_itr_P->vertex().x());

    while (!wstack.empty()) {
        auto wtop = wstack.Pop();
        auto ray_zv = Ray_2r(tentative_hull.back().vertex_sptr(), wtop->v());
        auto isect_L_zv = Intersection::Line_2rRay_2r(&L, &ray_zv);

        if (Predicate::AreParallel(L, ray_zv) || !Predicate::IsEmpty(isect_L_zv)) {
            while (Predicate::OrientationPQR(ray_zv, vert_itr_P->vertex()) != ORIENTATION_LEFT) {
                if (vert_itr_P == std::prev(end(boundary_.vertices()))) {
                    vert_itr_P = begin(boundary_.vertices());
                } else {
                    vert_itr_P++;
                }

                L = vert_itr_P->edge_prev().support();

                if (Predicate::AIsRightOfB(z, L)) {
                    return;
                }
            }
            z = Construction::LastBefore(L, ray_zv);
            tentative_hull.AppendVertex(z);
            wstack.AttachToOrigin(tentative_hull.back().vertex_sptr());
        } else {
            wstack.Push(std::make_shared<Wedge_2r>(wtop->u()+wtop->v(), wtop->v()));
            wstack.Push(std::make_shared<Wedge_2r>(wtop->u(), wtop->u()+wtop->v()));
        }
    }

    do {
        tentative_hull.RemoveFront();
    } while (begin(tentative_hull.vertices())->vertex() !=
             std::prev(end(tentative_hull.vertices()))->vertex());
             */

    return ihull;
}

//=============================================================================
// Polygon_2r
//=============================================================================

Polygon_2r::Polygon_2r() {
    boundary_.AddObserver(this);
}

Polygon_2r::~Polygon_2r() {

}

void Polygon_2r::push_back(const Point_2r& v) {
    push_back(std::make_shared<Point_2r>(v));
}

void Polygon_2r::push_back(SharedPoint_2r v) {
    boundary_.push_back(v);
}

void Polygon_2r::pop_back() {
    boundary_.pop_back();
}

SharedPoint_2r Polygon_2r::back(const size_t i) const {
    return boundary_.back(i);
}

void Polygon_2r::push_front(const Point_2r& v) {
    push_front(std::make_shared<Point_2r>(v));
}

void Polygon_2r::push_front(SharedPoint_2r v) {
    boundary_.push_front(v);
}

void Polygon_2r::pop_front() {
    boundary_.pop_front();
}

SharedPoint_2r Polygon_2r::front(const size_t i) const {
    return boundary_.front(i);
}

const size_t Polygon_2r::size() const {
    return boundary_.size();
}

SharedPoint_2r Polygon_2r::operator[](const size_t i) const {
    return boundary_[i];
}

void Polygon_2r::CloseBoundary() {
    boundary_.Close();
}

const Polyline_2r& Polygon_2r::boundary() const {
    return boundary_;
}

//=============================================================================
// Polyline_2r
//=============================================================================

Polyline_2r::Polyline_2r() :
    closed_(false) {}

Polyline_2r::~Polyline_2r() {

}

void Polyline_2r::Close() {

}

void Polyline_2r::Open() {

}

void Polyline_2r::RotateToMaxX() {
    /*
    bool (*compare)(const PolyChainVertex_2r&,
                    const PolyChainVertex_2r&) = Predicate::AIsLeftOfB;
    auto vmax_x = std::max_element(vertices_.begin(), vertices_.end(),
                                   compare);
    std::rotate(vertices_.begin(), vmax_x, vertices_.end());
    */
}

void Polyline_2r::push_back(const Point_2r& v) {
    push_back(std::make_shared<Point_2r>(v));
}

void Polyline_2r::push_back(SharedPoint_2r v) {
    LOG(INFO) << "push_back vertex to polyline_2r";
    vertices_.push_back(v);

    SigRegisterPoint_2r(*v);
    SigPushVisualPoint_2r(*v, Visual::Point(mat_vertex_));

    if (vertices_.size() > 1) {
        SigPushVisualSegment_2r(Segment_2r(back(1), back(0)),
                                Visual::Segment(mat_edge_));
    }
}

void Polyline_2r::pop_back() {
    vertices_.pop_back();
}

SharedPoint_2r Polyline_2r::back(const size_t i) const {
    return vertices_[vertices_.size()-1-i];
}

void Polyline_2r::push_front(const Point_2r& v) {
    push_front(std::make_shared<Point_2r>(v));
}

void Polyline_2r::push_front(SharedPoint_2r v) {
    vertices_.push_front(v);

    SigRegisterPoint_2r(*v);
    SigPushVisualPoint_2r(*v, Visual::Point(mat_vertex_));

    if (vertices_.size() > 1) {
        SigPushVisualSegment_2r(Segment_2r(front(0), front(1)),
                                Visual::Segment(mat_edge_));
    }
}

void Polyline_2r::pop_front() {
    vertices_.pop_front();
}

SharedPoint_2r Polyline_2r::front(const size_t i) const {
    return vertices_[i];
}

const size_t Polyline_2r::size() const {
    return vertices_.size();
}

SharedPoint_2r Polyline_2r::operator [](const size_t i) const {
    return vertices_[i];
}

const std::deque<SharedPoint_2r>& Polyline_2r::vertices() const {
    return vertices_;
}

void Polyline_2r::set_vertices(const std::deque<SharedPoint_2r>& vertices) {
    vertices_ = vertices;
}

const bool Polyline_2r::closed() const {
    return closed_;
}

} // namespace RCAD
