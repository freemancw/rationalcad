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
 * @brief 3-dimensional polytope types.
 * @author Clinton Freeman <admin@freemancw.com>
 * @date 2013-10-23
 */

#ifndef RC_ML_POLYTOPE_H
#define RC_ML_POLYTOPE_H

#include "ml_common.h"
#include "ml_observer.h"
#include "ml_point.h"
#include "ml_line.h"
#include "ml_triangle.h"

namespace RCAD {

namespace QuadEdge {

class Cell;
class Edge;
class Face;
class Vertex;
class CellVertexIterator;
class CellFaceIterator;
class VertexEdgeIterator;
class FaceEdgeIterator;
class QuadEdge;

//=============================================================================
// Interface: Vertex
//=============================================================================

class Vertex {
public:
    static Vertex *make(Cell *cell);
    static void kill(Vertex *vertex);

    Cell *getCell();
    unsigned int getID();
    void setID(unsigned int id);
    Edge *getEdge();
    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);

    SharedPoint_3r pos;
    const void *data;

protected:
    Vertex(Cell *cell);
    ~Vertex();

private:
    Cell *cell;
    unsigned int id;
    Edge *edge;
};

inline Cell *Vertex::getCell() {
    return cell;
}
inline unsigned int Vertex::getID() {
    return id;
}
inline Edge *Vertex::getEdge() {
    return edge;
}

//=============================================================================
// Interface: Face
//=============================================================================

class Face {
public:
    static Face *make(Cell *cell);
    static void kill(Face *face);

    const void *data;

    Cell *getCell();
    unsigned int getID();
    void setID(unsigned int id);
    Edge *getEdge();
    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);

protected:
    Face(Cell *cell);
    ~Face();

private:
    Cell *cell;
    unsigned int id;
    Edge *edge;
};

inline Cell *Face::getCell() {
  return cell;
}
inline unsigned int Face::getID() {
  return id;
}
inline Edge *Face::getEdge() {
  return edge;
}

//=============================================================================
// Interface: Edge
//=============================================================================

class Edge {
public:
    static Edge *make();
    static void kill(Edge *edge);
    static void splice(Edge *a, Edge *b);

    unsigned int getID();
    void setID(unsigned int id);
    const void *data;
    Vertex *Org();
    Vertex *Dest();
    void setOrg(Vertex *org);
    void setDest(Vertex *dest);
    Face *Left();
    Face *Right();
    void setLeft(Face *left);
    void setRight(Face *right);
    Edge *Rot();
    Edge *InvRot();
    Edge *Sym();
    Edge *Onext();
    Edge *Oprev();
    Edge *Dnext();
    Edge* Dprev();
    Edge* Lnext();
    Edge* Lprev();
    Edge* Rnext();
    Edge* Rprev();

protected:
    Edge();
    ~Edge();

private:
    static unsigned int nextID;

    unsigned int index;
    Edge *next;
    unsigned int id;
    Vertex *vertex;
    Face *face;

    friend class QuadEdge;
};

inline unsigned int Edge::getID() {
  return id;
}
inline Edge* Edge::Rot() {
  return index<3 ? this+1 : this-3;
}
inline Edge* Edge::InvRot() {
  return index>0 ? this-1 : this+3;
}
inline Edge* Edge::Sym() {
  return index<2 ? this+2 : this-2;
}
inline Edge* Edge::Onext() {
  return next;
}
inline Edge* Edge::Oprev() {
  return Rot()->Onext()->Rot();
}
inline Edge* Edge::Dnext() {
  return Sym()->Onext()->Sym();
}
inline Edge* Edge::Dprev() {
  return InvRot()->Onext()->InvRot();
}
inline Edge* Edge::Lnext() {
  return InvRot()->Onext()->Rot();
}
inline Edge* Edge::Lprev() {
  return Onext()->Sym();
}
inline Edge* Edge::Rnext() {
  return Rot()->Onext()->InvRot();
}
inline Edge* Edge::Rprev() {
  return Sym()->Onext();
}
inline Vertex* Edge::Org() {
  return vertex;
}
inline Vertex* Edge::Dest() {
  return Sym()->vertex;
}
inline Face* Edge::Left() {
  return Rot()->face;
}
inline Face* Edge::Right() {
  return InvRot()->face;
}

//=============================================================================
// Interface: Cell
//=============================================================================

class Cell {
public:
    static Cell *make();
    static Cell *makeTetrahedron();
    static void kill(Cell *cell);

    Edge *makeVertexEdge(Vertex *vertex, Face *left, Face *right);
    void killVertexEdge(Edge *edge);
    Edge *makeFaceEdge(Face *face, Vertex *org, Vertex *dest);
    void killFaceEdge(Edge *edge);

