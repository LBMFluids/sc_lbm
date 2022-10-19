#include <chrono>
#include <set>

#include "../../../include/geometry.h"
#include "../../../include/lbm.h"
#include "../../../include/rng.h"

class RNG;

/***************************************************** 
 *
 * Series of phase separation simulations
 *  
 *
 * - Simulates three different mass fractions of fluids
 * - Saves five intermediate results at the begining
 * 		of the simulations and then the final one
 * - They do run for up to several hours each
 *
 *****************************************************/

// Function that sets up and runs the simulation for different mass fractions
// and output name templates; last argument is a random number generator initialized in main
void phase_separation(const double mass_frac_fluid_1, const std::string fname, RNG& gen);

int main()
{
	// Random number generator
	RNG generator;
	// Simulations with different mass fractions
	phase_separation(0.335, "small_fraction_of_1", generator);
	phase_separation(0.665, "medium_fraction_of_1", generator);
	phase_separation(0.85, "high_fraction_of_1", generator);
}

void phase_separation(const double mass_frac_fluid_1, const std::string fname, RNG& gen)
{
	//
	// Data collection
	//

	const std::string path("output/");
	// Time steps for initial collection
	const std::set<int> dcol_steps{100, 500, 5000, 110000, 280000};

	//
	// Simulation settings
	//

	// Total number of iterations
	const int max_steps = 800000;

	//
	// Geometry
	//

	size_t Nx = 200, Ny = 200;
	Geometry geom(Nx, Ny);

	//
	// Immiscible fluids
	//

	// For correct initialization
	const bool is_mcmp = true;
	// Nominal density 
	const double rho_bar = 2.0;
	// Potential for repulsive interactions between fluids
	const double G_repulsive = 0.9;

	// Initialize fluids and interactions
	Fluid fluid_1("water");
	Fluid fluid_2("oil");

	// Initialize randomly perturbed densities and other variables
	fluid_1.initialize_randomly_perturbed_density(geom, mass_frac_fluid_1*rho_bar, gen, is_mcmp);
	fluid_2.initialize_randomly_perturbed_density(geom, (1 - mass_frac_fluid_1)*rho_bar, gen, is_mcmp);

	// Repulsive inter-fluid forces
	fluid_1.initialize_fluid_repulsion(G_repulsive);
	fluid_2.initialize_fluid_repulsion(G_repulsive);
	fluid_1.set_multicomponent_interactions(0.0);
	fluid_2.set_multicomponent_interactions(0.0);

	//
	// Simulation setup
	//	

	LBM lbm(geom);
	lbm.compute_solid_surface_force(geom, fluid_1, fluid_2);

	//
	// Simulation
	//

    // To measure the simulation time 
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (int step_i = 0; step_i < max_steps; ++step_i) {
		
		fluid_1.compute_density();
		fluid_2.compute_density();

		// Note - adding a (zero) volume force is omitted
		lbm.compute_fluid_repulsive_interactions(geom, fluid_1, fluid_2);
		lbm.compute_equilibrium_velocities(geom, fluid_1, fluid_2);
		lbm.collide(fluid_1, fluid_2);
		lbm.stream(geom, fluid_1, fluid_2);

		// Update and save
		if (dcol_steps.find(step_i) != dcol_steps.end()) {
			std::cout << "Reached simulation step " << step_i << " --- saving " << std::endl;
			fluid_1.write_density(path + fname + "_fluid_1_" + std::to_string(step_i) + ".txt");	
			fluid_2.write_density(path + fname + "_fluid_2_" + std::to_string(step_i) + ".txt");
		 }
	}

	// Simulation time
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Simulation time (main loop only) = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

	// Save end results
	fluid_1.write_density(path + fname + "_fluid_1_final.txt");	
	fluid_2.write_density(path + fname + "_fluid_2_final.txt");
}
