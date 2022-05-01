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
	// External forcing term
	const std::vector<double> no_force(9, 0.0);

	// Filename templates
	std::string fname_ini("test_data/no_force_empty_ini");
	std::string fname_col("test_data/no_force_empty_collide");
	std::string fname_force("test_data/no_force_empty_add_force");
	std::string fname_stream("test_data/no_force_empty_stream");

	// No solid nodes
	size_t Nx = 5, Ny = 10;
	Geometry geom(Nx, Ny);
	geom.write("test_data/empty_domain.txt");	

	// Create and initialize the fluid 
	double rho_ini = 2.5;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", 
					fname_ini + "_feq");

	// Simulation setup
	// Periodic boundaries in x and y 
	bool pb_x = true, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, test_fluid);	
	compute_and_write_values(geom, test_fluid, fname_col, 
					fname_col + "_f", fname_col + "_feq");

	lbm.add_volume_force(geom, test_fluid, no_force);		
	compute_and_write_values(geom, test_fluid, fname_force, 
					fname_force + "_f", fname_force + "_feq");

	lbm.stream(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_stream, 
					fname_stream + "_f", fname_stream + "_feq");

	// Check
/*	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
*/
	return true;	
}

/// Domain with walls spanning x direction and with no volume force
bool single_phase_x_walls_test()
{
	// External forcing term
	const std::vector<double> no_force(9, 0.0);

	// Filename templates
	std::string fname_ini("test_data/no_force_x_walls_ini");
	std::string fname_col("test_data/no_force_x_walls_collide");
	std::string fname_force("test_data/no_force_x_walls_add_force");
	std::string fname_stream("test_data/no_force_x_walls_stream");

	// A wall spanning the x direction (and growing in y) 
	size_t Nx = 20, Ny = 10;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	geom.write("test_data/domain_with_x_walls.txt");
	
	// Create and initialize the fluid
	double rho_ini = 2.0;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", 
					fname_ini + "_feq");

	// Simulation setup
	// Periodic boundaries in x, wall boundaries in y
	bool pb_x = true, pb_y = false;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_col, 
					fname_col + "_f", fname_col + "_feq");
	
	lbm.add_volume_force(geom, test_fluid, no_force);		
	compute_and_write_values(geom, test_fluid, fname_force, 
					fname_force + "_f", fname_force + "_feq");
	
	lbm.stream(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_stream, 
					fname_stream + "_f", fname_stream + "_feq");

	// Check
/*	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
*/
	return true;	
}

/// Domain with walls spanning y direction and with no volume force
bool single_phase_y_walls_test()
{
	// External forcing term
	const std::vector<double> no_force(9, 0.0);

	// Filename templates
	std::string fname_ini("test_data/no_force_y_walls_ini");
	std::string fname_col("test_data/no_force_y_walls_collide");
	std::string fname_force("test_data/no_force_y_walls_add_force");
	std::string fname_stream("test_data/no_force_y_walls_stream");

	// A wall spanning the y direction (and growing in x) 
	size_t Nx = 20, Ny = 10;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");
	geom.write("test_data/domain_with_y_walls.txt");
	
	// Create and initialize the fluid
	double rho_ini = 1.0;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", 
					fname_ini + "_feq");

	// Simulation setup
	// Periodic boundaries in y, wall boundaries in x
	bool pb_x = false, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_col, 
					fname_col + "_f", fname_col + "_feq");
	
	lbm.add_volume_force(geom, test_fluid, no_force);		
	compute_and_write_values(geom, test_fluid, fname_force, 
					fname_force + "_f", fname_force + "_feq");
	
	lbm.stream(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_stream, 
					fname_stream + "_f", fname_stream + "_feq");

	// Check
/*	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
*/
	return true;	
}

