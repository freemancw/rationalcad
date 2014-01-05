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
 * @date 10/26/2013
 */

#include "ml_common.h"

namespace DDAD {

namespace Visual {

//=============================================================================
// Implementation: Color
//=============================================================================

Color::Color() {
    rgba_.assign(0);
}

Color::Color(const unsigned char r, const unsigned char g,
             const unsigned char b, const unsigned char a) {
    rgba_[0] = r;
    rgba_[1] = g;
    rgba_[2] = b;
    rgba_[3] = a;
}

const unsigned char Color::r() const {
    return rgba_[0];
}
const unsigned char Color::g() const {
    return rgba_[1];
}
const unsigned char Color::b() const {
    return rgba_[2];
}
const unsigned char Color::a() const {
    return rgba_[3];
}
const std::array<unsigned char, 4>& Color::rgba() const {
    return rgba_;
}
void Color::set_r(const unsigned char r) {
    rgba_[0] = r;
}
void Color::set_g(const unsigned char g) {
    rgba_[1] = g;
}
void Color::set_b(const unsigned char b) {
    rgba_[2] = b;
}
void Color::set_a(const unsigned char a) {
    rgba_[3] = a;
}
void Color::set_rgba(const std::array<unsigned char, 4>& rgba) {
    rgba_ = rgba;
}

//=============================================================================
// Implementation: Material
//=============================================================================

Material::Material() {}

Material::Material(const Color& ambient, const Color& diffuse,
                   const Color& specular) :
    ambient_(ambient),
    diffuse_(diffuse),
    specular_(specular) {}

//=============================================================================
// Implementation: Point
//=============================================================================

Point::Point() :
    color_(0, 0, 0, 255) {}

Point::Point(const Color& color) :
    color_(color) {}

const Color& Point::color() const {
    return color_;
}
void Point::set_color(const Color& color) {
    color_ = color;
}

//=============================================================================
// Implementation: Segment
//=============================================================================

Segment::Segment() :
    color_(0, 0, 0, 255) {}

Segment::Segment(const Color& color) :
    color_(color) {}

const Color& Segment::color() const {
    return color_;
}
void Segment::set_color(const Color& color) {
    color_ = color;
}

//=============================================================================
// Implementation: Triangle
//=============================================================================

Triangle::Triangle() {}

Triangle::Triangle(const Color& diffuse) :
    diffuse_(diffuse) {}

const Color& Triangle::diffuse() const {
    return diffuse_;
}
void Triangle::set_diffuse(const Color& diffuse) {
    diffuse_ = diffuse;
}

} // namespace Visual

} // namespace DDAD
