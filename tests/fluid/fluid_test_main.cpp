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
bool object_array();

// Supporting functions
bool check_from_files(const std::string&, const double, const Geometry&);

int main()
{
	test_pass(empty_geom(), "Just the fluid");
	test_pass(fluid_with_walls(), "Fluid surrounded by walls");
	test_pass(object_array(), "Fluid and a staggered array");
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

	return check_from_files("test_data/", rho_0, geom);
}

bool fluid_with_walls()
{
	// Walls 
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

bool object_array()
{
	// Walls and a staggered array
	size_t Nx = 200, Ny = 100;
	size_t dh = 2;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");
	
	// Array and its construction
	// ellipse 200 100 5 7 10 15 5 190 3 70 0 0 20 5 test passed 
	// Array created by imposing the number of objects
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70, dx = 0, dy = 0;
	size_t ob_num_x = 20, ob_num_y = 5;
	
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);

	// All default
	double rho_0 = 2.5;
	Fluid water;
	water.simple_ini(geom, rho_0);

	// Output files
	std::string den_file{"test_data/oat_macro_density"};
	std::string ux_file{"test_data/oat_macro_vel_x"};
	std::string uy_file{"test_data/oat_macro_vel_y"};
	std::string den_dist_file{"test_data/oat_den_dist"};

	// The macroscopic density should be 1, velocities 0.0, and 
	// the distribution function should be 2/9 except for wall nodes
	water.save_state(den_file, ux_file, uy_file, 10);
	water.write_f(den_dist_file);

	return true;
}

/// Loads and checks files with macroscopic variables and density functions
/// @details fname is the precursor, like oat_ - rest is assumed as convention
bool check_from_files(const std::string& fname, const double exp_den, const Geometry& geom)
{
	// Common placeholders
	std::string delim{" "};
	bool single_file = true; 
	std::vector<size_t> dims = {geom.Ny(),geom.Nx(),0};

	// Macroscopic density
	LbmIO den_io(fname + "macro_density_10.txt", delim, single_file, dims);
	std::vector<std::vector<double>> density = den_io.read_vector<double>();
	
	return true;
}
