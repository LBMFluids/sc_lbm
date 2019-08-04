#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <limits>
#include "common.h"
#include "utils.h"
#include "geom_include.h"

/*************************************************************** 
 * class: Geometry
 *
 * Creation, storage, and access of model geometry
 *
 * Geometry consists of solid nodes (0s) and fluid nodes (1s). 
 * It can be created directly through various functions in this 
 * class or by reading from a file.
 *
 * Row-major order of the actual array geom [ row1 | row2 | ... | row_Nx ]
 * where row represents x direction, i.e. _Ny nodes in x direction.
***************************************************************/

// --------> this will only be a managing/serving class, make read
// and write a separate class, like geom_io or something, but not
// accessible to the user

class Geometry{
public:

	//
	// Constructors and copy/move operators
	//

	/** 
	 * \brief Creates a Geometry object with nodes \f$N_x\times N_y\f$ nodes
	 * \details Underlying array is initialized to 1 i.e. it is filled with
	 * 		fluid
	 * @param Nx [in] - number of nodes in x direction
	 * @param Ny [in] - number of nodes in y direction
	*/
	Geometry(const size_t Nx, const size_t Ny) : _Nx(Nx), _Ny(Ny) 
				{ geom = new bool[_Nx*_Ny](); std::fill(geom, geom + _Nx*_Ny, true); }
	/** \brief Copy constructor
	 *	\details Creates a new Geometry object through a deep copy 
	 *		of another Geometry object
	 *	@param gm [in] - Geometry object to be copy from
	*/
	Geometry(const Geometry& gm);
	/** 
	 * \brief Copy assignment operator 
	 * \details Copies the content of a Geometry object to 
	 *		another geometry object. Objects must have the same dimensions.
	 * @param rhs [in] - Geometry object to be copied/assigned from 
	 */
	Geometry& operator=(const Geometry& rhs);
	/** \brief Move constructor
	 *	\details Creates a new Geometry object by direct moving of 
	 *		another Geometry object
	 *	@param gm [in] - object to move from
	*/
	Geometry(Geometry&& gm) noexcept;
	/** 
	 * \brief Move assignment operator 
	 * \details Moves content of Geometry object rhs to 
	 *		another geometry object. Objects must have the same dimensions.
	 *	@param rhs [in] - object to move from
	 */
	Geometry& operator=(Geometry&& rhs);

	//
	// Walls
	//

	/** 
	 * \brief Create a segment of solid walls
	 *
	 * @param dH [in] - wall thickness, @param where [in] - wall direction
	 */ 	
	void add_walls(const size_t dH=1, const std::string where ="y");

	//
	// Objects
	//

	/** 
	* \brief Create a single rectangle
    * 
	* @param Lx [in] - number nodes in x direction
    * @param Ly [in] - number of nodes in y direction
    * @param xc [in] - center x coordinate
    * @param yc [in] - center y coordinate
	*/
	void add_rectangle(const size_t Lx, const size_t Ly, 
						const size_t xc, const size_t yc);
	void add_square(const std::vector<size_t>);
	void add_ellipse(const std::vector<size_t>);
	void add_circle(const std::vector<size_t>);
	// Arrays of objects 
	void add_array(const std::vector<size_t>, const std::vector<size_t>, 
					const std::string, const size_t alpha=0);
	// Indexing
	//const bool operator()(const size_t i, const size_t j) const { return geom[i][j]; } 	
	// I/O
	void write(const std::string) const;
	void read(const std::string);
	// Getters
	/// \brief Get pointer to constant geom object (can't change it!)
	const bool* get_geom() const { return geom; }
	/// \brief Get number of nodes in x (horizontal) direction
	size_t Nx() const { return _Nx; }
	/// \brief Get number of nodes in y (vertical) direction
	size_t Ny() const { return _Ny; }
	/// \brief Destructor
	~Geometry() { delete[] geom; }
private:
	// Geometric domain
	bool* geom = nullptr;
	// Dimensions of the domain (in number of nodes)
	size_t _Nx = 0, _Ny = 0;
	// Pi
	const double pi = std::acos(-1);
};

#endif

