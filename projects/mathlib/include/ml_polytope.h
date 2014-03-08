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

//=============================================================================
// Interface: Vertex
//=============================================================================

/*!
 * A vertex of a cell, with an outgoing set of directed edges.
 *
 * pos    the three-dimensional position of the vertex
 * Cell   the cell that the vertex belongs to;
 *        nonnull
 * ID     the ID number assigned to the vertex by its cell (or the client);
 *        positive
 * data   generic data attached to the vertex by the client
 * Edges  the edges in the orbit of the vertex;
 *        all are nonnull
 */
class Vertex {
public:

    /*!
     * Return a new vertex at the origin with no outgoing edges.
     * cell -> the cell that the vertex belongs to;
     *         must be nonnull
     * <- the new vertex;
     *    will be nonnull
     */
    static Vertex *make(Cell *cell);

    /*
     * Release the storage occupied by a given vertex.
     * vertex -> the vertex to kill;
     *           must be nonnull
     */
    static void kill(Vertex *vertex);

    SharedPoint_3r pos;
    const void *data;

    /*
   * Return the cell for this vertex.
   * <- the cell that the vertex belongs to;
   *    will be nonnull
   */
    Cell *getCell();

    /*
   * Return the ID of this vertex.
   * <- the ID of this vertex;
   *    will be positive
   */
    unsigned int getID();

    /*
   * Change the ID of this vertex.
   * id -> the new id for this vertex;
   *       must be positive
   */
    void setID(unsigned int id);

    /*
   * Return an arbitrary outgoing edge from this vertex.
   * <- an edge whose origin is this vertex;
   *    null if isolated
   */
    Edge *getEdge();

    /*
   * Add a given outgoing edge to this vertex.
   * edge -> an edge whose origin is this vertex;
   *         must be nonnull
   */
    void addEdge(Edge *edge);

    /*
   * Remove a given outgoing edge from this vertex.
   * edge -> an edge whose origin is no longer at this vertex;
   *         must be nonnull
   */
    void removeEdge(Edge *edge);

protected:

    /*
   * Initialize this vertex at the origin with no outgoing edges.
   * cell -> the cell that this vertex belongs to;
   *         must be nonnull
   */
    Vertex(Cell *cell);

    /*
   * Release the storage occupied by the contents of this vertex.
   */
    ~Vertex();

private:

    /*
   * The cell that this vertex belongs to.
   * Nonnull.
   */
    Cell *cell;

    /*
   * The ID of this vertex.
   * Positive.
   */
    unsigned int id;

    /*
   * An arbitrary outgoing edge of this vertex.
   * Null if isolated.
   */
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
// Interface: VertexEdgeIterator
//=============================================================================

/*
 * Enumerates the outgoing edges of a given vertex in counterclockwise order.
 */
class VertexEdgeIterator {
public:

  /*
   * Initialize this edge iterator over a given vertex.
   * vertex -> the vertex to iterate the edges of;
   *           must be nonnull
   */
  VertexEdgeIterator(Vertex *vertex)
  {
    // pick an arbitrary edge in the vertex orbit

    start = vertex->getEdge();
    edge  = start;
  }

  /*
   * Release the storage occupied by this edge iterator.
   */
  ~VertexEdgeIterator()
  {
  }

  /*
   * Return the next edge of this edge iterator, if any.
   * <- the next edge of this edge iterator;
   *    null if none
   */
  Edge *next()
  {
    // check for degeneracy or exhausted iteration

    Edge *current = edge;

    if (current==0)
    return 0;

    // get the next edge in the counterclockwise orbit of the vertex, but
    // return the current edge
    // reset to null if we've come back to the start

    Edge *next = current->Onext();

    edge = next!=start ? next : 0;

    return current;
  }

  /* -- private instance variables ----------------------------------------- */

  private:

  /*
   * The first edge to be iterated.
   * Nonnull.
   */
  Edge *start;

  /*
   * The next edge to be iterated.
   * Null if exhausted.
   */
  Edge *edge;

};

/* ----------------------------------------------------------------------------
 * Face
 * ------------------------------------------------------------------------- */

/*
 * A face of a cell, bounded by a set of directed edges.
 *
 * Cell   the cell that the face belongs to;
 *        nonnull
 * ID     the ID number assigned to the face by its cell (or the client);
 *        positive
 * data   generic data attached to the face by the client
 * Edges  the edges in the orbit of the face;
 *        all are nonnull
 */
class Face
{

