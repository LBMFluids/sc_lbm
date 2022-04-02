#include "../../include/lbm.h"
#include "../common/test_utils.h"

/***************************************************** 
 *
 * Test suite for the LBM class
 *
 *****************************************************/

bool single_phase_empty_test();

int main()
{
	test_pass(single_phase_empty_test(), "Single phase, empty domain");
}

/// Empty geometry with no volume force
/// @details The macroscopic results should be the same as initial
bool single_phase_empty_test()
{
	// No solid nodes
	size_t Nx = 2, Ny = 2;
	Geometry geom(Nx, Ny);
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

	// Simulation
	LBM lbm(geom);
	lbm.collide(coffee);	
	lbm.add_volume_force(geom, coffee, no_force);		
	lbm.stream(geom, coffee);

	// Initial values
	coffee.compute_macroscopic();
	const std::vector<double> rho_F = coffee.get_rho();
	const std::vector<double> ux_F = coffee.get_ux();
	const std::vector<double> uy_F = coffee.get_uy();

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
