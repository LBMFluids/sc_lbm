#ifndef SQUARE_H
#define SQUARE_H 

#include "geom_object.h"
#include "rectangle.h"

class geom_object;
class Rectangle;

/*************************************************************** 
 * class: Square
 *
 * Derived from Rectangle, defines a square by computing
 * the nodes it should occupy from given dimensions and 
 * center. Uses Rectangle node search. 
***************************************************************/

/**
 * \brief Square object
 */
class Square : public Rectangle
{
public:
	Square() = default;
	/// \brief Create a square with
	/// @param L [in] - number nodes in x/y direction
	/// @param xc [in] - center x coordinate
	/// @param yc [in] - center y coordinate
	Square(size_t L, size_t xc, size_t yc) :
		  Rectangle(L, L, xc, yc) { find_nodes(); }
};

#endif
