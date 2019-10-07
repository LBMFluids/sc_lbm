#ifndef CIRCLE_H
#define CIRCLE_H 

#include "geom_object.h"
#include "ellipse.h"

class geom_object;
class Ellipse;

/*************************************************************** 
 * class: Circle
 *
 * Derived from Ellipse, defines a circle by computing
 * the nodes it should occupy from given dimensions and 
 * center. Uses Ellipse node search. 
***************************************************************/

/**
 * \brief Circle object
 */
class Circle : public Ellipse
{
public:
	Circle() = default;
	/// \brief Create a circle with
	/// @param D [in] - number nodes in x/y direction (diameter)
	/// @param xc [in] - center x coordinate
	/// @param yc [in] - center y coordinate
	Circle(size_t D, size_t xc, size_t yc) :
		  Ellipse(D, D, xc, yc) { find_nodes(); }
};

#endif
