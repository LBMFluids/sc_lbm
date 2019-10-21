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

class RegularArray : public Array 
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
					const std::vector<std::vector<size_t>> array_outer_bounds,
					const size_t _dx, const size_t _dy) :  
			Array(nodes, array_outer_bounds), dx(_dx), dy(_dy)
			{ find_object_bounds(); compute_numbers(); find_nodes(); }
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
					const std::vector<std::vector<size_t>> array_outer_bounds,
					const std::vector<size_t> object_numbers) :  
			Array(nodes, array_outer_bounds), Nx(object_numbers[0]), Ny(object_numbers[1])
			{ find_object_bounds(); compute_displacements(); find_nodes(); }
protected:
	void find_nodes();
private:
	// Number of objects in each direction
	size_t Nx = 0, Ny = 0;
	// Object displacements
	size_t dx = 0, dy = 0;
	// Object bounds in each direction
	// [[xmin, xmax], [ymin, ymax]]
	std::vector<std::vector<size_t>> obj_bounds = {};

	/// \brief Find limits of the intial object
	void find_object_bounds();
	/// \brief Compute number of objects to replicate based on displacements
	void compute_numbers();
	/// \brief Compute x and y displacements of objects based on numbers in x and y
	void compute_displacements();

	/// \brief Find object dimensions in x and y
	/// @return Vector with x and y size, [x_dim, y_dim] in number of nodes
	std::vector<size_t> compute_object_size();
	/// \brief Find array length in x and y
	/// @param [in] obj_len_x - object dimensions in x in number of nodes
	/// @param [in] obj_len_y - object dimensions in y in number of nodes
	/// @return Vector with x and y length, [x_len, y_len] in number of nodes
	std::vector<size_t> compute_array_dimensions(size_t obj_len_x, size_t obj_len_y);
};

#endif
