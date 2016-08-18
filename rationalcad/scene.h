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

#ifndef RC_SCENE_H
#define RC_SCENE_H

// RCAD
#include "common.h"
#include "config.h"
#include "opengl.h"

// geometry kernel
#include "../geometry/arithmetic.h"
#include "../geometry/line.h"
#include "../geometry/visual.h"
#include "../geometry/triangle.h"
#include "../geometry/intersection.h"

// This function must be defined to maintain a QMap with key type
// QVector<uint32_t>.
bool operator<(const QVector<uint32_t>& a, const QVector<uint32_t>& b);

namespace RCAD {

//=============================================================================
// Intersection::Ray_3rSceneObject
//=============================================================================

namespace Intersection {

/**
 * The Ray_3rSceneObject class represents the intersection of a 3D rational
 * ray with an ISceneObject type. Used for picking.
 */
class Ray_3rSceneObject {
public:
    Ray_3rSceneObject();
    Ray_3rSceneObject(const bool empty, const rational& time);

    bool empty() const;
    const rational& time() const;

private:
    bool empty_;
    rational time_;
};

}

//=============================================================================
// Interface: ISceneObject
//=============================================================================

struct ISceneObject {
    virtual ~ISceneObject() {}
    virtual void Select() = 0;
    virtual void Deselect() = 0;
    virtual void UpdateColor(const QColor& color) = 0;
    virtual const QString& name() const = 0;
    virtual void set_name(const QString& name) = 0;
    virtual QString scene_object_type() const = 0;
    virtual Intersection::Ray_3rSceneObject intersect(const Ray_3r& ray) = 0;
};

//=============================================================================
// SceneObserver
//=============================================================================

class SceneObserver : public QObject, public Visual::IGeometryObserver {

    Q_OBJECT

public:
    SceneObserver();
    ~SceneObserver();

    /**
     * Provides @p with a unique identifier and creates an approximate observer
     * point for rendering @p.
     */
    void SlotRegisterPoint_2r(Point_2r& p) override;

    /**
     * Pushes a new visual representation for @p. Generates a new point VBO and
     * pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPushVisualPoint_2r(const Point_2r& p, const Visual::Point& vp,
        const uint32_t msec_delay = 0) override;

    /**
     * Pushes a new visual representation for @s. Generates a new line VBO and
     * pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPushVisualSegment_2r(const Segment_2r& s,
        const Visual::Segment& vs, const uint32_t msec_delay = 0) override;

    /**
     * Pushes a new visual representation for @t. Generates a new triangle VBO
     * and pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPushVisualTriangle_2r(const Triangle_2r& t,
        const Visual::Triangle& vt, const uint32_t msec_delay = 0) override;

    /**
     * Pops the top visual representation for @p. Generates a new point VBO and
     * pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPopVisualPoint_2r(const Point_2r& p,
        const uint32_t msec_delay = 0) override;

    /**
     * Pops the top visual representation for @s. Generates a new line VBO and
     * pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPopVisualSegment_2r(const Segment_2r& s,
        const uint32_t msec_delay = 0) override;

    /**
     * Pops the top visual representation for @t. Generates a new triangle VBO
     * and pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPopVisualTriangle_2r(const Triangle_2r& t,
        const uint32_t msec_delay = 0) override;

    /**
     * Provides @p with a unique identifier and creates an approximate observer
     * point for rendering @p.
     */
    void SlotRegisterPoint_3r(Point_3r& p) override;

    /**
     * Pushes a new visual representation for @p. Generates a new point VBO and
     * pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPushVisualPoint_3r(const Point_3r& p, const Visual::Point& vp,
        const uint32_t msec_delay = 0) override;

    /**
     * Pushes a new visual representation for @s. Generates a new line VBO and
     * pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPushVisualSegment_3r(const Segment_3r& s,
        const Visual::Segment& vs, const uint32_t msec_delay = 0) override;

    /**
     * Pushes a new visual representation for @t. Generates a new triangle VBO
     * and pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPushVisualTriangle_3r(const Triangle_3r& t,
        const Visual::Triangle& vt, const uint32_t msec_delay = 0) override;

    /**
     * Pops the top visual representation for @p. Generates a new point VBO and
     * pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPopVisualPoint_3r(const Point_3r& p,
        const uint32_t msec_delay = 0) override;

    /**
     * Pops the top visual representation for @s. Generates a new line VBO and
     * pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPopVisualSegment_3r(const Segment_3r& s,
        const uint32_t msec_delay = 0) override;

    /**
     * Pops the top visual representation for @t. Generates a new triangle VBO
     * and pauses the animation thread by @msec_delay unless this response is
     * batched with other signals.
     */
    void SlotPopVisualTriangle_3r(const Triangle_3r& t,
        const uint32_t msec_delay = 0) override;

