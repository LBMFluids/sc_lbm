#include <chrono>

#include "../../../include/geometry.h"
#include "../../../include/lbm.h"

/***************************************************** 
 *
 * Simulations of a droplet flowing through a
 * microchannel with hydrophilic or hydrophobic walls 
 *  
 * - Simulations and analysis follow this 
 *		publication: 
 *	
 *	 https://doi-org.elib.uah.edu/10.1002/jbm.b.33440 
 * 
 * - The droplet is hydrophobic and in the above
 *		publication it is refered to as a bubble
 *
 * - Saves the densities and velocities 
 *		of both fluids at the end		
 *
 *****************************************************/

// Sets up and runs the simulation of a droplet flowing
// through a microchannel of height N_y. The driving
// force is dPdL and the droplet is initialized with 
// height h_d at the channel center. Gs_bulk is the 
// interaction parameter between the bulk fluid and the
// solid channel walls. Top and bottom boundaries are
// walls and the sides are periodic. 
void flowing_droplet(const double dPdL, const double Gs_bulk, 
								const double h_d, const size_t N_y);

int main()
{
	// Pressure drops (driving forces)
	const std::vector<double> dPdL{1e-6, 5e-6, 7.5e-6, 1e-5};

	// Liquid-solid interaction parameters for the bulk liquid
 	// The parameter for the droplet is a negative of this value  
	// Negative value signifies hydrophilic and positivei (attractive),
	// hydrophobic (repulsive) interactions with the solid
	const std::vector<double> Gs_bulk{-0.294, -0.11};

	// Channel height
	const std::vector<size_t> Ny{318, 318};

	// Height of the initial droplet (wall thickness is 1 node on both sides)
	const std::vector<double> h_drop{316, 312};

	// Run the simulation for specific combinations of parameters 
	for (const auto& dr_force : dPdL) {
		for (size_t ip = 0; ip < Gs_bulk.size(); ++ip) {
			flowing_droplet(dr_force, Gs_bulk.at(ip), h_drop.at(ip), Ny.at(ip));
		}
	}
}

void flowing_droplet(const double dPdL_in, const double G_solids_bulk, 
								const double Ly, const size_t Ny)
{
	//
	// Data collection
	//

	const std::string fname("output/flowing_droplet");

	//
	// Simulation settings
	//

	// Total number of iterations
	const int max_steps = 25000;

	// External forcing term
	const double beta = 1.0/6.0;
	const double dPdL_fin = 1e-4*beta*dPdL_in;
	std::vector<double> vol_force{0, 1, 0, -1, 0, 1, -1, -1, 1};
	std::for_each(vol_force.begin(), vol_force.end(), [&dPdL_fin](double& el) { el *= dPdL_fin; });

	//
	// Geometry
	//

	size_t Nx = 4.0*318;
	size_t dh = 1;
	Geometry geom(Nx, Ny);

	// Walls
	geom.add_walls(dh, "x");

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
	const double G_solids_droplet = -1.0*G_solids_bulk;
	// Properties of the initial droplet
	// Coordinates of droplet center
	const double xc = Nx/2, yc = Ny/2;
	// x and y half-widths of the droplet
	const double half_Lx = 0.5*0.7*Nx;
	const double half_Ly = 0.5*Ly;

	Fluid bulk_fluid("liquid");
	Fluid droplet_fluid("droplet");

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

		lbm.compute_fluid_repulsive_interactions(geom, bulk_fluid, droplet_fluid);
		lbm.compute_equilibrium_velocities(geom, bulk_fluid, droplet_fluid);
		lbm.collide(bulk_fluid, droplet_fluid);
		lbm.add_volume_force(geom, bulk_fluid, droplet_fluid, vol_force);
		lbm.stream(geom, bulk_fluid, droplet_fluid);

	}

	// Simulation time
    std::chrono::steady_clock::time_point t_end = std::chrono::steady_clock::now();
    std::cout << "Simulation time (main loop only) = " << std::chrono::duration_cast<std::chrono::seconds> (t_end - t_begin).count() << "[s]" << std::endl;

	// Save end results
	bulk_fluid.write_density(fname + "_bulk_fluid_final_" + std::to_string(dPdL_in) + "_" + std::to_string(G_solids_bulk) + ".txt");	
	droplet_fluid.write_density(fname + "_droplet_fluid_final_" + std::to_string(dPdL_in) + "_" + std::to_string(G_solids_bulk) + ".txt");

}
