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

#ifndef GE_VISUAL_H
#define GE_VISUAL_H

#include "common.h"
#include "point.h"
#include "line.h"
#include "triangle.h"

namespace RCAD {

namespace Visual {

//=============================================================================
// Color
//=============================================================================

/**
 * The Color class represents 32-bit RGBA colors and provides a basic palette
 * as static members.
 */
class Color {
public:
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color BLACK;
    static const Color GRAY;
    static const Color WHITE;
    static const Color CYAN;
    static const Color YELLOW;
    static const Color MAGENTA;
    static const Color SKYBLUE;

    Color();
    Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

    const uint8_t r() const;
    const uint8_t g() const;
    const uint8_t b() const;
    const uint8_t a() const;
    const std::array<uint8_t, 4>& rgba() const;
    void set_r(const uint8_t r);
    void set_g(const uint8_t g);
    void set_b(const uint8_t b);
    void set_a(const uint8_t a);
    void set_rgba(const std::array<uint8_t, 4>& rgba);

private:
    std::array<uint8_t, 4> rgba_;
};

//=============================================================================
// Material
//=============================================================================

namespace Coverage {
    enum Type {
        E_OPAQUE,
        E_TRANSPARENT,
        E_MAX
    };
}

namespace Lighting {
    enum Type {
        E_UNLIT,
        E_FLAT,
        E_MAX
    };
}

class Material {
public:
    Material();
    Material(const Color& ambient, const Color& diffuse,
             const Color& specular);
    Material(const Color& ambient, const Color& diffuse,
             const Color& specular, Coverage::Type coverage,
             Lighting::Type lighting);

    const Color& ambient() const;
    const Color& diffuse() const;
    const Color& specular() const;
    Coverage::Type coverage() const;
    Lighting::Type lighting() const;
    void set_ambient(const Color& ambient);
    void set_diffuse(const Color& diffuse);
    void set_specular(const Color& specular);
    void set_coverage(Coverage::Type coverage);
    void set_lighting(Lighting::Type lighting);

private:
    Color ambient_;
    Color diffuse_;
    Color specular_;
    Coverage::Type coverage_;
    Lighting::Type lighting_;
};

//=============================================================================
// Point
//=============================================================================

class Point {
public:
    Point();
    Point(const Material& material, const int32_t z_order = 0);

    const int32_t z_order() const;
    const Material& material() const;
    void set_material(const Material& material);
    void set_z_order(const int32_t z_order);

private:
    Material material_;
    std::string sprite_;
    int32_t z_order_;
};

//=============================================================================
// Segment
//=============================================================================

class Segment {
public:
    Segment();
    Segment(const Material& material);

    const Material& material() const;
    void set_material(const Material& material);

private:
    Material material_;
};

//=============================================================================
// Triangle
//=============================================================================

class Triangle {
public:
    Triangle();
    Triangle(const Material& material);

    const Material& material() const;
    void set_material(const Material& material);

private:
    Material material_;
};

//=============================================================================
// Interface: IGeometryObserver
//=============================================================================

/**
 * The IGeometryObserver interface defines methods that allow an object to
 * respond to signals emitted from an IObservableGeometry object. Most
 * geometric types will be both observable and observers of their component
 * parts - in this case, use the Visual::Geometry abstract class.
 */
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

    virtual void SlotBeginBatch() = 0;

    virtual void SlotEndBatch() = 0;

    virtual void SlotUpdate() = 0;
};

//=============================================================================
// Interface: IObservableGeometry
//=============================================================================

/**
 * The IObservableGeometry interface defines methods that allow it to emit
 * visualization signals to any objects that are observing it. Most geometric
 * types will be both observable and observers of their component parts - in
 * this case, use the Visual::Geometry abstract class.
 */
struct IObservableGeometry {
    virtual void AddObserver(IGeometryObserver* geom_observer) = 0;
    virtual void RemoveObserver(IGeometryObserver* geom_observer) = 0;

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

    virtual void SigBeginBatch() const = 0;

    virtual void SigEndBatch() const = 0;

    virtual void SigUpdate() const = 0;
};

//=============================================================================
// Geometry
//=============================================================================

/**
 * The Visual::Geometry abstract class enables subclasses to both observe and
 * be observed by other types. This class propagates signals upward: slots
 * respond by emitting the corresponding signal, and signals call the
 * corresponding slot on any objects observing this object. This way, signals
 * bubble upward to the ultimate observer (usually a renderer) by default,
 * unless an object needs to do some special interception work.
 */
class Geometry : public IGeometryObserver, public IObservableGeometry {
public:
    virtual ~Geometry();

    // IGeometryObserver overrides

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

    void SlotBeginBatch() override;

    void SlotEndBatch() override;

    void SlotUpdate() override;

    // IObservableGeometry overrides

    void AddObserver(IGeometryObserver* geom_observer) override;
    void RemoveObserver(IGeometryObserver* geom_observer) override;

    void SigRegisterPoint_2r(Point_2r& p) override;

    void SigPushVisualPoint_2r(const Point_2r& p, const Visual::Point& vp,
                               const uint32_t msec_delay = 0) const override;

    void SigPushVisualSegment_2r(const Segment_2r& s, const Visual::Segment& vs,
                                 const uint32_t msec_delay = 0) const override;

    void SigPushVisualTriangle_2r(const Triangle_2r& t,
                                  const Visual::Triangle& vt,
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

    void SigPushVisualTriangle_3r(const Triangle_3r& t,
                                  const Visual::Triangle& vt,
                                  const uint32_t msec_delay = 0) const override;

    void SigPopVisualPoint_3r(const Point_3r& p,
                              const uint32_t msec_delay = 0) const override;

    void SigPopVisualSegment_3r(const Segment_3r& s,
                                const uint32_t msec_delay = 0) const override;

    void SigPopVisualTriangle_3r(const Triangle_3r& t,
                                 const uint32_t msec_delay = 0) const override;

    void SigBeginBatch() const override;

    void SigEndBatch() const override;

    void SigUpdate() const override;

protected:
    std::vector<IGeometryObserver*> observers_;
};

} // namespace Visual

} // namespace RCAD

#endif // GE_VISUAL_H
