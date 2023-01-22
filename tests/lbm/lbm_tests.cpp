#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test utilities for the LBM class test suites
 *
 *****************************************************/

// Run simulations with an option for full data collection
void run_and_collect_all(const std::string& gfile, const double rho_ini, 
			const std::vector<double>& vol_force, const std::string& path,
			const std::string& fname_ini, const std::string& fname_col,
			const std::string& fname_force, const std::string& fname_stream)
{
	Geometry geom;
	geom.read(gfile);

	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	test_fluid.compute_f_equilibrium(geom);

	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", fname_ini + "_feq", path);

	// Simulation setup
	LBM lbm(geom);

	// Simulation
	lbm.collide(geom, test_fluid);	

	compute_and_write_values(geom, test_fluid, fname_col, 
					fname_col + "_f", fname_col + "_feq", path);

	lbm.add_volume_force(geom, test_fluid, vol_force);		

	compute_and_write_values(geom, test_fluid, fname_force, 
					fname_force + "_f", fname_force + "_feq", path);

	lbm.stream(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_stream, 
					fname_stream + "_f", fname_stream + "_feq", path);
}

// Run simulations with an option for collecting only initial and final data (as full)
void run_and_collect(const std::string& gfile, const double rho_ini,
			const int max_iter, const std::vector<double>& vol_force, 
			const std::string& path, const std::string& fname_ini, 
			const std::string& fname_stream)
{
	Geometry geom;
	geom.read(gfile);

	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	test_fluid.compute_f_equilibrium(geom);

	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", fname_ini + "_feq", path);

	// Simulation setup
	LBM lbm(geom);

	// Simulation
	for (int iter = 0; iter<max_iter; ++iter) {
		lbm.collide(geom, test_fluid);	
		lbm.add_volume_force(geom, test_fluid, vol_force);		
		lbm.stream(geom, test_fluid);
	}

	compute_and_write_values(geom, test_fluid, fname_stream, 
		fname_stream + "_f", fname_stream + "_feq", path);
}

// One step run and very detailed check for two-phase flows
void run_and_collect(std::map<std::string, double>& parameters,  
			const std::string& gfile, const std::string& path, 
			const std::string& bulk_prefix, const std::string& droplet_prefix,
			const std::vector<double>& vol_force)
{
	// Geometry
	Geometry geom;
	geom.read(path + gfile);

	// Simulation interface
	LBM lbm(geom);

	// Initialize fluids and interactions
	Fluid bulk_fluid("water");
	Fluid droplet_fluid("oil");

	// Initialize all variables to zero, including densities
	bulk_fluid.zero_density_ini(geom);
	droplet_fluid.zero_density_ini(geom);

	// Initialize fluid-solid and fluid-fluid interactions
	bulk_fluid.initialize_interactions(parameters.at("G_solids_bulk"), parameters.at("G_repulsive"));
	droplet_fluid.initialize_interactions(parameters.at("G_solids_droplet"), parameters.at("G_repulsive"));

	// Initialize the continuous and dispersed phases
	// The dispersed phase - droplet - is initialized as a 
	// rectangular area immersed in the continuous fluid (bulk)	
	lbm.initialize_fluid_rectangle(geom, bulk_fluid, 
				droplet_fluid, parameters.at("rho_bulk"), parameters.at("rho_droplet"), 
				parameters.at("rho_b_in_d"), parameters.at("rho_d_in_b"), 
				parameters.at("xc"), parameters.at("yc"), parameters.at("half_Lx"), 
				parameters.at("half_Ly"));

	// Compute interactions with solids (valid for the entire run) 
	lbm.compute_solid_surface_force(geom, bulk_fluid, droplet_fluid);

	// Verification of the initial state
	compute_and_write_values(geom, bulk_fluid, bulk_prefix, 
						bulk_prefix + "_f", path);	
	compute_and_write_values(geom, droplet_fluid, droplet_prefix, 
						droplet_prefix + "_f", path);

	// One simulation step
	int step_i = 1;
	bulk_fluid.compute_density();
	droplet_fluid.compute_density();
	lbm.compute_fluid_repulsive_interactions(geom, bulk_fluid, droplet_fluid);
	lbm.compute_equilibrium_velocities(geom, bulk_fluid, droplet_fluid);
	lbm.collide(bulk_fluid, droplet_fluid);
	lbm.add_volume_force(geom, bulk_fluid, droplet_fluid, vol_force);
	lbm.stream(geom, bulk_fluid, droplet_fluid);

	// Save new values
	compute_and_write_values_multiphase(geom, bulk_fluid, 
				bulk_prefix + "_step_" + std::to_string(step_i),
				bulk_prefix + "_f_step_" + std::to_string(step_i), bulk_prefix + "_f_eq_step_" + std::to_string(step_i), path);	
	compute_and_write_values_multiphase(geom, droplet_fluid, 
				droplet_prefix + "_step_" + std::to_string(step_i),
				droplet_prefix + "_f_step_" + std::to_string(step_i), droplet_prefix + "_f_eq_step_" + std::to_string(step_i), path);
}

