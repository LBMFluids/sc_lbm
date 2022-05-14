#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test suite for the LBM class - no driving force
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

/// Empty geometry with no volume force
/// @details The macroscopic results should be the same as initial
bool single_phase_empty_test()
{
	// Initial density 
	double rho_ini = 2.5;
	// External forcing term
	const std::vector<double> no_force(9, 0.0);
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("no_force_empty_ini");
	const std::string fname_col("no_force_empty_collide");
	const std::string fname_force("no_force_empty_add_force");
	const std::string fname_stream("no_force_empty_stream");
	// File with the geometry
	const std::string gfile(path + "empty_domain.txt");

	run_and_collect_all(gfile, rho_ini, no_force, path, fname_ini, 
							fname_col, fname_force, fname_stream);

	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_col, fname_force, fname_stream}, path)) {
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

/// Domain with walls spanning x direction and with no volume force
bool single_phase_x_walls_test()
{
	// Initial density 
	double rho_ini = 2.0;
	// External forcing term
	const std::vector<double> no_force(9, 0.0);
	// Filename template
	const std::string path("test_data/");
	const std::string fname_ini("no_force_x_walls_ini");
	const std::string fname_col("no_force_x_walls_collide");
	const std::string fname_force("no_force_x_walls_add_force");
	const std::string fname_stream("no_force_x_walls_stream");
	// File with the geometry
	const std::string gfile(path + "domain_with_x_walls.txt");

	run_and_collect_all(gfile, rho_ini, no_force, path, fname_ini, 
							fname_col, fname_force, fname_stream);
	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_col, fname_force, fname_stream}, path)) {
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

/// Domain with walls spanning y direction and with no volume force
bool single_phase_y_walls_test()
{
	// Initial density 
	double rho_ini = 1.0;
	// External forcing term
	const std::vector<double> no_force(9, 0.0);
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("no_force_y_walls_ini");
	const std::string fname_col("no_force_y_walls_collide");
	const std::string fname_force("no_force_y_walls_add_force");
	const std::string fname_stream("no_force_y_walls_stream");
	// File with the geometry
	const std::string gfile(path + "domain_with_y_walls.txt");

	run_and_collect_all(gfile, rho_ini, no_force, path, fname_ini, 
							fname_col, fname_force, fname_stream);

	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_col, fname_force, fname_stream}, path)) {
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

/// Domain with an array of objects, no walls, and no volume force
bool single_phase_array_no_walls_test()
{
	// Initial density 
	double rho_ini = 1.5;
	// External forcing term
	const std::vector<double> no_force(9, 0.0);

	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("no_force_array_no_walls_ini");
	const std::string fname_col("no_force_array_no_walls_collide");
	const std::string fname_force("no_force_array_no_walls_add_force");
	const std::string fname_stream("no_force_array_no_walls_stream");
	// File with the geometry
	const std::string gfile(path + "domain_with_array_no_walls.txt");

	run_and_collect_all(gfile, rho_ini, no_force, path, fname_ini, 
							fname_col, fname_force, fname_stream);
	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_col, fname_force, fname_stream}, path)) {
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

/// Domain with an array of objects, x walls, and no volume force
bool single_phase_array_x_walls_test()
{
	// Initial density 
	double rho_ini = 1.5;
	// External forcing term
	const std::vector<double> no_force(9, 0.0);
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("no_force_array_x_walls_ini");
	const std::string fname_col("no_force_array_x_walls_collide");
	const std::string fname_force("no_force_array_x_walls_add_force");
	const std::string fname_stream("no_force_array_x_walls_stream");
	// File with the geometry
	const std::string gfile(path + "domain_with_array_x_walls.txt");

	run_and_collect_all(gfile, rho_ini, no_force, path, fname_ini, 
							fname_col, fname_force, fname_stream);
	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_col, fname_force, fname_stream}, path)) {
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

/// Domain with an array of objects, y walls, and no volume force
bool single_phase_array_y_walls_test()
{
	// Initial density 
	const double rho_ini = 5.25;
	// External forcing term
	const std::vector<double> no_force(9, 0.0);
	// Filename templates
	const std::string path("test_data/");
	const std::string fname_ini("no_force_array_y_walls_ini");
	const std::string fname_col("no_force_array_y_walls_collide");
	const std::string fname_force("no_force_array_y_walls_add_force");
	const std::string fname_stream("no_force_array_y_walls_stream");
	// File with the geometry
	const std::string gfile(path + "domain_with_array_y_walls.txt");

	run_and_collect_all(gfile, rho_ini, no_force, path, fname_ini, 
							fname_col, fname_force, fname_stream);

	// Check

#ifdef FULL_TEST
	if (!compare_with_correct({fname_ini, fname_col, fname_force, fname_stream}, path)) {
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
