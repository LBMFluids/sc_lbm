#include "geometry_tests.h"

/*************************************************************** 
 * Common functions for testing Geometry class 
***************************************************************/

bool check_geom_obj(const Geometry& geom, const size_t Nx, const size_t Ny)
{
	const bool* raw_ptr = geom.get_geom();
	std::vector<bool> raw(raw_ptr, raw_ptr+Nx*Ny);
	// Check dimensions	
	if (geom.Nx() != Nx || geom.Ny() != Ny || raw.size() != Nx*Ny)
		return false;
	// Check if all entries 1 i.e. tru
	if (std::find_if_not(raw.begin(), raw.end(), 
				[](const bool& x){ return (x == true); }) != raw.end())
		return false;
	return true;
}


