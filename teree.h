/**
 * CPSC 221 - 2021S
 * PA3
 * Ternary Tree
 * 
 * This file will be submitted for grading.
 *
 */

#ifndef _TEREE_H_
#define _TEREE_H_

#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"

using namespace std;
using namespace cs221util;

/**
 * Ternary Tree: This is a structure used in decomposing an image
 * into rectangular regions of similarly colored pixels.
 *
 * You should not remove anything from this class definition, but
 * you will find it helpful to add your own private helper functions to it.
 *
 * This file will be used for grading.
 */

class Teree {
private:

  /**
   * The Node class is private to the tree class via the principle of
   * encapsulation---the end user does not need to know our node-based
   * implementation details.
   * given for PA3
   */
  class Node {
  public:
    // Node constructors
    Node(pair<int, int> ul, int w, int h, Node* par);

    pair<int, int> upperleft;	// upper-left coordinates of Node's subimage
    int width;		 // horizontal dimension of Node's subimage in pixels
    int height;    // vertical dimension of Node's subimage in pixels
    Node* parent;  // parent
    HSLAPixel avg; // Average color of Node's subimage
    Node* A;	     // ptr to left or upper subtree
    Node* B;	     // ptr to middle subtree
    Node* C;	     // ptr to right or lower subtree
  };


public:

  /* =============== start of given functions ====================*/

  /**
   * Teree destructor.
   * Destroys all of the memory associated with the
   * current Teree. This function should ensure that
   * memory does not leak on destruction of a Teree.
   *
   * @see Teree.cpp
   */
  ~Teree();

  /**
   * Copy constructor for a Teree.
   * Since Teree allocate dynamic memory (i.e., they use "new", we
   * must define the Big Three). This uses your implementation
   * of the copy funtion.
   * @see Teree.cpp
   *
   * @param other = the Teree we are copying.
   */
  Teree(const Teree& other);

  /**
   * Overloaded assignment operator for Teree.
   * Part of the Big Three that we must define because the class
   * allocates dynamic memory. This uses your implementation
   * of the copy and clear funtions.
   *
   * @param rhs = the right hand side of the assignment statement.
   */
  Teree& operator=(const Teree& rhs);

  /* =============== end of given functions ====================*/

  /* =============== public PA3 FUNCTIONS =========================*/

  /**
   * Constructor that builds a Teree out of the given PNG.
   *
   * The Teree represents the subimage from (0,0) to (w-1, h-1) where
   * w-1 and h-1 are the largest valid image coordinates of the original PNG.
   * Every node corresponds to a rectangle of pixels in the original PNG,
   * represented by an (x,y) pair for the upper left corner of the
   * square and two integers for the number of pixels on the width and
   * height dimensions of the rectangular region the node defines.
   *
   * The node's three children correspond to a partition
   * of the node's rectangular region into three approximately equal-size strips.
   *
   * If the rectangular region is taller than it is wide, the region is divided
   * into horizontal strips:
   *  +-------+
   *  |   A   |
   *  |       |
   *  +-------+
   *  |   B   |
   *  |       |    (Split tall)
   *  +-------+
   *  |   C   | 
   *  |       |
   *  +-------+
   * 
   * If the rectangular region is wider than it is tall, the region is divided
   * into vertical strips:
   *  +---------+---------+---------+
   *  |    A    |    B    |    C    |
   *  |         |         |         |    (Split wide)
   *  +---------+---------+---------+
   * 
   * Your regions are not guaranteed to have dimensions exactly divisible by 3.
   * If the dimensions of your rectangular region are 3p x q or q x 3p where 3p
   * represents the length of the long side, then your rectangular regions will
   * each have dimensions p x q (or q x p)
   * 
   * If the dimensions are (3p+1) x q, subregion B gets the extra pixel of size
   * while subregions A and C have dimensions p x q.
   * 
   * If the dimensions are (3p+2) x q, subregions A and C each get an extra pixel
   * of size, while subregion B has dimensions p x q.
   * 
   * If the region to be divided is a square, then apply the Split wide behaviour.
   * 
   * Every leaf in the constructed tree corresponds to a pixel in the PNG.
   *
   */
  Teree(PNG& imIn);

  /**
   * Render returns a PNG image consisting of the pixels
   * stored in the tree. It may be used on pruned trees. Draws
   * every leaf node's rectangle onto a PNG canvas using the
   * average color stored in the node.
   */
  PNG Render() const;

  /*
 *  Prune function trims subtrees as high as possible in the tree.
 *  A subtree is pruned (cleared) if all of its leaves are within
 *  tol of the average color stored in the root of the subtree.
 *  Pruning criteria should be evaluated on the original tree, not
 *  on a pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this.
 */
  void Prune(double tol);

  /*
   * Returns the number of leaf nodes in the tree.
   * 
   * You may want a recursive helper function for this.
   */
  int NumLeaves() const;

  /* =============== end of public PA3 FUNCTIONS =========================*/

private:
  /*
   * Private member variables.
   *
   * You must use these as specified in the spec and may not rename them.
   * You may add more if you need them.
   */
  Node* root;	 // pointer to the root of the Teree
  int width;   // width of PNG represented by the tree
  int height;  // height of PNG represented by the tree

  /* =================== private PA3 functions ============== */

  /**
   * Destroys all dynamically allocated memory associated with the
   * current Teree object. To be completed for PA3.
   * You may want a recursive helper function for this one.
   */
  void Clear();

  /**
   * Copies the parameter other Teree into the current Teree.
   * Does not free any memory. Called by copy constructor and operator=.
   * You may want a recursive helper function for this one.
   * @param other The Teree to be copied.
   */
  void Copy(const Teree& other);

  /**
   * Private helper function for the constructor. Recursively builds
   * the tree according to the specification of the constructor.
   * @param im reference image used for construction
   * @param ul upper left point of node to be built's rectangle.
   * @param w width of node to be built's rectangle.
   * @param h height of node to be built's rectangle.
   * @param par pointer to node which will become the parent of the currently building node
   */
  Node* BuildNode(PNG& im, pair<int, int> ul, int w, int h, Node* par);

  /* =================== end of private PA3 functions ============== */

  /* ======== YOU MAY DEFINE YOUR OWN PRIVATE FUNCTIONS HERE ======= */

  /** Private helper function that recursively clears a teree from the given subRoot
  * @param subRoot root to be cleared at 
  */
  void clear(Node* subRoot);

  /** Private helper function that recursively copies a teree
  * @param subRoot root to be copied from
  */
  Node* copy(Node* subRoot);

  /** Private helper function that recursively copies a teree
  * @param subRoot root to whose leaves are to be counted
  */
  int NumLeaves(Node* subRoot) const;

  /** Private helper function that recursively renders the teree
  * @param canvas PNG to render node on
  * @param subRoot root whose leaves are to be rendered
  */
  void Render(PNG *canvas, Node* subRoot) const;

  /** Private helper function that recursively prunes the teree
  * @param subRoot root to be pruned 
  * @param tol maximum tolerance
  */
  void Prune(double tol, Node* subRoot);

  /** Private helper function that recursively checks the leaves of the teree
  * @param subRoot root to be cleared at 
  * @param tol maximum tolerance
  * @param avgParent the avg color of the root of the sub-teree
  */
  bool CheckLeaves(double tol, Node* subRoot, HSLAPixel avgParent);

};

#endif