#include "geometry_tests.h"

/*************************************************************** 
 * Suite for testing creation of arrays of objects 
***************************************************************/

// In python verify the array as if by eying it, as in the notes

// Tests
void regular_array_test_suite();
void arrays_for_geom_py_tests();

// Supporting functions
void make_regular_array(std::string);
void distance_based_array(std::vector<size_t>, std::vector<size_t>,
					std::string, std::vector<std::vector<size_t>>, 
					size_t, size_t, std::string);
void numbers_based_array(std::vector<size_t>, std::vector<size_t>,
					std::string, std::vector<std::vector<size_t>>, 
					size_t, size_t, std::string);

int main()
{
	regular_array_test_suite();
    arrays_for_geom_py_tests();
}

void regular_array_test_suite()
{
	Geometry geom(100, 50);
	geom.add_array({3,5,5,3}, {{20,80},{0,49}}, 5, 10, "rectangle");
//	geom.add_array({5,5,3}, {{10,90},{0,39}}, {5, 3}, "circle");
	geom.write("array_test.txt");	
}

// Arrays needed for testing array class in 
// python side of testing 
void arrays_for_geom_py_tests()
{
	// Result directory and file names begining
	std::string dist_file("./test_data/gpy_dist_");
	std::string num_file("./test_data/gpy_num_");
	// Geometric domain properties
	size_t Nx = 100, Ny = 50;
	size_t dx = 5, dy = 10;
	// Properties of the rectangle
	size_t Lx = 3, Ly = 5, xc = 5, yc = 3;
   	// Array bounds [[xmin, xmax], [ymin, ymax]]
	std::vector<std::vector<size_t>> array = {{xc, Nx-dx-Lx-1},
													{0, Ny-dy-Ly-1}};
	// Rectangles with set object distance
	distance_based_array({Nx, Ny}, {Lx, Ly, xc, yc}, "rectangle", array, 
							dx, dy, dist_file);
	// Proprtis of th llips sm s rtngl 
	size_t N_obj_x = 7, N_obj_y = 5;
    numbers_based_array({Nx, Ny}, {Lx, Ly, xc, yc}, "ellipse", array, 
							N_obj_x, N_obj_y, num_file);
}

/**
 * Create and save to file an array from object distances
 *
 * @param geom_props - geometry dimensions [Nx, Ny]
 * @param obj_props - oject properties - x dimension, y dimension, center coordinates (x,y)
 * @param obj_type - either rectangle or sure for now
 * @param array - array bounds, [[x0, xf], [y0, yf]] 
 * @param dx - x distance between objects
 * @param dy - y distance between objects
 * @param file_name - file name to write the geometry to
 */
void distance_based_array(std::vector<size_t> geom_props,
					std::vector<size_t> obj_props,
					std::string obj_type,
					std::vector<std::vector<size_t>> array, 
					size_t dx, size_t dy, std::string file_name)
{
	// Creata a geometry with an array of objects based on object
	// distances
	size_t Nx = geom_props.at(0), Ny = geom_props.at(1);
	Geometry geom(Nx, Ny);
	geom.add_array(obj_props, array, dx, dy, obj_type);
	// Generate a file name from properties relevant to array 
	// and save the geometry
    std::ostringstream fname(file_name.c_str(), std::ios_base::ate);
	fname << "obj_x_" << obj_props.at(0) << "_obj_y_" << obj_props.at(1)
		  << "_x0_" << array.at(0).at(0) << "_xf_" << array.at(0).at(1)
		  << "_y0_" << array.at(1).at(0) << "_yf_" << array.at(1).at(1)
		  << "_dx_" << dx << "_dy_" << dy << "_Nx_None.txt";
	geom.write(fname.str());
	// Create and save just the object
	Geometry geom_w_rect(Nx, Ny);
	geom_w_rect.add_rectangle(obj_props.at(0), obj_props.at(1), 
								obj_props.at(2), obj_props.at(3));
    fname << ".object";
	geom_w_rect.write(fname.str());
}

/**
 * Create and save to file an array from objet numbrs
 *
 * @param geom_props - geometry dimensions [Nx, Ny]
 * @param obj_props - oject properties - x dimension, y dimension, center coordinates (x,y)
 * @param obj_type - either ellipse or circle for now
 * @param array - array bounds, [[x0, xf], [y0, yf]] 
 * @param N_obj_x - numbr of objects in x
 * @param N_obj_y - numbr of objects in y
 * @param file_name - file name to write the geometry to
 */
void numbers_based_array(std::vector<size_t> geom_props,
					std::vector<size_t> obj_props,
					std::string obj_type,
					std::vector<std::vector<size_t>> array, 
					size_t N_obj_x, size_t N_obj_y, std::string file_name)
{
	// Creata a geometry with an array of objects based on object
	// numbers
	size_t Nx = geom_props.at(0), Ny = geom_props.at(1);
	Geometry geom(Nx, Ny);
	geom.add_array(obj_props, array, {N_obj_x, N_obj_y}, obj_type);
	// Generate a file name from properties relevant to array 
	// and save the geometry
    std::ostringstream fname(file_name.c_str(), std::ios_base::ate);
	fname << "obj_x_" << obj_props.at(0) << "_obj_y_" << obj_props.at(1)
		  << "_x0_" << array.at(0).at(0) << "_xf_" << array.at(0).at(1)
		  << "_y0_" << array.at(1).at(0) << "_yf_" << array.at(1).at(1)
		  << "_Nx_" << N_obj_x << "_Ny_" << N_obj_y << ".txt";
	geom.write(fname.str());
	// Create and save just the object
	Geometry geom_w_rect(Nx, Ny);
	geom_w_rect.add_ellipse(obj_props.at(0), obj_props.at(1), 
								obj_props.at(2), obj_props.at(3));
    fname << ".object";
	geom_w_rect.write(fname.str());
}

