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
 * @date 2013-07-21
 */

#include "ml_common.h"
#include "ml_rational.h"
#include "ml_matrix.h"
#include "ml_polytope.h"
#include "ml_intersection.h"

namespace RCAD {

namespace QuadEdge {

//=============================================================================
// Interface: QuadEdge
//=============================================================================

class QuadEdge {
public:
    QuadEdge() {
        edges[0].index = 0;
        edges[1].index = 1;
        edges[2].index = 2;
        edges[3].index = 3;

        edges[0].next = edges+0;
        edges[1].next = edges+3;
        edges[2].next = edges+2;
        edges[3].next = edges+1;

        unsigned int id = Edge::nextID;

        edges[0].id = id+0;
        edges[1].id = id+1;
        edges[2].id = id+2;
        edges[3].id = id+3;

        Edge::nextID = id+4;
    }

    Edge edges[4];
};

//=============================================================================
// Implementation: Vertex
//=============================================================================

Vertex *Vertex::make(Cell *cell) {
    assert(cell != 0);

    return new Vertex(cell);
}

void Vertex::kill(Vertex *vertex) {
    assert(vertex != 0);

    delete vertex;
}

void Vertex::setID(unsigned int id) {
    assert(id > 0);

    this->id = id;
}

void Vertex::addEdge(Edge *edge) {
    assert(edge != 0);

    // only keep track of one edge in the orbit--this one is as good as any
    this->edge = edge;
}

void Vertex::removeEdge(Edge *edge) {
    assert(edge != 0);

    // replace the arbitrary edge with another edge in the orbit
    // use null if this is the only edge
    // assumes that the edge hasn't been actually removed yet

    Edge *next = edge->Onext();
    this->edge = next != edge ? next : 0;
}

Vertex::Vertex(Cell *cell) {
    assert(cell != 0);

    // FIXME
    /*
    this->pos[0] = 0.0;
    this->pos[1] = 0.0;
    this->pos[2] = 0.0;
    */
    this->cell   = cell;
    this->id     = cell->makeVertexID();
    this->data   = 0;
    this->edge   = 0;

    cell->addVertex(this);
}

Vertex::~Vertex() {
    cell->removeVertex(this);
}

//=============================================================================
// Implementation: Face
//=============================================================================

Face *Face::make(Cell *cell) {
    assert(cell != 0);

    return new Face(cell);
}

void Face::kill(Face *face) {
    assert(face != 0);

    delete face;
}

void Face::setID(unsigned int id) {
    assert(id > 0);

    this->id = id;
}

void Face::addEdge(Edge *edge) {
    assert(edge != 0);

    // only keep track of one edge in the orbit--this one is as good as any
    this->edge = edge;
}

void Face::removeEdge(Edge *edge) {
    assert(edge != 0);

    // replace the arbitrary edge with another edge in the orbit
    // use null if this is the only edge
    // assumes that the edge hasn't been actually removed yet
    Edge *next = edge->Onext();
    this->edge = next != edge ? next : 0;
}

Face::Face(Cell *cell) {
    assert(cell != 0);

    this->cell = cell;
    this->id   = cell->makeFaceID();
    this->data = 0;
    this->edge = 0;

    cell->addFace(this);
}

Face::~Face() {
    cell->removeFace(this);
}

//=============================================================================
// Implementation: Edge
//=============================================================================

Edge *Edge::make() {
    return (new QuadEdge())->edges;
}

void Edge::kill(Edge *edge) {
    assert(edge != 0);

    // detach the edge from its cell
    splice(edge, edge->Oprev());
    splice(edge->Sym(), edge->Sym()->Oprev());

    // free the quad edge that the edge belongs to
    delete (QuadEdge*)(edge-edge->index);
}

void Edge::splice(Edge *a, Edge *b) {
    assert(a != 0);
    assert(b != 0);

    // see Guibas and Stolfi
    Edge* alpha = a->Onext()->Rot();
    Edge* beta  = b->Onext()->Rot();

    Edge* t1 = b->Onext();
    Edge* t2 = a->Onext();
    Edge* t3 = beta->Onext();
    Edge* t4 = alpha->Onext();

    a->next     = t1;
    b->next     = t2;
    alpha->next = t3;
    beta->next  = t4;
}

void Edge::setID(unsigned int id) {
    assert(id > 0);

    this->id = id;
}

void Edge::setOrg(Vertex *org) {
    // add this edge to the (vertex) orbit of _org_
    vertex = org;
    org->addEdge(this);
}

void Edge::setDest(Vertex *dest) {
    // add this edge to the (vertex) orbit of _dest_
    Sym()->vertex = dest;
    dest->addEdge(Sym());
}

void Edge::setLeft(Face *left) {
    // add this edge to the (face) orbit of _left_
    Rot()->face = left;
    left->addEdge(this);
}

void Edge::setRight(Face *right) {
    // add this edge to the (face) orbit of _right_
    InvRot()->face = right;
    right->addEdge(Sym());
}

Edge::Edge() {
    // _index_ is initialized by QuadEdge
    // _next_ is initialized by QuadEdge
    // _id_ is initialized by QuadEdge
    data   = 0;
    vertex = 0;
    face   = 0;
}

Edge::~Edge() {}

unsigned int Edge::nextID = 4;

//=============================================================================
// Implementation: Cell
//=============================================================================

Cell *Cell::make() {
    // create a looping edge that connects to itself at a single vertex
    // the edge delimits two faces
    // this is the smallest cell that is consistent with our invariants

    Cell *cell = new Cell();

    Vertex *vertex = Vertex::make(cell);
    Face   *left   = Face::make(cell);
    Face   *right  = Face::make(cell);
    Edge   *edge   = Edge::make()->InvRot();

    edge->setOrg(vertex);
    edge->setDest(vertex);
    edge->setLeft(left);
    edge->setRight(right);

    return cell;
}

Cell *Cell::makeTetrahedron() {
    // use the Euler operators to construct a tetrahedron
    Cell *cell = make();

    // grab the initial vertex
    Vertex *vertex1;
    {
        CellVertexIterator iterator(cell);
        vertex1 = iterator.next();
        assert(vertex1 != 0);
    }

    // grab the initial edge and the initial faces
    Edge *edge1 = vertex1->getEdge();
    Face *left  = edge1->Left();
    Face *right = edge1->Right();

    // drop in four vertices along the initial edge
    Vertex *vertex2 = cell->makeVertexEdge(vertex1, left, right)->Dest();
    Vertex *vertex3 = cell->makeVertexEdge(vertex2, left, right)->Dest();
    Vertex *vertex4 = cell->makeVertexEdge(vertex3, left, right)->Dest();

    // cut each of the faces in half from complementary vertices
    Face *front  = cell->makeFaceEdge(left, vertex2, vertex4)->Right();
    Face *bottom = cell->makeFaceEdge(right, vertex1, vertex3)->Right();

    return cell;
}

void Cell::kill(Cell *cell) {
    assert(cell != 0);

    delete cell;
}

Edge *Cell::makeVertexEdge(Vertex *vertex, Face *left, Face *right) {
    assert(vertex != 0);
    assert(left != 0);
    assert(right != 0);

    // locate the edges to the right of each of the faces in the orbit of the
    // vertex
    Edge *edge  = vertex->getEdge();
    Edge *edge1 = getOrbitLeft(edge, right);
    Edge *edge2 = getOrbitLeft(edge, left);

    if (edge1 == 0) {
        (void)fprintf(stderr, "Cell::makeVertexEdge: unable to locate right face %u on vertex %u\n",
                      right->getID(), vertex->getID());
        abort();
    }

    if (edge2 == 0) {
        (void)fprintf(stderr, "Cell::makeVertexEdge: unable to locate left face %u on vertex %u\n",
                      left->getID(), vertex->getID());
        abort();
    }

    // create a new vertex and copy the position of the vertex of origin
    Vertex *vertexNew = Vertex::make(this);
    vertexNew->pos = vertex->pos;

    // create a new edge and rotate it to make a clockwise loop
    Edge *edgeNew = Edge::make()->Rot();

    // connect the origin (and destination) of the new edge to _vertex_ so that
    // the left face of the edge is _left_
    // this makes a loop on the inside of _left_
    Edge::splice(edge2, edgeNew);

    // split the origin and destination of the loop so that the right face of the
    // edge is now _right_
    // this results in a non-loop edge dividing _left_ from _right_
    Edge::splice(edge1, edgeNew->Sym());

    // initialize the secondary attributes of the new edge
    edgeNew->setOrg(edge1->Org());
    edgeNew->setLeft(edge2->Left());
    edgeNew->setRight(edge1->Left());

    // all edges leaving the destination orbit of the new edge now have the new
    // vertex as their vertex of origin
    setOrbitOrg(edgeNew->Sym(), vertexNew);

    return edgeNew;
}

void Cell::killVertexEdge(Edge *edge) {
    assert(edge != 0);

    // locate _edge1_ and _edge2_ as in _makeVertexEdge_
    Edge *edge1 = edge->Oprev();
    Edge *edge2 = edge->Lnext();

    // use _edge1_ for _edge2_ if the destination vertex is isolated
    if (edge2==edge->Sym()) {
        edge2 = edge1;
    }

    // inverse of _makeVertexEdge_
    Edge::splice(edge1, edge->Sym());
    Edge::splice(edge2, edge);

    // all edges leaving the destination orbit of the deleted edge now have its
    // origin vertex as their vertex of origin
    setOrbitOrg(edge2, edge1->Org());

    // don't use the deleted edge as a reference edge any more
    edge1->Org()->addEdge(edge1);
    edge1->Left()->addEdge(edge1);
    edge2->Left()->addEdge(edge2);

    // reclaim the vertex and the edge
    Vertex::kill(edge->Dest());
    Edge::kill(edge);
}

Edge *Cell::makeFaceEdge(Face *face, Vertex *org, Vertex *dest) {
    assert(face != 0);
    assert(org != 0);
    assert(dest != 0);

    // locate the edges leaving each of the vertices in the orbit of the face
    Edge *edge  = face->getEdge();
    Edge *edge1 = getOrbitOrg(edge, org);
    Edge *edge2 = getOrbitOrg(edge, dest);

    if (edge1 == 0) {
        (void)fprintf(stderr, "Cell::makeFaceEdge: unable to locate origin vertex %u on face %u\n",
                      org->getID(), face->getID());
        abort();
    }

    if (edge2 == 0) {
        (void)fprintf(stderr, "Cell::makeFaceEdge: unable to locate destination vertex %u on face %u\n",
                      dest->getID(), face->getID());
        abort();
    }

    // create a new face
    Face *faceNew = Face::make(this);

    // create a new (non-loop) edge
    Edge *edgeNew = Edge::make();

    // connect the destination of the new edge to the origin of _edge2_
    // both faces of the edge are now _face_
    Edge::splice(edge2, edgeNew->Sym());

    // connect the origin of the new edge to _edge1_
    // _face_ is split in half along the new edge, with the new face introduced
    // on the right
    Edge::splice(edge1, edgeNew);

    // initialize the secondary attributes of the new edge
    edgeNew->setOrg(edge1->Org());
    edgeNew->setDest(edge2->Org());
    edgeNew->setLeft(edge2->Left());

    // all edges in the right orbit of the new edge (i.e. the left orbit of its
    // Sym) now have the new face as their left face
    setOrbitLeft(edgeNew->Sym(), faceNew);

    return edgeNew;
}

void Cell::killFaceEdge(Edge *edge) {
    assert(edge != 0);

    // locate _edge1_ and _edge2_ as in _makeFaceEdge_
    Edge *edge1 = edge->Oprev();
    Edge *edge2 = edge->Lnext();

    // use _edge2_ for _edge1_ if the right face is inside a loop
    if (edge1 == edge->Sym()) {
        edge1 = edge2;
    }

    // inverse of _makeFaceEdge_
    Edge::splice(edge2, edge->Sym());
    Edge::splice(edge1, edge);

    // all edges in the right orbit of the deleted edge now have its left face
    // as their left face
    setOrbitLeft(edge1, edge2->Left());

    // don't use the deleted edge as a reference edge any more
    edge1->Org()->addEdge(edge1);
    edge2->Org()->addEdge(edge2);
    edge2->Left()->addEdge(edge2);

    // reclaim the face and the edge
    Face::kill(edge->Right());
    Edge::kill(edge);
}

void Cell::addVertex(Vertex *vertex) {
    assert(vertex != 0);

    // expand the vertex array, if necessary
    if (vertexCount >= vertexSize) {
        unsigned int vertexSizeNew = vertexSize*2;
        Vertex     **verticesNew   = new Vertex*[vertexSizeNew];

        // FIXME?
        for (unsigned int i = 0; i < vertexCount; i++) {
            verticesNew[i] = vertices[i];
        }

        delete[] vertices;

        vertices   = verticesNew;
        vertexSize = vertexSizeNew;
    }

    // add the vertex to the end of the array

    vertices[vertexCount++] = vertex;
}

void Cell::removeVertex(Vertex *vertex) {
    assert(vertex != 0);

    // locate the vertex in the array and replace it with the current last vertex
    // if already the last vertex, just overwrite it
    // slow: should make this a doubly-linked list ???
    // FIXME?
    for (unsigned int i = vertexCount; i > 0; i--) {
        if (vertices[i-1] == vertex) {
            vertices[i-1] = vertices[--vertexCount];
            return;
        }
    }

    assert(0); // ??
}

void Cell::addFace(Face *face) {
    assert(face != 0);

    // expand the face array, if necessary
    if (faceCount >= faceSize) {
        unsigned int faceSizeNew = faceSize*2;
        Face       **facesNew    = new Face*[faceSizeNew];

        // FIXME?
        for (unsigned int i = 0; i < faceCount; i++) {
            facesNew[i] = faces[i];
        }

        delete[] faces;

        faces    = facesNew;
        faceSize = faceSizeNew;
    }

    // add the face to the end of the array
    faces[faceCount++] = face;
}

void Cell::removeFace(Face *face) {
    assert(face != 0);

    // locate the face in the array and replace it with the current last face
    // if already the last face, just overwrite it
    // slow: should make this a doubly-linked list ???
    // FIXME?
    for (unsigned int i = faceCount; i > 0; i--) {
        if (faces[i-1] == face) {
            faces[i-1] = faces[--faceCount];
            return;
        }
    }

    assert(0); // ??
}

Cell::Cell() {
    // preallocate enough elements for a cube
    vertices    = new Vertex*[8];
    vertexCount = 0;
    vertexSize  = 8;
    vertexID    = 1;

    faces     = new Face*[6];
    faceCount = 0;
    faceSize  = 6;
    faceID    = 1;
}

Cell::~Cell() {
    // reclaim each of the vertices and faces still owned by the cell
    // go in backwards order so that when the elements try to remove themselves,
    // it will be linear time

    // FIXME ....?????
    {
        for (unsigned int i = vertexCount; i > 0; i--) {
            Vertex::kill(vertices[i-1]);
        }
    }

    {
        for (unsigned int i = faceCount; i > 0; i--) {
            Face::kill(faces[i-1]);
        }
    }

    // reclaim the vertex and face arrays
    delete[] vertices;
    delete[] faces;
}

Edge *Cell::getOrbitOrg(Edge *edge, Vertex *org) {
    assert(edge != 0);
    assert(org != 0);

    // traverse the Lnext orbit of _edge_ looking for an edge whose origin is
    // _org_
    Edge *scan = edge;

    do {
        if (scan->Org() == org) {
            return scan;
        }

        scan = scan->Lnext();
    } while (scan != edge);

    return 0;
}

void Cell::setOrbitOrg(Edge *edge, Vertex *org) {
    assert(edge != 0);
    assert(org != 0);

    // traverse the Onext orbit of _edge_, setting the origin of each edge to
    // _org_
    Edge *scan = edge;

    do {
        scan->setOrg(org);
        scan = scan->Onext();
    } while (scan != edge);
}

Edge *Cell::getOrbitLeft(Edge *edge, Face *left) {
    assert(edge != 0);
    assert(left != 0);

    // traverse the Onext orbit of _edge_ looking for an edge whose left face is
    // _left
    Edge *scan = edge;

    do {
        if (scan->Left() == left) {
            return scan;
        }

        scan = scan->Onext();
    } while (scan != edge);

    return 0;
}

void Cell::setOrbitLeft(Edge *edge, Face *left) {
    assert(edge != 0);
    assert(left != 0);

    // traverse the Lnext orbit of _edge_, setting the left face of each edge to
    // _left_
    Edge *scan = edge;

    do {
        scan->setLeft(left);
        scan = scan->Lnext();
    } while (scan != edge);
}

} // namespace QuadEdge

/*
 * GOAL:
 *
 * Compute the integer hull of a polytope with rational vertex coordinates.
 *
 *
 *
 * Our main algorithm computes a Dobkin-Kirkpatrick hierarchy. At each round
 * of the DK computation, we choose a maximal independent set of vertices from
 * the polytope to remove. The idea is to consider each of these individually
 * and compute their integer hull in isolation.
 *
 * SUBGOAL:
 *
 * Compute the integer hull of a tetrahedron.
 *
 *
 * SIMPLEST QUESTION:
 *
 * Compute the integer hull of grid points behind an arbitrary plane.
 *
 * This question makes sense, but the answer may not be very interesting.
 * Since this plane is arbitrary, we have two cases:
 *
 * case 1: the plane passes through infinitely many grid points.
 * case 2: the plane passes through no grid points.
 *
 * In the first case, the integer hull is - imprecisely speaking - the
 * halfspace defined by the plane equation.
 *
 * In the second case, the integer hull is - imprecisely speaking - the
 * halfspace defined by shifting the plane inward along its normal until it
 * hits the first grid point. Once it hits one grid point, we have hit an
 * infinite plane of grid points, so we are back to the first case.
 *
 * NEXT QUESTION:
 *
 * Compute the integer hull of grid points behind two arbitrary planes.
 *
 * Given two arbitrary planes, we have a large number of situations that may
 * arise. Taken individually, each plane may be either of the two cases
 * outlined above. Taken together, they can:
 *
 * case 1: intersect in a line
 * case 2: have no intersection (parallel)
 * case 3: intersect in a plane (coplanar)
 *
 * Let's deal with the degenerate cases first. In case 2, the planes are
 * parallel. If they share the same normal, then one of the planes is redundant
 * and we are reduced to the previous question. If they have normals of opposite
 * signs, then the intersection of their halfspaces will be either the empty set
 * or an infinite slab of R^3. The empty case is easy to detect by simply
 * ordering the planes along their common axis. The slab case is more interesting.
 *
 * SLAB
 *
 * In the slab case, we can have 4 cases corresponding to the 4 different combinations
 * of the individual planes intersecting grid points or not. If neither of them
 * contain grid points initially, then we independently shift the planes inward
 * along their respective normals. This can result in the planes intersecting the same
 * set of grid points, or different sets of grid points. If the sets are equal,
 * then the integer hull is an infinitely thin disk parallel with the planes.
 * If they are not equal, then the integer hull is an infinite slab parallel with
 * the planes.
 *
 * COPLANAR
 *
 * If the planes are originally coplanar, then they may again have normals with
 * the same sign or opposite signs. If they have the same sign, then we are
 * reduced to the previous question since the planes are exactly equal. If they
 * have opposite signs, then the integer hull is either the infinitely thin
 * disk from the previous analysis (if the planes pass through grid points) or
 * the integer hull is the empty set.
 *
 * LINE
 *
 * Finally, the planes may intersect in a line. This is the first time where
 * the integer hull can actually be complicated. We have a large number of cases
 * to explore. We can note the following:
 *
 * - Either plane may again intersect grid points or not
 * - If either plane does not intersect any grid points, then the line of
 *   intersection also doesn't have any grid points (since its a subspace of
 *   that plane)
 * - If the line of intersection passes through grid points, then both planes
 *   also contain grid points. In this case, the integer hull is the infinite
 *   volume of R^3 defined by the two planes.
 * - We should be able to have cases where both planes pass through grid points
 *   individually, but the line of intersection does not contain grid points.
 * - If both planes pass through grid points, but the line of intersection
 *   does not, then the integer hull is described as follows. Compute the line
 *   of intersection. Consider this line with respect to one of the planes.
 *   In that plane, compute the normal to the line such that the normal points
 *   into the halfspace defined by the other plane. Shift a copy of the line
 *   inward along this normal until the first infinite row of grid points is
 *   hit. Repeat this procedure with the original line and the other plane to
 *   produce another line in the other plane. The integer hull is then defined
 *   by the convex hull of the original planes up to these cutoff lines?
 *
 *
 * - Choose a grid point on one of the planes to serve as the origin. Flatten
 *   that plane the normal way, using the line of intersection as the first
 *   vector, and apply the same transformation to the second plane. What does
 *   the second plane look like?
 *
 * - There should be one component of the second plane aligned with one of the
 *   axes in the final space. The other two components are unknown, and the
 *   plane is not passing through the origin. However, since the plane originally
 *   passed through integer points, and the transformation was unimodular, the
 *   second plane should still be passing through grid points (just not the origin).
 *
 *
 * One way of making the problem more complicated is to ask
 *
 * It does not make sense to ask what the integer hull below plane is.
 *
 *
 * I want to see which parts of the integer hull are identified by the process
 * of flattening out a plane (if any).
 *
 * One way of investigating this is as follows: Create a simplicial cone with
 * its vertex at the origin. Brute-force compute the integer hull. This is
 * problematic because the cone is not bounded.
 *
 * Another way could instead create a tetrahedron with one vertex at the
 * origin. This would be easier to brute force compute the ihull because the
 * volume can be bounded by an AABB. This would require implementing a 3D
 * convex hull algorithm to feed grid points into.
 *
 * 1. Need ability to create tetrahedra with one vertex at the origin.
 *      - this means that we are first investigating
 */

namespace Construction {

/*!
 * Implementation of a randomized convex hull algorithm.
 */
Polytope_3r ConvexHull(const std::vector<SharedPoint_3r>& P) {
    Polytope_3r hull;

    
    // find four points p1, p2, p3, p4 in P that form a tetrahedron

    // C = CH({p1, p2, p3, p4})

    // compute a random permutation p5, p6, ..., pn of the remaining points.

    // initialize the conflict graph G with all visible pairs (pt, f) where f
    // is a facet of C and t > 4.

    // for r = 5 to n do
    //  if F_conflict(pr) is not empty
    //  then delete all facets in F_conflict(pr) from C
    //


    return Polytope_3r();
}

}



Vector_3i ClearFractions(const Vector_3r& v) {
    return Vector_3i(v.x().get_num()*v.y().get_den()*v.z().get_den(),
                     v.y().get_num()*v.x().get_den()*v.z().get_den(),
                     v.z().get_num()*v.x().get_den()*v.y().get_den());
}

Vector_3i RemoveGCD(const Vector_3i& v) {
    return RemoveFactor(v, GCD(v));
}

Vector_2i RemoveGCD(const Vector_2i& v) {
    return RemoveFactor(v, GCD(v));
}


Polytope_3r IntegerHull(const Polytope_3r& P) {




    return Polytope_3r();
}

/*!
 * Input: A vector v whose components are relatively prime. A 3x3 unimodular
 * matrix B whose rows form a basis for Z^3.
 *
 * Output: Either a unimodular matrix B' with one row equal to v, or the
 * identity matrix if no such matrix can be found.
 *
 * Invariants: Every matrix given to a recursive call is unimodular.
 *
 * Notes: Unimodular means det(B) = +-1. split basis into 6 tetrahedra.
 * rewrite vector in terms of input basis. sort vector coordinates by
 * magnitude. use the ordering to determine which of the 6 tetrahedra the
 * vector lies within, and branch accordingly.
 */
Matrix_3x3i UnimodularBasisForVector_r(const Vector_3i& v,
                                       const Matrix_3x3i& B) {

    // base case: the input basis contains the vector
    if (RowVec(B, 0) == v || RowVec(B, 1) == v || RowVec(B, 2) == v) {
        assert(Determinant(B) == 1 || Determinant(B) == -1);
        assert(GCD(RowVec(B, 0)) == 1);
        assert(GCD(RowVec(B, 1)) == 1);
        assert(GCD(RowVec(B, 2)) == 1);
        return B;
    }

    Vector_3i vPrime = v*Inverse(B);
    Matrix_3x3i BPrime = B;

    // sort vPrime's components and keep track of indices
    std::array<std::pair<integer, size_t>, 3> vPrimeElts;
    vPrimeElts[0].first = vPrime.x(); vPrimeElts[0].second = 0;
    vPrimeElts[1].first = vPrime.y(); vPrimeElts[1].second = 1;
    vPrimeElts[2].first = vPrime.z(); vPrimeElts[2].second = 2;
    std::sort(begin(vPrimeElts), end(vPrimeElts), []
    (const std::pair<integer, size_t>& lhs,
     const std::pair<integer, size_t>& rhs) {
        return lhs.first < rhs.first;
    });

    BPrime(vPrimeElts[0].second, 0) += BPrime(vPrimeElts[2].second, 0);
    BPrime(vPrimeElts[0].second, 1) += BPrime(vPrimeElts[2].second, 1);
    BPrime(vPrimeElts[0].second, 2) += BPrime(vPrimeElts[2].second, 2);

    BPrime(vPrimeElts[0].second, 0) += BPrime(vPrimeElts[1].second, 0);
    BPrime(vPrimeElts[0].second, 1) += BPrime(vPrimeElts[1].second, 1);
    BPrime(vPrimeElts[0].second, 2) += BPrime(vPrimeElts[1].second, 2);

    BPrime(vPrimeElts[1].second, 0) += BPrime(vPrimeElts[2].second, 0);
    BPrime(vPrimeElts[1].second, 1) += BPrime(vPrimeElts[2].second, 1);
    BPrime(vPrimeElts[1].second, 2) += BPrime(vPrimeElts[2].second, 2);

    return UnimodularBasisForVector_r(v, BPrime);
}

/*!
 * See comments for recursive function above.
 */
Matrix_3x3i UnimodularBasisForVector(const Vector_3i& v) {
    // correctly orient starting basis
    Matrix_3x3i signMat(v.x() < 0 ? -1 : 1, 0, 0,
                        0, v.y() < 0 ? -1 : 1, 0,
                        0, 0, v.z() < 0 ? -1 : 1);

    // recursively find a basis starting with the identity
    return UnimodularBasisForVector_r(RemoveGCD(v), signMat);
}

Matrix_2x2i UnimodularBasisForVector_r(const Vector_2i& v,
                                       const Matrix_2x2i& B) {
    // base case: the input basis contains the vector
    if (RowVec(B, 0) == v || RowVec(B, 1) == v) {
        assert(Determinant(B) == 1 || Determinant(B) == -1);
        assert(GCD(RowVec(B, 0)) == 1);
        assert(GCD(RowVec(B, 1)) == 1);
        return B;
    }

    Vector_2i vPrime = v*Inverse(B);
    Matrix_2x2i BPrime = B;

    if (vPrime.x() > vPrime.y()) {
        BPrime(1, 0) += BPrime(0, 0);
        BPrime(1, 1) += BPrime(0, 1);
    } else {
        BPrime(0, 0) += BPrime(1, 0);
        BPrime(0, 1) += BPrime(1, 1);
    }

    return UnimodularBasisForVector_r(v, BPrime);
}

Matrix_2x2i UnimodularBasisForVector(const Vector_2i& v) {
    // correctly orient starting basis
    Matrix_2x2i signMat(v.x() < 0 ? -1 : 1, 0,
                        0, v.y() < 0 ? -1 : 1);

    // recursively find a basis starting with the identity
    return UnimodularBasisForVector_r(RemoveGCD(v), signMat);
}

/*!
 * Iterative version of unimodular basis for vector.
 */
Matrix_3x3i UnimodularBasisForVectorItr(const Vector_3i& v) {
    auto shortV = RemoveGCD(v);

    // correctly orient starting basis
    Matrix_3x3i basis(shortV.x() < 0 ? -1 : 1, 0, 0,
                      0, shortV.y() < 0 ? -1 : 1, 0,
                      0, 0, shortV.z() < 0 ? -1 : 1);

    while (RowVec(basis, 0) != shortV && RowVec(basis, 1) != shortV &&
           RowVec(basis, 2) != shortV) {
        // rewrite v in the newest basis
        Vector_3i vPrime = shortV*Inverse(basis);

        // sort vPrime's components and keep track of indices
        std::array<std::pair<integer, size_t>, 3> vPrimeElts;
        vPrimeElts[0].first = vPrime.x(); vPrimeElts[0].second = 0;
        vPrimeElts[1].first = vPrime.y(); vPrimeElts[1].second = 1;
        vPrimeElts[2].first = vPrime.z(); vPrimeElts[2].second = 2;
        std::sort(begin(vPrimeElts), end(vPrimeElts), []
        (const std::pair<integer, size_t>& lhs,
         const std::pair<integer, size_t>& rhs) {
            return lhs.first < rhs.first;
        });

        basis(vPrimeElts[0].second, 0) += basis(vPrimeElts[2].second, 0);
        basis(vPrimeElts[0].second, 1) += basis(vPrimeElts[2].second, 1);
        basis(vPrimeElts[0].second, 2) += basis(vPrimeElts[2].second, 2);

        basis(vPrimeElts[0].second, 0) += basis(vPrimeElts[1].second, 0);
        basis(vPrimeElts[0].second, 1) += basis(vPrimeElts[1].second, 1);
        basis(vPrimeElts[0].second, 2) += basis(vPrimeElts[1].second, 2);

        basis(vPrimeElts[1].second, 0) += basis(vPrimeElts[2].second, 0);
        basis(vPrimeElts[1].second, 1) += basis(vPrimeElts[2].second, 1);
        basis(vPrimeElts[1].second, 2) += basis(vPrimeElts[2].second, 2);
    }

    // permute the rows such that v is always the first
    /*
    int row;
    if (RowVec(basis, 0) == shortV) {
        row = 0;
    } else if (RowVec(basis, 1) == shortV) {
        row = 1;
    } else {
        row = 2;
    }
    */

    // verify output assumptions
    assert(Determinant(basis) == 1 || Determinant(basis) == -1);
    assert(GCD(RowVec(basis, 0)) == 1);
    assert(GCD(RowVec(basis, 1)) == 1);
    assert(GCD(RowVec(basis, 2)) == 1);

    return basis;
}

Matrix_2x2i UnimodularBasisForVectorItr(const Vector_2i& v) {
    auto shortV = RemoveGCD(v);

    // correctly orient starting basis
    Matrix_2x2i basis(shortV.x() < 0 ? -1 : 1, 0,
                      0, shortV.y() < 0 ? -1 : 1);

    while (RowVec(basis, 0) != shortV && RowVec(basis, 1) != shortV) {
        Vector_2i vPrime = shortV*Inverse(basis);

        if (vPrime.x() > vPrime.y()) {
            basis(1, 0) += basis(0, 0);
            basis(1, 1) += basis(0, 1);
        } else {
            basis(0, 0) += basis(1, 0);
            basis(0, 1) += basis(1, 1);
        }
    }

    // verify output assumptions
    assert(Determinant(basis) == 1 || Determinant(basis) == -1);
    assert(GCD(RowVec(basis, 0)) == 1);
    assert(GCD(RowVec(basis, 1)) == 1);

    return basis;
}

Matrix_3x3i UnimodularBasisForPlane(const Vector_3i& u, const Vector_3i& v) {
    auto basisU = UnimodularBasisForVectorItr(u);
    auto vPrime = u*Inverse(basisU);
    auto basisV = UnimodularBasisForVectorItr(Vector_2i(vPrime.y(),
                                                        vPrime.z()));

    return Matrix_3x3i(1, 0, 0,
                       0, basisV(0,0), basisV(0,1),
                       0, basisV(1,0), basisV(1,1))*basisU;
}

//=============================================================================
// Polytope_3r
//=============================================================================

Polytope_3r::Polytope_3r() {

}

void Polytope_3r::Initialize(const Point_3f &min, const Point_3f &max) {
    cell_ = QuadEdge::Cell::make();

    // grab the initial vertex
    QuadEdge::Vertex *vertex1;
    {
        QuadEdge::CellVertexIterator iterator(cell_);
        vertex1 = iterator.next();
        assert(vertex1 != 0);
    }

    // grab the initial edge and the initial faces
    QuadEdge::Edge *edge1 = vertex1->getEdge();
    QuadEdge::Face *left  = edge1->Left();
    QuadEdge::Face *right = edge1->Right();

    QuadEdge::Vertex *vertex2 = cell_->makeVertexEdge(vertex1, left, right)->Dest();
    QuadEdge::Vertex *vertex3 = cell_->makeVertexEdge(vertex2, left, right)->Dest();
    QuadEdge::Vertex *vertex4 = cell_->makeVertexEdge(vertex3, left, right)->Dest();
    QuadEdge::Vertex *vertex5 = cell_->makeVertexEdge(vertex4, left, right)->Dest();
    QuadEdge::Vertex *vertex6 = cell_->makeVertexEdge(vertex5, left, right)->Dest();
    QuadEdge::Vertex *vertex7 = cell_->makeVertexEdge(vertex6, left, right)->Dest();
    QuadEdge::Vertex *vertex8 = cell_->makeVertexEdge(vertex7, left, right)->Dest();

    /*
    vertex1->pos = std::make_shared<Point_3r>(0, 0, 0);
    vertex2->pos = std::make_shared<Point_3r>(8, 0, 0);
    vertex3->pos = std::make_shared<Point_3r>(8, 0, 8);
    vertex4->pos = std::make_shared<Point_3r>(8, 8, 8);
    vertex5->pos = std::make_shared<Point_3r>(8, 8, 0);
    vertex6->pos = std::make_shared<Point_3r>(0, 8, 0);
    vertex7->pos = std::make_shared<Point_3r>(0, 8, 8);
    vertex8->pos = std::make_shared<Point_3r>(0, 0, 8);
    */

    float minX = std::min(min.x(), max.x());
    float maxX = std::max(min.x(), max.x());
    float minY = std::min(min.y(), max.y());
    float maxY = std::max(min.y(), max.y());
    float minZ = std::min(min.z(), max.z());
    float maxZ = std::max(min.z(), max.z());

    vertex1->pos = std::make_shared<Point_3r>(maxX, minY, minZ);
    vertex2->pos = std::make_shared<Point_3r>(minX, minY, minZ);
    vertex3->pos = std::make_shared<Point_3r>(minX, minY, maxZ);
    vertex4->pos = std::make_shared<Point_3r>(minX, maxY, maxZ);
    vertex5->pos = std::make_shared<Point_3r>(minX, maxY, minZ);
    vertex6->pos = std::make_shared<Point_3r>(maxX, maxY, minZ);
    vertex7->pos = std::make_shared<Point_3r>(maxX, maxY, maxZ);
    vertex8->pos = std::make_shared<Point_3r>(maxX, minY, maxZ);

    // construct the bottom face
    auto bfE = cell_->makeFaceEdge(left, vertex6, vertex1);
    QuadEdge::Face *bottom = bfE->Left();
    auto rfE = cell_->makeFaceEdge(bottom, vertex5, vertex2);
    auto fE = cell_->makeFaceEdge(right, vertex7, vertex4);
    auto tE = cell_->makeFaceEdge(fE->Left(), vertex8, vertex3);

    Visual::Color vColor(0, 151, 255, 255);

    QuadEdge::CellVertexIterator cellVerts(cell_);
    QuadEdge::Vertex *v;
    while ((v = cellVerts.next()) != 0) {
        SigRegisterPoint_3r(*v->pos);
        SigPushVisualPoint_3r(*v->pos, Visual::Point(vColor));
    }

    Visual::Color fColor(200, 200, 200, 255);
    Visual::Triangle pVt;
    pVt.set_diffuse(fColor);

    QuadEdge::CellFaceIterator cellFaces(cell_);
    QuadEdge::Face *f;
    while ((f = cellFaces.next()) != 0) {
        QuadEdge::FaceEdgeIterator faceEdges(f);
        QuadEdge::Edge *e;
        auto fan_pivot = faceEdges.next()->Org()->pos;
        auto fan_middle = faceEdges.next()->Org()->pos;
        auto fan_last = faceEdges.next()->Org()->pos;
        Triangle_3r tri0(fan_pivot, fan_middle, fan_last);
        auto v0 = *fan_middle-*fan_pivot;
        auto v1 = *fan_last-*fan_pivot;
        auto norm = Cross(v0, v1);
        std::cout << tri0 << "\n" << norm << "\n";
        std::cout << std::endl;
        SigPushVisualTriangle_3r(tri0, pVt);
        while ((e = faceEdges.next()) != 0) {
            fan_middle = fan_last;
            fan_last = e->Org()->pos;
            Triangle_3r tri(fan_pivot, fan_middle, fan_last);
            v0 = *fan_middle-*fan_pivot;
            v1 = *fan_last-*fan_pivot;
            norm = Cross(v0, v1);
            std::cout << tri0 << "\n" << norm << "\n";
            std::cout << std::endl;
            SigPushVisualTriangle_3r(tri, pVt);
        }
    }


    /*
    QuadEdge::Vertex *vertex5 = cell_->makeVertexEdge(vertex4, left, right)->Dest();
    QuadEdge::Vertex *vertex6 = cell_->makeVertexEdge(vertex5, left, right)->Dest();
    QuadEdge::Vertex *vertex7 = cell_->makeVertexEdge(vertex6, left, right)->Dest();
    QuadEdge::Vertex *vertex8 = cell_->makeVertexEdge(vertex7, left, right)->Dest();
    */

    /*
    vertex5->pos = std::make_shared<Point_3r>(0, 0, 8);
    vertex6->pos = std::make_shared<Point_3r>(8, 0, 8);
    vertex7->pos = std::make_shared<Point_3r>(8, 8, 8);
    vertex8->pos = std::make_shared<Point_3r>(0, 8, 8);
    */


    /*
    Triangle_3r pTri0(pPt0, pPt1, pPt2);
    Triangle_3r pTri1(pPt0, pPt2, pPt3);

    Visual::Triangle pVt;
    pVt.set_diffuse(Visual::Color(50, 200, 200, 255));
*/
    /*
    SigPushVisualTriangle_3r(pTri0, pVt);
    SigPushVisualTriangle_3r(pTri1, pVt);
    */


    /*
    QuadEdge::Face *front = cell->makeFaceEdge(left, vertex1, vertex4)->Right();
    QuadEdge::Face *left  = cell->makeFaceEdge(, vertex1, vertex4)->Right();
    QuadEdge::Face *front = cell->makeFaceEdge(right, vertex2, vertex7)->Right();

    QuadEdge::Face *front  = cell->makeFaceEdge(left, vertex2, vertex4)->Right();
    QuadEdge::Face *bottom = cell->makeFaceEdge(right, vertex1, vertex3)->Right();
*/


/*
    // create edges
    edges_.push_back(Segment_3r(vertices_[0], vertices_[1]));
    edges_.push_back(Segment_3r(vertices_[1], vertices_[2]));
    edges_.push_back(Segment_3r(vertices_[2], vertices_[3]));
    edges_.push_back(Segment_3r(vertices_[3], vertices_[0]));

    edges_.push_back(Segment_3r(vertices_[0], vertices_[4]));
    edges_.push_back(Segment_3r(vertices_[1], vertices_[5]));
    edges_.push_back(Segment_3r(vertices_[2], vertices_[6]));
    edges_.push_back(Segment_3r(vertices_[3], vertices_[7]));

    edges_.push_back(Segment_3r(vertices_[4], vertices_[5]));
    edges_.push_back(Segment_3r(vertices_[5], vertices_[6]));
    edges_.push_back(Segment_3r(vertices_[6], vertices_[7]));
    edges_.push_back(Segment_3r(vertices_[7], vertices_[4]));

*/
}

/*!
 * INPUT. 2 affine planes, P and Q, that intersect in a line L.
 *
 * OUTPUT. The integer hull of the region behind or on both P and Q, where
 * the hull is represented as a collection of planes.
 *
 * ALGORITHM. Construct an integer vector that points along the direction of
 * L (it should not matter which orientation). Construct a second integer
 * vector that together with the first vector spans P. Use these vectors to
 * construct a basis such that P is now a coordinate plane in two of the
 * dimensions, and transform Q into this new space (unimodular).
 *
 * In this new space, the line of intersection L is parallel to one of the
 * coordinate axes, and is contained within the coordinate plane corresponding
 * to P. The second plane Q is at some rotation around L. We care about the area
 * above the coordinate plane and below transformed Q. The integer hull of this
 * area, transformed back into the original space, will give the integer hull
 * of the original region.
 */
void Polytope_3r::Update() {

    // create plane P from three rational points
    auto planeptP0 = std::make_shared<Point_3r>(1, 2, 0);
    auto planeptP1 = std::make_shared<Point_3r>(5, 1, 2);
    auto planeptP2 = std::make_shared<Point_3r>(1, 16, 7);
    std::cout << "planeptP0 " << *planeptP0 << std::endl;
    std::cout << "planeptP1 " << *planeptP1 << std::endl;
    std::cout << "planeptP2 " << *planeptP2 << std::endl;

    // compute P's normal
    auto planevecP0 = *planeptP1-*planeptP0;
    auto planevecP1 = *planeptP2-*planeptP0;
    auto planenormP = Cross(planevecP0, planevecP1);
    std::cout << "planenormP " << planenormP << std::endl;

    // create plane Q from three rational points
    auto planeptQ0 = std::make_shared<Point_3r>(2, 1, 0);
    auto planeptQ1 = std::make_shared<Point_3r>(16, 6, 5);
    auto planeptQ2 = std::make_shared<Point_3r>(10, 19, 6);
    std::cout << "planeptQ0 " << *planeptQ0 << std::endl;
    std::cout << "planeptQ1 " << *planeptQ1 << std::endl;
    std::cout << "planeptQ2 " << *planeptQ2 << std::endl;

    // compute Q's normal
    auto planevecQ0 = *planeptQ1-*planeptQ0;
    auto planevecQ1 = *planeptQ2-*planeptQ0;
    auto planenormQ = Cross(planevecQ0, planevecQ1);
    std::cout << "planenormQ " << planenormQ << std::endl;

    // compute a vector along the line of intersection L between P and Q
    // since L lies in both planes, it must be perpendicular to both normals
    auto linevecL = RemoveGCD(ClearFractions(Cross(planenormP, planenormQ)));
    std::cout << "linevecL " << linevecL << std::endl;

    // choose a second vector that, with L's direction vector, spans P.
    // for now, assume planevecP0 != linevecL
    auto spanvecP0 = RemoveGCD(ClearFractions(planevecP0));
    assert(spanvecP0 != linevecL);
    std::cout << "spanvecP0 " << spanvecP0 << std::endl;

    // compute a basis in which L is the x axis and P is parallel to the
    // xy coordinate plane
    auto basisFinal = UnimodularBasisForPlane(linevecL, spanvecP0);
    auto inverseBasis = Inverse(basisFinal);

    // rewrite P's points
    auto planeptP0Prime = std::make_shared<Point_3r>(*planeptP0*inverseBasis);
    auto planeptP1Prime = std::make_shared<Point_3r>(*planeptP1*inverseBasis);
    auto planeptP2Prime = std::make_shared<Point_3r>(*planeptP2*inverseBasis);

    // the z coordinate of all of P's points should be equal, but in general
    // will not be 0
    assert(planeptP0Prime->z() == planeptP1Prime->z() == planeptP2Prime->z());

    // the z coordinate defines a line in the yz plane in the new space
    // we are concerned with grid points above this line
    // todo : compute line

    // rewrite Q's points
    auto planeptQ0Prime = std::make_shared<Point_3r>(*planeptQ0*inverseBasis);
    auto planeptQ1Prime = std::make_shared<Point_3r>(*planeptQ1*inverseBasis);
    auto planeptQ2Prime = std::make_shared<Point_3r>(*planeptQ2*inverseBasis);

    // the projection of Q onto the yz plane should produce a second line
    // we are concerned with grid points below this line
    // todo: compute line

    // given the 2 halfspaces in the yz plane, compute the integer hull using
    // the 2D algorithm

    // vertices identified by the hull algorithm

    /*


    // compute a unimodular basis with L's direction vector
    auto basisL = UnimodularBasisForVectorItr(linevecL);
    std::cout << "basisL\n" << basisL << std::endl;







    // rewrite the second vector in the new basis
    auto spanvecP0Prime = RemoveGCD(spanvecP0*Inverse(basisL));
    std::cout << "spanvecP0Prime " << spanvecP0Prime << std::endl;

    // compute
    Matrix_2x2i basis2P0;
    Matrix_3x3i basis3P0;
    if (RowVec(basisL, 0) == linevecL) {
        std::cout << "row0" << std::endl;
        basis2P0 = UnimodularBasisForVectorItr(Vector_2i(spanvecP0Prime.y(),
                                                         spanvecP0Prime.z()));
        basis3P0 = Matrix_3x3i(1, 0, 0,
                               0, basis2P0(0,0), basis2P0(0,1),
                               0, basis2P0(1,0), basis2P0(1,1));
        assert(Determinant(basis3P0) == 1 || Determinant(basis3P0) == -1);

    } else if (RowVec(basisL, 1) == linevecL) {
        std::cout << "row1" << std::endl;
        basis2P0 = UnimodularBasisForVectorItr(Vector_2i(spanvecP0Prime.x(),
                                                         spanvecP0Prime.z()));
        basis3P0 = Matrix_3x3i(0, basis2P0(0,0), basis2P0(0,1),
                               1, 0, 0,
                               0, basis2P0(1,0), basis2P0(1,1));
        assert(Determinant(basis3P0) == 1 || Determinant(basis3P0) == -1);
    } else {
        std::cout << "row2" << std::endl;
        basis2P0 = UnimodularBasisForVectorItr(Vector_2i(spanvecP0Prime.x(),
                                                         spanvecP0Prime.y()));
        basis3P0 = Matrix_3x3i(0, basis2P0(0,0), basis2P0(0,1),
                               0, basis2P0(1,0), basis2P0(1,1),
                               1, 0, 0);
        assert(Determinant(basis3P0) == 1 || Determinant(basis3P0) == -1);
    }
    std::cout << "basis3P0\n" << basis3P0 << std::endl;

    auto inverseBasis = Inverse(basisL)*Inverse(basis3P0);
    std::cout << "inverseBasis\n" << inverseBasis << std::endl;
    std::cout << Inverse(basis3P0*basisL) << std::endl;

    // flatten plane P
    auto planeptP0Prime = std::make_shared<Point_3r>(*planeptP0*inverseBasis);
    auto planeptP1Prime = std::make_shared<Point_3r>(*planeptP1*inverseBasis);
    auto planeptP2Prime = std::make_shared<Point_3r>(*planeptP2*inverseBasis);

    // transform plane Q
    auto planeptQ0Prime = std::make_shared<Point_3r>(*planeptQ0*inverseBasis);
    auto planeptQ1Prime = std::make_shared<Point_3r>(*planeptQ1*inverseBasis);
    auto planeptQ2Prime = std::make_shared<Point_3r>(*planeptQ2*inverseBasis);

*/

    /*
    // visualize the plane
    // N.X = 0
    // nx*x+ny*y+nz*z = 0
    // nz*z = -nx*x-ny*y
    // z = (-nx*x-ny*y)/nz
    auto pVec0 = *tb-*ta;
    auto pVec1 = *tc-*ta;
    auto pNorm = Cross(pVec0, pVec1);
    auto ZForXY = [&](const rational& x, const rational& y) -> rational {
        return (-pNorm.x()*x-pNorm.y()*y)/pNorm.z();
    };

    SharedPoint_3r pPt0 = std::make_shared<Point_3r>(-128, -128, ZForXY(-128, -128));
    SharedPoint_3r pPt1 = std::make_shared<Point_3r>( 128, -128, ZForXY( 128, -128));
    SharedPoint_3r pPt2 = std::make_shared<Point_3r>( 128,  128, ZForXY( 128,  128));
    SharedPoint_3r pPt3 = std::make_shared<Point_3r>(-128,  128, ZForXY(-128,  128));

    SigRegisterPoint_3r(*pPt0);
    SigRegisterPoint_3r(*pPt1);
    SigRegisterPoint_3r(*pPt2);
    SigRegisterPoint_3r(*pPt3);
    Triangle_3r pTri0(pPt0, pPt1, pPt2);
    Triangle_3r pTri1(pPt0, pPt2, pPt3);

    Visual::Triangle pVt;
    pVt.set_diffuse(Visual::Color(50, 200, 200, 50));
    */

    /*
    SigPushVisualTriangle_3r(pTri0, pVt);
    SigPushVisualTriangle_3r(pTri1, pVt);
    */


    // visualize triangle
    /*
    vertices_.push_back(ta);
    vertices_.push_back(tb);
    vertices_.push_back(tc);
    SigRegisterPoint_3r(*ta);
    SigRegisterPoint_3r(*tb);
    SigRegisterPoint_3r(*tc);
    SigPushVisualPoint_3r(*ta, Visual::Point());
    SigPushVisualPoint_3r(*tb, Visual::Point());
    SigPushVisualPoint_3r(*tc, Visual::Point());
    Triangle_3r tri(ta, tb, tc);

    std::cout << "Original Triangle:\n" << tri << std::endl;
    Visual::Triangle vt;
    vt.set_diffuse(Visual::Color(50, 200, 200, 255));
    SigPushVisualTriangle_3r(tri, vt);

    // construct two linearly independent integer vectors that span the
    // triangle's plane.

    Vector_3i planeV0 = RemoveGCD(ClearFractions(*tb-*ta));
    Vector_3i planeV1 = RemoveGCD(ClearFractions(*tc-*ta));

    std::cout << "planeV0 = " << planeV0 << std::endl;
    std::cout << "planeV1 = " << planeV1 << std::endl;

    // first we consider just one vector, it can be either but we choose V0.

    auto V0Basis = UnimodularBasisForVector(planeV0);
    std::cout << "V0Basis\n" << V0Basis << std::endl;

    auto invV0Basis = Inverse(V0Basis);
    auto planeV0prime = planeV0*invV0Basis;
    auto planeV1prime = planeV1*invV0Basis;

    std::cout << "planeV0*invV0Basis = " << planeV0prime << std::endl;
    std::cout << "planeV1*invV0Basis = " << planeV1prime << std::endl;

    Matrix_2x2i V1Basis2;
    if (planeV0prime.x() != 0) {
        V1Basis2 = UnimodularBasisForVector(Vector_2i(planeV1prime.y(),
                                                      planeV1prime.z()));
    } else if (planeV0prime.y() != 0) {
        V1Basis2 = UnimodularBasisForVector(Vector_2i(planeV1prime.x(),
                                                      planeV1prime.z()));
    } else {
        V1Basis2 = UnimodularBasisForVector(Vector_2i(planeV1prime.x(),
                                                      planeV1prime.y()));
    }

    Matrix_3x3i V1Basis(1, 0, 0,
                        0, V1Basis2(0, 0), V1Basis2(0, 1),
                        0, V1Basis2(1, 0), V1Basis2(1, 1));

    auto finalBasis = Inverse(V0Basis)*Inverse(V1Basis);

    std::cout << "planeV0*finalBasis = " << planeV0*finalBasis << std::endl;
    std::cout << "planeV1*finalBasis = " << planeV1*finalBasis << std::endl;

    auto fPointA = *ta*finalBasis;

    SharedPoint_3r pivotTriA = std::make_shared<Point_3r>(*ta*finalBasis);
    SharedPoint_3r pivotTriB = std::make_shared<Point_3r>(*tb*finalBasis);
    SharedPoint_3r pivotTriC = std::make_shared<Point_3r>(*tc*finalBasis);

    std::cout << "Pivot Tri Point A: " << *pivotTriA << std::endl;
    std::cout << "Pivot Tri Point B: " << *pivotTriB << std::endl;
    std::cout << "Pivot Tri Point C: " << *pivotTriC << std::endl;

    // visualize triangle

    vertices_.push_back(pivotTriA);
    vertices_.push_back(pivotTriB);
    vertices_.push_back(pivotTriC);
    SigRegisterPoint_3r(*pivotTriA);
    SigRegisterPoint_3r(*pivotTriB);
    SigRegisterPoint_3r(*pivotTriC);
    SigPushVisualPoint_3r(*pivotTriA, Visual::Point());
    SigPushVisualPoint_3r(*pivotTriB, Visual::Point());
    SigPushVisualPoint_3r(*pivotTriC, Visual::Point());
    Triangle_3r pivotTri(pivotTriA, pivotTriB, pivotTriC);
    Visual::Triangle visualPivotTri;
    visualPivotTri.set_diffuse(Visual::Color(255, 255, 0, 255));
    SigPushVisualTriangle_3r(pivotTri, visualPivotTri);
    */

    // draw the coordinate axes

    SharedPoint_3r axesO = std::make_shared<Point_3r>(0, 0, 0);
    SharedPoint_3r axesX = std::make_shared<Point_3r>(32, 0, 0);
    SharedPoint_3r axesY = std::make_shared<Point_3r>(0, 32, 0);
    SharedPoint_3r axesZ = std::make_shared<Point_3r>(0, 0, 32);

/*
    vertices_.push_back(axesO);
    vertices_.push_back(axesX);
    vertices_.push_back(axesY);
    vertices_.push_back(axesZ);
    SigRegisterPoint_3r(*axesO);
    SigRegisterPoint_3r(*axesX);
    SigRegisterPoint_3r(*axesY);
    SigRegisterPoint_3r(*axesZ);
    Segment_3r OX(axesO, axesX);
    Segment_3r OY(axesO, axesY);
    Segment_3r OZ(axesO, axesZ);
    SigPushVisualSegment_3r(OX, Visual::Segment(Visual::Color(255, 0, 0, 255)));
    SigPushVisualSegment_3r(OY, Visual::Segment(Visual::Color(0, 255, 0, 255)));
    SigPushVisualSegment_3r(OZ, Visual::Segment(Visual::Color(0, 0, 255, 255)));
*/
}

} // namespace RCAD
