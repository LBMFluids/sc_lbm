#include <chrono>

#include "../../include/geometry.h"
#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

int main()
{

	//
	// Data collection
	//

	const std::string path("test_data/");
	const std::string bulk_prefix("stationary_droplet_bulk");
	const std::string droplet_prefix("stationary_droplet_droplet");
	std::vector<std::string> saved_steps;

	//
	// Restart settings
	// 

	const std::string bulk_fread("matlab_stationary_droplet_bulk_f_step_2000");
	const std::string droplet_fread("matlab_stationary_droplet_droplet_f_step_2000");

	//
	// Simulation settings
	//

	// Total number of iterations
	const int max_steps = 1;
	// Save variables this many steps
	const int save_every = 1;

	//
	// Geometry - fluid and walls spanning y direction
	// 

	size_t Nx = 1070, Ny = 297;
	size_t dh = 1;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");

	//
	// Simulation interface
	//

	LBM lbm(geom);

	//
	// Initialize fluids and interactions
	// 

	// Potential for repulsive interactions between fluids
	const double G_repulsive = 0.9;
	// Potential for interactions with solids
	const double G_solids_bulk = 0.0;
	const double G_solids_droplet = -1.0*G_solids_bulk;

	Fluid bulk_fluid("water");
	Fluid droplet_fluid("oil");

	// Initialize all variables to zero, including densities
	bulk_fluid.zero_density_ini(geom);
	droplet_fluid.zero_density_ini(geom);

	// Repulsive inter-fluid forces
	bulk_fluid.initialize_fluid_repulsion(G_repulsive);
	droplet_fluid.initialize_fluid_repulsion(G_repulsive);

	// Load the density distributions of both fluids from file	
	bulk_fluid.restart_from_file(path + bulk_fread);
	droplet_fluid.restart_from_file(path + droplet_fread);

	// Compute interactions with solids (valid for the entire run) 
	bulk_fluid.set_multicomponent_interactions(G_solids_bulk);
	droplet_fluid.set_multicomponent_interactions(G_solids_droplet);
	lbm.compute_solid_surface_force(geom, bulk_fluid, droplet_fluid);

	//
	// Verification of the initial state
	//	
	
	compute_and_write_values(geom, bulk_fluid, bulk_prefix, 
						bulk_prefix + "_f", path);	
	compute_and_write_values(geom, droplet_fluid, droplet_prefix, 
						droplet_prefix + "_f", path);

	//
	// Simulation
	//

    // To measure the simulation time 
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// The steps follow MATLAB's implementation	
	for (int step_i = 2; step_i <= max_steps + 1; ++step_i) {
		
		// Simulation steps  	
		bulk_fluid.compute_density();
		droplet_fluid.compute_density();
		lbm.compute_fluid_repulsive_interactions(geom, bulk_fluid, droplet_fluid);
		lbm.compute_equilibrium_velocities(geom, bulk_fluid, droplet_fluid);
		lbm.collide(bulk_fluid, droplet_fluid);
		lbm.stream(geom, bulk_fluid, droplet_fluid);

		// Update and save
		if (!(step_i % save_every)) {
			std::cout << "Reached simulation step " << step_i << " --- saving " << std::endl;
			compute_and_write_values(geom, bulk_fluid, 
						bulk_prefix + "_step_" + std::to_string(step_i),
						bulk_prefix + "_f_step_" + std::to_string(step_i), path);	
			compute_and_write_values(geom, droplet_fluid, 
						droplet_prefix + "_step_" + std::to_string(step_i),
						droplet_prefix + "_f_step_" + std::to_string(step_i), path);
			saved_steps.push_back(std::to_string(step_i));
		 }
	}

	// Simulation time
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Simulation time (main loop only) = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

	// Comparison
	for (const auto& si : saved_steps) {
		if (!compare_with_correct({bulk_prefix + "_step_" + si, droplet_prefix + "_" + si}, path, "_f_step_" + si + "_")) {
			std::cerr << "Mismatch with MATLAB solution" << std::endl;
			return false;
		}
	}
}



