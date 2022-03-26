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
/// @details The result should be the same as initial
bool single_phase_empty_test()
{
	// No solid nodes
	size_t Nx = 2, Ny = 2;
	Geometry geom(Nx, Ny);
	// All default
	double rho_0 = 2.5;
	Fluid coffee;
	coffee.simple_ini(geom, rho_0);
	// Add a zero force just for testing purposes
	const std::vector<double> no_force(9, 0.0);
	double tol = 1e-5;

	// Initial value
	const std::vector<double> f_dist_ini = coffee.get_f_dist();
   std::cout << "ini "<< std::endl;
	for (const auto f : f_dist_ini) {
        std::cout << f << " ";
    }  
  std::cout << std::endl;
	LBM lbm(geom);

	// Simulation
	lbm.collide(coffee);	
	lbm.add_volume_force(geom, coffee, no_force);		
	lbm.stream(geom, coffee);

	// Check
	// Final value
	const std::vector<double> f_dist_final = coffee.get_f_dist();

	for (const auto f : f_dist_final) {
        std::cout << f << " ";
    }  
  std::cout << std::endl;




	if (!is_equal_floats<double>({f_dist_ini}, {f_dist_final}, tol)) {
		std::cerr << "Initial and final density distributions don't match" << std::endl;
		return false;
	} 
	return true;	
}
