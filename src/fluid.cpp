#include "../include/fluid.h"

// 
// Initialization
//

// Initialization of density distributions, density and velocity arrays
void Fluid::simple_ini(const Geometry& geom, const double rho_0)
{
	// Number of nodes in each direction
	Nx = geom.Nx();		
	Ny = geom.Ny();
	const double rho_factor = static_cast<double>(Ndir);
	// Coordinates
	size_t xi = 0, yi = 0, ind_fi = 0, yfi = 0, dir_i = 0;
	// Just so they are of the right size
	rho.resize(Nx*Ny, 0.0);
	ux.resize(Nx*Ny, 0.0);
	uy.resize(Nx*Ny, 0.0);
	// Zero-initialized distribution function
	f_dist.resize(Nx*Ny*Ndir, 0.0);
	for (size_t i=0; i<Nx; ++i) {
		for (size_t j=0; j<Ny; ++j) {
			yi = j;
			xi = yi*Nx + xi;
			// If a fluid node, fill out the value in all directions
			if (geom(xi,yi) == 1) {
				for (size_t k=0; k<Ndir; ++k) {
					dir_i = k;
					yfi = dir_i*Nx*Ny + yi*Nx;
					ind_fi = yfi + xi;
					f_dist.at(ind_fi) = rho_0/rho_factor;
				}			
			} 
		}
	}
}

//
// Macroscopic properties
// 

// Compute macroscopic density
void Fluid::compute_density()
{
	std::fill(rho.begin(), rho.end(), 0.0);
	for (i=0; i<Nx*Ny; ++i) {
		for (j=0; j<Ndir; ++j) {
			rho.at(i) += f_dist.at(j*Nx*Ny+i); 
		}
	}				
}

// Compute macroscopic velocities
void Fluid::compute_velocities()
{
	compute_density();
	for (i=0; i<Nx*Ny; ++i) {
		for (j=0; j<Ndir; ++j) {
			ux.at(i) += f_dist.at(j*Nx*Ny+i)*Cx.at(j); 
			uy.at(i) += f_dist.at(j*Nx*Ny+i)*Cy.at(j);
		}
		ux.at(i) *= rho.at(i);
		uy.at(i) *= rho.at(i);
	}	
}

// Compute density and x and y velocity components
void Fluid::compute_macroscopic()
{
	compute_density();
	compute_velocities();
}
