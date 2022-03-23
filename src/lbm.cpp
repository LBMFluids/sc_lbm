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
	std::vector<double>& f_dist = fluid.get_f_dist();
	std::vector<double>& f_eq_dist = fluid.get_f_eq_dist();
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
	std::vector<double>& f_dist = fluid.get_f_dist();
	for (size_t ai = 0; ai < Nx*Ny; ++ai) {
		if (geom(ai) == 1) {
			for (size_t dj = 0; dj < Ndir; ++dj) {						
				f_dist.at(ai + dj*Ntot) += force.at(dj);							
			}
		}
	}
}

// Streaming step for a single fluid
void LBM::stream(const Geometry& geom, Fluid& fluid)
{
	std::vector<double>& f_dist = fluid.get_f_dist();
	size_t ist = 0, jst = 0;
	size_t xi = 0, yj =0, aik = 0, bb_aik = 0;

	for (size_t ai = 0; ai < Ntot; ++ai) {
		xi = ai%Nx; 
		yj = ai/Nx;
		if (geom(ai) == 1) {
			for (size_t dj = 0; dj < Ndir; ++dj) {
				// Periodic boundaries
				if (Cx[dj] > 0) {
					ist = (xi+Cx[dj] < Nx+1) ? (xi+Cx[dj]) : 0;
				} else {
					ist = (xi+Cx[dj] >= 0) ? (xi+Cx[dj]) : Nx;
				}
				if (Cy[dj] > 0) {
					jst = (yj+Cy[dj] < Ny+1) ? (yj+Cy[dj]) : 0;
				} else {
					jst = (yj+Cy[dj] >= 0) ? (yj+Cy[dj]) : Ny;
				}
				// Streaming with bounce-back
				aik = dj*Ntot + Nyj*Nx + xi;
				if (geom(ist,jst) == 1) {
					temp.at(aik) = f_dist.at(aik);
				} else {
					bb_aik = bb_rules[dj-1]*Ntot + Nyj*Nx + xi;
					temp.at(bb_aik) = f_dist.at(aik);	
				}			
			}
		}
	}
	// -------- copy temp to f_dist

}
