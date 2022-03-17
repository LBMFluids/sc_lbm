#include "../include/lbm.h"

/***************************************************** 
 * class: LBM 
 * 
 * Interface class that provides all the LBM operations
 *
 ******************************************************/

// Collision step for a single fluid
void LBM::collide(Fluid& fluid)
{
	std::vector<double>& f_dist = get_f_dist();
	std::vector<double>& f_eq_dist = get_f_eq_dist();
	size_t Nx = fluid.get_Nx();
	size_t Ny = fluid.get_Ny();
	size_t Ntot = Nx*Ny;
	double omega = fluid.get_omega();

	for (size_t ai = 0; ai < Nx*Ny; ++ai) {
		for (size_t dj = 0; dj < Ndir; ++dj){						
			f_dist.at(ai + dj*Ntot) = (1.0 - omega)*f_dist.at(ai + dj*Ntot) + omega*f_eq_dist.at(ai + dj*Ntot);							
		}
	}
}

// Add an external volume force to a single fluid (gravity, pressure drop)
void LBM::add_volume_force(const Geometry& geom, Fluid& fluid, const std::vector<double>& force)
{
	std::vector<double>& f_dist = get_f_dist();
	size_t Nx = fluid.get_Nx();
	size_t Ny = fluid.get_Ny();
	size_t Ntot = Nx*Ny;

	for (size_t ai = 0; ai < Nx*Ny; ++ai) {
		if (geom(ai) == 1) {
			for (size_t dj = 0; dj < Ndir; ++dj) {						
				f_dist.at(ai + dj*Ntot) += force.at(dj);							
			}
		}
	}
}

/// Streaming step for a single fluid
void LBM::stream(const Geometry& geom, Fluid& fluid)
{

}