  /* -- public class methods ----------------------------------------------- */

  public:

  /*
   * Return a new face no adjacent edges.
   * cell -> the cell that the face belongs to;
   *         must be nonnull
   * <- the new degenerate face;
   *    will be nonnull
   */
  static Face *make(Cell *cell);

  /*
   * Release the storage occupied by a given face.
   * face -> the face to kill;
   *         must be nonnull
   */
  static void kill(Face *face);

  /* -- public instance variables ------------------------------------------ */

  /*
   * The data associated with this edge.
   */
  const void *data;

  /* -- public instance methods -------------------------------------------- */

  public:

  /*
   * Return the cell for this face.
   * <- the cell that the face belongs to;
   *    will be nonnull
   */
  Cell *getCell();

  /*
   * Return the ID of this face.
   * <- the ID of this face;
   *    will be positive
   */
  unsigned int getID();

  /*
   * Change the ID of this face.
   * id -> the new id for this face;
   *       must be positive
   */
  void setID(unsigned int id);

  /*
   * Return an arbitrary adjacent edge for this face.
   * <- an edge that is adjacent to this face;
   *    null if degenerate
   */
  Edge *getEdge();

  /*
   * Add a given adjacent edge to this face.
   * edge -> an edge that is adjacent to this face;
   *         must be nonnull
   */
  void addEdge(Edge *edge);

  /*
   * Remove a given adjacent from this face.
   * edge -> an edge that is no longer adjacent to this face;
   *         must be nonnull
   */
  void removeEdge(Edge *edge);

  /* -- protected instance methods ----------------------------------------- */

  protected:

  /*
   * Initialize this face with no adjacent edges.
   * cell -> the cell that this face belongs to;
   *         must be nonnull
   */
  Face(Cell *cell);

  /*
   * Release the storage occupied by the contents of this face.
   */
  ~Face();

  /* -- private instance variables ----------------------------------------- */

  private:

  /*
   * The cell that this face belongs to.
   * Nonnull.
   */
  Cell *cell;

  /*
   * The ID of this face.
   * Positive.
   */
  unsigned int id;

  /*
   * An arbitrary adjacent edge to this face.
   * Null if degenerate.
   */
  Edge *edge;

};

/* -- inline instance methods ---------------------------------------------- */

inline Cell *Face::getCell()
{
  return cell;
}

inline unsigned int Face::getID()
{
  return id;
}

inline Edge *Face::getEdge()
{
  return edge;
}

/* ----------------------------------------------------------------------------
 * FaceEdgeIterator
 * ------------------------------------------------------------------------- */

/*
 * Enumerates the bounding edges of a given face in counterclockwise order.
 */
class FaceEdgeIterator
{

  /* -- public instance methods -------------------------------------------- */

  public:

  /*
   * Initialize this edge iterator over a given face.
   * face -> the face to iterate the edges of;
   *         must be nonnull
   */
  FaceEdgeIterator(Face *face)
  {
    // pick an arbitrary edge in the face orbit

    start = face->getEdge();
    edge  = start;
  }

  /*
   * Release the storage occupied by this edge iterator.
   */
  ~FaceEdgeIterator()
  {
  }

  /*
   * Return the next edge of this edge iterator, if any.
   * <- the next edge of this edge iterator;
   *    null if none
   */
  Edge *next()
  {
    // check for degeneracy or exhausted iteration

    Edge *current = edge;

    if (current==0)
    return 0;

    // get the next edge in the left orbit of the face, but return the current
    // edge
    // reset to null if we've come back to the start

    Edge *next = current->Lnext();

    edge = next!=start ? next : 0;

    return current;
  }

  /* -- private instance variables ----------------------------------------- */

  private:

  /*
   * The first edge to be iterated.
   * Nonnull.
   */
  Edge *start;

  /*
   * The next edge to be iterated.
   * Null if exhausted.
   */
  Edge *edge;

};

/* ----------------------------------------------------------------------------
 * Edge
 * ------------------------------------------------------------------------- */

/*
 * A directed edge from one vertex to another, adjacent to two faces.
 * Based on Dani Lischinski's code from Graphics Gems IV.
 * Original quad-edge data structure due to Guibas and Stolfi (1985).
 *
 * ID     the ID number assigned to the edge;
 *        positive
 * data   generic data attached to the edge by the client
 * Org    the vertex of origin for the edge;
 *        null if currently unknown
 * Dest   the destination vertex for the edge;
 *        null if currently unknown
 * Left   the left face of the edge;
 *        null if currently unknown
 * Right  the right face of the edge;
 *        null if currently unknown
 */
class Edge
{

