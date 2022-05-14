#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test suite for the LBM class - unidirectional 
 *	driving force
 *
 *****************************************************/

bool single_phase_empty_test();
bool single_phase_x_walls_test();
bool single_phase_y_walls_test();
bool single_phase_array_no_walls_test();
bool single_phase_array_x_walls_test();
bool single_phase_array_y_walls_test();

int main()
{
	test_pass(single_phase_empty_test(), "Single phase, empty domain");
	test_pass(single_phase_x_walls_test(), "Single phase, x walls");
	test_pass(single_phase_y_walls_test(), "Single phase, y walls");
	test_pass(single_phase_array_no_walls_test(), "Single phase, array of objects, no walls");
	test_pass(single_phase_array_x_walls_test(), "Single phase, array of objects, x walls");
	test_pass(single_phase_array_y_walls_test(), "Single phase, array of objects, y walls");
}

/// Empty geometry with unidirectional volume force
/// @details The macroscopic results should be the same as initial
bool single_phase_empty_test()
{
	// Initial density 
	double rho_ini = 2.5;
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> uni_force{0, 1, 0, -1, 0, 1, -1, -1, 1};
	std::for_each(uni_force.begin(), uni_force.end(), [&dPdL](double& el) { el *= dPdL; });
	// Other settings
	int max_iter = 100;
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("uni_force_empty_ini");
	const std::string fname_stream("uni_force_empty_stream");
	// File with geometry
	const std::string gfile(path + "empty_domain.txt");	

	run_and_collect(gfile, rho_ini, max_iter, uni_force, path, 
				fname_ini, fname_stream);

	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for empty domain" << std::endl;
		return false;
	}
#endif

#ifndef FULL_TEST
	if (!compare_with_correct({fname_stream}, path)) {
		std::cerr << "Mismatch with expected for empty domain" << std::endl;
		return false;
	}
#endif

	return true;	
}

/// Domain with walls spanning x direction and with unidirectional volume force
bool single_phase_x_walls_test()
{
	// Initial density 
	double rho_ini = 2.0;
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> uni_force{0, 1, 0, -1, 0, 1, -1, -1, 1};
	std::for_each(uni_force.begin(), uni_force.end(), [&dPdL](double& el) { el *= dPdL; });
	// Other settings
	int max_iter = 1000;
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("uni_force_x_walls_ini");
	const std::string fname_stream("uni_force_x_walls_stream");
	// File with geometry
	const std::string gfile(path + "domain_with_x_walls.txt");

	run_and_collect(gfile, rho_ini, max_iter, uni_force, path, 
				fname_ini, fname_stream);

	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with x walls" << std::endl;
		return false;
	}	
#endif

#ifndef FULL_TEST
	if (!compare_with_correct({fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with x walls" << std::endl;
		return false;
	}	
#endif

	return true;	
}

/// Domain with walls spanning y direction and with unidirectional volume force
bool single_phase_y_walls_test()
{
	// Initial density 
	double rho_ini = 1.0;
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> uni_force{0, 0, 1, 0, -1, 1, 1, -1, -1};
	std::for_each(uni_force.begin(), uni_force.end(), [&dPdL](double& el) { el *= dPdL; });
	// Other settings
	int max_iter = 1000;
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("uni_force_y_walls_ini");
	const std::string fname_stream("uni_force_y_walls_stream");
	const std::string gfile(path + "domain_with_y_walls.txt");
	
	run_and_collect(gfile, rho_ini, max_iter, uni_force, path, 
				fname_ini, fname_stream);

	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with y walls" << std::endl;
		return false;
	}
#endif

#ifndef FULL_TEST
	if (!compare_with_correct({fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with y walls" << std::endl;
		return false;
	}
#endif
	return true;	
}

/// Domain with an array of objects, no walls, and unidirectional volume force
bool single_phase_array_no_walls_test()
{
	// Initial density 
	double rho_ini = 1.5;
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> uni_force{0, 0, 1, 0, -1, 1, 1, -1, -1};
	std::for_each(uni_force.begin(), uni_force.end(), [&dPdL](double& el) { el *= dPdL; });
	// Other settings
	int max_iter = 200;
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("uni_force_array_no_walls_ini");
	const std::string fname_stream("uni_force_array_no_walls_stream");
	// File with geometry
	const std::string gfile(path + "domain_with_array_no_walls.txt");

	run_and_collect(gfile, rho_ini, max_iter, uni_force, path, 
				fname_ini, fname_stream);

	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and no walls" << std::endl;
		return false;
	}
#endif

#ifndef FULL_TEST
	if (!compare_with_correct({fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and no walls" << std::endl;
		return false;
	}
#endif

	return true;	
}

/// Domain with an array of objects, x walls, and unidirectional volume force
bool single_phase_array_x_walls_test()
{
	// Initial density 
	double rho_ini = 1.5;
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> uni_force{0, 1, 0, -1, 0, 1, -1, -1, 1};
	std::for_each(uni_force.begin(), uni_force.end(), [&dPdL](double& el) { el *= dPdL; });
	// Other settings
	int max_iter = 200;
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("uni_force_array_x_walls_ini");
	const std::string fname_stream("uni_force_array_x_walls_stream");
	// File with geometry
	const std::string gfile(path + "domain_with_array_x_walls.txt");

	// Check
	run_and_collect(gfile, rho_ini, max_iter, uni_force, path, 
				fname_ini, fname_stream);
#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and x walls" << std::endl;
		return false;
	}
#endif

#ifndef FULL_TEST
	if (!compare_with_correct({fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and x walls" << std::endl;
		return false;
	}
#endif

	return true;	
}

/// Domain with an array of objects, y walls, and unidirectional volume force
bool single_phase_array_y_walls_test()
{
	// Initial density 
	double rho_ini = 5.25;
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> uni_force{0, 0, 1, 0, -1, 1, 1, -1, -1};
	std::for_each(uni_force.begin(), uni_force.end(), [&dPdL](double& el) { el *= dPdL; });
	// Other settings
	int max_iter = 200;
	// Filename templates
	std::string path("test_data/");
	std::string fname_ini("uni_force_array_y_walls_ini");
	std::string fname_stream("uni_force_array_y_walls_stream");
	// File with geometry
	const std::string gfile(path + "domain_with_array_y_walls.txt");

	run_and_collect(gfile, rho_ini, max_iter, uni_force, path, 
				fname_ini, fname_stream);

	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and y walls" << std::endl;
		return false;
	}	
#endif

#ifndef FULL_TEST
	if (!compare_with_correct({fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and y walls" << std::endl;
		return false;
	}	
#endif

	return true;	
}
