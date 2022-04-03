#include "../../include/lbm.h"
#include "../common/test_utils.h"

/***************************************************** 
 *
 * Test suite for the LBM class
 *
 *****************************************************/

bool single_phase_empty_test();
bool single_phase_walls_test();

int main()
{
	test_pass(single_phase_empty_test(), "Single phase, empty domain");
	test_pass(single_phase_walls_test(), "Single phase, walls");
}

/// Empty geometry with no volume force
/// @details The macroscopic results should be the same as initial
bool single_phase_empty_test()
{
	// No solid nodes
	size_t Nx = 5, Ny = 10;
	Geometry geom(Nx, Ny);
	geom.write("test_data/empty_domain.txt");	

	// All default
	double rho_ini = 2.5;

	Fluid coffee;
	coffee.simple_ini(geom, rho_ini);

	// Add a zero force just for testing purposes
	const std::vector<double> no_force(9, 0.0);

	// Initial values
	coffee.compute_macroscopic();
	const std::vector<double> rho_0 = coffee.get_rho();
	const std::vector<double> ux_0 = coffee.get_ux();
	const std::vector<double> uy_0 = coffee.get_uy();
	coffee.write_f("test_data/empty_f_dist");

	// Simulation
	LBM lbm(geom);
	lbm.compute_macroscopic(coffee);
	lbm.collide(coffee);	
	lbm.add_volume_force(geom, coffee, no_force);		
	lbm.stream(geom, coffee);

	// Final values
	coffee.compute_macroscopic();
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
	
	// All default
	double rho_ini = 2.0;

	Fluid water;
	water.simple_ini(geom, rho_ini);

	// Add a zero force just for testing purposes
	const std::vector<double> no_force(9, 0.0);

	// Initial values
	water.compute_macroscopic();
	const std::vector<double> rho_0 = water.get_rho();
	const std::vector<double> ux_0 = water.get_ux();
	const std::vector<double> uy_0 = water.get_uy();

	// Simulation
	LBM lbm(geom);
	lbm.collide(water);	
	lbm.add_volume_force(geom, water, no_force);		
	lbm.stream(geom, water);

	// Initial values
	water.compute_macroscopic();
	const std::vector<double> rho_F = water.get_rho();
	const std::vector<double> ux_F = water.get_ux();
	const std::vector<double> uy_F = water.get_uy();

	// Check
	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		for (const auto& r : rho_F) {
			std::cout << r << " ";
		}
		std::cout << std::endl;
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
