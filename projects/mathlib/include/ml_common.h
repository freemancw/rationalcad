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
 * @date 12/30/2012
 */

#ifndef DDAD_ML_COMMON_H
#define DDAD_ML_COMMON_H

#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4244)
#pragma warning(disable:4800)
#pragma warning(disable:4146)
#include "mpirxx.h"
#pragma warning(pop)
#pragma warning(disable:4100)

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <list>
#include <limits>
#include <array>
#include <memory>
#include <cstdint>
#include <float.h>
#include <tuple>

// logging
//#include "rlog.h"
//#include "rloglocation.h"
//#include "RLogChannel.h"
//#include "RLogTime.h"

#define UNUSED(x) (void)x;

namespace DDAD {

typedef mpz_class integer;
typedef mpq_class rational;

namespace Visual {

//=============================================================================
// Interface: Color
//=============================================================================

class Color {
public:
    Color();
    Color(const unsigned char r, const unsigned char g,
          const unsigned char b, const unsigned char a);

    const unsigned char r() const;
    const unsigned char g() const;
    const unsigned char b() const;
    const unsigned char a() const;
    const std::array<unsigned char, 4>& rgba() const;
    void set_r(const unsigned char r);
    void set_g(const unsigned char g);
    void set_b(const unsigned char b);
    void set_a(const unsigned char a);
    void set_rgba(const std::array<unsigned char, 4>& rgba);

private:
    std::array<unsigned char, 4> rgba_;
};

//=============================================================================
// Interface: Material
//=============================================================================

class Material {
public:
    Material();
    Material(const Color& ambient, const Color& diffuse,
             const Color& specular);

private:
    Color ambient_;
    Color diffuse_;
    Color specular_;
};

//=============================================================================
// Interface: Point
//=============================================================================

class Point {
public:
    Point();
    Point(const Color& color);

    const Color& color() const;
    void set_color(const Color& color);

private:
    Color color_;
    std::string sprite_;
};

//=============================================================================
// Interface: Segment
//=============================================================================

class Segment {
public:
    Segment();
    Segment(const Color& color);

    const Color& color() const;
    void set_color(const Color& color);

private:
    Color color_;
};

//=============================================================================
// Interface: Triangle
//=============================================================================

class Triangle {
public:
    Triangle();
    Triangle(const Color& diffuse);

    const Color& diffuse() const;
    void set_diffuse(const Color& diffuse);

private:
    Color diffuse_;
};

} // namespace Visual


enum Orientation {
    ORIENTATION_LEFT,
    ORIENTATION_RIGHT,
    ORIENTATION_COLINEAR
};

enum Side {
    SIDE_NORTH,
    SIDE_SOUTH,
    SIDE_EAST,
    SIDE_WEST
};

enum Direction {
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_EAST,
    DIRECTION_WEST
};

enum Quadrant {
    QUADRANT_NORTHEAST,
    QUADRANT_NORTHWEST,
    QUADRANT_SOUTHEAST,
    QUADRANT_SOUTHWEST
};

enum Sign {
    SIGN_POSITIVE   =  1,
    SIGN_NEGATIVE   = -1,
    SIGN_ZERO       =  0
};

} // namespace DDAD

#endif // DDAD_ML_COMMON_H