  /* -- public class methods ----------------------------------------------- */

  public:

  /*
   * Return a new, unconnected edge.
   * <- the new edge;
   *    will be nonnull
   */
  static Edge *make();

  /*
   * Release the storage occupied by a given edge.
   * edge -> the edge to kill;
   *         must be nonnull
   */
  static void kill(Edge *edge);

  /*
   * Splice a given pair of edges.
   * a, b -> the edges to splice;
   *         must be nonnull
   *
   * This operator affects the two edge rings around the origins of a and b,
   * and, independently, the two edge rings around the left faces of a and b.
   * In each case, (i) if the two rings are distinct, Splice will combine
   * them into one; (ii) if the two are the same ring, Splice will break it
   * into two separate pieces.
   * Thus, Splice can be used both to attach the two edges together, and
   * to break them apart. See Guibas and Stolfi (1985) p.96 for more details
   * and illustrations.
   */
  static void splice(Edge *a, Edge *b);

  /* -- public instance variables ------------------------------------------ */

  /*
   * The data associated with this edge.
   */
  const void *data;

  /* -- public instance methods -------------------------------------------- */

  public:

  /*
   * Return the ID of this edge.
   * <- the ID of this edge;
   *    will be positive
   */
  unsigned int getID();

  /*
   * Change the ID of this edge.
   * id -> the new id for this edge;
   *       must be positive
   */
  void setID(unsigned int id);

  /*
   * Return the origin vertex of this edge.
   * <- the origin of this edge;
   *    null if currently unknown
   */
  Vertex *Org();

  /*
   * Return the destination vertex of this edge.
   * <- the destination of this edge;
   *    null if currently unknown
   */
  Vertex *Dest();

  /*
   * Change the origin vertex of this edge to a given vertex.
   * org  -> the new origin vertex of this edge;
   *         null if currently unknown
   */
  void setOrg(Vertex *org);

  /*
   * Change the destination vertex of this edge to a given vertex.
   * dest -> the new destination vertex of this edge;
   *         null if currently unknown
   */
  void setDest(Vertex *dest);

  /*
   * Return the left face of this edge.
   * <- the left face of this edge;
   *    null if currently unknown
   */
  Face *Left();

  /*
   * Return the right face of this edge.
   * <- the right face of this edge;
   *    null if currently unknown
   */
  Face *Right();

  /*
   * Change the left face of this edge to a given face.
   * left  -> the new left face of this edge;
   *          null if currently unknown
   */
  void setLeft(Face *left);

  /*
   * Change the right face of this edge to a given face.
   * right -> the new right face of this edge;
   *          null if currently unknown
   */
  void setRight(Face *right);

  /*
   * Return the dual of this edge, directed from its right to its left.
   * <- the right to left dual of this edge;
   *    will be nonnull
   */
  Edge *Rot();

  /*
   * Return the dual of this edge, directed from its left to its right.
   * <- the left to right dual of this edge;
   *    will be nonnull
   */
  Edge *InvRot();

  /*
   * Return the edge from the destination to the origin of this edge.
   * <- the symmetric of this edge;
   *    will be nonnull
   */
  Edge *Sym();

  /*
   * Return the next ccw edge around (from) the origin of this edge.
   * <- the next edge from the origin;
   *    will be nonnull
   */
  Edge *Onext();

  /*
   * Return the next cw edge around (from) the origin of this edge.
   * <- the previous edge from the origin;
   *    will be nonnull
   */
  Edge *Oprev();

  /*
   * Return the next ccw edge around (into) the destination of this edge.
   * <- the next edge to the destination;
   *    will be nonnull
   */
  Edge *Dnext();

  /*
   * Return the next cw edge around (into) the destination of this edge.
   * <- the previous edge to the destination;
   *    will be nonnull
   */
  Edge* Dprev();

  /*
   * Return the ccw edge around the left face following this edge.
   * <- the next left face edge;
   *    will be nonnull
   */
  Edge* Lnext();

  /*
   * Return the ccw edge around the left face before this edge.
   * <- the previous left face edge;
   *    will be nonnull
   */
  Edge* Lprev();

