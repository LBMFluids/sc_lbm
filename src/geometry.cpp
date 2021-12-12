#include "../include/geometry.h"

/*************************************************************** 
 * class: Geometry
 *
 * Creation, storage, and access of model geometry
 *
 * Geometry consists of solid nodes (0s) and fluid nodes (1s). 
 * It can be created directly through various functions in this 
 * class or by reading from a file.
 *
 * Row-major order of the actual array geom 
 * 	[ row_1 | row_2 | ... | row_Ny ]
 * 	row_i : [node_1, ..., node_Nx]
 * There are _Nx nodes in a row and _Ny rows. 
 * row represents x and a column y direction.
 *
 * The user indexing starts with 0 and ends with N_x or N_y -1,
 * so it is the same as the underlying C++ indexing. To request
 * the first node, ask for geom(0,0) and last node for 
 * geom(_Nx-1, _Ny-1). 
 *
 * First index is the x direction and second is the y. 
 * First index is the position within a row, second indicates
 * which row the user asks for.
 *
 ***************************************************************/

//
// Walls
//

// Pair of walls in x or y direction
void Geometry::add_walls(const size_t dH, const std::string& where)
{
	// Convert the flat vector to a 2D vector
	// y - outer, x - inner
	std::vector<std::vector<int>> geom_vec_2D;
	geom_vec_2D = flat_vector_2_vector_2D(_Nx, _Ny, geom);

	// Walls in y or x direction with thickness dH
	// spanning the entire height/width
	if (where == "y") {
		if (dH > _Nx) { 
			throw std::runtime_error("Wall thickness larger than domain width (x)");
		} 
		for (size_t iy=0; iy<_Ny; iy++) {
			for (size_t ix=0; ix<dH; ix++) {
				geom_vec_2D.at(iy).at(ix) = 0;
				geom_vec_2D.at(iy).at((_Nx-1)-ix) = 0;
			}
		}
	} else if (where == "x") {
		if (dH > _Ny) {
			throw std::runtime_error("Wall thickness larger than domain height (y)");
		}
		size_t top = 0;
		for (size_t iy=0; iy<dH; iy++) {
			std::fill(geom_vec_2D.at(iy).begin(), geom_vec_2D.at(iy).end(), 0);
			top = _Ny - 1 - iy;
			std::fill(geom_vec_2D.at(top).begin(), geom_vec_2D.at(top).end(), 0);
		}
	} else {
		throw std::invalid_argument( "No options for position argument: " + where);
	}

	// Convert back to flat array
	geom = vector_2D_2_flat_vector(geom_vec_2D);
}

// 
// Contruction of individual objects
//

// Rectangle
void Geometry::add_rectangle(const size_t Lx, const size_t Ly,
                        const size_t xc, const size_t yc)
{
	check_object_bounds(Lx, Ly, xc, yc, "Rectangle");

	// Rectangle object and indices
	std::vector<size_t> obj;
	Rectangle rect(Lx, Ly, xc, yc);
	obj = sub2ind<size_t>(rect.get_nodes(), _Nx);

	check_constructed_object(obj, "rectangle");

	// Include in the geometry array
	for (const auto& node : obj) {
		geom.at(node) = 0;
	}
}

// Square
void Geometry::add_square(const size_t Ls, const size_t xc, const size_t yc)
{
	check_object_bounds(Ls, Ls, xc, yc, "Square");

	// Square object and indices
	std::vector<size_t> obj;
	Square square(Ls, xc, yc);
	obj = sub2ind<size_t>(square.get_nodes(), _Nx);

	check_constructed_object(obj, "square");

	// Include in the geometry array
	for (const auto& node : obj) {
		geom.at(node) = 0;
	}
}

// Ellipse
void Geometry::add_ellipse(const size_t Lx, const size_t Ly,
                        const size_t xc, const size_t yc)
{
	check_object_bounds(Lx, Ly, xc, yc, "Ellipse");

	// Ellipse object and indices
	std::vector<size_t> obj;
	Ellipse ellipse(Lx, Ly, xc, yc);
	obj = sub2ind<size_t>(ellipse.get_nodes(), _Nx);

	check_constructed_object(obj, "ellipse");

	// Include in the geometry array
	for (const auto& node : obj) {
		geom.at(node) = 0;
	}
}

// Circle
void Geometry::add_circle(const size_t D, const size_t xc, const size_t yc)
{
	check_object_bounds(D, D, xc, yc, "Circle");

	// Circle object and indices
	std::vector<size_t> obj;
	Circle circle(D, xc, yc);
	obj = sub2ind<size_t>(circle.get_nodes(), _Nx);

	check_constructed_object(obj, "circle");

	// Include in the geometry array
	for (const auto& node : obj) {
		geom.at(node) = 0;
	}
}

