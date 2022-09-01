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
		temp_f_dist_spare.resize(Ntot*Ndir, 0.0);
		temp_uc_x.resize(Ntot, 0.0); 
		temp_uc_y.resize(Ntot, 0.0); 
	}  

	/** 
	 * Initializes a droplet of one fluid in the other fluid
	 * @details This initialization will not put fluid nodes inside a solid
	 * @details The droplet is initialized as a circle (not a square)	
	 * 
	 * @param geom - geometry object
	 * @param bulk - continuous fluid
 	 * @param droplet - dispersed phase (the droplet)
	 * @param rho_b_in_d - density of the continuous fluid inside the droplet
	 * @param rho_d_in_b - density of the droplet inside the continuous fluid
	 * @param xc - droplet center, x coordinate 
	 * @param yc - droplet center, y corrdinate
	 * @param radius - droplet radius
	 */ 
	void initialize_droplet(const Geometry& geom, Fluid& bulk, Fluid& droplet, 
								const double rho_bulk, const double rho_droplet,
								const double rho_b_in_d, const double rho_d_in_b, 
								const double xc, const double yc, const double radius);

	/** 
	 * Initializes a rectangular area of one fluid in the other fluid
	 * @details This initialization will not put fluid nodes inside a solid
	 * @details The droplet is initialized as a circle (not a square)	
	 * 
	 * @param geom - geometry object
	 * @param bulk - continuous fluid
 	 * @param droplet - dispersed phase (the droplet)
	 * @param rho_b_in_d - density of the continuous fluid inside the droplet
	 * @param rho_d_in_b - density of the droplet inside the continuous fluid
	 * @param xc - rectangle center, x coordinate 
	 * @param yc - rectangle center, y corrdinate
	 * @param half_Lx - half the number of nodes that make up the x length of the rectangle
	 * @param half_Ly - half the number of nodes that make up the y length of the rectangle
	 */ 
	void initialize_fluid_rectangle(const Geometry& geom, Fluid& bulk, Fluid& droplet, 
								const double rho_bulk, const double rho_droplet,
								const double rho_b_in_d, const double rho_d_in_b, 
								const double xc, const double yc,
								const double half_Lx, const double half_Ly);


	/// Computes the force from fluid-solid interactions
	/// @details Stores it in the fluid objects  
	void compute_solid_surface_force(const Geometry&, Fluid&, Fluid&);

	/// Computes the force from fluid-fluid interactions for both fluids
	/// @details Stores it in the fluid objects 
	void compute_fluid_repulsive_interactions(const Geometry&, Fluid&, Fluid&);

	/// Calculate the macroscopic, composite, and equilibrium velocity
	void compute_equilibrium_velocities(Geometry& geom, Fluid&, Fluid&);

	/// Collision step for a single fluid
	void collide(const Geometry& geom, Fluid&);

	/// Collision step for a two fluids
	void collide(Fluid&, Fluid&);

	/// Add an external volume force to a single fluid (gravity, pressure drop)	
	/// @details The force is specified for each lattice direction (check manual)
	void add_volume_force(const Geometry&, Fluid&, const std::vector<double>&);

	/// Add an external volume force to a two species - two fluid system (gravity, pressure drop)	
	/// @details The force is specified for each lattice direction (check manual)
	void add_volume_force(const Geometry&, Fluid&, Fluid&, const std::vector<double>&);
 
	/// Streaming step for a single fluid
	void stream(const Geometry&, Fluid&);

	/// Streaming step for a two fluid species and two phases
	void stream(const Geometry&, Fluid&, Fluid&);

private:
	// Number of directions (Ntot is Nx*Ny)
	size_t Nx = 0, Ny = 0, Ntot = 0, Ndir = 9;
	// Boundary conditions (default periodic in all directions)
	bool xperiodic = true;
	bool yperiodic = true;
	// Weights for computing fluid-solid interactions
	const std::vector<double> solid_weights = {0.0, 1.0/9, 1.0/9, 1.0/9, 1.0/9,
							1.0/36, 1.0/36, 1.0/36, 1.0/36};
	// Weights for computing repulsive fluid-fluid interactions
	const std::vector<double> repulsion_weights = {0.0, 1.0/9, 1.0/9, 1.0/9, 1.0/9,
							1.0/36, 1.0/36, 1.0/36, 1.0/36};
	// Bounce-back direction conversions
	const std::vector<int> bb_rules = {3, 4, 1, 2, 7, 8, 5, 6};
	// Discerete velocities - x components
	const std::vector<int> Cx = {0, 1, 0, -1, 0, 1, -1, -1, 1};
	// Discerete velocities - y components
	const std::vector<int> Cy = {0, 0, 1, 0, -1, 1, 1, -1, -1};
	// Temporary containers for streaming operations
	std::vector<double> temp_f_dist;
 	std::vector<double> temp_f_dist_spare;
	// Temporary containers for composite velocities
	std::vector<double> temp_uc_x;
	std::vector<double> temp_uc_y;
};

#endif