// Compute macroscopic properties, save to files along with density distributions
void compute_and_write_values(Geometry& geom, Fluid& fluid_1, const std::string& fname,
						const std::string& fname_f, const std::string& fname_feq, const std::string& path)
{
	// Density distribution functions are already computed and stored
	fluid_1.compute_macroscopic(geom);
	fluid_1.write_density(path + fname + "_density.txt");
	fluid_1.write_ux(path + fname + "_ux.txt", geom);
	fluid_1.write_uy(path + fname + "_uy.txt", geom);
	fluid_1.write_feq(path + fname_feq);
	fluid_1.write_f(path + fname_f);
}

// Compute macroscopic properties, save to files along with density distributions - wo the equilibrium distribution
void compute_and_write_values(Geometry& geom, Fluid& fluid_1, const std::string& fname,
						const std::string& fname_f, const std::string& path)
{
	// Density distribution functions are already computed and stored
	fluid_1.compute_macroscopic(geom);
	fluid_1.write_density(path + fname + "_density.txt");
	fluid_1.write_ux(path + fname + "_ux.txt", geom);
	fluid_1.write_uy(path + fname + "_uy.txt", geom);
	fluid_1.write_f(path + fname_f);
}

// Compute macroscopic properties, save to files - main variables in multiphase flow
void compute_and_write_values_multiphase(Geometry& geom, Fluid& fluid_1, const std::string& fname,
						const std::string& fname_f, const std::string& fname_feq, 
						const std::string& path)
{
	fluid_1.compute_macroscopic(geom);
	fluid_1.write_density(path + fname + "_density.txt");
	fluid_1.write_ux(path + fname + "_ux.txt", geom);
	fluid_1.write_uy(path + fname + "_uy.txt", geom);
	fluid_1.write_feq(path + fname_feq);
	fluid_1.write_f(path + fname_f);
	fluid_1.write_F_repulsive_x(path + fname + "_Fx.txt");
	fluid_1.write_F_repulsive_y(path + fname + "_Fy.txt");
	fluid_1.write_u_eq_x(path + fname + "_ucx.txt");
	fluid_1.write_u_eq_y(path + fname + "_ucy.txt");
}

// Compare C++ generated file and the correct solution (from MATLAB)  
bool compare_with_correct(const std::vector<std::string>&& file_list, const std::string& path)
{
	for (const auto& fname : file_list) {
		if (!check_macroscopic(fname, path, "_density.txt", "densities")) {
			return false;
		}
		if (!check_macroscopic(fname, path, "_ux.txt", "x velocity components")) {
			return false;
		}
		if (!check_macroscopic(fname, path, "_uy.txt", "y velocity components")) {
			return false;
		}
		if (!check_distributions(fname, path, "_f_", "density distributions")) {
			return false;
		}
		if (!check_distributions(fname, path, "_feq_", "equilibrium density distributions")) {
			return false;
		}
	}
	return true;
}