// Nominal object bounds check
void Geometry::check_object_bounds(const size_t Lx, const size_t Ly, 
				                     const size_t xc, const size_t yc, 
									 const std::string& name) const
{
	// This assumes that when Ls is even, it is shortened by 1 to retain symmetry
	if (((static_cast<int>(xc) - static_cast<int>(Lx)/2) < 0) 
			|| ((static_cast<int>(yc) - static_cast<int>(Ly)/2) < 0)) {
		throw std::runtime_error(name +  " lower bounds do not fit the domain.");
	}
	if (((xc + Lx/2) >= _Nx) || ((yc + Ly/2) >= _Ny)){
		throw std::runtime_error(name +  " upper bounds do not fit the domain.");
	}
}

// Verification of a constructed object
// Tested "manually" - currently no permanent test, first bound check captures any incorrect 
// input and this one really checks if the object creation program is wrong
void Geometry::check_constructed_object(const std::vector<size_t>& obj, 
											const std::string& name) const
{
	// Check if max index smaller than domain size 
	const auto iter = std::max_element(obj.begin(), obj.end());
	if ((!obj.empty()) && (*iter >= _Nx*_Ny)) {
		throw std::runtime_error("Constructed " + name + " upper bounds do not fit the domain.");
	}
}

//
// Arrays of objects
//

// Create an array of built in objects given displacements
void Geometry::add_array(const std::vector<size_t>& object_properties,
					const std::vector<std::vector<size_t>>& array_bounds, 
					const size_t dx, const size_t dy, 
					const std::string& object_type, const size_t alpha)
{
	std::vector<size_t> array_nodes;
	const std::vector<std::vector<size_t>> object_nodes = 
			create_built_in_object(object_type, object_properties);

	RegularArray array(object_nodes, array_bounds, dx, dy);
	array_nodes = sub2ind<size_t>(array.get_nodes(), _Nx);

	// Include in the geometry array
	for (const auto& node : array_nodes) {
		geom.at(node) = 0;
	}
}	

// Create an array of built in objects given object numbers
void Geometry::add_array(const std::vector<size_t>& object_properties,
					const std::vector<std::vector<size_t>>& array_bounds,
					const std::vector<size_t>& object_numbers,
					const std::string& object_type, const size_t alpha)
{
	std::vector<size_t> array_nodes;
	const std::vector<std::vector<size_t>> object_nodes = 
			create_built_in_object(object_type, object_properties);

	RegularArray array(object_nodes, array_bounds, object_numbers);
	array_nodes = sub2ind<size_t>(array.get_nodes(), _Nx);

	// Include in the geometry array
	for (const auto& node : array_nodes) {
		geom.at(node) = 0;
	}
}

// Create a built in object and return its nodes
std::vector<std::vector<size_t>> Geometry::create_built_in_object
						(const std::string& object_type, 
						const std::vector<size_t>& p)
{
	auto get_object_nodes = [](geom_object&& obj){ return obj.get_nodes(); };
	std::string vtypes("rectangle, square, ellipse, and circle.");

	// Retrieve the nodes of a build in object with a given type
	std::string type = str_to_lower(object_type);
	if (type == "rectangle") {
		return get_object_nodes(Rectangle(p[0], p[1], p[2], p[3]));
	} else if (type == "square") {
		return get_object_nodes(Square(p[0], p[1], p[2]));
	} else if (type == "ellipse") {
		return get_object_nodes(Ellipse(p[0], p[1], p[2], p[3]));
	} else if (type == "circle") {
		return get_object_nodes(Circle(p[0], p[1], p[2]));
	} else {
		throw std::invalid_argument("Non-existing object type. Valid types are: " + vtypes);
	}
}

//
// I/O
//

// Load a geometry from file
void Geometry::read(const std::string& fname)
{
	// LbmIO settings
	std::string delim(" ");
	bool sflag = true;
	std::vector<size_t> dims = {0,0,0};

	// Intermediate 2D vector
	std::vector<std::vector<int>> geom_vec_2D;

	// Read as a 2D vector and collect the dimensions
	LbmIO geom_io(fname, delim, sflag, dims);
	geom_vec_2D = geom_io.read_vector<int>();
	_Nx = geom_vec_2D.at(0).size();
	_Ny = geom_vec_2D.size();
	// Convert to target flat vector 
	geom = vector_2D_2_flat_vector(geom_vec_2D);
}

// Save geometry to file
void Geometry::write(const std::string& fname) const
{
	// LbmIO settings
	std::string delim(" ");
	bool sflag = true;
	std::vector<size_t> dims = {0,0,0};

	// Intermediate 2D vector
	std::vector<std::vector<int>> geom_vec_2D;

	// Convert geom array to a 2D vector
	geom_vec_2D = flat_vector_2_vector_2D(_Ny, _Nx, geom);

	// Write the 2D vector to file 
	LbmIO geom_io(fname, delim, sflag, dims);
	geom_io.write_vector<int>(geom_vec_2D);
}
