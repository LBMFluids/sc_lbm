#include "geometry_tests.h"

/*************************************************************** 
 * Suite for testing Geometry class contructors, destructor,
 * and assignment operators
***************************************************************/

// Tests
bool constructor_test();
bool copy_ctor_test();
bool mv_ctor_test();
bool copy_op_test();
bool mv_op_test();
void copy_op_dim_err();
void mv_op_dim_err();

int main()
{
	// Correctness tests
	test_pass(constructor_test(), "Constructor");
	test_pass(copy_ctor_test(), "Copy constructor");
	test_pass(mv_ctor_test(), "Move constructor");
	test_pass(copy_op_test(), "Copy assignment");
	test_pass(mv_op_test(), "Move assignment");
	// Exception tests
	const std::runtime_error rtime("Run time error");
	test_pass(exception_test(true, &rtime, copy_op_dim_err), "Copy assignment dimension mismatch");
	test_pass(exception_test(true, &rtime, mv_op_dim_err), "Move assignment dimension mismatch");
}

/// \brief Checks if constructor is working correctly 
bool constructor_test()
{
	size_t Nx = 500, Ny = 100;
	Geometry geom(Nx, Ny);
	return check_geom_obj(geom, Nx, Ny);
}

/// \brief Checks if copy constructor is working correctly 
bool copy_ctor_test()
{
	size_t Nx = 500, Ny = 100;
	Geometry geom(Nx, Ny);
	Geometry geom_copy(geom);
	return check_geom_obj(geom_copy, Nx, Ny);
}

/// \brief Checks if move constructor is working correctly 
bool mv_ctor_test()
{
	size_t Nx = 500, Ny = 100;
	Geometry geom(std::move(Geometry(Nx, Ny)));
	return check_geom_obj(geom, Nx, Ny);
}

/// \brief Checks if copy assignment is working correctly
/// \details This will be expanded once objects are added
bool copy_op_test()
{
	size_t Nx = 500, Ny = 100;
	Geometry geom(Nx, Ny);
	Geometry geom_copy(Nx, Ny);
	geom_copy = geom;
	return check_geom_obj(geom_copy, Nx, Ny);
}

/// \brief Checks if move constructor is working correctly 
bool mv_op_test()
{
	size_t Nx = 500, Ny = 100;
	Geometry geom(Nx, Ny);
	geom = std::move(Geometry(Nx, Ny));
	return check_geom_obj(geom, Nx, Ny);
}

/// \brief Tests if dimension mismatch detected when copy assigning
void copy_op_dim_err()
{	
	size_t Nx = 500, Ny = 100;
	Geometry geom(Nx, Ny);
	Geometry geom_copy(Nx*2, Ny);
	geom_copy = geom;
}

/// \brief Tests if dimension mismatch detected when copy assigning
void mv_op_dim_err()
{	
	size_t Nx = 500, Ny = 100;
	Geometry geom(Nx, Ny);
	geom = std::move(Geometry(Nx, Ny*2));
}


