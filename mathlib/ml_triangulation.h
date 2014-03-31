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

namespace RCAD {

class TerrainMesh_3r : public VisualGeometry {
public:
    TerrainMesh_3r() {}

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

        // define materials for vertices, edges, and faces
        Visual::Material vMat;
        vMat.set_ambient(Visual::Color(0, 151, 255, 255));
        Visual::Point vPoint(vMat);
        Visual::Material eMat;
        eMat.set_ambient(Visual::Color(0, 151, 255, 255));
        Visual::Segment vSegment(eMat);
        Visual::Material fMat;
        fMat.set_ambient(Visual::Color(0, 151, 255, 255));
        Visual::Triangle vTriangle(fMat);

        // register vertices
        QuadEdge::CellVertexIterator cellVerts(cell_);
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            SigRegisterPoint_3r(*v->pos);
            SigPushVisualPoint_3r(*v->pos, vPoint);
        }

        // define convenience lambda for edge viz
        auto SigPushDirectedEdge = [&](QuadEdge::Edge* e) {
            if (e->Org() < e->Dest()) {
                SigPushVisualSegment_3r(
                    Segment_3r(e->Org()->pos, e->Dest()->pos),
                    vSegment
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
            SigPushVisualTriangle_3r(tri0, vTriangle);
            while ((e = faceEdges.next()) != 0) {
                fan_middle = fan_last;
                fan_last = e->Org()->pos;
                Triangle_3r tri(fan_pivot, fan_middle, fan_last);
                SigPushVisualTriangle_3r(tri, vTriangle);
                SigPushDirectedEdge(e);
            }
        }

        // incrementally construct the triangulation
        for (auto i = begin(samples); i != end(samples); ++i) {
            AddPoint(*i);
        }
    }

    void AddPoint(const Point_3f& sample) {
        // locate point
        QuadEdge::Edge *e0;
        QuadEdge::CellFaceIterator fitr(c);
        e0 = fitr.next()->getEdge();

        QuadEdge::Edge *e1 = LocalizePoint(sample);

        QuadEdge::Vertex *v1 = e1->Org(), *v2 = e1->Dest();
        QuadEdge::Face *f = e1->Left();
        QuadEdge::Edge *e2 = e1->Lnext();
        QuadEdge::Edge *e3 = e1->Lprev();
        QuadEdge::Vertex *v3 = e2->Dest();
        QuadEdge::Face *f2 = cell_->makeFaceEdge(f, v1, v2)->Right();
        QuadEdge::Vertex *vnew = cell_->makeVertexEdge(v2, f2, f)->Dest();
        //vnew->pos.set(pt.x,pt.y,pt.z);
        cell_->makeFaceEdge(f, vnew, v3);
        std::vector<Edge*> neighbors;
        neighbors.push_back(e1->Sym());
        neighbors.push_back(e2->Sym());
        neighbors.push_back(e3->Sym());
        TestAndSwapEdges(neighbors, sample);
    }

    QuadEdge::Edge LocalizePoint(const Point_3f& sample) {
        cout << "Now inserting point " << newPoint << " in cell with vertices:\n";
        CellVertexIterator iter(c);
        Vertex *v = iter.next();
        for (; v != NULL; v = iter.next()) {
            cout << v->pos << endl;
        }
        CellFaceIterator faces(c);
        Face *f = NULL;
        pos p[4];
        p[3][0] = newPoint.x;
        p[3][1] = newPoint.y;
        p[3][2] = newPoint.z;
        while ((f = faces.next())) {
            Edge *e1 = f->getEdge();
            Edge *e2 = e1->Lnext();
            Vertex *v1 = e1->Org();
            Vertex *v2 = e1->Dest();
            Vertex *v3 = e2->Dest();
            p[0][0] = v1->pos.x;
            p[0][1] = v1->pos.y;
            p[0][2] = v1->pos.z;
            p[1][0] = v2->pos.x;
            p[1][1] = v2->pos.y;
            p[1][2] = v2->pos.z;
            p[2][0] = v3->pos.x;
            p[2][1] = v3->pos.y;
            p[2][2] = v3->pos.z;
            if ((orient2d(p[0], p[1], p[3]) >= 0) &&
                (orient2d(p[1], p[2], p[3]) >= 0) &&
                (orient2d(p[2], p[0], p[3]) >= 0)) {
                return e1;
            }
        }
        cout << "Returned NULL!" << endl;
        return NULL;
    }

    void TestAndSwapEdges(std::vector<QuadEdge::Edge*>& edges,
                          const Point_3f& newPoint) {
        while (!edges.empty()) {
            QuadEdge::Edge *e1 = edges.back();
            edges.pop_back();
            QuadEdge::Vertex *v1 = e1->Org(), *v2 = e1->Dest();
            QuadEdge::Edge *e2 = e1->Lnext();
            QuadEdge::Edge *e3 = e1->Lprev();
            QuadEdge::Vertex *v3 = e2->Dest();
            QuadEdge::Edge *e4 = e1->Rnext();
            QuadEdge::Edge *e5 = e1->Rprev();
            pos p[4];
            p[0][0] = v1->pos.x;
            p[0][1] = v1->pos.y;
            p[0][2] = v1->pos.z;
            p[1][0] = v2->pos.x;
            p[1][1] = v2->pos.y;
            p[1][2] = v2->pos.z;
            p[2][0] = v3->pos.x;
            p[2][1] = v3->pos.y;
            p[2][2] = v3->pos.z;
            p[3][0] = newPoint.x;
            p[3][1] = newPoint.y;
            p[3][2] = newPoint.z;

            if (incircle(p[0],p[1],p[2],p[3]) > 0) {
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
};

} // namespace RCAD

#endif // RC_ML_TRIANGULATION_H
