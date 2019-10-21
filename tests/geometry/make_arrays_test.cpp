#include "geometry_tests.h"

/*************************************************************** 
 * Suite for testing creation of arrays of objects 
***************************************************************/

// In python verify the array as if by eying it, as in the notes

// Tests
void regular_array_test_suite();

// Supporting functions
void make_regular_array(std::string);

int main()
{
	regular_array_test_suite();
}

void regular_array_test_suite()
{
	Geometry geom(100, 50);
//	geom.add_array({3,5,5,3}, {{20,80},{0,49}}, 5, 10, "rectangle");
	geom.add_array({5,5,3}, {{10,90},{0,39}}, {5, 3}, "circle");
	geom.write("array_test.txt");	
}