/// Domain with an array of objects, no walls, and no volume force
bool single_phase_array_no_walls_test()
{
	// External forcing term
	const std::vector<double> no_force(9, 0.0);

	// Filename templates
	std::string fname_ini("test_data/no_force_array_no_walls_ini");
	std::string fname_col("test_data/no_force_array_no_walls_collide");
	std::string fname_force("test_data/no_force_array_no_walls_add_force");
	std::string fname_stream("test_data/no_force_array_no_walls_stream");

	// A staggered array
	size_t Nx = 200, Ny = 100;
	Geometry geom(Nx, Ny);
	geom.write("test_data/domain_with_array_no_walls.txt");
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);

	// Create and initialize the fluid
	double rho_ini = 1.5;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", 
					fname_ini + "_feq");

	// Simulation setup
	// Periodic boundaries in x and y 
	bool pb_x = true, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_col, 
					fname_col + "_f", fname_col + "_feq");
	
	lbm.add_volume_force(geom, test_fluid, no_force);		
	compute_and_write_values(geom, test_fluid, fname_force, 
					fname_force + "_f", fname_force + "_feq");
	
	lbm.stream(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_stream, 
					fname_stream + "_f", fname_stream + "_feq");

	// Check
/*	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
*/
	return true;	
}

/// Domain with an array of objects, x walls, and no volume force
bool single_phase_array_x_walls_test()
{
	// External forcing term
	const std::vector<double> no_force(9, 0.0);

	// Filename templates
	std::string fname_ini("test_data/no_force_array_x_walls_ini");
	std::string fname_col("test_data/no_force_array_x_walls_collide");
	std::string fname_force("test_data/no_force_array_x_walls_add_force");
	std::string fname_stream("test_data/no_force_array_x_walls_stream");

	// A staggered array
	size_t Nx = 200, Ny = 100;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	geom.write("test_data/domain_with_array_x_walls.txt");
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);

	// Create and initialize the fluid
	double rho_ini = 1.5;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", 
					fname_ini + "_feq");

	// Simulation setup
	// Periodic boundaries in x and wall boundaries in y 
	bool pb_x = true, pb_y = false;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_col, 
					fname_col + "_f", fname_col + "_feq");
	
	lbm.add_volume_force(geom, test_fluid, no_force);		
	compute_and_write_values(geom, test_fluid, fname_force, 
					fname_force + "_f", fname_force + "_feq");
	
	lbm.stream(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_stream, 
					fname_stream + "_f", fname_stream + "_feq");

	// Check
/*	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
*/
	return true;	
}

/// Domain with an array of objects, y walls, and no volume force
bool single_phase_array_y_walls_test()
{
	// External forcing term
	const std::vector<double> no_force(9, 0.0);

	// Filename templates
	std::string fname_ini("test_data/no_force_array_y_walls_ini");
	std::string fname_col("test_data/no_force_array_y_walls_collide");
	std::string fname_force("test_data/no_force_array_y_walls_add_force");
	std::string fname_stream("test_data/no_force_array_y_walls_stream");

	// A staggered array
	size_t Nx = 200, Ny = 100;
	size_t dh = 2;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");
	geom.write("test_data/domain_with_array_x_walls.txt");
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);

	// Create and initialize the fluid
	double rho_ini = 5.25;
	Fluid test_fluid;
	test_fluid.simple_ini(geom, rho_ini);
	compute_and_write_values(geom, test_fluid, fname_ini, 
					fname_ini + "_f", 
					fname_ini + "_feq");

	// Simulation setup
	// Periodic boundaries in x and wall boundaries in y 
	bool pb_x = false, pb_y = true;
	LBM lbm(geom, pb_x, pb_y);

	// Simulation
	lbm.collide(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_col, 
					fname_col + "_f", fname_col + "_feq");
	
	lbm.add_volume_force(geom, test_fluid, no_force);		
	compute_and_write_values(geom, test_fluid, fname_force, 
					fname_force + "_f", fname_force + "_feq");
	
	lbm.stream(geom, test_fluid);
	compute_and_write_values(geom, test_fluid, fname_stream, 
					fname_stream + "_f", fname_stream + "_feq");

	// Check
/*	double tol = 1e-5;
	if (!is_equal_floats<double>({rho_0}, {rho_F}, tol)) {
		std::cerr << "Initial and final densities don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({ux_0}, {ux_F}, tol)) {
		std::cerr << "Initial and final x velocity components don't match" << std::endl;
		return false;
	}
 	if (!is_equal_floats<double>({uy_0}, {uy_F}, tol)) {
		std::cerr << "Initial and final y velocity components don't match" << std::endl;
		return false;
	}
*/
	return true;	
}
