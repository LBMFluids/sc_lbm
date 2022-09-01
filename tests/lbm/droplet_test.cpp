#include "../../include/geometry.h"
#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test suite for the LBM class - MCMP stationary 
 * 	droplet evolving in a continuous fluid of a 
 *	different type
 *
 * Notes:
 *
 * - Can serve as a starting point for next 
 *		simulations
 * - There are not solid interactions here
 *
 *****************************************************/

int main()
{

	//
	// Data collection
	//

	const std::string path("test_data/");
	const std::string bulk_prefix("stationary_droplet_bulk");
	const std::string droplet_prefix("stationary_droplet_droplet");

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

	// Bulk and dissolved densities of the 
	// continuous (bulk) liquid and the dispersed phase
	const double rho_bulk = 1.0, rho_droplet = 1.0;
	const double rho_b_in_d = 0.06, rho_d_in_b = 0.06;
	// Potential for repulsive interactions
	const double G_repulsive = 0.9;
	// Properties of the initial droplet
	// Coordinates of droplet center
	const double xc = 534, yc = 149;
	// x and y half-widths
	const double half_Lx = 31, half_Ly = 61;

	Fluid bulk_fluid("water");
	Fluid droplet_fluid("oil");

	// Repulsive inter-fluid forces
	bulk_fluid.initialize_fluid_repulsion(G_repulsive);
	droplet_fluid.initialize_fluid_repulsion(G_repulsive);

	// Initialize all variables to zero, including densities
	bulk_fluid.zero_density_ini(geom);
	droplet_fluid.zero_density_ini(geom);

	// Initialize the continuous and dispersed phases
	// The dispersed phase - droplet - is initialized as a 
	// rectangular area immersed in the continuous fluid (bulk)	
	lbm.initialize_fluid_rectangle(geom, bulk_fluid, 
		droplet_fluid, rho_bulk, rho_droplet, rho_b_in_d, 
		rho_d_in_b, xc, yc, half_Lx, half_Ly);

	//
	// Verification of the initial state
	//	
	
	compute_and_write_values(geom, bulk_fluid, bulk_prefix, 
						bulk_prefix + "_f", bulk_prefix + "_f_eq", path);	
	compute_and_write_values(geom, droplet_fluid, droplet_prefix, 
						droplet_prefix + "_f", droplet_prefix + "_f_eq", path);
}



