#include "../../include/geom_object/ellipse.h"
#include "../../include/misc_checks.h"

/*************************************************************** 
 * class: Ellipse 
 *
 * Derived from geom_object, defines a ellipse by computing
 * the nodes it should occupy from given dimensions and 
 * center.  
***************************************************************/

void Ellipse::find_nodes()
{
	std::vector<size_t> ctr = get_pos();
	// If number of nodes is even in any direction, decrease by 1
	// so the square can be built evenly from the center
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
			if (((ix-ctr[0])*(ix-ctr[0])*_Ly*_Ly/4 + 
				 (iy-ctr[1])*(iy-ctr[1])*_Lx*_Lx/4) <=_Lx*_Lx*_Ly*_Ly/16) {
				object_nodes.push_back({ix, iy});
			}
		}
	}
}
