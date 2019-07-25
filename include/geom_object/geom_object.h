#ifndef GEOM_OBJECT_H
#define GEOM_OBJECT_H

#include "common.h"

/*************************************************************** 
 * class: geom_object
 *
 * Classes for creating geometric objects such as squares, 
 * 	ellipses, triangles.
 *
 * geom_object is an abstract base class from which each new
 * 	object inherits. Special cases of existing objects should
 * 	inherit from those objects (like square should inherit 
 * 	from a recatngle).
 *
 *	Current approach is not to store any grids and build the
 *	object. Instead, the user supplies position and required
 *	dimensions and the class computes and returns indices the
 *	object will occupy. It is up to the user/caller to verify
 *	if these indices fit the grid it's working on. 
 *
 *  Indices are stored as a vector of vectors, each vector
 *  corresponding to x and y index components.
***************************************************************/

/**
 * \brief Abstract base class for a geometry object. 
 * \details Defines required interface	for any 2D geometric object.
 */
class geom_object
{
public:
	/// \brief Return indices/nodes that the object occupies 
	std::vector<size_t> get_nodes() const { return object_nodes; }
protected:
	/// \brief Specifies the begining node of the object
	/// \details Constructor explicitly available to derived 
	///		classes. No need for users to access it directly.
	geom_object() = default;
	geom_object(x0, y0) : xc(x0), yc(y0) { }
	/// \brief Return origin of the object
	std::vector<size_t> get_pos() const { return std::vector({xc, yc}); } 
	/// \brief Nodes/indices occupied by the object
	std::vector<std::vector<size_t>> obj_nodes = {};
	/// \brief Finds/computes the nodes occupied by an object
	virtual void find_nodes() = 0;  
private:
	size_t xc = 0, yc = 0;
};

#endif
