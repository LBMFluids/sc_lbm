#include "geometry_tests.h"

/*************************************************************** 
 * Suite for testing creation of arrays of objects 
***************************************************************/

// In python verify the array as if by eying it, as in the notes

// Tests
void regular_array_test_suite();
void arrays_for_geom_py_tests();

// Supporting functions
void distance_based_array(std::vector<size_t>, std::vector<size_t>,
					std::string, std::vector<std::vector<size_t>>, 
					size_t, size_t, std::string);
void numbers_based_array(std::vector<size_t>, std::vector<size_t>,
					std::string, std::vector<std::vector<size_t>>, 
					size_t, size_t, std::string);
void make_array_from_file(std::string);

// Required files
// ./test_data/array_tests_input.txt - inputs for array testing

int main()
{
	regular_array_test_suite();
    arrays_for_geom_py_tests();
}

// Creates geometries with arrays defined in the property 
// file - property order in the file is hardcoded:
// object_type | geom dim x | geom dim y | obj dim x | obj dim y |
// first object center xc, yc
// array bounds x0, xf, y0, yf | object distances dx, dy |
// object numbers Nx, Ny - specify either distances or numbers, leave
// the other set as 0
void regular_array_test_suite()
{
	// File to read the properties from
	std::ifstream infile("./test_data/array_tests_input.txt");	
	std::string line;
	while (std::getline(infile, line)){
		make_array_from_file(line);		
	}
}

void make_array_from_file(std::string line)
{
		std::string object_type;
		size_t geom_dim_x = 0, geom_dim_y = 0;
		size_t obj_x = 0, obj_y = 0, xc = 0, yc = 0;
		size_t x0 = 0, xf = 0, y0 = 0, yf = 0, dx = 0, dy = 0, Nx = 0, Ny =0;
		
		// Read property set
		std::istringstream text(line);
		text >> object_type;
		text >> geom_dim_x >> geom_dim_y;
		text >> obj_x >> obj_y >> xc >> yc;
		text >> x0 >> xf >> y0 >> yf >> dx >> dy >> Nx >> Ny;	

		// Create the geometry
		Geometry geom(geom_dim_x, geom_dim_y);
		if (Nx == 0 || Ny == 0){
			if (object_type == "rectangle" || object_type == "ellipse"){
				geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, dx, dy, object_type);
			}else{
				geom.add_array({obj_x, xc, yc}, {{x0, xf},{y0, yf}}, dx, dy, object_type);
			}
		}else if (dx == 0 || dy == 0){
			if (object_type == "rectangle" || object_type == "ellipse"){
				geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {Nx, Ny}, object_type);
			}else{
				geom.add_array({obj_x, xc, yc}, {{x0, xf},{y0, yf}}, {Nx, Ny}, object_type);
			}
		}else
			std::cout << "Wrong input properties" << std::endl;
		
		// Create a file for this specific geometry
		std::string arr_file("./test_data/array_type_");
		arr_file += line; 
		geom.write(arr_file);

		// Create and save just the object
		Geometry geom_w_obj(geom_dim_x, geom_dim_y);
		if (object_type == "rectangle")
			geom_w_obj.add_rectangle(obj_x, obj_y, xc, yc);
   		if (object_type == "square")
			geom_w_obj.add_square(obj_x, xc, yc);
		if (object_type == "ellipse")
			geom_w_obj.add_ellipse(obj_x, obj_y, xc, yc);
   		if (object_type == "circle")
			geom_w_obj.add_circle(obj_x, xc, yc);
		arr_file += ".object";
		geom_w_obj.write(arr_file);
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
	// Properties of the ellipse same as retngle 
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


