#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <limits>
#include "common.h"
#include "utils.h"
#include "geom_include.h"
#include "./io_operations/lbm_io.h"

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
	* \brief Creates a Geometry object with default properties
	* \details This constructor should be used when reading a geometry from a file 
	* 		fluid
	*/
	Geometry() = default; 
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
	 * \brief Create a pair of solid walls
	 * \details Create parallel walls in x or y directions at first/outmost nodes
	 * @param dH [in] - wall thickness, default 1 node
	 * @param where [in] - wall direction, "x" or "y", default "y"
	 */ 	
	void add_walls(const size_t dH = 1, const std::string where = "y");

	//
	// Objects
	//

	/** 
	* \brief Create a single rectangle
    * 
	* @param Lx [in] - number of nodes in x direction
    * @param Ly [in] - number of nodes in y direction
    * @param xc [in] - center x coordinate
    * @param yc [in] - center y coordinate
	*/
	void add_rectangle(const size_t Lx, const size_t Ly, 
						const size_t xc, const size_t yc);
	/** 
	* \brief Create a single square 
    * 
	* @param Ls [in] - number of nodes in x/y direction
    * @param xc [in] - center x coordinate
    * @param yc [in] - center y coordinate
	*/
	void add_square(const size_t Ls, const size_t xc, const size_t yc);	

	/** 
	* \brief Create a single ellipse 
    * 
	* @param Lx [in] - number of nodes in x direction axis
    * @param Ly [in] - number of nodes in y direction axis
    * @param xc [in] - center x coordinate
    * @param yc [in] - center y coordinate
	*/
	void add_ellipse(const size_t Lx, const size_t Ly, 
						const size_t xc, const size_t yc);

	/** 
	* \brief Create a single circle 
    * 
	* @param D [in] - number of nodes in x/y direction (diameter)
    * @param xc [in] - center x coordinate
    * @param yc [in] - center y coordinate
	*/
	void add_circle(const size_t D, const size_t xc, const size_t yc);	

	/** 
	* \brief Verify if the upper and lower bounds of an object to be cobnstructed are inside the domain 
    * 
	* @param Lx [in] - number of nodes in x direction
    * @param Ly [in] - number of nodes in y direction
    * @param xc [in] - center x coordinate
    * @param yc [in] - center y coordinate
	* @param name [in] - object name for the error message
	*/
	void check_object_bounds(const size_t Lx, const size_t Ly, 
				                           const size_t xc, const size_t yc, const std::string name);

	/** 
 	* \brief Verify if the object is not constructed outside the domain's upper bounds
   	* 
   	* \details This actually checks the nodes returned to the Geometry object by the 
   	* 	program that constructs the object, specifically if the max node returned by the 
   	* 	construction program doesn't exceed total size of geom array - 1.  
   	* 
	* @param obj [in] - vector of nodes that are considered an object 
	* @param name [in] - object name for the error message
	*/
	void check_constructed_object(const std::vector<size_t> obj, const std::string name);

	// 
	// Arrays of objects
	//
	
	/**
	 * \brief Create an array of built in objects given displacements
	 * @param [in] object_properties - vector with object parameters;
	 * 									parameters appear in the same
	 * 									order as in the add_ function
	 * 									normally used to construct
	 * 									that object 
	 * @param [in] array_bounds - nested vector with lower and upper outer bounds 
	 * 								of the array in following order: 
	 * 								[[lower x, upper x], [lower y, upper y]]
	 * @param [in] dx - displacement in x - difference between outermost
	 * 						bounds of the object in x; this is in number
	 * 						of nodes
	 * @param [in] dy - displacement in y - difference between outermost
	 * 						bounds of the object in y; this is in number
	 * 						of nodes
	 * @param [in] object_type - built in object type, case insensitive
	 * @param [in] alpha - staggering angle, default 0 (regular array)
	 */
	void add_array(const std::vector<size_t> object_properties,
					const std::vector<std::vector<size_t>> array_bounds, 
					const size_t dx, const size_t dy, 
					const std::string object_type, const size_t alpha=0);
	
	/**
	 * \brief Create an array of built in objects given object numbers
	 * @param [in] object_properties - vector with object parameters;
	 * 									parameters appear in the same
	 * 									order as in the add_ function
	 * 									normally used to construct
	 * 									that object 
	 * @param [in] array_bounds - nested vector with lower and upper outer bounds 
	 * 								of the array in following order: 
	 * 								[[lower x, upper x], [lower y, upper y]]
	 * @param [in] object_numbers - vector with numbers of objects in x and y 
	 * @param [in] object_type - built in object type, case insensitive
	 * @param [in] alpha - staggering angle, default 0 (regular array)
	 */
	void add_array(const std::vector<size_t> object_properties,
					const std::vector<std::vector<size_t>> array_bounds, 
					const std::vector<size_t> object_numbers,
					const std::string object_type, const size_t alpha=0);

	//
	// Change individual nodes
	//
	
	/** 
	* \brief Set geometry node to 1 (fluid node)
    * 
	* @param xi [in] - x coordinate 
	* @param yi [in] - y coordinate
	*/
	void set_node_fluid(const size_t xi, const size_t yi)
		{ geom[yi*_Nx + xi] = 1; }
	/** 
	* \brief Set geometry node to 0 (solid node) 
    * 
	* @param xi [in] - x coordinate 
	* @param yi [in] - y coordinate
	*/
	void set_node_solid(const size_t xi, const size_t yi)
		{ geom[yi*_Nx + xi] = 0; }

	// 
	// I/O
	//

	/** 
	* \brief Read geometry from file 
    * 
	* @param fname [in] - file name 
	*/
	void read(const std::string fname);
	/** 
	* \brief Save geometry to file 
    * 
	* @param fname [in] - file name 
	*/
	void write(const std::string fname) const;

	//
	// Indexing
	//

	/** 
	* \brief Returns geometry value at a node 
    * 
	* @param xi [in] - x coordinate 
	* @param yi [in] - y coordinate
	*/
	const bool operator()(const size_t xi, const size_t yi) const { return geom[yi*_Nx + xi]; } 	

	//	
	// Getters
	//
	
	/// \brief Get pointer to constant geom object (can't change it!)
	const bool* get_geom() const { return geom; }
	/// \brief Get number of nodes in x (horizontal) direction
	size_t Nx() const { return _Nx; }
	/// \brief Get number of nodes in y (vertical) direction
	size_t Ny() const { return _Ny; }
	
	//
	// Misc
	//
	
	/// \brief Destructor
	~Geometry() { delete[] geom; }
private:
	// Geometric domain
	bool* geom = nullptr;
	// Dimensions of the domain (in number of nodes)
	size_t _Nx = 0, _Ny = 0;
	// Pi
	const double pi = std::acos(-1);
	/** 
	 * \brief Generate nodes of an object for array creation
	 * \details Can construct any object already defined in 
	 * 				the Geometry class (rectangle, ellipse, etc.)
	 * @param [in] type - object type, case insensitive
	 * @param [in] p - vector of parameters for construction
	 * 					of the object
	 * @return Linear indices of the object 
	 */
	std::vector<std::vector<size_t>> create_built_in_object(std::string type, 
						const std::vector<size_t> p);
};

#endif

