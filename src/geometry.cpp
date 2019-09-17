#include "../include/geometry.h"

/*************************************************************** 
 * class: Geometry
 *
 * Creation, storage, and access of model geometry
 *
 * Geometry consists of solid nodes (0s) and fluid nodes (1s). 
 * It can be created directly through various functions in this 
 * class or by reading from a file.
***************************************************************/

//
// Constructors and copy/move assignment
//

// Copy constructor
Geometry::Geometry(const Geometry& gm)
{ 
	_Nx = gm._Nx; 
	_Ny = gm._Ny;
	geom = new bool[_Nx*_Ny];
	std::copy(gm.geom, gm.geom + _Nx*_Ny, geom); 
}
// Copy assignment 
// Object must have the same dimensions and allocated geom
Geometry& Geometry::operator=(const Geometry& rhs)
{ 
	if (_Nx != rhs._Nx || _Ny != rhs._Ny){ 
		throw std::runtime_error("Geometry objects must have the same size to be copy-assigned");
	}else{
		if (geom == nullptr)
			throw std::runtime_error("Geometry array not allocated");
		std::copy(rhs.geom, rhs.geom + _Nx*_Ny, geom);
	}
	return *this;	
}

// Move constructor 
Geometry::Geometry(Geometry&& gm) noexcept
{
	_Nx = gm._Nx;
	_Ny = gm._Ny;
	geom = gm.geom;
	gm.geom = nullptr;
}

// Move assignment
// Object must have the same dimensions and allocated geom
Geometry& Geometry::operator=(Geometry&& rhs)
{ 
	if (_Nx != rhs._Nx || _Ny != rhs._Ny){ 
		throw std::runtime_error("Geometry objects must have the same size to be move-assigned");
	}else{
		if (geom == nullptr)
			throw std::runtime_error("Geometry array not allocated");
		if (this != &rhs){
			geom = rhs.geom;
			rhs.geom = nullptr;
		}
	}
	return *this;	
}

//
// Walls
//

// Pair of walls in x or y direction
void Geometry::add_walls(const size_t dH, const std::string where)
{
	// Convert the flat array to a 2D vector
	std::vector<std::vector<bool>> geom_vec_2D;
	geom_vec_2D = flat_array_2_vector_2D(_Ny, _Nx, geom);

	// Walls in y or x direction with thickness dH
	// spanning the entire height/width
	if (where == "y"){
		if (dH > _Nx) 
			throw std::runtime_error("Wall thickness larger than domain width (x)");
		for (size_t irow=0; irow<_Ny; irow++){
			for (size_t icol=0; icol<dH; icol++){
				geom_vec_2D[irow][icol] = 0;
				geom_vec_2D[irow][(_Nx-1)-icol] = 0;
			}
		}
	}else if (where == "x"){
		if (dH > _Ny)
			throw std::runtime_error("Wall thickness larger than domain height (y)");
		size_t top = 0;
		for (size_t irow=0; irow<dH; irow++){
			std::fill(geom_vec_2D[irow].begin(), geom_vec_2D[irow].end(), 0);
			top = _Ny - 1 - irow;
			std::fill(geom_vec_2D[top].begin(), geom_vec_2D[top].end(), 0);
		}
	}else{
		throw std::invalid_argument( "No options for position argument: " + where);
	}

	// Convert back to flat array
	vector_2D_2_flat_array(geom_vec_2D, _Nx*_Ny, geom);
}

// 
// Contruction of individual objects
//

// Rectangle
void Geometry::add_rectangle(const size_t Lx, const size_t Ly,
                        const size_t xc, const size_t yc)
{
	// size_t is unsigned (always >= 0), need to compare only with its
	// upper numeric limit
	size_t max_st = std::numeric_limits<std::size_t>::max(); 
	if (Lx > max_st || Ly > max_st || xc > max_st || yc > max_st)
		throw std::runtime_error("Rectangle dimensions or position larger than numeric limit of size_t");
	std::vector<size_t> obj;
	Rectangle rect(Lx, Ly, xc, yc);
	obj = sub2ind<size_t>(rect.get_nodes(), _Nx);
	for (auto node : obj)
		geom[node] = false;
}

//
// I/O
//

// Load a geometry from file
void Geometry::read(const std::string fname)
{
	// LbmIO settings
	std::string delim(" ");
	bool sflag = true;
	std::vector<size_t> dims = {0,0,0};

	// Intermediate 2D vector
	std::vector<std::vector<bool>> geom_vec_2D;

	// Read as a 2D vector and collect the dimensions
	LbmIO geom_io(fname, delim, sflag, dims);
	geom_vec_2D = geom_io.read_vector<bool>();
	_Nx = geom_vec_2D.at(0).size();
	_Ny = geom_vec_2D.size();
	// Initialize the geom array and convert the vector 
	// to it
	geom = new bool[_Nx*_Ny]; 
	vector_2D_2_flat_array(geom_vec_2D, _Nx*_Ny, geom);
}

// Save geometry to file
void Geometry::write(const std::string fname) const
{
	// LbmIO settings
	std::string delim(" ");
	bool sflag = true;
	std::vector<size_t> dims = {0,0,0};

	// Intermediate 2D vector
	std::vector<std::vector<bool>> geom_vec_2D;

	// Convert geom array to a 2D vector
	geom_vec_2D = flat_array_2_vector_2D(_Ny, _Nx, geom);

	// Write the 2D vector to file 
	LbmIO geom_io(fname, delim, sflag, dims);
	geom_io.write_vector<bool>(geom_vec_2D);
}
