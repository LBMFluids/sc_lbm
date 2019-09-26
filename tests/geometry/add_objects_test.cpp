#include "geometry_tests.h"
#include <functional>

/*************************************************************** 
 * Suite for testing object creation for Geometry class 
***************************************************************/

// Tests
void rectangle_test_suite();
void rectangle_exception_test_suite();

// Supporting functions
void make_and_save_rectangle(const size_t, const size_t, 
							 const size_t, const size_t,
							 const size_t, const size_t,
							 const std::string);

// Files for removal (to prevent biased tests)
// ./test_data/rectangle_test_#.txt

int main()
{
	rectangle_test_suite();
 	rectangle_exception_test_suite();
}

///\brief Test various rectangle objects for correctness
void rectangle_test_suite()
{
	// Geometry width and height
	size_t Nx = 50, Ny = 10;
	// Properties of test rectangles (Rx, Ry, xc, yc)
	std::vector<std::string> prop_names = {"Rx", "Ry", "xc", "yc"};
	std::vector<std::vector<size_t>> test_rectangles = 
		{{9, 5, 41, 3}, {1, 5, 1, 5}, {1, 1, 0, 0}, {7, 1, 30, 2}, 
	  	 {8, 5, 37, 3}, {9, 4, 22, 3}};
	// Create and save geometries with all tested rectangles
	for (const auto props : test_rectangles){
		// Makes a name out of all target properties
		std::ostringstream fname("./test_data/rectangle_test", std::ios_base::ate);
		size_t iprop = 0;
		for (const auto pname : prop_names)
			fname << "_" << pname << "_" << props.at(iprop++);
		fname << ".txt";
		// 
		make_and_save_rectangle(Nx, Ny, props.at(0), props.at(1), 
								props.at(2), props.at(3), fname.str());
	}
}

/// \brief Tests correct exception behavior in object adding code
void rectangle_exception_test_suite()
{
	using namespace std::placeholders;	
	// Pointer to rectangle adding function
	auto add_rectangle_ptr = &Geometry::add_rectangle;	
	// Exception tests settings
	bool verbose = true;
	const std::runtime_error rtime("Run time error");
	// Geometry properties
	size_t Nx = 100, Ny = 300;
	Geometry geom(Nx, Ny);
	// Rectangle - dimensions outside, center inside
	size_t Rx = 5, Ry = 10, xc = 0, yc =50;
//	tst_pass(exception_test(verbose, &rtime, 
//							std::bind(&Geometry::add_rectangle, geom, _1, _2, _3, _4), 
//							Rx, Ry, xc, yc), "Rectangle too large");
	// Rectangle - dimensions ok, center outside
	xc = 50; 
	yc =500;
	//tst_pass(exception_test(verbose, &rtime, make_and_save_rectangle, Nx, Ny, Rx, Ry, xc, yc), "Rectangle center outside the domain");
}

/** 
 * \brief Creates and saves a geometry with a rectangle
 * @param Nx [in] - number of nodes in x direction
 * @param Ny [in] - number of nodes in y direction
 * @param Rx [in] - number of nodes in rectangle width (x)
 * @param Ry [in] - number of nodes in rectangle height (y)
 * @param xc [in] - rectangle center, x coordinate
 * @param yc [in] - rectangle center, y coordinate
 * @param fname [in] - name of the file to save the geometry to
 */
void make_and_save_rectangle(const size_t Nx, const size_t Ny, 
							 const size_t Rx, const size_t Ry,
							 const size_t xc, const size_t yc,
							 const std::string fname)
{
	Geometry geom(Nx, Ny);
	geom.add_rectangle(Rx, Ry, xc, yc);
	geom.write(fname);
}
