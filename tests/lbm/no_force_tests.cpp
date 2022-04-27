#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test suite for the LBM class - no driving force
 *
 *****************************************************/

bool single_phase_empty_test();
bool single_phase_x_walls_test();
bool single_phase_y_walls_test();
bool single_phase_array_no_walls_test();
bool single_phase_array_x_walls_test();
bool single_phase_array_y_walls_test();

int main()
{
	test_pass(single_phase_empty_test(), "Single phase, empty domain");
	test_pass(single_phase_walls_test(), "Single phase, walls");
	test_pass(single_phase_array_test(), "Single phase, array of objects");
}

/// Empty geometry with no volume force
/// @details The macroscopic results should be the same as initial
bool single_phase_empty_test()
{
	// External forcing term
	const std::vector<double> no_force(9, 0.0);

	// Filename templates
	std::string fname_ini("test_data/no_force_empty_ini");
	std::string fname_col("test_data/no_force_empty_collide");
	std::string fname_col("test_data/no_force_empty_add_force");
	std::string fname_col("test_data/no_force_empty_stream");

	// No solid nodes
	size_t Nx = 5, Ny = 10;
	Geometry geom(Nx, Ny);
	geom.write("test_data/empty_domain.txt");	

	// Create and initialize the fluid 
	double rho_ini = 2.5;

	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	compute_and_write_values(test_fluid, fname_ini);

	// Simulation setup
	// Periodic boundaries in x and y 
	bool pb_x = true, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, coffee);	
	lbm.add_volume_force(geom, coffee, no_force);		
	lbm.stream(geom, coffee);

	// Final values
	coffee.compute_macroscopic(geom);
	const std::vector<double> rho_F = coffee.get_rho();
	const std::vector<double> ux_F = coffee.get_ux();
	const std::vector<double> uy_F = coffee.get_uy();
	coffee.write_f("test_data/final_empty_f_dist");

	// Check
	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
	return true;	
}

/// Domain with walls and with no volume force
/// @details The macroscopic results should be the same as initial
bool single_phase_walls_test()
{
	// No solid nodes
	size_t Nx = 20, Ny = 10;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	geom.write("test_data/domain_with_walls.txt");
	
	// All default
	double rho_ini = 2.0;

	Fluid water;
	water.simple_ini(geom, rho_ini);

	// Add a zero force just for testing purposes
	const std::vector<double> no_force(9, 0.0);

	// Initial values
	water.compute_macroscopic(geom);
	const std::vector<double> rho_0 = water.get_rho();
	const std::vector<double> ux_0 = water.get_ux();
	const std::vector<double> uy_0 = water.get_uy();
	water.write_f("test_data/initial_walls_f_dist");

	// Setup with non-periodic boundaries in y direction
	bool pb_x = true, pb_y = false;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, water);	
	lbm.add_volume_force(geom, water, no_force);		
	lbm.stream(geom, water);

	// Final values
	water.compute_macroscopic(geom);
	const std::vector<double> rho_F = water.get_rho();
	const std::vector<double> ux_F = water.get_ux();
	const std::vector<double> uy_F = water.get_uy();
	water.write_f("test_data/final_walls_f_dist");

	// Check
	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
	return true;	
}

/// Domain with an array of objects and walls and with no volume force
/// @details The macroscopic results should be the same as initial
bool single_phase_array_test()
{
	// Walls and a staggered array
	size_t Nx = 200, Ny = 100;
	size_t dh = 2;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");
	geom.write("test_data/domain_with_array.txt");
	// Array and its construction
	// ellipse 200 100 5 7 10 15 5 190 3 70 0 0 20 5 test passed 
	// Array created by imposing the number of objects
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70, dx = 0, dy = 0;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);

	// All default
	double rho_ini = 2.5;

	Fluid water;
	water.simple_ini(geom, rho_ini);

	// Add a zero force just for testing purposes
	const std::vector<double> no_force(9, 0.0);

	// Initial values
	water.compute_macroscopic(geom);
	const std::vector<double> rho_0 = water.get_rho();
	const std::vector<double> ux_0 = water.get_ux();
	const std::vector<double> uy_0 = water.get_uy();
	water.write_f("test_data/initial_array_f_dist");

	// Setup with non-periodic boundaries in x direction
	bool pb_x = false, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, water);	
	lbm.add_volume_force(geom, water, no_force);		
	lbm.stream(geom, water);

	// Final values
	water.compute_macroscopic(geom);
	const std::vector<double> rho_F = water.get_rho();
	const std::vector<double> ux_F = water.get_ux();
	const std::vector<double> uy_F = water.get_uy();
	water.write_f("test_data/final_array_f_dist");

	// Check
	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
	return true;	
}
