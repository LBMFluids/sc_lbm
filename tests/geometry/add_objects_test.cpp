#include "geometry_tests.h"

/*************************************************************** 
 * Suite for testing object creation for Geometry class 
***************************************************************/

// Tests
void rectangle_test_suite();
void rectangle_exception_test_suite();
void square_test_suite();
void square_exception_test_suite();
void ellipse_test_suite();
void ellipse_exception_test_suite();
void circle_test_suite();
void circle_exception_test_suite();

// Supporting functions
void make_and_save_rectangle(const size_t, const size_t, 
							 const size_t, const size_t,
							 const size_t, const size_t,
							 const std::string);
void make_and_save_square(const size_t, const size_t, 
							 const size_t, const size_t,
							 const size_t, const std::string);
void make_and_save_ellipse(const size_t, const size_t, 
							 const size_t, const size_t,
							 const size_t, const size_t,
							 const std::string);
void make_and_save_circle(const size_t, const size_t, 
							 const size_t, const size_t,
							 const size_t, const std::string);

// Files for removal (to prevent biased tests)
// ./test_data/rectangle_test_#.txt
// ./test_data/square_test_#.txt
// ./test_data/ellipse_test_#.txt
// ./test_data/circle_test_#.txt

int main()
{
	rectangle_test_suite();
 	rectangle_exception_test_suite();
	square_test_suite();
	square_exception_test_suite();
	ellipse_test_suite();
	ellipse_exception_test_suite();
	circle_test_suite();
	circle_exception_test_suite();
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

/// \brief Tests correct exception behavior in rectangle adding code
void rectangle_exception_test_suite()
{
	// Exception tests settings
	bool verbose = true;
	const std::runtime_error rtime("Runtime error");
	// Geometry properties
	size_t Nx = 100, Ny = 300;
	Geometry geom(Nx, Ny);
	// Rectangle - dimensions outside x, center inside
	size_t Rx = 5, Ry = 10, xc = 0, yc =50;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_rectangle, geom, Rx, Ry, xc, yc), "Rectangle too large in x");
	// Rectangle - dimensions outside y, center inside
	Ry = 1000;
	xc = 30;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_rectangle, geom, Rx, Ry, xc, yc), "Rectangle too large in y");
	// Rectangle - dimensions ok, x center outside
	Ry = 10;
	xc = 1050; 
	test_pass(exception_test(verbose, &rtime, &Geometry::add_rectangle, geom, Rx, Ry, xc, yc), "Rectangle x center outside the domain");
	// Rectangle - dimensions ok, y center outside
	xc = 50; 
	yc = 500;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_rectangle, geom, Rx, Ry, xc, yc), "Rectangle y center outside the domain");
}

///\brief Test various square objects for correctness
void square_test_suite()
{
	// Geometry width and height
	size_t Nx = 50, Ny = 10;
	// Properties of test squares (Ls, xc, yc)
	std::vector<std::string> prop_names = {"Ls", "xc", "yc"};
	std::vector<std::vector<size_t>> test_squares = 
		{{5, 41, 3}, {1, 1, 5}, {1, 0, 0}, {6, 37, 5}, {6, 37, 6}};
	// Create and save geometries with all tested squares
	for (const auto props : test_squares){
		// Makes a name out of all target properties
		std::ostringstream fname("./test_data/square_test", std::ios_base::ate);
		size_t iprop = 0;
		for (const auto pname : prop_names)
			fname << "_" << pname << "_" << props.at(iprop++);
		fname << ".txt";
		// 
		make_and_save_square(Nx, Ny, props.at(0), props.at(1), props.at(2), fname.str());
	}
}

/// \brief Tests correct exception behavior in square adding code
void square_exception_test_suite()
{
	// Exception tests settings
	bool verbose = true;
	const std::runtime_error rtime("Runtime error");
	// Geometry properties
	size_t Nx = 100, Ny = 300;
	Geometry geom(Nx, Ny);
	// Square - dimensions outside x, center inside
	size_t Ls = 5, xc = 0, yc =50;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_square, geom, Ls, xc, yc), "Square too large in x");
	// Square - dimensions outside y, center inside
	xc = 10; 
	yc =299;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_square, geom, Ls, xc, yc), "Square too large in y");
	// Square - dimensions ok, x center outside
	xc = 150; 
	test_pass(exception_test(verbose, &rtime, &Geometry::add_square, geom, Ls, xc, yc), "Square x center outside the domain");
	// Square - dimensions ok, y center outside
	xc = 50; 
	yc =500;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_square, geom, Ls, xc, yc), "Square y center outside the domain");
}

///\brief Test various ellipse objects for correctness
void ellipse_test_suite()
{
	// Geometry width and height
	size_t Nx = 500, Ny = 200;
	// Properties of test ellipses (Ex, Ey, xc, yc)
	std::vector<std::string> prop_names = {"Ex", "Ey", "xc", "yc"};
	std::vector<std::vector<size_t>> test_ellipses = 
		{{9, 5, 41, 3}, {1, 5, 1, 5}, {1, 1, 0, 0}, {7, 1, 30, 2}, 
	  	 {8, 5, 37, 3}, {9, 4, 22, 3}, {100, 50, 250, 100}};
	// Create and save geometries with all tested ellipses 
	for (const auto props : test_ellipses){
		// Makes a name out of all target properties
		std::ostringstream fname("./test_data/ellipse_test", std::ios_base::ate);
		size_t iprop = 0;
		for (const auto pname : prop_names)
			fname << "_" << pname << "_" << props.at(iprop++);
		fname << ".txt";
		// 
		make_and_save_ellipse(Nx, Ny, props.at(0), props.at(1), 
								props.at(2), props.at(3), fname.str());
	}
}

