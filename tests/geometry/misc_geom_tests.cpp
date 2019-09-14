#include "geometry_tests.h"

/*************************************************************** 
 * Various tests for geometry class 
***************************************************************/

// Tests
void read_and_write_test();
void x_wall_test_suite();
void y_wall_test_suite();
void wall_exception_suite();

// Supporting functions
void make_walls(const size_t, const size_t, const size_t, const std::string);
void make_and_save_walls(const size_t, const size_t, const std::string, 
				const size_t, const std::string);

// Necessary files
// ./test_data/rw_test_geom.txt

// Files to delete (not to bias testing)
// ./test_data/rw_test_geom_out.txt
// The whole ./test_data/x_wall_dH_#.txt sequence
// 	and ./test_data/y_wall_dH_#.txt sequence

int main()
{
	read_and_write_test();
	x_wall_test_suite();
	y_wall_test_suite();
	wall_exception_suite();	
}

/// \brief Reads a geometry file, then writes it to a separate file
void read_and_write_test()
{
	std::string in_file("./test_data/rw_test_geom.txt");
	std::string out_file("./test_data/rw_test_geom_out.txt");

	Geometry geom;
	geom.read(in_file);
	geom.write(out_file);
}

/// \brief Create geometries with x walls and write them to files
void x_wall_test_suite()
{
	// Domain dimensions
	size_t Nx = 15, Ny = 20;
	// Representative wall thicknesses to check
	std::vector<size_t> dHs = {0, 1, 5, Ny};
	// Ceate and save every setup
	for (const auto dh : dHs){
		std::string fname("./test_data/x_wall_dH_");
		fname += (std::to_string(dh) + ".txt");  
		make_and_save_walls(Nx, Ny, fname, dh, "x");
	}
}

/// \brief Create geometries with y walls and write them to files
void y_wall_test_suite()
{
	// Domain dimensions
	size_t Nx = 65, Ny = 70;
	// Representative wall thicknesses to check
	std::vector<size_t> dHs = {0, 1, 5, Nx};
	// Ceate and save every setup
	for (const auto dh : dHs){
		std::string fname("./test_data/y_wall_dH_");
		fname += (std::to_string(dh) + ".txt");  
		make_and_save_walls(Nx, Ny, fname, dh, "y");
	}
}

/// \brief Tests correct exception behavior in wall adding code
void wall_exception_suite()
{
	// Exception tests
	bool verbose = true;
	const std::runtime_error rtime("Run time error");
	const std::invalid_argument argerr("Wrong argument");
	// Geometry properties
	size_t Nx = 10, Ny = 30;
	tst_pass(exception_test(verbose, &rtime, make_walls, Nx, Ny, Ny + 10, std::string("x")), "x wall to thick");
	tst_pass(exception_test(verbose, &rtime, make_walls, Nx, Ny, Nx + 10, std::string("y")), "y wall to thick");
	tst_pass(exception_test(verbose, &argerr, make_walls, Nx, Ny, Nx - 5, std::string("xyz")), "Wrong direction option");
}

/**
 * \brief Create a geometry object with walls
 * @param Nx - number of nodes in x direction
 * @param Ny - number of nodes in y direction
 * @param dh - wall thickness (number of nodes)
 * @param dir - wall direction 
 */
void make_walls(const size_t Nx, const size_t Ny, const size_t dh, const std::string dir)
{
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, dir);
}

/**
 * \brief Create and save a Geometry object with a pair of walls
 * @param Nx - number of nodes in x direction
 * @param Ny - number of nodes in y direction
 * @param fname - file name to save the geometry to
 * @param dh - wall thickness (number of nodes)
 * @param dir - wall direction 
 */
void make_and_save_walls(const size_t Nx, const size_t Ny, const std::string fname, 
				const size_t dh, const std::string dir)
{
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, dir);
	geom.write(fname);
}
