#include "create_geometries.h"

//
// Creates and saves hardcoed geometries for all tests
//

int main()
{
	make_empty();
	make_x_walls();
	make_y_walls();
	make_array_no_walls();
	make_array_x_walls();
	make_array_y_walls();
}

// No solid nodes
void make_empty()
{
	std::string path("test_data/");
	size_t Nx = 5, Ny = 10;
	Geometry geom(Nx, Ny);
	geom.write(path + "empty_domain.txt");	
}

// A wall spanning the x direction (and growing in y) 
void make_x_walls()
{
	std::string path("test_data/");
	size_t Nx = 20, Ny = 10;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "x");
	geom.write(path + "domain_with_x_walls.txt");
}

// A wall spanning the y direction (and growing in x)
void make_y_walls()
{
	std::string path("test_data/");
	size_t Nx = 20, Ny = 10;
	size_t dh = 3;
	Geometry geom(Nx, Ny);
	geom.add_walls(dh, "y");
	geom.write(path + "domain_with_y_walls.txt");
}

// A staggered array no walls
void make_array_no_walls()
{
	std::string path("test_data/");
	size_t Nx = 200, Ny = 100;
	Geometry geom(Nx, Ny);
	std::string object_type("ellipse");
	size_t obj_x = 5, obj_y = 7, xc = 10, yc = 15;
	size_t x0 = 5, xf = 190, y0 = 3, yf = 70;
	size_t ob_num_x = 20, ob_num_y = 5;
	geom.add_array({obj_x, obj_y, xc, yc}, {{x0, xf},{y0, yf}}, {ob_num_x, ob_num_y}, object_type);
	geom.write(path + "domain_with_array_no_walls.txt");
}

// A staggered array with x walls
void make_array_x_walls()
{
	std::string path("test_data/");
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
}

// A staggered array with y walls
void make_array_y_walls()
{
	std::string path("test_data/");
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
}
