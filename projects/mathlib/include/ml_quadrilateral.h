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

#ifndef DDAD_ML_QUADRILATERAL_H
#define DDAD_ML_QUADRILATERAL_H

#include "ml_common.h"
#include "ml_observer.h"
#include "ml_point.h"
#include "ml_vector.h"
#include "ml_triangle.h"

namespace DDAD {

class Parallelogram_2r : public VisualGeometry {
public:
    Parallelogram_2r();
    Parallelogram_2r(const Vector_2r& north, const Vector_2r& east,
                     SharedPoint_2r a,
                     IGeometryObserver* obs = nullptr);
    Parallelogram_2r(SharedPoint_2r a, SharedPoint_2r b,
                     SharedPoint_2r c, SharedPoint_2r d,
                     IGeometryObserver* obs = nullptr);
    ~Parallelogram_2r();

    const Segment_2r& GetSide(Direction side) const;
    Vector_2r GetVector(Direction dir) const;

    const Point_2r& a() const;
    const Point_2r& b() const;
    const Point_2r& c() const;
    const Point_2r& d() const;
    SharedPoint_2r a_sptr();
    SharedPoint_2r b_sptr();
    SharedPoint_2r c_sptr();
    SharedPoint_2r d_sptr();

private:
    SharedPoint_2r a_;
    SharedPoint_2r b_;
    SharedPoint_2r c_;
    SharedPoint_2r d_;
    Segment_2r seg_ab_;
    Segment_2r seg_bc_;
    Segment_2r seg_cd_;
    Segment_2r seg_da_;
    Triangle_2r tri_abc_;
    Triangle_2r tri_acd_;
    Visual::Color face_diffuse_;
    Visual::Color edge_diffuse_;
};

namespace Construction {
std::shared_ptr<Parallelogram_2r> Shear(const Parallelogram_2r& basis,
                                        Direction dir,
                                        IGeometryObserver* obs = nullptr);
std::shared_ptr<Parallelogram_2r> Flip(const Parallelogram_2r& basis,
                                       Side side,
                                       IGeometryObserver* obs = nullptr);
std::shared_ptr<Parallelogram_2r> Translate(const Parallelogram_2r& basis,
                                            Direction dir, uint32_t len = 1,
                                            IGeometryObserver* obs = nullptr);
std::shared_ptr<Parallelogram_2r> Z2Neighbors(const Point_2r& p,
                                              IGeometryObserver* obs = nullptr);
}

} // namespace DDAD

#endif // ML_QUADRILATERAL_H
