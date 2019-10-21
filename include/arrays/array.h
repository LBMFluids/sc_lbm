#ifndef ARRAY_H
#define ARRAY_H

#include "../common.h"

/*************************************************************** 
 * class: Array
 *
 * Interface for creating arrays of objects.  
 *	
 * Does not store any grids, just generates indices that should 
 * become solid nodes. It is up to the user/caller to verify
 * if these indices fit the grid it's working on.
 *
 * User inputs the indices of nodes to make an array of and
 * the distance between objects or their number in each direction.
 * Each derived, concrete class may have additional parameters of
 * its own.
 *
 * The array interface generates the indices of the array based 
 * on the input object indices. The indices are stored as a 
 * nested vector of x,y coordinates and returned to the user
 * when requested. 
***************************************************************/

/**
 * \brief Abstract base class for an Array type object
 * \details Defines minimum interface for creating a 2D array of geometry objects; 
 * 		Does not perform any bound check, this is on the object that is using it.
 */ 
class Array
{
public:
	/// \brief Return indices/nodes that are solids
	std::vector<std::vector<size_t>> get_nodes() const { return array_nodes; }
protected:
	Array() = default;
	/**
	 * \brief Specifies the bounds of the array and the object
	 * @param [in] nodes - nodes to replicate into an array (fluid or solid)
	 * @param [in] array_outer_bounds - vector with lower and upper outer bounds 
	 * 								of the array in following order: 
	 * 								lower x, upper x, lower y, upper y
	 */ 
	Array(const std::vector<std::vector<size_t>> nodes, 
			const std::vector<std::vector<size_t>> array_outer_bounds) : 
			input_object(nodes), array_bounds(array_outer_bounds){ }
	/// \brief Nodes/indices occupied by the array
	std::vector<std::vector<size_t>> array_nodes = {};
	/// \brief Return nodes of the input object
	std::vector<std::vector<size_t>> get_input_nodes() const { return input_object; }
	/// \brief Return array bounds
	std::vector<std::vector<size_t>> get_array_bounds() const { return array_bounds; }
	/// \brief Finds/computes the nodes occupied by an array
	virtual void find_nodes() = 0;
private:
	// Array bounds
	size_t x0 = 0, xf = 0;
	size_t y0 =0, yf = 0;
	// Object/nodes to create array from
	std::vector<std::vector<size_t>> input_object = {};
	// Outer bounds of the array
	std::vector<std::vector<size_t>> array_bounds = {};
};

#endif

