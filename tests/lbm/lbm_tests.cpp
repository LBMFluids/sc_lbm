#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test utilities for the LBM class test suites
 *
 *****************************************************/

void compute_and_write_values(Fluid& fluid_1, const std::string& fname)
{
	fluid_1.compute_macroscopic(geom);
	fluid_1.compute_f_equilibrium(geom);
	// Add separate f_equilibrium save
	fluid_1.save_state(fname);
}
