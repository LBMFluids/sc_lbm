#include "../include/lbm.h"

/***************************************************** 
 * class: LBM 
 * 
 * Interface class that provides all the LBM operations
 *
 ******************************************************/

// Collision step for a single fluid
void LBM::collide(const Geometry& geom, Fluid& fluid_1)
{
	// Equilibrium distribution and arrays
	fluid_1.compute_f_equilibrium(geom);
	std::vector<double>& f_dist = fluid_1.get_f_dist();
	const std::vector<double>& f_eq_dist = fluid_1.get_f_eq_dist();
	double omega = fluid_1.get_omega();
	// Collision
	for (size_t ai = 0; ai < Ntot; ++ai) {
		for (size_t dj = 0; dj < Ndir; ++dj) {						
			f_dist.at(ai + dj*Ntot) = (1.0 - omega)*f_dist.at(ai + dj*Ntot) + omega*f_eq_dist.at(ai + dj*Ntot);							
		}
	}
}

// Add an external volume force to a single fluid (gravity, pressure drop)
void LBM::add_volume_force(const Geometry& geom, Fluid& fluid_1, const std::vector<double>& force)
{
	std::vector<double>& f_dist = fluid_1.get_f_dist();
	for (size_t ai = 0; ai < Ntot; ++ai) {
		if (geom(ai) == 1) {
			for (size_t dj = 0; dj < Ndir; ++dj) {						
				f_dist.at(ai + dj*Ntot) += force.at(dj);							
			}
		}
	}
}

// Streaming step for a single phase fluid
void LBM::stream(const Geometry& geom, Fluid& fluid_1)
{
	std::vector<double>& f_dist = fluid_1.get_f_dist();
	int ist = 0, jst = 0;
	int xi = 0, yj =0, ijk_final = 0, bb_ijk_final = 0, ijk_ini = 0;
	// Stream with boundary conditions
	for (size_t ai = 0; ai < Ntot; ++ai) {
		xi = ai%Nx; 
		yj = ((ai-xi)/Nx)%Ny;
		// 1 - fluid node, 0 - solid
		if (geom(ai) == 1) {
			// Lattice direction 0
			temp_f_dist.at(ai) = f_dist.at(ai);
			// Remaining directions
			for (size_t dj = 1; dj < Ndir; ++dj) {
				// Counting for periodic boundaries
				if (Cx[dj] > 0) {
					ist = (xi+Cx[dj] < static_cast<int>(Nx)) ? (xi+Cx[dj]) : 0;
				} else {
					ist = (xi+Cx[dj] >= 0) ? (xi+Cx[dj]) : static_cast<int>(Nx)-1;
				}	
				if (Cy[dj] > 0) {
					jst = (yj+Cy[dj] < static_cast<int>(Ny)) ? (yj+Cy[dj]) : 0;
				} else {
					jst = (yj+Cy[dj] >= 0) ? (yj+Cy[dj]) : static_cast<int>(Ny)-1;
				}
				// Streaming with bounce-back
				ijk_ini = static_cast<size_t>(dj*Ntot + yj*Nx + xi);
				if (geom(ist,jst) == 1) {
					ijk_final = static_cast<size_t>(dj*Ntot + jst*Nx + ist);
					temp_f_dist.at(ijk_final) = f_dist.at(ijk_ini);
				} else {
					bb_ijk_final = static_cast<size_t>(bb_rules[dj-1]*Ntot + yj*Nx + xi);
					temp_f_dist.at(bb_ijk_final) = f_dist.at(ijk_ini);	
				}			
			}
		}
	}
	// Reassign and fill temp with 0s just in case
	std::swap(temp_f_dist, f_dist);
	std::fill(temp_f_dist.begin(), temp_f_dist.end(), 0.0);
}