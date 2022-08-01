#ifndef LBM_H
#define LBM_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "geometry.h"
#include "fluid.h"
#include "logger.h"
#include "common.h"
#include "utils.h"
#include "./io_operations/lbm_io.h"

/***************************************************** 
 * class: LBM 
 * 
 * Interface class that provides all the LBM operations
 *
 ******************************************************/

class Fluid;

class LBM {
public:

	/// Need to assign the right size to temporary arrays
	LBM() = delete;
	
	/// Constructor: stores dimensions and initializes temporary arrays
	LBM(const Geometry& geom) 
	{	
		Nx = geom.Nx(); Ny = geom.Ny(); Ntot = Nx*Ny; 
		temp_f_dist.resize(Ntot*Ndir, 0.0); 
	}  

	/// Computes the force from fluid-solid interactions
	/// @details Stores it in the fluid objects  
	void compute_solid_surface_force(const Geometry&, Fluid&, Fluid&);

	/// Computes the force from fluid-fluid interactions for both fluids
	/// @details Stores it in the fluid objects 
	void compute_fluid_repulsive_interactions(const Geometry&, Fluid&, Fluid&);

	/// Collision step for a single fluid
	void collide(const Geometry& geom, Fluid&);

	/// Add an external volume force to a single fluid (gravity, pressure drop)	
	/// @details The force is specified for each lattice direction (check manual)
	void add_volume_force(const Geometry&, Fluid&, const std::vector<double>&); 

	/// Streaming step for a single fluid
	void stream(const Geometry& geom, Fluid&);

// stream(geom, fluid1, fluid2)
// stream(geom, fluid1)

private:
	// Number of directions (Ntot is Nx*Ny)
	size_t Nx = 0, Ny = 0, Ntot = 0, Ndir = 9;
	// Boundary conditions (default periodic in all directions)
	bool xperiodic = true;
	bool yperiodic = true;
	// Weights for computing fluid-solid interactions
	const std::vector<double> solid_weights = {0.0, 1./9, 1./9, 1./9, 1./9, ...
							1./36, 1./36, 1./36, 1./36};
	// Bounce-back direction conversions
	const std::vector<int> bb_rules = {3, 4, 1, 2, 7, 8, 5, 6};
	// Discerete velocities - x components
	const std::vector<int> Cx = {0, 1, 0, -1, 0, 1, -1, -1, 1};
	// Discerete velocities - y components
	const std::vector<int> Cy = {0, 0, 1, 0, -1, 1, 1, -1, -1};
	// Temporary container for streaming operations
	std::vector<double> temp_f_dist; 
};

#endif
