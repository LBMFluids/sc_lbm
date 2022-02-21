#include "../../include/lbm.h"
#include "../common/test_utils.h"

/***************************************************** 
 *
 * Test suite for the LBM class
 *
 *****************************************************/

bool single_phase_empty_collision_test();

int main()
{
	test_pass(single_phase_empty_collision_test(), "Single phase, empty domain");
}

bool single_phase_empty_collision_test()
{
	// No solid nodes
	size_t Nx = 20, Ny = 10;
	Geometry geom(Nx, Ny);
	// All default
	double rho_0 = 2.5;
	Fluid coffee;
	coffee.simple_ini(geom, rho_0);

	// Interface
	LBM lbm;

	return true;	
}
