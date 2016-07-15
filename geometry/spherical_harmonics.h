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

#ifndef GE_SPHERICAL_HARMONICS_H
#define GE_SPHERICAL_HARMONICS_H

#include "common.h"
#include "vector.h"

namespace RCAD {

uint32_t Factorial(const uint32_t x) {
    uint32_t factorial = 1;

    for (uint32_t i = 2; i <= x; ++i) {
        factorial *= i;
    }

    return factorial;
}

/*!
 * The double factorial of a nonnegative integer x is defined as the product of
 * all integers between 1 and x that have the same parity as x.
 */
uint32_t DoubleFactorial(const uint32_t x) {
    uint32_t double_factorial = 1;
    uint32_t start = x % 2 ? 1 : 2;

    for (uint32_t i = start; i <= x; i += 2) {
        double_factorial *= i;
    }

    return double_factorial;
}

/*!
 *
 * \param l - band index
 * \param m - 0 <= m <= l
 * \param x - -1 <= x <= 1
 * \return
 */
double AssociatedLegendrePolynomial(const uint32_t l, const uint32_t m,
                                    const double x) {
    //double Pmm = (-1)^m*DoubleFactorial(2*m-1)*sqrt(1-x*x);
    return 0;
}

}

#endif // GE_SPHERICAL_HARMONICS_H