/// \brief Tests correct exception behavior in ellipse adding code
void ellipse_exception_test_suite()
{
	// Exception tests settings
	bool verbose = true;
	const std::runtime_error rtime("Runtime error");
	// Geometry properties
	size_t Nx = 100, Ny = 300;
	Geometry geom(Nx, Ny);
	// Ellipse - dimensions outside x, center inside
	size_t Ex = 5, Ey = 10, xc = 0, yc =50;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_ellipse, geom, Ex, Ey, xc, yc), "Ellipse too large in x");
	// Ellipse - dimensions outside y, center inside
	Ey = 1000;
	xc = 30;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_ellipse, geom, Ex, Ey, xc, yc), "Ellipse too large in y");
	// Ellipse - dimensions ok, x center outside
	Ey = 10;
	xc = 1050; 
	test_pass(exception_test(verbose, &rtime, &Geometry::add_ellipse, geom, Ex, Ey, xc, yc), "Ellipse x center outside the domain");
	// Ellipse - dimensions ok, y center outside
	xc = 50; 
	yc = 500;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_ellipse, geom, Ex, Ey, xc, yc), "Ellipse y center outside the domain");
}

///\brief Test various circle objects for correctness
void circle_test_suite()
{
	// Geometry width and height
	size_t Nx = 500, Ny = 200;
	// Properties of test circles (R, xc, yc)
	std::vector<std::string> prop_names = {"D", "xc", "yc"};
	std::vector<std::vector<size_t>> test_circles = 
		{{9, 41, 30}, {1, 1, 5}, {1, 1, 1}, {7, 30, 4}, 
	  	 {5, 37, 3}, {4, 22, 3}, {100, 250, 100}};
	// Create and save geometries with all tested circles 
	for (const auto props : test_circles){
		// Makes a name out of all target properties
		std::ostringstream fname("./test_data/circle_test", std::ios_base::ate);
		size_t iprop = 0;
		for (const auto pname : prop_names)
			fname << "_" << pname << "_" << props.at(iprop++);
		fname << ".txt";
		// 
		make_and_save_circle(Nx, Ny, props.at(0), props.at(1), 
								props.at(2), fname.str());
	}
}

/// \brief Tests correct exception behavior in circle adding code
void circle_exception_test_suite()
{
	// Exception tests settings
	bool verbose = true;
	const std::runtime_error rtime("Runtime error");
	// Geometry properties
	size_t Nx = 100, Ny = 300;
	Geometry geom(Nx, Ny);
	// Circle - dimensions outside x, center inside
	size_t D = 10, xc = 0, yc =50;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_circle, geom, D, xc, yc), "Circle too large in x");
	// Circle - dimensions outside y, center inside
	D = 10;
	xc = 30;
	yc = 299;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_circle, geom, D, xc, yc), "Circle too large in y");
	// Circle - dimensions ok, x center outside
	D = 10;
	yc = 200;
	xc = 1050; 
	test_pass(exception_test(verbose, &rtime, &Geometry::add_circle, geom, D, xc, yc), "Circle x center outside the domain");
	// Circle - dimensions ok, y center outside
	xc = 50; 
	yc = 5000;
	test_pass(exception_test(verbose, &rtime, &Geometry::add_circle, geom, D, xc, yc), "Circle y center outside the domain");
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

/** 
 * \brief Creates and saves a geometry with a square 
 * @param Nx [in] - number of nodes in x direction
 * @param Ny [in] - number of nodes in y direction
 * @param Ls [in] - number of nodes in square widht/height
 * @param xc [in] - square center, x coordinate
 * @param yc [in] - square center, y coordinate
 * @param fname [in] - name of the file to save the geometry to
 */
void make_and_save_square(const size_t Nx, const size_t Ny, 
							 const size_t Ls, const size_t xc, const size_t yc,
							 const std::string fname)
{
	Geometry geom(Nx, Ny);
	geom.add_square(Ls, xc, yc);
	geom.write(fname);
}

/** 
 * \brief Creates and saves a geometry with an ellipse 
 * @param Nx [in] - number of nodes in x direction
 * @param Ny [in] - number of nodes in y direction
 * @param Ex [in] - number of nodes in ellipse width (x)
 * @param Ey [in] - number of nodes in ellipse height (y)
 * @param xc [in] - ellipse center, x coordinate
 * @param yc [in] - ellipse center, y coordinate
 * @param fname [in] - name of the file to save the geometry to
 */
void make_and_save_ellipse(const size_t Nx, const size_t Ny, 
							 const size_t Ex, const size_t Ey,
							 const size_t xc, const size_t yc,
							 const std::string fname)
{
	Geometry geom(Nx, Ny);
	geom.add_ellipse(Ex, Ey, xc, yc);
	geom.write(fname);
}

/** 
 * \brief Creates and saves a geometry with an circle 
 * @param Nx [in] - number of nodes in x direction
 * @param Ny [in] - number of nodes in y direction
 * @param D [in] - number of nodes in circle diameter 
 * @param xc [in] - ellipse center, x coordinate
 * @param yc [in] - ellipse center, y coordinate
 * @param fname [in] - name of the file to save the geometry to
 */
void make_and_save_circle(const size_t Nx, const size_t Ny, 
							 const size_t D, const size_t xc,
							 const size_t yc, const std::string fname)
{
	Geometry geom(Nx, Ny);
	geom.add_circle(D, xc, yc);
	geom.write(fname);
}