    /**
     * Updates internal state so that future responses to visual signals will
     * not update vertex buffers or pause the animation thread until we
     * receive a signal to end the batch. This is used to avoid stuttery
     * rendering when updating a large amount of visual information at once.
     * Users of this function are responsible for signalling SlotUpdate after
     * ending the batch in order to update all vertex buffers.
     */
    void SlotBeginBatch() override;

    /**
     * Updates internal state to reenable vertex buffer updates and pausing
     * the animation thread. Users of this function are responsible for
     * signalling SlotUpdate in order to update all vertex buffers.
     */
    void SlotEndBatch() override;

    /**
     * Updates all vertex buffers.
     */
    void SlotUpdate() override;

    int NumObjects() const;
    const QString& selected_name() const;

public slots:
    // point set
    void onBeginCreatePointSet(const QVector2D& cur);
    void onCreatePointSet(const QVector<QVector3D>& data);
    void onUpdateNewPointSet(const QVector2D& cur);
    void onEndCreatePointSet();
    void onComputeTerrainMeshForSelectedPointSet();

    // polyline
    void onBeginCreatePolyline(const QVector2D& cur);
    void onUpdateNewPolyline(const QVector2D& cur);
    void onEndCreatePolyline();
    void onComputeMelkmanForSelectedPolyline();

    // polygon
    void onBeginCreatePolygon(const QVector2D& cur);
    void onUpdateNewPolygon(const QVector2D& cur);
    void onEndCreatePolygon();

    // polytope
    void onBeginCreatePolytope(const QVector2D& start, const QVector2D& cur);
    void onUpdateNewPolytope(const QVector2D& cur);
    void onEndCreatePolytope();

    // terrain mesh
    void onCreateTerrainMesh(const QVector<QVector3D>& data);

    // triangle soup
    void onCreateTriangleSoup(const QVector<QVector3D>& vertices,
                              const QVector<qint32>& indices);

    // picking and selection
    void onUpdateSelectedObjectName(const QString& name);
    void onUpdateSelectedObjectColor(const QColor& color);
    void onDeleteSelectedObject();
    void onDeselect();
    void onSelectObjectFromOrtho(const QVector2D& coords);
    void onSelectObjectFromPerspective(const QVector3D& origin,
                                       const QVector3D& dir);

signals:
    void UpdateVertexBuffer(const quint32 coverage_idx,
                            const quint32 lighting_idx,
                            const quint32 primtype_idx,
                            QVector<GL::Vertex> verts);

    void UpdateContextSensitiveMenus(const QString& selected_obj_type,
                                     const QString& selected_obj_name);

private:
    void GenerateVboPoints();
    void GenerateVboLines();
    void GenerateVboTriangles();

    void onSelectObject(const Ray_3r& selection_ray);

    bool ObjectIsSelected() const;
    ISceneObject* SelectedObject();
    class ScenePointSet_3* SelectedPointSet_3();
    class ScenePolyline_2* SelectedPolyline_2();
    class ScenePolygon_2* SelectedPolygon_2();
    class ScenePolytope_3* SelectedPolytope_3();
    class SceneTerrainMesh_3* SelectedTerrainMesh_3();
    class SceneTriangleSoup_3* SelectedTriangleSoup_3();

    QHash<QString, QSharedPointer<ISceneObject>> scene_objects_;
    QVector<QSharedPointer<ISceneObject>> selected_objects_;

    quint32 cur_point_uid_;
    QHash<uint32_t, QSharedPointer<ApproxPoint_3f>> approx_points_;
    QHash<uint32_t, QStack<Visual::Point>> viz_points_;
    QMap<QPair<uint32_t, uint32_t>, QStack<Visual::Segment>> viz_segments_;
    QMap<QVector<uint32_t>, QStack<Visual::Triangle>> viz_triangles_;

    bool viz_batched_;
};

//=============================================================================
// SceneManager
//=============================================================================

class SceneManager : public QObject {

    Q_OBJECT

public:
    SceneManager(Renderer* renderer);
    ~SceneManager();

    SceneObserver scene_observer_;

public slots:
    void onUpdateVertexBuffer(const quint32 coverage_idx,
                              const quint32 lighting_idx,
                              const quint32 primtype_idx,
                              QVector<GL::Vertex> verts);

private:
    Renderer* renderer_;
    QSharedPointer<QThread> animation_thread_;
};

} // namespace RCAD

#endif // RC_SCENE_H
