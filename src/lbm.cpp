#include "../include/lbm.h"

/***************************************************** 
 * class: LBM 
 * 
 * Interface class that provides all the LBM operations
 *
 ******************************************************/

// Computes the force from fluid-solid interactions
void LBM::compute_solid_surface_force(const Geometry& geom, Fluid& fluid_1, Fluid& fluid_2)
{
	// Compute the common force components (fixed for stationary solids)
	std::vector<double> Fxs(Ntot, 0.0);
	std::vector<double> Fys(Ntot, 0.0);
	int inei = 0, jnei = 0;
	int xi = 0, yj =0, ijk_final = 0, bb_ijk_final = 0, ijk_ini = 0;

	for (size_t ai = 0; ai < Ntot; ++ai) {
		xi = ai%Nx; 
		yj = ((ai-xi)/Nx)%Ny;
		// Skip solid nodes 
		if (geom(ai) == 0) {
			continue;
		}
		// All non-stationary lattice directions
		for (size_t dj = 1; dj < Ndir; ++dj) {
			// Counting for periodic boundaries
			if (Cx[dj] > 0) {
				inei = (xi+Cx[dj] < static_cast<int>(Nx)) ? (xi+Cx[dj]) : 0;
			} else {
				inei = (xi+Cx[dj] >= 0) ? (xi+Cx[dj]) : static_cast<int>(Nx)-1;
			}	
			if (Cy[dj] > 0) {
				jnei = (yj+Cy[dj] < static_cast<int>(Ny)) ? (yj+Cy[dj]) : 0;
			} else {
				jnei = (yj+Cy[dj] >= 0) ? (yj+Cy[dj]) : static_cast<int>(Ny)-1;
			}
			// Force is non-zero only if the neighbor is a solid node
			if (geom(inei, jnei) == 0) {
				Fxs(ai) += solid_weights(dj)*Cx(dj);
				Fys(ai) += solid_weights(dj)*Cy(dj);				
			} 		
		}
	}

	// Specific values for each fluid
	fluid_1.add_surface_forces(Fxs, Fys);
	fluid_2.add_surface_forces(Fxs, Fys);
}

// Computes the force from the repulsive fluid-fluid interactions for both fluids
void LBM::compute_fluid_repulsive_interactions(const Geometry& geom, Fluid& fluid_1, Fluid& fluid_2)
{
	// Compute the x and y force components in one loop for both fluids 
	std::vector<double>& Fx_1 = fluid_1.get_repulsive_force_x();
	std::vector<double>& Fy_1 = fluid_1.get_repulsive_force_y();
	std::vector<double>& Fx_2 = fluid_2.get_repulsive_force_x();
	std::vector<double>& Fy_2 = fluid_2.get_repulsive_force_y();

	// Assuming the potential is equal to density and the density
	// is precomputed
	const std::vector<double>& psi_1 = fluid_1.get_rho();
    const std::vector<double>& psi_2 = fluid_2.get_rho();

	// Interaction potentials
	const double Gf_1 = fluid_1.get_repulsive_g_fluid();
	const double Gf_2 = fluid_2.get_repulsive_g_fluid();

	int inei = 0, jnei = 0;
	int xi = 0, yj =0, ijk_final = 0, bb_ijk_final = 0, ijk_ini = 0;

	for (size_t ai = 0; ai < Ntot; ++ai) {
		xi = ai%Nx; 
		yj = ((ai-xi)/Nx)%Ny;
		// Skip solid nodes 
		if (geom(ai) == 0) {
			continue;
		}
		// All non-stationary lattice directions
		for (size_t dj = 1; dj < Ndir; ++dj) {
			// Counting for periodic boundaries
			if (Cx[dj] > 0) {
				inei = (xi+Cx[dj] < static_cast<int>(Nx)) ? (xi+Cx[dj]) : 0;
			} else {
				inei = (xi+Cx[dj] >= 0) ? (xi+Cx[dj]) : static_cast<int>(Nx)-1;
			}	
			if (Cy[dj] > 0) {
				jnei = (yj+Cy[dj] < static_cast<int>(Ny)) ? (yj+Cy[dj]) : 0;
			} else {
				jnei = (yj+Cy[dj] >= 0) ? (yj+Cy[dj]) : static_cast<int>(Ny)-1;
			}
			// Skip solid nodes 
			if (geom(inei, jnei) == 0) {
				continue;
			}
			// Back to linear index
			ij = static_cast<size_t>(jnei*Nx + inei);
			// Compute the forces and accumulate
			Fx_1(ai) += repulsion_weights(dj)*Cx(dj)*psi_2(ij);
			Fy_1(ai) += repulsion_weights(dj)*Cy(dj)*psi_2(ij);
			Fx_2(ai) += repulsion_weights(dj)*Cx(dj)*psi_1(ij);
			Fy_2(ai) += repulsion_weights(dj)*Cy(dj)*psi_1(ij);	
		}
		Fx_1(ai) *= Gf_1*psi_1(ai);	
		Fy_1(ai) *= Gf_1*psi_1(ai);
		Fx_2(ai) *= Gf_2*psi_2(ai);	
		Fy_2(ai) *= Gf_2*psi_2(ai);
	}
}

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
