#ifndef FLUID_H
#define FLUID_H

#include <iostream>
#include <vector>
#include "logger.h"

/***************************************************** 
 * class: Fluid 
 * 
 * Functionality for logging the runtime information   
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

	// Initialization of density distributions
	//void simple_ini(const Geometry<Lx,Ly>&, double);

	//
	// Getters 
	//

	//
	// Setters
	//

	//
	// I/O
	//
	

private:

	//
	// Parameters
	//

	// Name of the fluid
	std::string name;
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

	std::vector<double> f_dist; 

};

#endif
