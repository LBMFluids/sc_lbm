#include <chrono>

#include "../../include/geometry.h"
#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test suite for the LBM class - two phase, two
 *	component flows 
 *
 * This is a regression test suite that checks the
 * 	simulation outcomes against a MATLAB simulation
 * 	./lbm_matlab/two_phase_flow/main_bubble.m
 *
 * This test is an exact reproduction of this script.
 * To succesfully run the test the user needs to 
 * have all the test_data/matlab_stationary_droplet*
 * type files located in the GitHub repository.  
 *
 * Notes:
 *
 * - Currently runs four tests, with different
 *		volume forces and solid interactions
 * - Values used in the tests are exaggerated to 
 *		capture any differences
 * - The tests run only one simulation step and
 *		compare before and after 
 *
 *****************************************************/

int main()
{

	//
	// Data collection
	//

	const std::string path("test_data/");
	const std::string gfile("mcmp_geom.txt");
	const std::string bulk_prefix("stationary_droplet_bulk");
	const std::string droplet_prefix("stationary_droplet_droplet");

	// All parameters (converted to const double)
	std::map<std::string, double> parameters;

	// Driving force
	std::vector<double> vol_force{0, 0, 0, 0, 0, 0, 0, 0, 0};

	//
	// Geometry - fluid and walls spanning y direction
	// 

	size_t Nx = 200, Ny = 99;
	size_t dh = 1;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");
	geom.write(path + gfile);

	parameters["Nx"] = Nx;
	parameters["Ny"] = Ny;
	parameters["dh"] = dh;

	// Bulk and dissolved densities of the 
	// continuous (bulk) liquid and the dispersed phase
	parameters["rho_bulk"] = 2.0; 
	parameters["rho_droplet"] = 2.0;
	parameters["rho_b_in_d"] = 0.6; 
	parameters["rho_d_in_b"] = 0.6;
	// Potential for repulsive interactions between fluids
	parameters["G_repulsive"] = 109.0;
	// Potential for interactions with solids
	parameters["G_solids_bulk"] = 0.0;
	parameters["G_solids_droplet"] = -1.0*parameters.at("G_solids_bulk");
	// Properties of the initial droplet
	// Coordinates of droplet center
	parameters["xc"] = 99; 
	parameters["yc"] = 58;
	// x and y half-widths
	parameters["half_Lx"] = 21; 
 	parameters["half_Ly"] = 21;

	run_and_collect(parameters, gfile, path, bulk_prefix, droplet_prefix, vol_force);

	// Comparison
	if (!compare_with_correct({bulk_prefix, droplet_prefix}, path, "_f_step_1_")) {
		std::cerr << "Mismatch with MATLAB solution" << std::endl;
		return false;
	}

}



