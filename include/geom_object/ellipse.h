#ifndef ELLIPSE_H
#define ELLIPSE_H 

#include "geom_object.h"

class geom_object;

/*************************************************************** 
 * class: Ellipse
 *
 * Derived from geom_object, defines an ellipse by computing
 * the nodes it should occupy from given dimensions and 
 * center.  
***************************************************************/

/**
 * \brief Ellipse object
 */
class Ellipse : public geom_object
{
public:
	Ellipse() = default;
	/// \brief Create an ellipse with
	/// @param Lx [in] - number nodes in x direction axis
	/// @param Ly [in] - number of nodes in y direction axis
	/// @param xc [in] - center x coordinate
	/// @param yc [in] - center y coordinate
	Ellipse(size_t Lx, size_t Ly, size_t xc, size_t yc) :
		  geom_object(xc, yc), _Lx(Lx), _Ly(Ly) { find_nodes(); }	
protected:
	void find_nodes();
private:
	size_t _Lx = 0, _Ly = 0; 
};

#endif
