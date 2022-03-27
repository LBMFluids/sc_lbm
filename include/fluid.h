#ifndef FLUID_H
#define FLUID_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "geometry.h"
#include "lbm.h"
#include "logger.h"
#include "common.h"
#include "utils.h"
#include "./io_operations/lbm_io.h"

/***************************************************** 
 * class: Fluid 
 * 
 * Initializes single fluid, stores its density
 *	distribution and other properties. Computes 
 *  macroscopic quantities: density and velocity.
 *
 * The arrays are flat STL vectors of dimensions
 * Nx*Ny (for macroscopic) and Nx*Ny*9 for the 
 * distribution.
 * 
 * First direction (0) is the center, last (8th) is 
 * North-West.
 *
 * The order in arrays is row-major, for macroscopic 
 * 		[ row1 | row2 | ... | row_Ny ]
 * with row_i: [ col1 | col2 | ... | col_Nx] 
 *
 * and for distributions:
 *		[Direction 0][Direction 1] ... [Direction 9]
 * with each direction being
 * 		[ row1 | row2 | ... | row_Ny ]
 * and row structure as above
 *  
 * The user indexing starts with 0 and ends with N_x-1 or N_y-1,
 * so it is the same as the underlying C++ indexing. To request
 * the first node, ask for property(0,0) and last node for 
 * property(_Nx-1, _Ny-1). 
 *
 * First index is the x direction and second is the y. 
 * First index is the position within a row, second indicates
 * which row the user asks for.
 *
 ******************************************************/

class Fluid {
public:
		
	// 
	//Constructors
	//	
	
	/// Custom name, squared lattice speed of sound, and relaxation time
	Fluid(const std::string& s, const double speed, const double rel) :
			name(s), cs2(speed), tau(rel)
		{ omega = 1/tau; nu = cs2*(tau - 0.5); }

	/// Custom name, other parameters default (0.333 and 1.0)
	Fluid(const std::string& s) : Fluid(s, 1./3, 1.0)
		{ }
	
	/// All parameters are defaults ("fluid", 0.333, and 1.0)
	Fluid() : Fluid("fluid", 1./3, 1.0)
		{ }

	// 
	// Initialization
	//

	/// Initialization of density distributions
	/// @param geom - complete domain geometry
	/// @param rho_0 - initial density of this fluid, same everywhere
	void simple_ini(const Geometry& geom, const double rho_0);

	//
	// Macroscopic properties
	// 
	
	/// Compute macroscopic density
	void compute_density();

	/// Compute macroscopic velocities
	void compute_velocities();

	// Compute density and x and y velocity components
	void compute_macroscopic();

	//
	// Other properties
	//

	/// Compute the equilibrium distribution function
	void compute_f_equilibrium();

	//
	// Getters 
	//

	/// Reference to density distribution, flat array of size Nx*Ny*9 
	std::vector<double>& get_f_dist() { return f_dist; } 
	/// Reference to equilibrium density distribution, flat array of size Nx*Ny*9 
    std::vector<double>& get_f_eq_dist() { return f_eq_dist; }
	/// Reference to macroscopic density Nx*Ny
	std::vector<double>& get_rho() { return rho; }
	/// Reference to macroscopic x velocity component
	std::vector<double>& get_ux() { return ux; }
	/// Reference to macroscopic y velocity component
	std::vector<double>& get_uy() { return uy; }
	/// x dimension
	size_t get_Nx() const { return Nx; }
	/// y dimension
	size_t get_Ny() const { return Ny; }
	/// Omega (fluid constant)
	double get_omega() const { return omega; }
	
	/// Const reference to density distribution, flat array of size Nx*Ny*9 
    const std::vector<double>& get_f_dist() const { return f_dist; } 
	/// Const reference to equilibrium density distribution, flat array of size Nx*Ny*9 
    const std::vector<double>& get_f_eq_dist() const { return f_eq_dist; }
	/// Const reference to macroscopic density Nx*Ny
	const std::vector<double>& get_rho() const { return rho; }
	/// Const reference to macroscopic x velocity component
	const std::vector<double>& get_ux() const { return ux; }
	/// Const reference to macroscopic y velocity component
	const std::vector<double>& get_uy() const { return uy; }

	/// Density weights for computing the equilibrium distribution
	std::vector<double> get_wrts() const { return {wrt0, wrt1, wrt2}; }  

	//
	// Setters
	//

	//
	// I/O
	//

	/// Outputs fluid properties to the screen or a log	
	//void print_properties() const;

	/// Save macroscopic density to file
	void write_density(const std::string& fname)
			{ compute_density(); write_var(rho, fname); }
	/// Save macroscopic x velocity component to file
	void write_ux(const std::string& fname)
			{ compute_velocities(); write_var(ux, fname); }
	/// Save macroscopic y velocity component to file
	void write_uy(const std::string& fname)
			{ compute_velocities(); write_var(uy, fname); }
	/// Save the density distribution to file
	/// @details This produces a sequence of numbered files fname_dir, where 
	/// 	dir is an integer that represents the direction (0 to 8);
	///		this adds a "_" to fname so no need for it
	void write_f(const std::string& fname) const 
			{ write_var(f_dist, fname, true); }
	/// Save all macroscopic properties at this step to files
	/// @details Appends the step number and an "_"
	void save_state(const std::string& frho, const std::string& fux, 
						const std::string& fuy, const int step);

private:

	//
	// Parameters
	//

	// Name of the fluid
	std::string name;
	// Number of directions
	const size_t Ndir = 9;
	// Squared lattice speed
	double cs2 = 0.0;
	// Relaxation time 
	double tau = 0.0;
	// Kinematic viscosity 
	double nu = 0.0;
	// Inverse of relaxation time
	double omega = 0.0;
	// Discrete lattice velocities
	const std::vector<double> Cx = {0.0, 1.0, 0.0, -1.0, 0.0, 1.0, -1.0, -1.0, 1.0};
    const std::vector<double> Cy = {0.0, 0.0, 1.0, 0.0, -1.0, 1.0, 1.0, -1.0, -1.0};	
	// Weights for the equilibrium distribution
	double wrt0 = 4.0/9.0, wrt1 = 1.0/9.0, wrt2 = 1.0/36.0;
	double feq1 = 3.0, feq2 = 4.5, feq3 = 1.5;

	//
	// Variables
	//

	// Horizontal and vertical dimensions (number of nodes)
	size_t Nx = 0, Ny = 0, Ntot = 0;
	// Density distribution function, flat array of size Nx*Ny*9 
	std::vector<double> f_dist; 
	// Equilibrium density distribution function, flat array of size Nx*Ny*9 
	std::vector<double> f_eq_dist;
	// Macroscopic density Nx*Ny
	std::vector<double> rho;
	// Macroscopic velocity components
	std::vector<double> ux;
	std::vector<double> uy;

	//
	// Private methods
	//

	/// Write a 2D variable to file fname
	void write_var(const std::vector<double>& variable, const std::string& fname) const;

	/// Write a 3D variable to file fname
	void write_var(const std::vector<double>& variable, const std::string& fname, const bool is_3D) const;
};

#endif
