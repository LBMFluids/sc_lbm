#include "geometry_tests.h"

/*************************************************************** 
 * Suite for testing object creation for Geometry class 
***************************************************************/

// Tests
bool rectangle_test();

int main()
{
	tst_pass(rectangle_test(), "Rectangle");
}

///\brief Checks if the built rectangle is correct
bool rectangle_test()
{
	size_t Nx = 500, Ny = 100;
	Geometry geom(Nx, Ny);
	geom.add_rectangle(11, 5, 401, 39);
	return true;
}


