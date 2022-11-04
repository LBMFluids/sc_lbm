#include <chrono>

#include "../../../include/geometry.h"
#include "../../../include/lbm.h"

/***************************************************** 
 *
 * Simulations for computation of contact angles 
 *	with different solid affinities
 *  
 * - Droplet with a fixed radius positioned on a 
 *		solid "shelf" 
 * - Saves the densities of both fluids at the end		
 *
 *****************************************************/

// Sets up the simulation in which a droplet is initialized 
// as a square on a solid rectangle and let equilibrate. 
// As the affinity of both fluids changes with different 
// solid-liquid interaction parameter, different contact
// angles between the droplet and the solid surface arise.
// This is a periodic domain with no flow. The program 
// saves the densities at the end The saved items have a 
// common name that is hardcoded.
void droplet_on_a_solid(const double);

int main()
{
	// Liquid-solid interaction parameters for a droplet
 	// The parameter for the surrounding liquid is a negative of this value  
	const std::vector<double> Gs_droplet{-0.4, -0.3, -0.2, -0.1, 0.1, 0.2, 0.3, 0.4};

	// Run the simulation for each liquid-solid interaction parameter
	for (const auto& Gsd : Gs_droplet) {
		droplet_on_a_solid(Gsd);
	}
}

void droplet_on_a_solid(const double Gs_droplet)
{
	//
	// Data collection
	//

	const std::string fname("output/contact_angle");

	//
	// Simulation settings
	//

	// Total number of iterations
	const int max_steps = 30000;

	//
	// Geometry
	//

	size_t Nx = 400, Ny = 306;
	Geometry geom(Nx, Ny);
	
	// Rectangular solid
	const double rec_dx = 341, rec_dy = 20, rec_xc = 199, rec_yc = 152;
	geom.add_rectangle(rec_dx, rec_dy, rec_xc, rec_yc);

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
	const double G_solids_bulk = -1.0*Gs_droplet;
	const double G_solids_droplet = Gs_droplet;
	// Properties of the initial droplet
	// Coordinates of droplet center
	const double xc = 199, yc = 192;
	// x and y half-widths
	const double half_Lx = 30, half_Ly = 30;

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

	}

	// Simulation time
    std::chrono::steady_clock::time_point t_end = std::chrono::steady_clock::now();
    std::cout << "Simulation time (main loop only) = " << std::chrono::duration_cast<std::chrono::seconds> (t_end - t_begin).count() << "[s]" << std::endl;

	// Save end results
	bulk_fluid.write_density(fname + "_bulk_fluid_final_" + std::to_string(Gs_droplet) + ".txt");	
	droplet_fluid.write_density(fname + "_droplet_fluid_final_" + std::to_string(Gs_droplet) + ".txt");
}
