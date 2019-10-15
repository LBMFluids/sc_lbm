#ifndef REGULAR_ARRAY_H
#define REGULAR_ARRAY_H

#include "array.h"
#include <limits>

class Array;

/*************************************************************** 
 * class: RegularArray
 *
 * Derived from Array, defines an array of objects 
***************************************************************/

class RegularArray
{
public:
	RegularArray() = default;
	/**
	 * \brief Create a regular array of nodes given displacements
	 * @param [in] nodes - nodes to replicate into an array (fluid or solid)
	 * @param [in] array_outer_bounds - vector with lower and upper outer bounds 
	 * 								of the array in following order: 
	 * 								lower x, upper x, lower y, upper y
	 * @param [in] _dx - displacement in x - difference between outermost
	 * 						bounds of the object in x; this is in number
	 * 						of nodes
	 * @param [in] _dy - displacement in y - difference between outermost
	 * 						bounds of the object in y; this is in number
	 * 						of nodes
	 */
	RegularArray(const std::vector<std::vector<size_t>> nodes, 
					const std::vector<size_t> array_outer_bounds,
					const size_t _dx, const size_t _dy) :  
			Array(nodes, array_outer_bounds), dx(_dx), dy(_dy)
			{ compute_numbers(); find_nodes(); }
	/**
	 * \brief Create a regular array of nodes given numbers of objects 
	 * @param [in] nodes - nodes to replicate into an array (fluid or solid)
	 * @param [in] array_outer_bounds - vector with lower and upper outer bounds 
	 * 								of the array in following order: 
	 * 								lower x, upper x, lower y, upper y
	 * @param [in] object_numbers - vector with number of objects in x and in y
	 * 									respectively 
	 */
	RegularArray(const std::vector<std::vector<size_t>> nodes, 
					const std::vector<size_t> array_outer_bounds,
					const std::vector<size_t> object_numbers) :  
			Array(nodes, array_outer_bounds), Nx(object_numbers[0]), Ny(object_numbers[1])
			{ compute_displacements(); find_nodes(); }
protected:
	void find_nodes();
private:
	size_t Nx = 0, Ny = 0;
	size_t dx = 0, dy = 0;
	/// \brief Compute number of objects to replicate based on displacements
	void compute_numbers();
	/// \brief Compute x and y displacements of objects based on numbers in x and y
	void compute_displacements();
};

#endif
