#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test utilities for the LBM class test suites
 *
 *****************************************************/

// Compute macroscopic properties, save to files along wiht density distributions
void compute_and_write_values(Geometry& geom, Fluid& fluid_1, const std::string& fname,
						const std::string& fname_f, const std::string& fname_feq)
{
	// Density distribution functions are already computed and stored
	fluid_1.compute_macroscopic(geom);
	fluid_1.write_density(fname + "density");
	/*fluid_1.write_ux(fname + std::to_string("ux"), geom);
	fluid_1.write_ux(fname + std::to_string("uy"), geom);
	*/fluid_1.write_feq(fname_feq);
	fluid_1.write_f(fname_f);
}
