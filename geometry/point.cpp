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
#include "point.h"

namespace RCAD {

//=============================================================================
// Implementation: ApproximatePoint_3f
//=============================================================================

ApproxPoint_3f::ApproxPoint_3f() {}

ApproxPoint_3f::ApproxPoint_3f(const Point_2r &p) {
    approx_.set_x(p.x().get_d());
    approx_.set_y(p.y().get_d());
    approx_.set_z(0);
}

ApproxPoint_3f::ApproxPoint_3f(const Point_3r &p) {
    approx_.set_x(p.x().get_d());
    approx_.set_y(p.y().get_d());
    approx_.set_z(p.z().get_d());
}

void ApproxPoint_3f::set_z_order(const int32_t z_order) {
    approx_.set_z(z_order);
}

void ApproxPoint_3f::SlotPositionChanged_2r(const Point_2r &p) {
    approx_.set_x(p.x().get_d());
    approx_.set_y(p.y().get_d());
}

void ApproxPoint_3f::SlotPositionChanged_3r(const Point_3r& p) {
    approx_.set_x(p.x().get_d());
    approx_.set_y(p.y().get_d());
    approx_.set_z(p.z().get_d());
}

const Point_3f& ApproxPoint_3f::approx() const {
    return approx_;
}

} // namespace RCAD