// Compare C++ generated file and the correct solution (from MATLAB) with special name for density distribution and no equilibrium density distribution 
bool compare_with_correct(const std::vector<std::string>&& file_list, const std::string& path, const std::string& f_extension)
{
	for (const auto& fname : file_list) {
		if (!check_macroscopic(fname, path, "_density.txt", "densities")) {
			return false;
		}
		if (!check_macroscopic(fname, path, "_ux.txt", "x velocity components")) {
			return false;
		}
		if (!check_macroscopic(fname, path, "_uy.txt", "y velocity components")) {
			return false;
		}
		if (!check_distributions(fname, path, f_extension, "density distributions")) {
			return false;
		}
	}
	return true;
}

// Compare C++ generated file and the correct solution (from MATLAB) with special name for density distribution and no equilibrium density distribution 
bool compare_with_correct(const std::vector<std::string>&& file_list, const std::string& extra_prefix, const std::string& path, const std::string& f_extension, const std::string& f_eq_extension)
{
	for (const auto& fname : file_list) {
		if (!check_macroscopic(fname + extra_prefix, path, "_density.txt", "densities")) {
			return false;
		}
		if (!check_macroscopic(fname + extra_prefix, path, "_ux.txt", "x velocity components")) {
			return false;
		}
		if (!check_macroscopic(fname + extra_prefix, path, "_uy.txt", "y velocity components")) {
			return false;
		}
		if (!check_macroscopic(fname + extra_prefix, path, "_ucx.txt", "x equilibrium velocity components")) {
			return false;
		}
		if (!check_macroscopic(fname + extra_prefix, path, "_ucy.txt", "y equilibrium velocity components")) {
			return false;
		}
		if (!check_macroscopic(fname + extra_prefix, path, "_Fx.txt", "repulsive force x component")) {
			return false;
		}
		if (!check_macroscopic(fname + extra_prefix, path, "_Fy.txt", "repulsive force y component")) {
			return false;
		}
		if (!check_distributions(fname, path, f_extension, "density distributions")) {
			return false;
		}
		if (!check_distributions(fname, path, f_eq_extension, "equilibrium density distributions")) {
			return false;
		}
	}
	return true;
}

// Perform the actual comparisong between files
bool check_macroscopic(const std::string& fname, const std::string& path, 
				const std::string& file_extension, const std::string& prop_name)
{
	double tol = 1e-5;
	std::string res_name, exp_name;
	std::vector<std::vector<double>> res, exp;

	// Current solution
	res_name = path + fname + file_extension;
	LbmIO res_lbm(res_name, " ", true, {0, 0, 0});	
 	res = res_lbm.read_vector<double>(); 

	// Expected solution
	exp_name = path +"matlab_" + fname + file_extension;
	LbmIO exp_lbm(exp_name, " ", true, {0, 0, 0});	
 	exp = exp_lbm.read_vector<double>();

	if (!is_equal_floats<double>(res, exp, tol)) {
		std::cerr << "Macroscopic " << prop_name << " don't match for " << fname << std::endl;
		return false;
	}

	return true;
}

// Perform the actual comparisong between files - version for distributions
bool check_distributions(const std::string& fname, const std::string& path, 
				const std::string& file_extension, const std::string& prop_name)
{
	double tol = 1e-5;
	int Nc = 9;
	std::string res_name, exp_name;
	std::vector<std::vector<double>> res, exp;

	for (int i = 0; i<Nc; ++i) {
		// Current solution
		res_name = path + fname + file_extension + std::to_string(i) + ".txt";
		LbmIO res_lbm(res_name, " ", true, {0, 0, 0});	
	 	res = res_lbm.read_vector<double>(); 
	
		// Expected solution
		exp_name = path + "matlab_" + fname + file_extension + std::to_string(i) + ".txt";
		LbmIO exp_lbm(exp_name, " ", true, {0, 0, 0});	
	 	exp = exp_lbm.read_vector<double>();
	
		if (!is_equal_floats<double>(res, exp, tol)) {
			std::cerr << prop_name << " don't match for " << fname 
				<< " direction " << i << std::endl;
			return false;
		}
	}
	return true;
}
