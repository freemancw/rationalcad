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
 * @date 08/14/2013
 */

#ifndef RC_ML_TRIANGLE_H
#define RC_ML_TRIANGLE_H

#include "ml_common.h"
#include "ml_point.h"
#include "ml_line.h"

namespace RCAD {

class Triangle_2r {
public:
    Triangle_2r();
    Triangle_2r(SharedPoint_2r a, SharedPoint_2r b,
                SharedPoint_2r c);

    const Point_2r& a() const;
    const Point_2r& b() const;
    const Point_2r& c() const;
    void set_a(SharedPoint_2r a);
    void set_b(SharedPoint_2r b);
    void set_c(SharedPoint_2r c);

    SharedPoint_2r a_sptr();
    SharedPoint_2r b_sptr();
    SharedPoint_2r c_sptr();

private:
    SharedPoint_2r a_;
    SharedPoint_2r b_;
    SharedPoint_2r c_;
};

std::ostream& operator<<(std::ostream& o, const Triangle_2r& tri);

class Triangle_3r {
public:
    Triangle_3r();
    Triangle_3r(SharedPoint_3r a, SharedPoint_3r b,
                SharedPoint_3r c);

    const Point_3r& a() const;
    const Point_3r& b() const;
    const Point_3r& c() const;
    void set_a(SharedPoint_3r a);
    void set_b(SharedPoint_3r b);
    void set_c(SharedPoint_3r c);

    SharedPoint_3r a_sptr();
    SharedPoint_3r b_sptr();
    SharedPoint_3r c_sptr();

private:
    SharedPoint_3r a_;
    SharedPoint_3r b_;
    SharedPoint_3r c_;
};

std::ostream& operator<<(std::ostream& o, const Triangle_3r& tri);

} // namespace RCAD

#endif // RC_ML_TRIANGLE_H