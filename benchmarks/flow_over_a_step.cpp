#include <chrono>
#include "../include/lbm.h"

/***************************************************** 
 *
 * Laminar flow past a rectangular wall at the bottom
 *	of a channel 
 * 
 * The channel is periodic in x and has walls in y
 * The flow is in x direction
 *
 *****************************************************/

int main()
{
	//
	// Simulation settings
	//

	// Nominal density 
	const double rho_ini = 1.0;
	// External forcing term
	const double beta = 1.0/6.0;
	const double dPdL = 1e-4*beta;
	std::vector<double> vol_force{0, 0, 1, 0, -1, 1, 1, -1, -1};
	std::for_each(vol_force.begin(), vol_force.end(), [&dPdL](double& el) { el *= dPdL; });

	// Number of steps to simulate
	int max_iter = 30000;

	// Filename templates
	// Directory with all the results
	const std::string out_path("output/");
	// All simulation results 
	const std::string fname_out("step_results");
	// File for saving the geometry
	const std::string gfile(out_path + "domain_with_step.txt");
	
	//
	// Geometry setup
	//
	
	// Channel with walls spanning in y direction	
	size_t Nx = 200, Ny = 100;
	size_t dh = 1;
	// Dimensions and position of the wall (obstacle)
	size_t h_wall = 40;
	size_t w_wall = 20;
	size_t x0 = Nx/2-1, y0 = h_wall/2; 

	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	geom.add_rectangle(w_wall, h_wall, x0, y0);
	geom.write(gfile);
	
	//
	// Fluid setup and initialization
	//
	
	Fluid working_fluid;
	working_fluid.simple_ini(geom, rho_ini);
	LBM lbm(geom);

	//
	// Simulation
	//

	// For time measurement
	std::chrono::steady_clock::time_point sim_t0 = std::chrono::steady_clock::now();
	
	for (int iter = 0; iter<max_iter; ++iter) {
		lbm.collide(geom, working_fluid);	
		lbm.add_volume_force(geom, working_fluid, vol_force);		
		lbm.stream(geom, working_fluid);
	}
	
	// Print time needed
	std::chrono::steady_clock::time_point sim_t_end = std::chrono::steady_clock::now();
	std::cout << "Simulation time: " << std::chrono::duration_cast<std::chrono::milliseconds> (sim_t_end - sim_t0).count() << "[ms]" << std::endl;
	std::cout << "Simulation time: " << std::chrono::duration_cast<std::chrono::seconds> (sim_t_end - sim_t0).count() << "[s]" << std::endl;

	// 
	// Post-processing
	//

	// Compute and save macroscopic variables
	working_fluid.save_state(out_path + fname_out + "_density",
			out_path + fname_out + "_ux",
			out_path + fname_out + "_uy", max_iter, geom);	
	// Save the density distribution function
	working_fluid.write_f(out_path + fname_out + "_f_dist");

}
