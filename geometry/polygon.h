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
 * @brief 2-dimensional polyline and polygon types.
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-02-12
 */

#ifndef GE_POLYGON_H
#define GE_POLYGON_H

#include "common.h"
#include "visual.h"
#include "point.h"
#include "line.h"
#include "triangle.h"

namespace RCAD {

//=============================================================================
// Interface: Polyline_2r
//=============================================================================

class Polyline_2r : public Visual::Geometry {
public:
    Polyline_2r();
    ~Polyline_2r();

    void Close();
    void Open();
    void RotateToMaxX();

    void push_back(const Point_2r& v);
    void push_back(SharedPoint_2r v);
    void pop_back();
    SharedPoint_2r back(const size_t i = 0) const;

    void push_front(const Point_2r& v);
    void push_front(SharedPoint_2r v);
    void pop_front();
    SharedPoint_2r front(const size_t i = 0) const;

    SharedPoint_2r operator[](const size_t i) const;
    const size_t size() const;

    const std::deque<SharedPoint_2r>& vertices() const;
    void set_vertices(const std::deque<SharedPoint_2r>& vertices);
    const bool closed() const;

private:
    std::deque<SharedPoint_2r> vertices_;
    Visual::Material mat_vertex_;
    Visual::Material mat_edge_;
    bool closed_;
};

//=============================================================================
// Interface: Polygon_2r
//=============================================================================

class Polygon_2r : public Visual::Geometry {
public:
    Polygon_2r();
    ~Polygon_2r();

    void push_back(const Point_2r& v);
    void push_back(SharedPoint_2r v);
    void pop_back();
    SharedPoint_2r back(const size_t i = 0) const;

    void push_front(const Point_2r& v);
    void push_front(SharedPoint_2r v);
    void pop_front();
    SharedPoint_2r front(const size_t i = 0) const;

    SharedPoint_2r operator[](const size_t i) const;
    const size_t size() const;

    void CloseBoundary();

    const Polyline_2r& boundary() const;

private:
    Polyline_2r boundary_;
    Visual::Material mat_face_;
};

Polygon_2r Melkman(const Polyline_2r& P, Visual::IGeometryObserver* observer = nullptr);
Polygon_2r IntegerHull(const Polygon_2r& P, Visual::IGeometryObserver* observer = nullptr);

} // namespace RCAD

#endif // GE_POLYGON_H