  /*
   * Return the edge around the right face ccw following this edge.
   * <- the next right face edge;
   *    will be nonnull
   */
  Edge* Rnext();

  /*
   * Return the edge around the right face ccw before this edge.
   * <- the previous right face edge;
   *    will be nonnull
   */
  Edge* Rprev();

  /* -- protected instance methods ----------------------------------------- */

  protected:

  /*
   * Initialize this edge with no connections.
   */
  Edge();

  /*
   * Release the storage occupied by the contents of this edge.
   */
  ~Edge();

  /* -- private class variables -------------------------------------------- */

  private:

  /*
   * The next unused edge ID number.
   * Positive.
   */
  static unsigned int nextID;

  /* -- private instance variables ----------------------------------------- */

  private:

  /*
   * The index of this edge in its quad-edge structure.
   * Between 0 and 3 inclusive.
   */
  unsigned int index;

  /*
   * The next ccw edge around (from) the origin of this edge.
   * Nonnull.
   */
  Edge *next;

  /*
   * The ID of this edge.
   * Positive.
   */
  unsigned int id;

  /*
   * The origin vertex of this edge, if prime.
   * Null if not prime.
   */
  Vertex *vertex;

  /*
   * The target face of this edge, if dual.
   * Null if not dual.
   */
  Face *face;

  /* -- friend classes ----------------------------------------------------- */

  friend QuadEdge;

};

/* -- inline instance methods ---------------------------------------------- */

inline unsigned int Edge::getID()
{
  return id;
}

inline Edge* Edge::Rot()
{
  return index<3 ? this+1 : this-3;
}

inline Edge* Edge::InvRot()
{
  return index>0 ? this-1 : this+3;
}

inline Edge* Edge::Sym()
{
  return index<2 ? this+2 : this-2;
}

inline Edge* Edge::Onext()
{
  return next;
}

inline Edge* Edge::Oprev()
{
  return Rot()->Onext()->Rot();
}

inline Edge* Edge::Dnext()
{
  return Sym()->Onext()->Sym();
}

inline Edge* Edge::Dprev()
{
  return InvRot()->Onext()->InvRot();
}

inline Edge* Edge::Lnext()
{
  return InvRot()->Onext()->Rot();
}

inline Edge* Edge::Lprev()
{
  return Onext()->Sym();
}

inline Edge* Edge::Rnext()
{
  return Rot()->Onext()->InvRot();
}

inline Edge* Edge::Rprev()
{
  return Sym()->Onext();
}

inline Vertex* Edge::Org()
{
  return vertex;
}

inline Vertex* Edge::Dest()
{
  return Sym()->vertex;
}

inline Face* Edge::Left()
{
  return Rot()->face;
}

inline Face* Edge::Right()
{
  return InvRot()->face;
}

class CellVertexIterator;
class CellFaceIterator;

/* ----------------------------------------------------------------------------
 * Cell
 * ------------------------------------------------------------------------- */

/*
 * An enclosed volume, bounded by a set of vertices and faces.
 *
 * Vertices   the vertices of the cell;
 *            all are nonnull
 * VertexIDs  an increasing sequence of positive integers used to number
 *            distinct vertices;
 *            all are positive
 * Faces      the faces of the cell;
 *            all are nonnull
 * FaceIDs    an increasing sequence of positive integers used to number
 *            distinct faces;
 *            all are positive
 */
class Cell
{

  /* -- public class methods ----------------------------------------------- */

  public:

  /*
   * Return a new, degenerate cell consisting of a single closed edge (loop),
   * a single vertex at the origin, and a pair of faces.
   * <- the new cell;
   *    will be nonnull
   */
  static Cell *make();

  /*
   * Return a new cell with the topology of a tetrahedron and all vertices at
   * the origin.
   * <- the new tetrahedron;
   *    will be nonnull
   */
  static Cell *makeTetrahedron();

  /*
   * Release the storage occupied by a given cell.
   * cell -> the cell to kill;
   *         must be nonnull
   */
  static void kill(Cell *cell);

  /* -- public instance methods (Euler operators) -------------------------- */

  /*
   * Use these methods to construct cells with guaranteed consistent topology.
   * Other means of modifying a cell can potentially produce bizarre results.
   */

  public:

