#include "../../include/fluid.h"
#include "../../include/utils.h"
#include "../common/test_utils.h"

/***************************************************** 
 *
 * Test suite for the Fluid class
 *
 *****************************************************/

// Tests
bool empty_geom();
bool fluid_with_walls();
bool staggered_array();

int main()
{
	test_pass(empty_geom(), "Just the fluid");
	test_pass(fluid_with_walls(), "Fluid surrounded by walls");
	test_pass(staggered_array(), "Fluid and a staggered array");
}

bool empty_geom()
{
	// No solid nodes
	size_t Nx = 20, Ny = 10;
	Geometry geom(Nx, Ny);
	// All default
	double rho_0 = 2.0;
	Fluid water;
	water.simple_ini(geom, rho_0);
	// Output files
	std::string den_file{"test_data/macro_density"};
	std::string ux_file{"test_data/macro_vel_x"};
	std::string uy_file{"test_data/macro_vel_y"};
	std::string den_dist_file{"test_data/den_dist"};

	// The macroscopic density should be 2, velocities 0.0, and 
	// the distribution function should be 2/9 everywhere
	water.save_state(den_file, ux_file, uy_file, 10);
	water.write_f(den_dist_file);	

	return true;
}

bool fluid_with_walls()
{
	// No solid nodes
	size_t Nx = 20, Ny = 10;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	
	// All default
	double rho_0 = 1.0;
	Fluid water;
	water.simple_ini(geom, rho_0);

	// Output files
	std::string den_file{"test_data/wt_macro_density"};
	std::string ux_file{"test_data/wt_macro_vel_x"};
	std::string uy_file{"test_data/wt_macro_vel_y"};
	std::string den_dist_file{"test_data/wt_den_dist"};

	// The macroscopic density should be 1, velocities 0.0, and 
	// the distribution function should be 1/9 except for wall nodes
	water.save_state(den_file, ux_file, uy_file, 10);
	water.write_f(den_dist_file);

	return true;
}

bool staggered_array()
{
	return true;
}


