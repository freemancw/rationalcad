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
 * @date 2014-03-30
 */

#ifndef RC_ML_TRIANGULATION_H
#define RC_ML_TRIANGULATION_H

#include "ml_common.h"
#include "ml_observer.h"
#include "ml_quadedge.h"
#include "ml_matrix.h"

namespace RCAD {

namespace Predicate {

inline rational InCircle(const Point_3r& a, const Point_3r& b,
                         const Point_3r& c, const Point_3r& d) {
    auto m00 = a.x()-d.x();
    auto m01 = a.y()-d.y();
    auto m02 = m00*m00+m01*m01;
    auto m10 = b.x()-d.x();
    auto m11 = b.y()-d.y();
    auto m12 = m10*m10+m11*m11;
    auto m20 = c.x()-d.x();
    auto m21 = c.y()-d.y();
    auto m22 = m20*m20+m21*m21;
    return Determinant(Matrix_3x3r(
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22
    ));
}

inline rational Orient2D(const Point_3r& a, const Point_3r& b,
                         const Point_3r& c) {
    return Determinant(Matrix_2x2r(
        a.x()-c.x(), a.y()-c.y(),
        b.x()-c.x(), b.y()-c.y()
    ));
}

}

class TerrainMesh_3r : public VisualGeometry {
public:
    TerrainMesh_3r() {
        Visual::Color blue(0, 151, 255, 255);
        Visual::Material blue_mat;
        blue_mat.set_ambient(blue);

        viz_point_.set_material(blue_mat);
        viz_segment_.set_material(blue_mat);
        viz_triangle_.set_material(blue_mat);
    }

    void Initialize(const std::vector<Point_3f>& samples) {
        assert(!samples.empty());

        // setup bounding triangle topology
        cell_ = QuadEdge::Cell::make();

        QuadEdge::CellVertexIterator iter(cell_);
        QuadEdge::Vertex *v1 = iter.next();

        QuadEdge::Edge *a = v1->getEdge();
        QuadEdge::Face *left = a->Left();
        QuadEdge::Face *right = a->Right();

        QuadEdge::Vertex *v2 = cell_->makeVertexEdge(v1, left, right)->Dest();
        QuadEdge::Vertex *v3 = cell_->makeVertexEdge(v2, left, right)->Dest();

        // compute a bounding box of the input points xy coordinates
        float maxx, maxy, maxz, minx, miny, minz;
        minx = maxx = samples.at(0).x();
        miny = maxy = samples.at(0).y();
        minz = maxz = samples.at(0).z();
        for (auto i = begin(samples); i != end(samples); ++i) {
            minx = std::min(i->x(), minx);
            miny = std::min(i->y(), miny);
            minz = std::min(i->z(), minz);
            maxx = std::max(i->x(), maxx);
            maxy = std::max(i->y(), maxy);
            maxz = std::max(i->z(), maxz);
        }

        // compute the max variance along a cardinal axis
        float dx = maxx - minx;
        float dy = maxy - miny;
        float dmax = std::max(dx, dy);
        dmax = std::max(dmax, 10.0f);   // in case of a single point
        float cx = dx * 0.5f;
        float cy = dy * 0.5f;

        // set triangle vertex positions
        v1->pos = std::make_shared<Point_3r>(cx - 20.0f * dmax, cy - dmax, 0);
        v2->pos = std::make_shared<Point_3r>(cx + 20.0f * dmax, cy - dmax, 0);
        v3->pos = std::make_shared<Point_3r>(cx, cy + 20.0f * dmax, 0);

        // register vertices
        QuadEdge::CellVertexIterator cellVerts(cell_);
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            SigRegisterPoint_3r(*v->pos);
            SigPushVisualPoint_3r(*v->pos, viz_point_);
        }

        // define convenience lambda for edge viz
        auto SigPushDirectedEdge = [&](QuadEdge::Edge* e) {
            if (e->Org() < e->Dest()) {
                SigPushVisualSegment_3r(
                    Segment_3r(e->Org()->pos, e->Dest()->pos),
                    viz_segment_
                );
            }
        };

