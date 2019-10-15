#include "../../arrays/regular_array.h"

/*************************************************************** 
 * class: RegularArray
 *
 * Derived from Array, defines an array of objects 
***************************************************************/

// Max number of objects in each direction
// given displacements
void RegularArray::compute_numbers()
{

	// Find min and max node numbers for the object 
	// in x and y
	const std::vector<std::vector<size_t>> input_nodes = get_input_nodes();
	std::vector<size_t>	obj_bounds_x = {std::numeric_limits_max<size_t>,0}; 
	std::vector<size_t> obj_bounds_y = {std::numeric_limits_max<size_t>,0};
	for (const auto& node : input_nodes){
		// Min x
		if (node.at(0) < obj_bounds_x.at(0))
			obj_bounds_x.at(0) = node.at(0);
		// Max x
		if (node.at(0) > obj_bounds_x.at(1))
			obj_bounds_x.at(1) = node.at(0);		
		// Min y
		if (node.at(1) < obj_bounds_y.at(0))
			obj_bounds_y.at(0) = node.at(1);
		// Max y
		if (node.at(1) > obj_bounds_y.at(1))
			obj_bounds_y.at(1) = node.at(1);			
	}

	// Then compute the object number in x and y
	// Dimensions of the object in number of nodes
	size_t obj_len_x = obj_bounds_x.at(1) - obj_bounds_x.at(0) + 1;
	size_t obj_len_y = obj_bounds_y.at(1) - obj_bounds_y.at(0) + 1;
	// Dimensions of the array in each direction
	const std::vector<std::vector<size_t>> array_bounds = get_array_bounds();	
	size_t array_len_x = array_bounds.at(0).at(0) - array_bounds.at(1).at(0) + 1;
	size_t array_len_y = array_bounds.at(0).at(1) - array_bounds.at(1).at(1) + 1;
	// Number of objects in each direction - +1 for the first object
	Nx = array_len_x/(dx + obj_len_x) + 1;
	Ny = array_len_y/(dy + obj_len_y) + 1;
}
