#include <chrono>

#include "../../../include/geometry.h"
#include "../../../include/lbm.h"

/***************************************************** 
 *
 * Simulations for computation of surface tension
 *	with the Laplace law 
 *  
 * - Simulates seven different radiuses of droplets
 * - Saves the densities of both fluids at the end		
 * - Saves densities for intermediate steps as part 
 *    of a convergence study for one case 
 *
 *****************************************************/

// Sets up the simulation in which a drople is initialized 
// as a square with side "side" and let equilibrate. All in
// a periodic domain with no flow. The program saves the
// densities at the end but can also save intermediate 
// results if save_intermediate is set to true. The saved 
// items have a common name defined as a function argument.
void droplet_equilibration(const int, const std::string&, 
							bool save_intermediate = false);

int main()
{
	// Initializes the droplet as a suare
	// The side is roughly twice the radius
	const std::vector<int> square_sides{80, 70, 60, 50, 40, 30, 20};

	// Common file name prefix
	const std::string fname{"laplace_law_diameter"};
	
	// Run the simulation for each square side
	for (const auto& side : square_sides) {
		if (side != 30) {
			droplet_equilibration(side, fname + "_" + std::to_string(side));
		} else {
			droplet_equilibration(side, fname + "_" + std::to_string(side), true);
		}
	}
}

void droplet_equilibration(const int droplet_side, const std::string& fname, 
			bool save_intermediate)
{
	//
	// Data collection
	//

	const std::string path("output/");
	const int save_every = 1000;

	//
	// Simulation settings
	//

	// Total number of iterations
	const int max_steps = 40000;

	//
	// Geometry
	//

	size_t Nx = 400, Ny = 400;
	Geometry geom(Nx, Ny);

	//
	// Simulation interface
	//

	LBM lbm(geom);

	//
	// Immiscible fluids
	//

	// Bulk and dissolved densities of the 
	// continuous (bulk) liquid and the dispersed phase
	const double rho_bulk = 2.0, rho_droplet = 2.0;
	const double rho_b_in_d = 0.06, rho_d_in_b = 0.06;
	// Potential for repulsive interactions between fluids
	const double G_repulsive = 0.9;
	// Potential for interactions with solids
	const double G_solids_bulk = 0.0;
	const double G_solids_droplet = -1.0*G_solids_bulk;
	// Properties of the initial droplet
	// Coordinates of droplet center
	const double xc = 200, yc = 200;
	// x and y half-widths
	const double half_Lx = droplet_side, half_Ly = droplet_side;

	Fluid bulk_fluid("water");
	Fluid droplet_fluid("oil");

	// Initialize all variables to zero, including densities
	bulk_fluid.zero_density_ini(geom);
	droplet_fluid.zero_density_ini(geom);

	// Repulsive inter-fluid forces
	bulk_fluid.initialize_fluid_repulsion(G_repulsive);
	droplet_fluid.initialize_fluid_repulsion(G_repulsive);

	// Initialize the continuous phase and the immersed droplet
	lbm.initialize_fluid_rectangle(geom, bulk_fluid, 
		droplet_fluid, rho_bulk, rho_droplet, rho_b_in_d, 
		rho_d_in_b, xc, yc, half_Lx, half_Ly);

	// Compute interactions with solids 
	bulk_fluid.set_multicomponent_interactions(G_solids_bulk);
	droplet_fluid.set_multicomponent_interactions(G_solids_droplet);
	lbm.compute_solid_surface_force(geom, bulk_fluid, droplet_fluid);

	//
	// Simulation
	//

    // To measure the simulation time 
    std::chrono::steady_clock::time_point t_begin = std::chrono::steady_clock::now();

	for (int step_i = 0; step_i < max_steps; ++step_i) {
		
		bulk_fluid.compute_density();
		droplet_fluid.compute_density();

		// Note - adding a (zero) volume force is omitted
		lbm.compute_fluid_repulsive_interactions(geom, bulk_fluid, droplet_fluid);
		lbm.compute_equilibrium_velocities(geom, bulk_fluid, droplet_fluid);
		lbm.collide(bulk_fluid, droplet_fluid);
		lbm.stream(geom, bulk_fluid, droplet_fluid);

		// Update and save
		if ((save_intermediate) && !(step_i % save_every)) {
			std::cout << "Reached simulation step " << step_i << " --- saving " << std::endl;
			bulk_fluid.write_density(path + fname + "_bulk_fluid_" + std::to_string(step_i) + ".txt");	
			droplet_fluid.write_density(path + fname + "_droplet_fluid_" + std::to_string(step_i) + ".txt");
		 }
	}

	// Simulation time
    std::chrono::steady_clock::time_point t_end = std::chrono::steady_clock::now();
    std::cout << "Simulation time (main loop only) = " << std::chrono::duration_cast<std::chrono::seconds> (t_end - t_begin).count() << "[s]" << std::endl;

	// Save end results
	bulk_fluid.write_density(path + fname + "_bulk_fluid_final.txt");	
	droplet_fluid.write_density(path + fname + "_droplet_fluid_final.txt");
}
