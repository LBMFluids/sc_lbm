#include <chrono>
#include "../include/lbm.h"

/***************************************************** 
 *
 * Laminar flow past a cylinder (in x direction)
 * 
 *****************************************************/

int main(int argc, char *argv[])
{

	//
	// Collect the input
	//

	if (argc < 3) {
		std::cout << "Usage: fpc <pressure drop> <output files name template>";
		throw std::invalid_argument("Not enough input arguments - see examples");
	}

	// Corrected pressure drop
	const double beta = 1.0/6.0;
	const double dPdL = std::atof(argv[1])*beta;

	// Template
	// All simulation results 
	const std::string fname_out(argv[2]);

	//
	// Simulation settings
	//

	// Nominal density 
	const double rho_ini = 2.0;
	// External forcing term
	std::vector<double> vol_force{0, 1, 0, -1, 0, 1, -1, -1, 1};
	std::for_each(vol_force.begin(), vol_force.end(), [&dPdL](double& el) { el *= dPdL; });

	// Number of steps to simulate
	int max_iter = 30000;
	// When to print an update status
	int disp_every = 10000;

	// Filename templates
	// Directory with all the results
	const std::string out_path("output/");
	// File for saving the geometry
	const std::string gfile(out_path + "domain_with_cylinder.txt");
	
	//
	// Geometry setup
	//
	
	// Channel with walls spanning in y direction	
	size_t Nx = 600, Ny = 300;
	size_t dh = 1;
	// Dimensions and position of the wall (obstacle)
	size_t Ds = 31;
	size_t x0 = 100, y0 = Ny/2-1; 

	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	geom.add_circle(Ds, x0, y0);
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

		if (iter+1 == disp_every) {
			std::cout << "Simulation step  " << iter+1 << std::endl;
		}
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