    unsigned int countVertices();
    void addVertex(Vertex *vertex);
    void removeVertex(Vertex *vertex);
    unsigned int makeVertexID();

    unsigned int countFaces();
    void addFace(Face *face);
    void removeFace(Face *face);
    unsigned int makeFaceID();

protected:
    Cell();
    ~Cell();

private:
    Edge *getOrbitOrg(Edge *edge, Vertex *org);
    void setOrbitOrg(Edge *edge, Vertex *org);
    Edge *getOrbitLeft(Edge *edge, Face *left);
    void setOrbitLeft(Edge *edge, Face *left);
    Vertex **vertices;
    unsigned int vertexCount;
    unsigned int vertexSize;
    unsigned int vertexID;
    Face **faces;
    unsigned int faceCount;
    unsigned int faceSize;
    unsigned int faceID;

    friend class CellVertexIterator;
    friend class CellFaceIterator;
};

inline unsigned int Cell::countVertices() {
    return vertexCount;
}

inline unsigned int Cell::makeVertexID() {
    return vertexID++;
}

inline unsigned int Cell::countFaces() {
    return faceCount;
}

inline unsigned int Cell::makeFaceID() {
    return faceID++;
}

//=============================================================================
// Interface: CellVertexIterator
//=============================================================================

class CellVertexIterator {
public:

    CellVertexIterator(Cell *cell) {
        this->cell  = cell;
        this->count = cell->vertexCount;
    }

    ~CellVertexIterator() {}

    Vertex *next() {
        // iterate the array in reverse order so that the current vertex can be
        // removed during iteration

        if (count < 1) {
            return 0;
        }

        return cell->vertices[--count];
    }

private:
    Cell *cell;
    unsigned int count;
};

//=============================================================================
// Interface: CellFaceIterator
//=============================================================================

class CellFaceIterator {
public:

    CellFaceIterator(Cell *cell) {
        this->cell  = cell;
        this->count = cell->faceCount;
    }

    ~CellFaceIterator() {}

    Face *next() {
        // iterate the array in reverse order so that the current face can be
        // removed during iteration

        if (count < 1) {
            return 0;
        }

        return cell->faces[--count];
    }

private:
    Cell *cell;
    unsigned int count;

};

//=============================================================================
// Interface: VertexEdgeIterator
//=============================================================================

class VertexEdgeIterator {
public:

    VertexEdgeIterator(Vertex *vertex) {
        // pick an arbitrary edge in the vertex orbit
        start = vertex->getEdge();
        edge  = start;
    }

    ~VertexEdgeIterator() {}

    Edge *next() {
        // check for degeneracy or exhausted iteration
        Edge *current = edge;

        if (current == 0) {
            return 0;
        }

        // get the next edge in the counterclockwise orbit of the vertex, but
        // return the current edge
        // reset to null if we've come back to the start

        Edge *next = current->Onext();

        edge = next != start ? next : 0;

        return current;
    }

private:
    Edge *start;
    Edge *edge;
};

//=============================================================================
// Interface: FaceEdgeIterator
//=============================================================================

class FaceEdgeIterator {
public:

    FaceEdgeIterator(Face *face) {
        start = face->getEdge();
        edge  = start;
    }

    ~FaceEdgeIterator() {}


    Edge *next() {
        // check for degeneracy or exhausted iteration

        Edge *current = edge;

        if (current == 0) {
            return 0;
        }

        // get the next edge in the left orbit of the face, but return the
        // current edge. reset to null if we've come back to the start.
        Edge *next = current->Lnext();

        edge = next != start ? next : 0;

        return current;
    }

private:
    Edge *start;
    Edge *edge;
};

} // namespace QuadEdge

//=============================================================================
// Interface: Polytope_3r
//=============================================================================

class Polytope_3r : public VisualGeometry {
public:
    Polytope_3r();

    void Initialize(const Point_3f& min, const Point_3f& max);

    void Update();
    
private:
    QuadEdge::Cell* cell_;
};

namespace Construction {

Polytope_3r ConvexHull(const std::vector<SharedPoint_3r>& P);

}

//=============================================================================
// Interface: Tetrahedron_3r
//=============================================================================

/*
class Tetrahedron_3r : public VisualGeometry {
public:
    Tetrahedron_3r();

private:
    SharedPoint_3r a_;
    SharedPoint_3r b_;
    SharedPoint_3r c_;
    SharedPoint_3r d_;
};
*/

} // namespace RCAD

#endif // RC_ML_POLYTOPE_H