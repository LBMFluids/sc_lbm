#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test utilities for the LBM class test suites
 *
 *****************************************************/

// Compute macroscopic properties, save to files along wiht density distributions
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
		exp_name = path +"matlab_" + fname + file_extension + std::to_string(i) + ".txt";
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
