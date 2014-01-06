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

#include "ml_common.h"
#include "ml_cone.h"
#include "ml_line.h"
#include "ml_wedge.h"
#include "ml_intersection.h"
#include "ml_quadrilateral.h"

namespace RCAD {

//=============================================================================
// Implementation: Matrix_3x3r
//=============================================================================

Direction DirectionConeExitsBasis(const Cone_2r& cone, const Parallelogram_2r& basis) {
    auto a_ray_a = Predicate::OrientationPQR(cone.ray_a(), basis.a());
    //auto a_ray_b = Predicate::OrientationPQR(cone.ray_b(), basis.a());
    auto b_ray_a = Predicate::OrientationPQR(cone.ray_a(), basis.b());
    auto b_ray_b = Predicate::OrientationPQR(cone.ray_b(), basis.b());
    auto c_ray_a = Predicate::OrientationPQR(cone.ray_a(), basis.c());
    auto c_ray_b = Predicate::OrientationPQR(cone.ray_b(), basis.c());
    //auto d_ray_a = Predicate::OrientationPQR(cone.ray_a(), basis.d());
    auto d_ray_b = Predicate::OrientationPQR(cone.ray_b(), basis.d());

    // check for NORTH: d is left of rayb and c is right of raya
    if(d_ray_b == ORIENTATION_LEFT && c_ray_a == ORIENTATION_RIGHT)
        return DIRECTION_NORTH;
    // check for SOUTH: b is left of rayb and a is right of raya
    else if(b_ray_b == ORIENTATION_LEFT && a_ray_a == ORIENTATION_RIGHT)
        return DIRECTION_SOUTH;
    // check for EAST: c is left of rayb and b is right of raya
    else if(c_ray_b == ORIENTATION_LEFT && b_ray_a == ORIENTATION_RIGHT)
        return DIRECTION_EAST;
    else
        return DIRECTION_WEST;
}

struct GCDNode {
    GCDNode() : lchild_(nullptr), rchild_(nullptr) {}
    GCDNode(std::shared_ptr<Parallelogram_2r> p) : lchild_(nullptr),
        rchild_(nullptr), p_(p) {}
    std::shared_ptr<Parallelogram_2r> p_;
    GCDNode* lchild_;
    GCDNode* rchild_;
};

GCDNode* GCDTree(GCDNode* root, IGeometryObserver* obs, int level) {
    if(level == 5) return root;
    GCDNode* lchild = new GCDNode(Construction::Shear(*root->p_, DIRECTION_NORTH, obs));
    GCDNode* rchild = new GCDNode(Construction::Shear(*root->p_, DIRECTION_EAST, obs));
    root->lchild_ = GCDTree(lchild, obs, level+1);
    root->rchild_ = GCDTree(rchild, obs, level+1);
    return root;
}

void Cone_2r::ComputeCIGP() {
    //auto root = GCDTree(new GCDNode(Construction::Z2Neighbors(vertex(), this)), this, 0);

    // construct a guide line through the origin that is parallel to ray_a
    auto vec_a = ray_a().direction();
    Line_2r guide_a(std::make_shared<Point_2r>(0, 0),
                    std::make_shared<Point_2r>(vec_a.x(), vec_a.y()));

    // construct a guide line through the origin that is parallel to ray_b
    auto vec_b = ray_b().direction();
    Line_2r guide_b(std::make_shared<Point_2r>(0, 0),
                    std::make_shared<Point_2r>(vec_b.x(), vec_b.y()));

    std::vector<std::shared_ptr<Parallelogram_2r>> pstack;
    pstack.push_back(Construction::Z2Neighbors(vertex(), this));

    auto uv = pstack.back()->GetVector(DIRECTION_NORTH) +
              pstack.back()->GetVector(DIRECTION_EAST);

    while(Predicate::OrientationPQR(guide_a, uv) == Predicate::OrientationPQR(guide_b, uv)) {
        switch(Predicate::OrientationPQR(guide_a, uv)) {
        case ORIENTATION_LEFT:
            pstack.push_back(Construction::Shear(*pstack.back(),
                                                   DIRECTION_EAST, this));
            break;
        case ORIENTATION_RIGHT:
            pstack.push_back(Construction::Shear(*pstack.back(),
                                                 DIRECTION_NORTH, this));
            break;
        case ORIENTATION_COLINEAR:
            return;
        }
        uv = pstack.back()->GetVector(DIRECTION_NORTH) +
             pstack.back()->GetVector(DIRECTION_EAST);
    }

    auto victory = Construction::Z2Neighbors(vertex()+uv, this);

    /*

    // initialize an approximating parallelogram to the guide
    std::vector<std::shared_ptr<Parallelogram_2r>> pstack_a;
    pstack_a.push_back(Construction::Z2Neighbors(vertex(), this));

    // initialize an approximating parallelogram to the guide
    std::vector<std::shared_ptr<Parallelogram_2r>> pstack_b;
    pstack_b.push_back(Construction::Z2Neighbors(vertex(), this));

    while(1) {
        // do a ===============================================================
        auto uv_a = pstack_a.back()->GetVector(DIRECTION_NORTH) +
                    pstack_a.back()->GetVector(DIRECTION_EAST);

        while(Predicate::OrientationPQR(guide_a, uv_a) == Predicate::OrientationPQR(guide_b, uv_a)) {
            switch(Predicate::OrientationPQR(guide_a, uv_a)) {
            case ORIENTATION_LEFT:
                pstack_a.push_back(Construction::Shear(*pstack_a.back(),
                                                       DIRECTION_EAST, this));
                break;
            case ORIENTATION_RIGHT:
                pstack_a.push_back(Construction::Shear(*pstack_a.back(),
                                                       DIRECTION_NORTH, this));
                break;
            case ORIENTATION_COLINEAR:
                return;
            }
        }


        if(Predicate::OrientationPQR(ray_b(), pstack_b.back()->b()) == ORIENTATION_LEFT) {
            pstack_a.pop_back();
            pstack_a.push_back(Construction::Translate(*pstack_a.back(), DIRECTION_EAST, 1, this));
        } else if(Predicate::OrientationPQR(ray_a(), pstack_a.back()->d()) == ORIENTATION_RIGHT) {
            pstack_a.pop_back();
            pstack_a.push_back(Construction::Translate(*pstack_a.back(), DIRECTION_NORTH, 1, this));
        }

        // do b ===============================================================
        auto uv_b = pstack_b.back()->GetVector(DIRECTION_NORTH) +
                    pstack_b.back()->GetVector(DIRECTION_EAST);

        switch(Predicate::OrientationPQR(guide_b, uv_b)) {
        case ORIENTATION_LEFT:
            pstack_b.push_back(Construction::Shear(*pstack_b.back(),
                                                   DIRECTION_EAST, this));
            break;
        case ORIENTATION_RIGHT:
            pstack_b.push_back(Construction::Shear(*pstack_b.back(),
                                                   DIRECTION_NORTH, this));
            break;
        case ORIENTATION_COLINEAR:
            return;
        }

        if(Predicate::OrientationPQR(ray_b(), pstack_b.back()->b()) == ORIENTATION_LEFT) {
            pstack_b.pop_back();
            pstack_b.push_back(Construction::Translate(*pstack_b.back(), DIRECTION_EAST, 1, this));
        } else if(Predicate::OrientationPQR(ray_b(), pstack_b.back()->d()) == ORIENTATION_RIGHT) {
            pstack_b.pop_back();
            pstack_b.push_back(Construction::Translate(*pstack_b.back(), DIRECTION_NORTH, 1, this));
        }
    }

*/

    /*
    // construct a guide line through the origin that is parallel to ray_a
    auto slope_vec = ray_a().direction();
    Line_2r guide(std::make_shared<Point_2r>(0, 0),
                  std::make_shared<Point_2r>(slope_vec.x(), slope_vec.y()));


    // initialize an approximating parallelogram to the guide
    std::vector<std::shared_ptr<Parallelogram_2r>> bases;
    bases.push_back(Construction::Z2Neighbors(vertex(), this));
    while(!Predicate::AContainsVertexOfB(*this, *bases.back())) {
        auto uv = bases.back()->GetVector(DIRECTION_NORTH) +
                  bases.back()->GetVector(DIRECTION_EAST);

        switch(Predicate::OrientationPQR(guide, uv)) {
        case ORIENTATION_LEFT:
            bases.push_back(Construction::Shear(*bases.back(),
                                                DIRECTION_EAST, this));
            break;
        case ORIENTATION_RIGHT:
            bases.push_back(Construction::Shear(*bases.back(),
                                                DIRECTION_NORTH, this));
            break;
        case ORIENTATION_COLINEAR:
            return;
        }

        if(Predicate::OrientationPQR(ray_a(), bases.back()->b()) == ORIENTATION_LEFT) {
            bases.pop_back();
            bases.push_back(Construction::Translate(*bases.back(), DIRECTION_EAST, 1, this));
        } else if(Predicate::OrientationPQR(ray_a(), bases.back()->d()) == ORIENTATION_RIGHT) {
            bases.pop_back();
            bases.push_back(Construction::Translate(*bases.back(), DIRECTION_NORTH, 1, this));
        }
    }
    */

/*
    if(Predicate::AContainsVertexOfB(*this, *bases.back())) {
        return;
    }

    Direction exit_curr = DirectionConeExitsBasis(*this, *bases.back());
    Direction exit_prev = exit_curr;

    uint32_t t_first = 0;
    do {
        bases.push_back(Construction::Translate(*bases.back(), exit_curr, 1, this));
        exit_prev = exit_curr;
        exit_curr = DirectionConeExitsBasis(*this, *bases.back());
        ++t_first;
    } while(!Predicate::AContainsVertexOfB(*this, *bases.back()) &&
            exit_curr == exit_prev);

    Direction exit_first = exit_prev;

    uint32_t t_second = 0;
    do {
        bases.push_back(Construction::Translate(*bases.back(), exit_curr, 1, this));
        exit_prev = exit_curr;
        exit_curr = DirectionConeExitsBasis(*this, *bases.back());
        ++t_second;
    } while(!Predicate::AContainsVertexOfB(*this, *bases.back()) &&
            exit_curr == exit_prev);

    Direction exit_second = exit_prev;

    if(t_first > t_second) {
        while(t_second--)
            bases.pop_back();
        while(t_first--)
            bases.push_back(Construction::Shear(*bases.back(), exit_first, this));
    }
*/

    // invariant: the cone exits through a single side of the parallelogram

    // invariant: u and v represent upper and lower bounds on the slope of
    // one of the rays of the cone

    // run the mediant process until the invariant is broken.

    //bases.push_back(Construction::Shear(*bases.back(), DIRECTION_NORTH, this));
    //bases.push_back(Construction::Shear(*bases.back(), DIRECTION_EAST, this));
    //bases.push_back(Construction::Translate(*bases.back(), DIRECTION_EAST, 1, this));


    /*
    while(Predicate::AExitsOneSideOfB(*this, bases.back().data()) &&
          (exit_curr == exit_prev)) {
        bases.push_back(Construction::Translate(*bases.back(), DIRECTION_EAST,
                                                1, this));
        exit_prev = exit_curr;
        exit_curr =
    }

    Intersection::Cone_2rBasis_2r isect_c_b(this, bases.back().data());
    Side exit_curr, exit_prev;

    while(exit_curr == exit_prev) {
        bases.push_back(Construction::Translate(*bases.back(), DIRECTION_EAST, 1, this));
        isect_c_b = Intersection::Cone_2rBasis_2r(this, bases.back().data());
        exit_prev = exit_prev;
        exit_curr =
    }
    */
    /*
    std::vector<std::shared_ptr<Basis_2r>> bases;
    bases.push_back(std::make_shared<Basis_2r>(SW, SE, NE, NW));
    bases.back()->AddObserver(this);

    // start with it going out EAST
    // shear EAST until cone goes out NORTH
    // tile NORTH until it goes out WEST
    // pop until it goes out NORTH
    // shear NORTH until it goes out EAST
    // tile EAST until it goes out NORTH

    // tile the basis along the first side the cone exits
    Intersection::Cone_2rBasis_2r isect_c_b(this, bases.back().data());
    auto sexit_prev = isec_c_b.GetExitSide();
    auto sexit_cur = isect_c_b.GetExitSide();
    int i = 0;
    while(Predicate::RaysExitOneSide(isect_c_b) && (sexit_cur == sexit_prev)) {

        sexit_prev = sexit_cur;
        sexit_cur = isect_c_b.GetExitSide();
        while()) {
            bases.push_back(Construction::);
        }
        ++i;
    }

    // tile the basis along the second side the cone exits
    Intersection::Cone_2rBasis_2r isect_c_b(this, bases.back().data());
    auto sexit_prev = isec_c_b.GetExitSide();
    auto sexit_cur = isect_c_b.GetExitSide();
    int j = 0;
    while(Predicate::RaysExitOneSide(isect_c_b) && (sexit_cur == sexit_prev)) {

        sexit_prev = sexit_cur;
        sexit_cur = isect_c_b.GetExitSide();
        while()) {
            bases.push_back(Construction::);
        }
        ++j;
    }
    */




    // for now I am going to assume that neither the first nor the second
    // levels are in the cone.

    // if we reach this point, we have two basic cases: the cone's rays
    // straddle two quadrants, or they both point into a single quadrant.
    // in the first case, we can just shoot appropriately oriented rays
    // toward the cone, and check the two intersections. in the second
    // case, we know how to initialize the parallelogram by the particular
    // quadrant.

    // for now, I am going to assume that both rays point into the first
    // quadrant.

    /*
    std::vector<std::shared_ptr<WedgeStack_2r>> wss;
    wss.push_back(std::make_shared<WedgeStack_2r>());
    wss.back()->AddObserver(this);
    wss.back()->AttachToOrigin(SW);
    wss.back()->Push(std::make_shared<Wedge_2r>(Vector_2r(1, 0), Vector_2r(0, 1)));
    */
    /*
    WedgeStack_2r wstack;
    wstack.AddObserver(this);
    wstack.AttachToOrigin(SW);
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r(1, 0), Vector_2r(0, 1)));
    */
    /*
    while(!Predicate::AContainsB(*this, wss.back()->Top()->ouv())) { */
        /*
        while(Predicate::AIsRightOfB(wss.back()->Top()->ou(), ray_b_)) {
            wss.back()->Push(std::make_shared<Wedge_2r>(wss.back()->Top()->u()+wss.back()->Top()->v(),
                                                        wss.back()->Top()->v()));
        }
        auto top = wss.back()->Top();
        wss.push_back(std::make_shared<WedgeStack_2r>());
        wss.back()->AddObserver(this);
        wss.back()->AttachToOrigin(top->ou_sptr());
        wss.back()->Push(std::make_shared<Wedge_2r>(top->u(), top->v()));
        */
        /*
        while(Predicate::AIsRightOfB(wstack.Top()->ouv(), ray_a_)) {
            wstack.Push(std::make_shared<Wedge_2r>(wstack.Top()->u()+wstack.Top()->v(),
                                                   wstack.Top()->v()));
        }
        while(Predicate::AIsLeftOfB(wstack.Top()->ouv(), ray_b_)) {
            wstack.Push(std::make_shared<Wedge_2r>(wstack.Top()->u(),
                                                   wstack.Top()->v()+wstack.Top()->u()));
        }
        */
        //if the cone is going out of a side, rearrange until it goes out
        // the other side, then flip across the other side and repeat.
        /*
        while(Predicate::AIsLeftOfB(wss.back()->Top()->ouv(), ray_a_)) {
            // rearrange
            wss.back()->Push(std::make_shared<Wedge_2r>(wss.back()->Top()->u(),
                                                        wss.back()->Top()->u()+wss.back()->Top()->v()));
        }
        while(Predicate::AIsLeftOfB(wss.back()->Top()->ov(), ray_a_)) {
            // tile
            auto top = wss.back()->Top();
            wss.push_back(std::make_shared<WedgeStack_2r>());
            wss.back()->AddObserver(this);
            wss.back()->AttachToOrigin(top->ov_sptr());
            //Vector_2r new_v(-top->v().x(), top->v().y());
            wss.back()->Push(std::make_shared<Wedge_2r>(top->u(), top->v()));
            if(Predicate::AContainsB(*this, wss.back()->Top()->ouv())) {
                return;
            }
        }
        while(Predicate::AIsRightOfB(wss.back()->Top()->ov(), ray_b_)) {
            wss.back()->Pop();
        }
        */
        // flip across the top
        /*
        auto top = wss.back()->Top();
        wss.push_back(std::make_shared<WedgeStack_2r>());
        wss.back()->AddObserver(this);
        wss.back()->AttachToOrigin(top->ov_sptr());
        Vector_2r new_v(-top->v().x(), top->v().y());
        wss.back()->Push(std::make_shared<Wedge_2r>(top->u(), new_v));
        */


        // if the cone is going out of side A, rearrange until it goes out of side B, then tile it adjacent to successive side B's until
        // segment B is totally on one side of the cone. Then, reset at the current origin, and rearrange in the opposite direction.
    //}

    //wstack.Pop();
}

namespace Predicate {

bool AContainsB(const Cone_2r& a, const Point_2r& b) {
    return AIsLeftOfB(b, a.ray_a()) && AIsRightOfB(b, a.ray_b());
}

bool AExitsOneSideOfB(const Cone_2r& a, const Parallelogram_2r& b) {
    return false;
}

bool AContainsVertexOfB(const Cone_2r& a, const Parallelogram_2r& b) {
    return AContainsB(a, b.a()) ||
           AContainsB(a, b.b()) ||
           AContainsB(a, b.c()) ||
           AContainsB(a, b.d());
}

} // namespace Predicate

} // namespace RCAD
