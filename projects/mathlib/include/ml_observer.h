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
 * @date 07/17/2013
 */

#ifndef DDAD_ML_OBSERVER_H
#define DDAD_ML_OBSERVER_H

#include "ml_common.h"
#include "ml_point.h"
#include "ml_line.h"
#include "ml_triangle.h"

namespace DDAD {

struct IGeometryObserver {
    virtual void SlotRegisterPoint_2r(Point_2r& p) = 0;

    virtual void SlotPushVisualPoint_2r(const Point_2r& p,
                                        const Visual::Point& vp,
                                        const uint32_t msec_delay) = 0;

    virtual void SlotPushVisualSegment_2r(const Segment_2r& s,
                                          const Visual::Segment& vs,
                                          const uint32_t msec_delay) = 0;

    virtual void SlotPushVisualTriangle_2r(const Triangle_2r& t,
                                           const Visual::Triangle& vt,
                                           const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualPoint_2r(const Point_2r& p,
                                       const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualSegment_2r(const Segment_2r& s,
                                      const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualTriangle_2r(const Triangle_2r& t,
                                          const uint32_t msec_delay) = 0;

    virtual void SlotRegisterPoint_3r(Point_3r& p) = 0;

    virtual void SlotPushVisualPoint_3r(const Point_3r& p,
                                        const Visual::Point& vp,
                                        const uint32_t msec_delay) = 0;

    virtual void SlotPushVisualSegment_3r(const Segment_3r& s,
                                          const Visual::Segment& vs,
                                          const uint32_t msec_delay) = 0;

    virtual void SlotPushVisualTriangle_3r(const Triangle_3r& t,
                                           const Visual::Triangle& vt,
                                           const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualPoint_3r(const Point_3r& p,
                                       const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualSegment_3r(const Segment_3r& s,
                                      const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualTriangle_3r(const Triangle_3r& t,
                                          const uint32_t msec_delay) = 0;

    virtual void SlotUpdate() = 0;
};

struct IObservableGeometry {
    virtual void AddObserver(IGeometryObserver* geom_observer) = 0;

    virtual void SigRegisterPoint_2r(Point_2r& p) = 0;

    virtual void SigPushVisualPoint_2r(const Point_2r& p,
                                       const Visual::Point& vp,
                                       const uint32_t msec_delay) const = 0;

    virtual void SigPushVisualSegment_2r(const Segment_2r& s,
                                         const Visual::Segment& vs,
                                         const uint32_t msec_delay) const = 0;

    virtual void SigPushVisualTriangle_2r(const Triangle_2r& t,
                                          const Visual::Triangle& vt,
                                          const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualPoint_2r(const Point_2r& p,
                                      const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualSegment_2r(const Segment_2r& s,
                                        const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualTriangle_2r(const Triangle_2r& t,
                                         const uint32_t msec_delay) const = 0;

    virtual void SigRegisterPoint_3r(Point_3r& p) = 0;

    virtual void SigPushVisualPoint_3r(const Point_3r& p,
                                       const Visual::Point& vp,
                                       const uint32_t msec_delay) const = 0;

    virtual void SigPushVisualSegment_3r(const Segment_3r& s,
                                         const Visual::Segment& vs,
                                         const uint32_t msec_delay) const = 0;

    virtual void SigPushVisualTriangle_3r(const Triangle_3r& t,
                                          const Visual::Triangle& vt,
                                          const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualPoint_3r(const Point_3r& p,
                                      const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualSegment_3r(const Segment_3r& s,
                                        const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualTriangle_3r(const Triangle_3r& t,
                                         const uint32_t msec_delay) const = 0;

    virtual void SigUpdate() const = 0;
};

class VisualGeometry : public IGeometryObserver, public IObservableGeometry {
public:
    virtual ~VisualGeometry();

    /* Observer */

    void SlotRegisterPoint_2r(Point_2r& p) override;

    void SlotPushVisualPoint_2r(const Point_2r& p,
                                const Visual::Point& vp,
                                const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_2r(const Segment_2r& s,
                                  const Visual::Segment& vs,
                                  const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_2r(const Triangle_2r& t,
                                   const Visual::Triangle& vt,
                                   const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_2r(const Point_2r& p,
                               const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_2r(const Segment_2r& s,
                                 const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_2r(const Triangle_2r& t,
                                  const uint32_t msec_delay = 0) override;


    void SlotRegisterPoint_3r(Point_3r& p) override;

    void SlotPushVisualPoint_3r(const Point_3r& p,
                                const Visual::Point& vp,
                                const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_3r(const Segment_3r& s,
                                  const Visual::Segment& vs,
                                  const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_3r(const Triangle_3r& t,
                                   const Visual::Triangle& vt,
                                   const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_3r(const Point_3r& p,
                               const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_3r(const Segment_3r& s,
                                 const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_3r(const Triangle_3r& t,
                                  const uint32_t msec_delay = 0) override;


    void SlotUpdate() override;

    /* Observable */

    void AddObserver(IGeometryObserver* geom_observer) override;

    void SigRegisterPoint_2r(Point_2r& p) override;

    void SigPushVisualPoint_2r(const Point_2r& p, const Visual::Point& vp,
                            const uint32_t msec_delay = 0) const override;

    void SigPushVisualSegment_2r(const Segment_2r& s, const Visual::Segment& vs,
                              const uint32_t msec_delay = 0) const override;

    void SigPushVisualTriangle_2r(const Triangle_2r& t, const Visual::Triangle& vt,
                               const uint32_t msec_delay = 0) const override;

    void SigPopVisualPoint_2r(const Point_2r& p,
                           const uint32_t msec_delay = 0) const override;

    void SigPopVisualSegment_2r(const Segment_2r& s,
                             const uint32_t msec_delay = 0) const override;

    void SigPopVisualTriangle_2r(const Triangle_2r& t,
                             const uint32_t msec_delay = 0) const override;

    void SigRegisterPoint_3r(Point_3r& p) override;

    void SigPushVisualPoint_3r(const Point_3r& p, const Visual::Point& vp,
                            const uint32_t msec_delay = 0) const override;

    void SigPushVisualSegment_3r(const Segment_3r& s, const Visual::Segment& vs,
                              const uint32_t msec_delay = 0) const override;

    void SigPushVisualTriangle_3r(const Triangle_3r& t, const Visual::Triangle& vt,
                               const uint32_t msec_delay = 0) const override;

    void SigPopVisualPoint_3r(const Point_3r& p,
                           const uint32_t msec_delay = 0) const override;

    void SigPopVisualSegment_3r(const Segment_3r& s,
                             const uint32_t msec_delay = 0) const override;

    void SigPopVisualTriangle_3r(const Triangle_3r& t,
                             const uint32_t msec_delay = 0) const override;

    void SigUpdate() const override;

private:
    std::vector<IGeometryObserver*> observers_;
};

}

#endif // DDAD_ML_OBSERVER_H