        // register edges and faces
        QuadEdge::CellFaceIterator cellFaces(cell_);
        QuadEdge::Face *f;
        while ((f = cellFaces.next()) != 0) {
            QuadEdge::FaceEdgeIterator faceEdges(f);
            QuadEdge::Edge *e;

            e = faceEdges.next();
            SigPushDirectedEdge(e);
            auto fan_pivot = e->Org()->pos;
            e = faceEdges.next();
            SigPushDirectedEdge(e);
            auto fan_middle = e->Org()->pos;
            e = faceEdges.next();
            SigPushDirectedEdge(e);
            auto fan_last = e->Org()->pos;
            Triangle_3r tri0(fan_pivot, fan_middle, fan_last);
            SigPushVisualTriangle_3r(tri0, viz_triangle_);
            f->triangulation_.push_back(tri0);
            while ((e = faceEdges.next()) != 0) {
                fan_middle = fan_last;
                fan_last = e->Org()->pos;
                Triangle_3r tri(fan_pivot, fan_middle, fan_last);
                SigPushVisualTriangle_3r(tri, viz_triangle_);
                f->triangulation_.push_back(tri);
                SigPushDirectedEdge(e);
            }
        }

        // incrementally construct the triangulation
        for (auto i = begin(samples); i != end(samples); ++i) {
            AddPoint(*i);
        }
    }



    void AddPoint(const Point_3f& sample) {
        SharedPoint_3r sample_r = std::make_shared<Point_3r>(
            sample.x(), sample.y(), sample.z()
        );
        Visual::Material vMat;
        vMat.set_ambient(Visual::Color(0, 151, 255, 255));
        Visual::Point vPoint(vMat);

        SigRegisterPoint_3r(*sample_r);
        SigPushVisualPoint_3r(*sample_r, vPoint, 2000);

        // locate point
        QuadEdge::Edge *e0;
        QuadEdge::CellFaceIterator fitr(cell_);
        e0 = fitr.next()->getEdge();
        QuadEdge::Edge *e1 = LocalizePoint(*sample_r);

        QuadEdge::Vertex *v1 = e1->Org();
        QuadEdge::Vertex *v2 = e1->Dest();
        QuadEdge::Face *f = e1->Left();
        QuadEdge::Edge *e2 = e1->Lnext();
        QuadEdge::Edge *e3 = e1->Lprev();
        QuadEdge::Vertex *v3 = e2->Dest();

        QuadEdge::Face *f2 = cell_->makeFaceEdge(f, v1, v2)->Right();



        QuadEdge::Vertex *vnew = cell_->makeVertexEdge(v2, f2, f)->Dest();
        vnew->pos = sample_r;
        cell_->makeFaceEdge(f, vnew, v3);
        std::vector<QuadEdge::Edge*> neighbors;
        neighbors.push_back(e1->Sym());
        neighbors.push_back(e2->Sym());
        neighbors.push_back(e3->Sym());
        TestAndSwapEdges(neighbors, *sample_r);
    }

    QuadEdge::Edge* LocalizePoint(const Point_3r& sample) {
        QuadEdge::CellFaceIterator faces(cell_);
        QuadEdge::Face *f = NULL;

        while ((f = faces.next())) {
            QuadEdge::Edge *e1 = f->getEdge();
            QuadEdge::Edge *e2 = e1->Lnext();
            QuadEdge::Vertex *v1 = e1->Org();
            QuadEdge::Vertex *v2 = e1->Dest();
            QuadEdge::Vertex *v3 = e2->Dest();
            if (Predicate::Orient2D(*v1->pos, *v2->pos, sample) >= 0 &&
                Predicate::Orient2D(*v2->pos, *v3->pos, sample) >= 0 &&
                Predicate::Orient2D(*v3->pos, *v1->pos, sample) >= 0) {
                return e1;
            }
        }

        return nullptr;
    }

    void TestAndSwapEdges(std::vector<QuadEdge::Edge*>& edges,
                          const Point_3r& sample) {
        while (!edges.empty()) {
            QuadEdge::Edge *e1 = edges.back();
            edges.pop_back();
            QuadEdge::Vertex *v1 = e1->Org(), *v2 = e1->Dest();
            QuadEdge::Edge *e2 = e1->Lnext();
            QuadEdge::Edge *e3 = e1->Lprev();
            QuadEdge::Vertex *v3 = e2->Dest();
            QuadEdge::Edge *e4 = e1->Rnext();
            QuadEdge::Edge *e5 = e1->Rprev();

            if (Predicate::InCircle(*v1->pos, *v2->pos, *v3->pos, sample) > 0) {
                QuadEdge::Face *left = e1->Left();
                cell_->killFaceEdge(e1);
                cell_->makeFaceEdge(left, e2->Dest(), e5->Dest());
                edges.push_back(e2->Sym());
                edges.push_back(e3->Sym());
            }
        }
    }

private:
    QuadEdge::Cell* cell_;
    Visual::Point viz_point_;
    Visual::Segment viz_segment_;
    Visual::Triangle viz_triangle_;
};

} // namespace RCAD

#endif // RC_ML_TRIANGULATION_H