  /*
   * Return a new edge formed by splitting a given vertex between a given pair
   * of faces.
   * A new vertex is introduced at the destination of the new edge.
   * The new edge has _left_ along its left and _right_ along its right.
   * vertex      -> the vertex to split to make the new edge;
   *                must be nonnull;
   *                must share an edge with both _left_ and _right_
   * left, right -> the faces adjacent to the new edge;
   *                must be nonnull;
   *                must share an edge with _vertex_
   * <- the new edge;
   *    will be nonnull
   */
  Edge *makeVertexEdge(Vertex *vertex, Face *left, Face *right);

  /*
   * Delete a given edge from this cell, along with its destination vertex.
   * edge -> the edge to kill;
   *         must be nonnull
   */
  void killVertexEdge(Edge *edge);

  /*
   * Return a new edge formed by splitting a given face through a given pair
   * of vertices.
   * A new face is introduced to the right of the new edge.
   * The new edge has _org_ as its origin and _dest_ as its destination.
   * face      -> the face to divide to make the new edge;
   *              must be nonnull;
   *              must have both _org_ and _dest_ on its perimiter
   * org, dest -> the vertices for the endpoints of the new edge;
   *              must be nonnull;
   *              must be located on the perimiter of _face_
   * <- the new edge;
   *    will be nonnull
   */
  Edge *makeFaceEdge(Face *face, Vertex *org, Vertex *dest);

  /*
   * Delete a given edge from this cell, along with its right face.
   * edge -> the edge to kill;
   *         must be nonnull
   */
  void killFaceEdge(Edge *edge);

  /* -- public instance methods -------------------------------------------- */

  public:

  /*
   * Return the number of vertices in this cell.
   * <- the number of vertices
   */
  unsigned int countVertices();

  /*
   * Add a given vertex to this cell.
   * vertex -> the vertex to add;
   *           must be nonnull;
   *           must not be in the cell
   */
  void addVertex(Vertex *vertex);

  /*
   * Remove a given vertex from this cell.
   * vertex -> the vertex to remove;
   *           must be nonnull;
   *           must be in the cell
   */
  void removeVertex(Vertex *vertex);

  /*
   * Return a new vertex ID.
   * <- a new vertex ID;
   *    will be positive
   */
  unsigned int makeVertexID();

  /*
   * Return the number of faces in this cell.
   * <- the number of faces
   */
  unsigned int countFaces();

  /*
   * Add a given face to this cell.
   * face -> the face to add;
   *         must be nonnull
   *         must not be in the cell
   */
  void addFace(Face *face);

  /*
   * Remove a given face from this cell.
   * face -> the face to remove;
   *         must be nonnull;
   *         must be in the cell
   */
  void removeFace(Face *face);

  /*
   * Return a new face ID.
   * <- a new face ID;
   *    will be positive
   */
  unsigned int makeFaceID();

  /* -- protected instance methods ----------------------------------------- */

  protected:

  /*
   * Initialize this cell consisting of no vertices and no faces.
   */
  Cell();

  /*
   * Release the storage occupied by the contents of this cell.
   */
  ~Cell();

  /* -- private instance methods ------------------------------------------- */

  private:

  /*
   * Return the edge with a given origin vertex in the face orbit of a given
   * edge.
   * edge -> an edge of the orbit to look for the vertex in;
   *         must be nonnull
   * org  -> the origin vertex to look for;
   *         must be nonnull
   * <- the edge in the same face orbit as _edge_ with origin vertex _org_;
   *    null if not found
   */
  Edge *getOrbitOrg(Edge *edge, Vertex *org);

  /*
   * Set the origin of the vertex orbit of a given edge to a given vertex.
   * edge -> an edge of the orbit to set the origin vertex of;
   *         must be nonnull
   * org  -> the new origin vertex;
   *         must be nonnull
   */
  void setOrbitOrg(Edge *edge, Vertex *org);

  /*
   * Return the edge with a given left face in the vertex orbit of a given
   * edge.
   * edge -> an edge of the orbit to look for the face in;
   *         must be nonnull
   * left -> the left face to look for;
   *         must be nonnull
   * <- the edge in the same vertex orbit as _edge_ with left face _left_;
   *    null if not found
   */
  Edge *getOrbitLeft(Edge *edge, Face *left);

