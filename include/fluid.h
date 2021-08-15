#ifndef FLUID_H
#define FLUID_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "logger.h"

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
 * The order in arrays is row-major, for macroscopic 
 * 		[ row1 | row2 | ... | row_Nx ] 
 * and for distributions:
 *		[Direction 0][Direction 1] ... [Direction 9]
 * with each direction being
 * 		[ row1 | row2 | ... | row_Nx ]
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
		{ omega = 1/tau; nu = cs2*(tau - 0.5); prop_ini(f); }

	/// Custom name, other parameters default (0.333 and 1.0)
	Fluid(const std::string& s) : Fluid(s, 1./3, 1.0)
		{ prop_ini(f); }
	
	/// All parameters are defaults ("fluid", 0.333, and 1.0)
	Fluid() : Fluid("fluid", 1./3, 1.0)
		{ prop_ini(f); }

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
	// Getters 
	//

	//
	// Setters
	//

	//
	// I/O
	//

	/// Outputs fluid properties to the screen or a log	
	//void print_properties() const;

	/// Save macroscopic density to file
	void write_rho(const std::string fname)
			{ compute_rho(); write_var(rho, fname); }
	/// Save macroscopic x velocity component to file
	void write_ux(const std::string fname)
			{ compute_velocities(); write_var(ux, fname); }
	/// Save macroscopic y velocity component to file
	void write_uy(const std::string fname)
			{ compute_velocities(); write_var(uy, fname); }
	/// Save the density distribution to file
	/// @details This produces a sequence of numbered files fname_dir, where 
	/// 	dir is an integer that represents the direction (0 to 8)
	void write_f(const std::string fname) const 
			{ write_var(f_dist, fname); }
	/// Save all macroscopic properties at this step to files
	void save_state(const std::string frho, const std::string fux, 
						const std::string fuy, const int step);

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

	//
	// Variables
	//

	// Horizontal and vertical dimensions (number of nodes)
	size_t Nx = 0, Ny = 0;
	// Density distribution, flat array of size Nx*Ny*9 
	std::vector<double> f_dist; 
	// Macroscopic density Nx*Ny
	std::vector<double> rho;
	// Macroscopic velocity components
	std::vector<double> ux;
	std::vector<double> uy;

	//
	// Private methods
	//

	/// Write a 2D variable to file fname
	void write_var(const std::vector<double>& variable, const std::string& fname);

};

#endif
