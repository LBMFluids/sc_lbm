#include "../../include/lbm.h"
#include "../common/test_utils.h"
#include "lbm_tests.h"

/***************************************************** 
 *
 * Test suite for the LBM class - multidirectional 
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

/// Empty geometry with multidirectional volume force
/// @details The macroscopic results should be the same as initial
bool single_phase_empty_test()
{
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> multi_force{0, 1, 1, -1, 0, 1, -1, -1, 1};
	std::for_each(multi_force.begin(), multi_force.end(), [&dPdL](double& el) { el *= dPdL; });

	// Other settings
	int max_iter = 100;

	// Filename templates
	std::string path("test_data/");
	std::string fname_ini("multi_force_empty_ini");
	std::string fname_stream("multi_force_empty_stream");

	// No solid nodes
	size_t Nx = 5, Ny = 10;
	Geometry geom(Nx, Ny);
	geom.write(path + "empty_domain.txt");	

	// Create and initialize the fluid 
	double rho_ini = 2.5;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	test_fluid.compute_f_equilibrium(geom);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", fname_ini + "_feq", path);

	// Simulation setup
	// Periodic boundaries in x and y 
	bool pb_x = true, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	for (int iter = 0; iter<max_iter; ++iter) {
		lbm.collide(geom, test_fluid);	
		lbm.add_volume_force(geom, test_fluid, multi_force);		
		lbm.stream(geom, test_fluid);
	}
	compute_and_write_values(geom, test_fluid, fname_stream, 
		fname_stream + "_f", fname_stream + "_feq", path);

	// Check
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for empty domain" << std::endl;
		return false;
	}

	return true;	
}

/// Domain with walls spanning x direction and with multidirectional volume force
bool single_phase_x_walls_test()
{
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> multi_force{0, 1, 1, -1, 0, 1, -1, 0, 1};
	std::for_each(multi_force.begin(), multi_force.end(), [&dPdL](double& el) { el *= dPdL; });
	
	// Other settings
	int max_iter = 1000;
	
	// Filename templates
	std::string path("test_data/");
	std::string fname_ini("multi_force_x_walls_ini");
	std::string fname_stream("multi_force_x_walls_stream");

	// A wall spanning the x direction (and growing in y) 
	size_t Nx = 20, Ny = 10;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	geom.write(path + "domain_with_x_walls.txt");
	
	// Create and initialize the fluid
	double rho_ini = 2.0;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	test_fluid.compute_f_equilibrium(geom);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", fname_ini + "_feq", path);

	// Simulation setup
	// Periodic boundaries in x, wall boundaries in y
	bool pb_x = true, pb_y = false;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	for (int iter = 0; iter<max_iter; ++iter) {
		lbm.collide(geom, test_fluid);	
		lbm.add_volume_force(geom, test_fluid, multi_force);		
		lbm.stream(geom, test_fluid);
	}
	compute_and_write_values(geom, test_fluid, fname_stream, 
		fname_stream + "_f", fname_stream + "_feq", path);

	// Check
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with x walls" << std::endl;
		return false;
	}	

	return true;	
}

/// Domain with walls spanning y direction and with multidirectional volume force
bool single_phase_y_walls_test()
{
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> multi_force{0, 1, 1, -1, -1, 1, 1, -1, -1};
	std::for_each(multi_force.begin(), multi_force.end(), [&dPdL](double& el) { el *= dPdL; });
	
	// Other settings
	int max_iter = 1000;
	
	// Filename templates
	std::string path("test_data/");
	std::string fname_ini("multi_force_y_walls_ini");
	std::string fname_stream("multi_force_y_walls_stream");

	// A wall spanning the y direction (and growing in x) 
	size_t Nx = 20, Ny = 10;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");
	geom.write(path + "domain_with_y_walls.txt");
	
	// Create and initialize the fluid
	double rho_ini = 1.0;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	test_fluid.compute_f_equilibrium(geom);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", fname_ini + "_feq", path);

	// Simulation setup
	// Periodic boundaries in y, wall boundaries in x
	bool pb_x = false, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	for (int iter = 0; iter<max_iter; ++iter) {
		lbm.collide(geom, test_fluid);	
		lbm.add_volume_force(geom, test_fluid, multi_force);		
		lbm.stream(geom, test_fluid);
	}
	compute_and_write_values(geom, test_fluid, fname_stream, 
		fname_stream + "_f", fname_stream + "_feq", path);

	// Check
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with y walls" << std::endl;
		return false;
	}	
	return true;	
}

/// Domain with an array of objects, no walls, and multidirectional volume force
bool single_phase_array_no_walls_test()
{
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> multi_force{0, -1, 1, 1, -1, 1, 1, -1, -1};
	std::for_each(multi_force.begin(), multi_force.end(), [&dPdL](double& el) { el *= dPdL; });
	
	// Other settings
	int max_iter = 200;
	
	// Filename templates
	std::string path("test_data/");
	std::string fname_ini("multi_force_array_no_walls_ini");
	std::string fname_stream("multi_force_array_no_walls_stream");

	// A staggered array
	size_t Nx = 200, Ny = 100;
	Geometry geom(Nx, Ny);
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);
	geom.write(path + "domain_with_array_no_walls.txt");

	// Create and initialize the fluid
	double rho_ini = 1.5;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	test_fluid.compute_f_equilibrium(geom);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", 
					fname_ini + "_feq", path);

	// Simulation setup
	// Periodic boundaries in x and y 
	bool pb_x = true, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	for (int iter = 0; iter<max_iter; ++iter) {
		lbm.collide(geom, test_fluid);	
		lbm.add_volume_force(geom, test_fluid, multi_force);		
		lbm.stream(geom, test_fluid);
	}
	compute_and_write_values(geom, test_fluid, fname_stream, 
		fname_stream + "_f", fname_stream + "_feq", path);

	// Check
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and no walls" << std::endl;
		return false;
	}

	return true;	
}

/// Domain with an array of objects, x walls, and multidirectional volume force
bool single_phase_array_x_walls_test()
{
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> multi_force{0, 1, -1, -1, 0, 1, -1, -1, 1};
	std::for_each(multi_force.begin(), multi_force.end(), [&dPdL](double& el) { el *= dPdL; });
	
	// Other settings
	int max_iter = 200;

	// Filename templates
	std::string path("test_data/");
	std::string fname_ini("multi_force_array_x_walls_ini");
	std::string fname_stream("multi_force_array_x_walls_stream");

	// A staggered array
	size_t Nx = 200, Ny = 100;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);
	geom.write(path + "domain_with_array_x_walls.txt");

	// Create and initialize the fluid
	double rho_ini = 1.5;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	test_fluid.compute_f_equilibrium(geom);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", fname_ini + "_feq", path);

	// Simulation setup
	// Periodic boundaries in x and wall boundaries in y 
	bool pb_x = true, pb_y = false;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	for (int iter = 0; iter<max_iter; ++iter) {
		lbm.collide(geom, test_fluid);	
		lbm.add_volume_force(geom, test_fluid, multi_force);		
		lbm.stream(geom, test_fluid);
	}
	compute_and_write_values(geom, test_fluid, fname_stream, 
		fname_stream + "_f", fname_stream + "_feq", path);

	// Check
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and x walls" << std::endl;
		return false;
	}

	return true;	
}

/// Domain with an array of objects, y walls, and multidirectional volume force
bool single_phase_array_y_walls_test()
{
	// External forcing term
	const double dPdL = -1e-5/6;
	std::vector<double> multi_force{0, 1, 1, 0, 0, 1, 1, -1, -1};
	std::for_each(multi_force.begin(), multi_force.end(), [&dPdL](double& el) { el *= dPdL; });
	
	// Other settings
	int max_iter = 200;

	// Filename templates
	std::string path("test_data/");
	std::string fname_ini("multi_force_array_y_walls_ini");
	std::string fname_stream("multi_force_array_y_walls_stream");

	// A staggered array
	size_t Nx = 200, Ny = 100;
	size_t dh = 2;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);
	geom.write(path + "domain_with_array_y_walls.txt");

	// Create and initialize the fluid
	double rho_ini = 5.25;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	test_fluid.compute_f_equilibrium(geom);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", fname_ini + "_feq", path);

	// Simulation setup
	// Periodic boundaries in x and wall boundaries in y 
	bool pb_x = false, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	for (int iter = 0; iter<max_iter; ++iter) {
		lbm.collide(geom, test_fluid);	
		lbm.add_volume_force(geom, test_fluid, multi_force);		
		lbm.stream(geom, test_fluid);
	}
	compute_and_write_values(geom, test_fluid, fname_stream, 
		fname_stream + "_f", fname_stream + "_feq", path);

	// Check
	if (!compare_with_correct({fname_ini, fname_stream}, path)) {
		std::cerr << "Mismatch with expected for domain with an array and y walls" << std::endl;
		return false;
	}	

	return true;	
}