  /*
   * Set the left face of the face orbit of a given edge to a given face.
   * edge -> an edge of the orbit to set the left face of;
   *         must be nonnull
   * left -> the new left face;
   *         must be nonnull
   */
  void setOrbitLeft(Edge *edge, Face *left);

  /* -- private instance variables ----------------------------------------- */

  private:

  /*
   * The vertices in this cell.
   * Nonnull.
   */
  Vertex **vertices;

  /*
   * The number of vertices in this cell.
   * Less than or equal to _vertexSize_.
   */
  unsigned int vertexCount;

  /*
   * The number of vertices allocated for this cell.
   * Greater than or equal to _vertexCount_.
   */
  unsigned int vertexSize;

  /*
   * The next unused vertex ID.
   */
  unsigned int vertexID;

  /*
   * The faces in this cell.
   * Nonnull.
   */
  Face **faces;

  /*
   * The number of faces in this cell.
   * Less than or equal to _faceSize_.
   */
  unsigned int faceCount;

  /*
   * The number of faces allocated for this cell.
   * Greater than or equal to _faceCount_.
   */
  unsigned int faceSize;

  /*
   * The next unused face ID.
   */
  unsigned int faceID;

  /* -- friend classes ----------------------------------------------------- */

  friend CellVertexIterator;
  friend CellFaceIterator;

};

/* -- inline instance methods ---------------------------------------------- */

inline unsigned int Cell::countVertices()
{
  return vertexCount;
}

inline unsigned int Cell::makeVertexID()
{
  return vertexID++;
}

inline unsigned int Cell::countFaces()
{
  return faceCount;
}

inline unsigned int Cell::makeFaceID()
{
  return faceID++;
}

/* ----------------------------------------------------------------------------
 * CellVertexIterator
 * ------------------------------------------------------------------------- */

/*
 * Enumerates the vertices of a given cell in arbitrary order.
 */
class CellVertexIterator
{

  /* -- public instance methods -------------------------------------------- */

  public:

  /*
   * Initialize this vertex iterator over a given cell.
   * cell -> the cell to iterate the vertices of;
   *         must be nonnull
   */
  CellVertexIterator(Cell *cell)
  {
    this->cell  = cell;
    this->count = cell->vertexCount;
  }

  /*
   * Release the storage occupied by this vertex iterator.
   */
  ~CellVertexIterator()
  {
  }

  /*
   * Return the next vertex of this vertex iterator, if any.
   * <- the next vertex of this vertex iterator;
   *    null if none
   */
  Vertex *next()
  {
    // iterate the array in reverse order so that the current vertex can be
    // removed during iteration

    if (count<1)
      return 0;

    return cell->vertices[--count];
  }

  /* -- private instance variables ----------------------------------------- */

  private:

  /*
   * The cell whose vertices are being iterated.
   * Nonnull.
   */
  Cell *cell;

  /*
   * The number of vertices left to iterate.
   */
  unsigned int count;

};

/* ----------------------------------------------------------------------------
 * CellFaceIterator
 * ------------------------------------------------------------------------- */

/*
 * Enumerates the faces of a given cell in arbitrary order.
 */
class CellFaceIterator
{

  /* -- public instance methods -------------------------------------------- */

  public:

  /*
   * Initialize this face iterator over a given cell.
   * cell -> the cell to iterate the faces of;
   *         must be nonnull
   */
  CellFaceIterator(Cell *cell)
  {
    this->cell  = cell;
    this->count = cell->faceCount;
  }

  /*
   * Release the storage occupied by this face iterator.
   */
  ~CellFaceIterator()
  {
  }

  /*
   * Return the next face of this face iterator, if any.
   * <- the next face of this face iterator;
   *    null if none
   */
  Face *next()
  {
    // iterate the array in reverse order so that the current face can be
    // removed during iteration

    if (count<1)
      return 0;

    return cell->faces[--count];
  }

  /* -- private instance variables ----------------------------------------- */

  private:

  /*
   * The cell whose faces are being iterated.
   * Nonnull.
   */
  Cell *cell;

  /*
   * The number of faces left to iterate.
   */
  unsigned int count;

};

} // namespace QuadEdge

//=============================================================================
// Interface: Polytope_3r
//=============================================================================

class Polytope_3r : public VisualGeometry {
public:
    Polytope_3r();

    void Update();
    
private:
    std::vector<SharedPoint_3r> vertices_;
    std::vector<Segment_3r> edges_;
    std::vector<Triangle_3r> faces_;
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
