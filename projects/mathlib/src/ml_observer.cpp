/*
 * This file is part of the Degree-Driven Algorithm Design Project (DDAD).
 *
 * DDAD is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \author {Clinton Freeman <freeman@cs.unc.edu>}
 * \date 07/20/2013
 */

#include "ml_common.h"
#include "ml_observer.h"

namespace DDAD {

VisualGeometry::~VisualGeometry() {}

void VisualGeometry::SlotRegisterPoint_2r(Point_2r& p) {
    SigRegisterPoint_2r(p);
}

void VisualGeometry::SlotPushVisualPoint_2r(const Point_2r& p,
                                         const Visual::Point& vp,
                                         const uint32_t msec_delay) {
    SigPushVisualPoint_2r(p, vp, msec_delay);
}

void VisualGeometry::SlotPushVisualSegment_2r(const Segment_2r& s,
                                           const Visual::Segment& vs,
                                           const uint32_t msec_delay) {
    SigPushVisualSegment_2r(s, vs, msec_delay);
}

void VisualGeometry::SlotPushVisualTriangle_2r(const Triangle_2r &t,
                                            const Visual::Triangle& vt,
                                            const uint32_t msec_delay) {
    SigPushVisualTriangle_2r(t, vt, msec_delay);
}

void VisualGeometry::SlotPopVisualPoint_2r(const Point_2r& p,
                                        const uint32_t msec_delay) {
    SigPopVisualPoint_2r(p, msec_delay);
}

void VisualGeometry::SlotPopVisualSegment_2r(const Segment_2r& s,
                                          const uint32_t msec_delay) {
    SigPopVisualSegment_2r(s, msec_delay);
}

void VisualGeometry::SlotPopVisualTriangle_2r(const Triangle_2r &t,
                                           const uint32_t msec_delay) {
    SigPopVisualTriangle_2r(t, msec_delay);
}

void VisualGeometry::SlotRegisterPoint_3r(Point_3r& p) {
    SigRegisterPoint_3r(p);
}

void VisualGeometry::SlotPushVisualPoint_3r(const Point_3r& p,
                                         const Visual::Point& vp,
                                         const uint32_t msec_delay) {
    SigPushVisualPoint_3r(p, vp, msec_delay);
}

void VisualGeometry::SlotPushVisualSegment_3r(const Segment_3r& s,
                                           const Visual::Segment& vs,
                                           const uint32_t msec_delay) {
    SigPushVisualSegment_3r(s, vs, msec_delay);
}

void VisualGeometry::SlotPushVisualTriangle_3r(const Triangle_3r &t,
                                            const Visual::Triangle& vt,
                                            const uint32_t msec_delay) {
    SigPushVisualTriangle_3r(t, vt, msec_delay);
}

void VisualGeometry::SlotPopVisualPoint_3r(const Point_3r& p,
                                        const uint32_t msec_delay) {
    SigPopVisualPoint_3r(p, msec_delay);
}

void VisualGeometry::SlotPopVisualSegment_3r(const Segment_3r& s,
                                          const uint32_t msec_delay) {
    SigPopVisualSegment_3r(s, msec_delay);
}

void VisualGeometry::SlotPopVisualTriangle_3r(const Triangle_3r &t,
                                           const uint32_t msec_delay) {
    SigPopVisualTriangle_3r(t, msec_delay);
}

void VisualGeometry::SlotUpdate() {
    SigUpdate();
}

void VisualGeometry::AddObserver(IGeometryObserver* geom_observer) {
    observers_.push_back(geom_observer);
}

void VisualGeometry::SigRegisterPoint_2r(Point_2r &p) {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotRegisterPoint_2r(p);
    }
}

void VisualGeometry::SigPushVisualPoint_2r(const Point_2r& p,
                                        const Visual::Point& vp,
                                        const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualPoint_2r(p, vp, msec_delay);
    }
}

void VisualGeometry::SigPushVisualSegment_2r(const Segment_2r& s,
                                          const Visual::Segment& vs,
                                          const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualSegment_2r(s, vs, msec_delay);
    }
}

void VisualGeometry::SigPushVisualTriangle_2r(const Triangle_2r& t,
                                           const Visual::Triangle& vt,
                                           const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualTriangle_2r(t, vt, msec_delay);
    }
}

void VisualGeometry::SigPopVisualPoint_2r(const Point_2r& p,
                                       const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualPoint_2r(p, msec_delay);
    }
}

void VisualGeometry::SigPopVisualSegment_2r(const Segment_2r& s,
                                         const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualSegment_2r(s, msec_delay);
    }
}

void VisualGeometry::SigPopVisualTriangle_2r(const Triangle_2r &t,
                                          const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualTriangle_2r(t, msec_delay);
    }
}

void VisualGeometry::SigRegisterPoint_3r(Point_3r &p) {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotRegisterPoint_3r(p);
    }
}

void VisualGeometry::SigPushVisualPoint_3r(const Point_3r& p,
                                        const Visual::Point& vp,
                                        const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualPoint_3r(p, vp, msec_delay);
    }
}

void VisualGeometry::SigPushVisualSegment_3r(const Segment_3r& s,
                                          const Visual::Segment& vs,
                                          const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualSegment_3r(s, vs, msec_delay);
    }
}

void VisualGeometry::SigPushVisualTriangle_3r(const Triangle_3r& t,
                                           const Visual::Triangle& vt,
                                           const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualTriangle_3r(t, vt, msec_delay);
    }
}

void VisualGeometry::SigPopVisualPoint_3r(const Point_3r& p,
                                       const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualPoint_3r(p, msec_delay);
    }
}

void VisualGeometry::SigPopVisualSegment_3r(const Segment_3r& s,
                                         const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualSegment_3r(s, msec_delay);
    }
}

void VisualGeometry::SigPopVisualTriangle_3r(const Triangle_3r &t,
                                          const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualTriangle_3r(t, msec_delay);
    }
}

void VisualGeometry::SigUpdate() const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotUpdate();
    }
}

} // namespace DDAD
