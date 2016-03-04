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

#ifndef GE_SPLINE_H
#define GE_SPLINE_H

#include "common.h"
#include "visual.h"

// Rough draft code from the NURBS book

namespace RCAD {

class BSplineSurface_3r : public Visual::Geometry {
public:
    constexpr BSplineSurface_3r() noexcept {}
    ~BSplineSurface_3r() {}

    /*!
     * \brief FindSpan
     * \param n
     * \param p - degree
     * \param u -
     * \param U - knot vector
     * \return
     */
    uint32_t FindSpan(const uint32_t n, const uint32_t p, const rational& u,
                      const std::vector<rational>& U) const noexcept {
        if (u == U[n+1]) {
            return n;
        }

        auto low = p;
        auto high = n+1;
        auto mid = (low+high)/2;

        while (u < U[mid] || u >= U[mid+1]) {
            if (u < U[mid]) {
                high = mid;
            } else {
                low = mid;
            }
            mid = (low+high)/2;
        }

        return mid;
    }

    /*!
     * \brief BasisFuns
     * \param i
     * \param u
     * \param p
     * \param U
     * \return
     */
    std::vector<rational> BasisFuns(const uint32_t i, rational u, uint32_t p,
                                    const std::vector<rational>& U) {
        std::vector<rational> N, left, right;
        N.push_back(1.0);

        for (int j = 1; j <= p; ++j) {
            left[j] = u-U[i+1-j];
            right[j] = U[i+j]-u;
            auto saved = 0.0;

            for (int r = 0; r < j; ++r) {
                auto temp = N[r]/(right[r+1]+left[j-r]);
                N[r] = saved+right[r+1]*temp;
                saved = left[j-r]*temp;
            }
            N[j] = saved;
        }

        return N;
    }

    constexpr std::vector<SharedPoint_3r>& control_points() const noexcept {
        return control_points_;
    }

private:
    std::vector<SharedPoint_3r> control_points_;
};

}

#endif // GE_SPLINE_H
