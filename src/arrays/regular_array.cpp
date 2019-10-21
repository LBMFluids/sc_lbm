#include "../../include/arrays/regular_array.h"

/*************************************************************** 
 * class: RegularArray
 *
 * Derived from Array, defines an array of objects 
***************************************************************/

// Find min and max node numbers for the object 
// in x and y, input nodes are [[x0, y0], ..., [xN, yN]]
void RegularArray::find_object_bounds()
{
	std::vector<std::vector<size_t>> input_nodes = get_input_nodes();
	// [xmin, xmax]	
	std::vector<size_t>	obj_bounds_x = {std::numeric_limits<size_t>::max(), 0};
 	// [yimn, ymax]	
	std::vector<size_t> obj_bounds_y = {std::numeric_limits<size_t>::max(), 0};

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
	
	obj_bounds.push_back(obj_bounds_x);
	obj_bounds.push_back(obj_bounds_y);
}

// Max number of objects in each direction
// given displacements
void RegularArray::compute_numbers()
{
	// Object and array properties
	std::vector<size_t> obj_size = compute_object_size();
	std::vector<size_t> array_dim = 
				compute_array_dimensions(obj_size.at(0), obj_size.at(1));	
	// Number of objects in each direction + 1 for the first object
	Nx = array_dim.at(0)/(dx + obj_size.at(0)) + 1;
	Ny = array_dim.at(1)/(dy + obj_size.at(1)) + 1;
}

// Object displacements given number of objects in each direction
void RegularArray::compute_displacements()
{
	// Object and array properties
	std::vector<size_t> obj_size = compute_object_size();
	std::vector<size_t> array_dim = 
				compute_array_dimensions(obj_size.at(0), obj_size.at(1));
	// Displacements
	dx = array_dim.at(0)/(Nx - 1) - obj_size.at(0);
	dy = array_dim.at(1)/(Ny - 1) - obj_size.at(1);
}

// Compute and store object size in x and y 
std::vector<size_t> RegularArray::compute_object_size()
{
	// Smallest and largest nodes the object occupies in each direction
	std::vector<size_t>	obj_bounds_x = obj_bounds.at(0); 
	std::vector<size_t> obj_bounds_y = obj_bounds.at(1); 
	// Dimensions of the object in number of nodes
	size_t obj_len_x = obj_bounds_x.at(1) - obj_bounds_x.at(0) + 1;
	size_t obj_len_y = obj_bounds_y.at(1) - obj_bounds_y.at(0) + 1;

	std::vector<size_t> obj_size = {obj_len_x, obj_len_y};
	return obj_size;
}

// Dimensions of the array in each direction in number of nodes - the 
// first object
std::vector<size_t> RegularArray::compute_array_dimensions(size_t obj_len_x, size_t obj_len_y)
{
	const std::vector<std::vector<size_t>> array_bounds = get_array_bounds();	
	size_t array_len_x = array_bounds.at(0).at(1) - array_bounds.at(0).at(0) - obj_len_x + 1;
	size_t array_len_y = array_bounds.at(1).at(1) - array_bounds.at(1).at(0) - obj_len_y + 1;

	std::vector<size_t> array_len = {array_len_x, array_len_y};
	return array_len;
}

// Compute the nodes occupied by array elements
void RegularArray::find_nodes()
{
	// Initial object, vector of [xi, yi] vectors
	std::vector<std::vector<size_t>> input_nodes = get_input_nodes();
	
	// Shift every node in each direction and store resulting cordinates 
	// Shifts valid for all nodes
	size_t x_shift = obj_bounds.at(0).at(1) + dx + 1; 
	size_t y_shift = obj_bounds.at(1).at(1) + dy + 1;

	size_t xpos = 0, ypos = 0;
	for (const auto& node : input_nodes){	
		for (size_t xi = 0; xi < Nx; ++xi){
			for (size_t yi = 0; yi < Ny; ++yi){
				xpos = node.at(0) + x_shift*xi;
				ypos = node.at(1) + y_shift*yi;
				array_nodes.push_back({xpos, ypos});
			}
		}
	}
}




