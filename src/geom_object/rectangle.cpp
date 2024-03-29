#include "../../include/geom_object/rectangle.h"
#include "../../include/misc_checks.h"

/*************************************************************** 
 * class: rectangle
 *
 * Derived from geom_object, defines a rectangle by computing
 * the nodes it should occupy from given dimensions and 
 * center.  
***************************************************************/

void Rectangle::find_nodes()
{
	std::vector<size_t> ctr = get_pos();
	// If number of nodes is even in any direction, decrease by 1
	// so the rectangle can be built evenly from the center
	std::vector<bool> node_check = check_is_even_num_nodes(_Lx, _Ly);
	if (node_check[0]) {
		_Lx--;
	}
	if (node_check[1]) {
		_Ly--;
	}
	size_t x0 = ctr[0] - _Lx/2, y0 = ctr[1] - _Ly/2; 
	size_t xf = ctr[0] + _Lx/2, yf = ctr[1] + _Ly/2;
	for (size_t ix = x0; ix <= xf; ix++) {
		for (size_t iy = y0; iy <= yf; iy++) {
			object_nodes.push_back({ix, iy});
		}
	}
}
