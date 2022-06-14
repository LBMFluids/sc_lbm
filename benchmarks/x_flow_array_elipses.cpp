#include <chrono>
#include "../include/lbm.h"

/***************************************************** 
 *
 * Flow in a 2D channel with an array of ellipses 
 *	in x-direction 
 * 
 * The channel is periodic in x and has walls in y
 *
 *****************************************************/

int main()
{
	//
	// Simulation settings
	//

	// Nominal density 
	const double rho_ini = 1.5;
	// External forcing term
	const double beta = 1.0/6.0;
	const double dPdL = 1e-5*beta;
	std::vector<double> vol_force{0, 1, 0, -1, 0, 1, -1, -1, 1};
	std::for_each(vol_force.begin(), vol_force.end(), [&dPdL](double& el) { el *= dPdL; });

	// Number of steps to simulate
	int max_iter = 30000;

	// Filename templates
	// Directory with all the results
	const std::string out_path("output/");
	// All simulation results 
	const std::string fname_out("array_of_ellipses_y_walls_results");
	// File for saving the geometry
	const std::string gfile(out_path + "array_of_ellipses_y_walls.txt");
	
	//
	// Geometry setup
	//
	
	// Channel with walls spanning in x direction	
	size_t Nx = 300, Ny = 500;
	size_t dh = 1;

	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");

	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);

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
