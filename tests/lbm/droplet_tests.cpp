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

//
// Test suite 
//

bool zero_solid_interactions_zero_flow();
bool zero_solid_interactions_with_flow();
bool with_solid_interactions_zero_flow();

//
// Supporting functions
//

// Constructs and fetches a map of default values, most of them
// constant across the tests
std::map<std::string, double> get_default_parameters(const size_t Nx, const size_t Ny, 
														const size_t dh);
// Geometry setup common for most of the tests 
Geometry make_geometry_multiphase(const size_t Nx, const size_t Ny, const size_t dh);

int main()
{
	test_pass(zero_solid_interactions_zero_flow(), "No solid interactions and no flow");
	test_pass(zero_solid_interactions_with_flow(), "No solid interactions and flow");
	test_pass(with_solid_interactions_zero_flow(), "With solid interactions and no flow");
}

bool zero_solid_interactions_zero_flow()
{
	// Data collection
	const std::string path("test_data/");
	const std::string gfile("mcmp_geom.txt");
	const std::string bulk_prefix("stationary_droplet_bulk");
	const std::string droplet_prefix("stationary_droplet_droplet");

	// Geometric parameters
	size_t Nx = 200, Ny = 99;
	size_t dh = 1;

	// Geometry - fluid and walls spanning y direction
	Geometry geom = make_geometry_multiphase(Nx, Ny, dh);
	geom.write(path + gfile);

	// All parameters (converted to const double) - default values
	std::map<std::string, double> parameters = get_default_parameters(Nx, Ny, dh);

	// Driving force
	std::vector<double> vol_force{0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Simulation
	run_and_collect(parameters, gfile, path, bulk_prefix, droplet_prefix, vol_force);

	// Comparison of the initial state
	if (!compare_with_correct({bulk_prefix, droplet_prefix}, path, "_f_")) {
		std::cerr << "Mismatch with MATLAB solution - initial state" << std::endl;
		return false;
	}

	// Comparison of the final (step 1) step
	int step_no = 1;
	if (!compare_with_correct({bulk_prefix,	droplet_prefix}, 
							"_step_" + std::to_string(step_no),		
							path, "_f_step_1_", "_f_eq_step_1_")) {
		std::cerr << "Mismatch with MATLAB solution" << std::endl;
		return false;
	}
	return true;
}

bool zero_solid_interactions_with_flow()
{
	// Data collection
	const std::string path("test_data/");
	const std::string gfile("mcmp_geom.txt");
	const std::string bulk_prefix("moving_droplet_bulk");
	const std::string droplet_prefix("moving_droplet_droplet");

	// Geometric parameters
	size_t Nx = 200, Ny = 99;
	size_t dh = 1;

	// Geometry - fluid and walls spanning y direction
	Geometry geom = make_geometry_multiphase(Nx, Ny, dh);
	geom.write(path + gfile);

	// All parameters (converted to const double) - default values
	std::map<std::string, double> parameters = get_default_parameters(Nx, Ny, dh);

	// Driving force
	std::vector<double> vol_force{0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::transform(vol_force.begin(), vol_force.end(), vol_force.begin(), [](double el) { return -1.0/6.0*el; });

	// Simulation
	run_and_collect(parameters, gfile, path, bulk_prefix, droplet_prefix, vol_force);

	// Comparison of the final (step 1) step
	int step_no = 1;
	if (!compare_with_correct({bulk_prefix,	droplet_prefix}, 
							"_step_" + std::to_string(step_no),		
							path, "_f_step_1_", "_f_eq_step_1_")) {
		std::cerr << "Mismatch with MATLAB solution" << std::endl;
		return false;
	}
	return true;
}

bool with_solid_interactions_zero_flow()
{
	// Data collection
	const std::string path("test_data/");
	const std::string gfile("mcmp_geom.txt");
	const std::string bulk_prefix("stationary_droplet_with_solid_bulk");
	const std::string droplet_prefix("stationary_droplet_with_solid_droplet");

	// Geometric parameters
	size_t Nx = 200, Ny = 99;
	size_t dh = 1;

	// Geometry - fluid and walls spanning y direction
	Geometry geom = make_geometry_multiphase(Nx, Ny, dh);
	geom.write(path + gfile);

	// All parameters (converted to const double) - default values
	std::map<std::string, double> parameters = get_default_parameters(Nx, Ny, dh);
	// Potential for interactions with solids
	parameters["G_solids_bulk"] = 20.3;
	parameters["G_solids_droplet"] = -1.0*parameters.at("G_solids_bulk");

	// Driving force
	std::vector<double> vol_force{0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Simulation
	run_and_collect(parameters, gfile, path, bulk_prefix, droplet_prefix, vol_force);

	// Comparison of the final (step 1) step
	int step_no = 1;
	if (!compare_with_correct({bulk_prefix,	droplet_prefix}, 
							"_step_" + std::to_string(step_no),		
							path, "_f_step_1_", "_f_eq_step_1_")) {
		std::cerr << "Mismatch with MATLAB solution" << std::endl;
		return false;
	}
	return true;
}
// Constructs and fetches a map of default values, most of them
// constant across the tests
std::map<std::string, double> get_default_parameters(const size_t Nx, const size_t Ny, 
														const size_t dh)
{	
	std::map<std::string, double> parameters; 

	// Geometric parameters
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

	return parameters;
}

// Geometry setup common for most of the tests
Geometry make_geometry_multiphase(const size_t Nx, const size_t Ny, const size_t dh)
{
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");

	return geom;
}
