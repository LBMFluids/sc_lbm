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
// Contruction of individual objects
//

// Rectangle
void Geometry::add_rectangle(const size_t Lx, const size_t Ly,
                        const size_t xc, const size_t yc)
{
	std::vector<size_t> obj;
	Rectangle rect(Lx, Ly, xc, yc);
	obj = rect.get_nodes();			
}
