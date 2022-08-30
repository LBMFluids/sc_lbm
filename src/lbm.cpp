#include "../include/lbm.h"

/***************************************************** 
 * class: LBM 
 * 
 * Interface class that provides all the LBM operations
 *
 ******************************************************/

// Initializes a droplet of one fluid in the other fluid 
void LBM::initialize_droplet(const Geometry& geom, Fluid& bulk, Fluid& droplet, 
								const double rho_bulk, const double rho_droplet,
								const double rho_b_in_d, const double rho_d_in_b, 
								const double xc, const double yc, const double radius)
{
	// Initialize directly through density distributions
	std::vector<double>& f_dist_bulk = bulk.get_f_dist();
	std::vector<double>& f_dist_droplet = droplet.get_f_dist();

	int xi = 0, yj =0;
	for (size_t ai = 0; ai < Ntot; ++ai) {
		xi = ai%Nx; 
		yj = ((ai-xi)/Nx)%Ny;
		
		// Skip solid nodes 
		if (geom(ai) == 0) {
			continue;
		}

		// If the point is in the droplet - initialize it with the droplet density set 
		if (((static_cast<double>(xi) - xc)*(static_cast<double>(xi) - xc) + (static_cast<double>(yj) - yc)*(static_cast<double>(yj) - yc)) <= radius*radius) {
			for (size_t dj = 0; dj < Ndir; ++dj) {
				// Dissolved density of the bulk fluid inside the droplet					
				f_dist_bulk.at(ai + dj*Ntot) = rho_b_in_d/Ndir;			
				// Nominal density of the droplet fluid
				f_dist_droplet.at(ai + dj*Ntot) = rho_droplet/Ndir;
			}		
		} else {
			for (size_t dj = 0; dj < Ndir; ++dj) {
				// Nominal density of the bulk fluid					
				f_dist_bulk.at(ai + dj*Ntot) = rho_bulk/Ndir;			
				// Dissolved density of the droplet fluid inside the bulk
				f_dist_droplet.at(ai + dj*Ntot) = rho_d_in_b/Ndir;
			}
		} 
	}
}

// Computes the force from fluid-solid interactions
void LBM::compute_solid_surface_force(const Geometry& geom, Fluid& fluid_1, Fluid& fluid_2)
{
	// Compute the common force components (fixed for stationary solids)
	std::vector<double> Fxs(Ntot, 0.0);
	std::vector<double> Fys(Ntot, 0.0);
	int inei = 0, jnei = 0;
	int xi = 0, yj =0;

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
				Fxs.at(ai) += solid_weights.at(dj)*Cx.at(dj);
				Fys.at(ai) += solid_weights.at(dj)*Cy.at(dj);				
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

	int inei = 0, jnei = 0, ij = 0;
	int xi = 0, yj =0;

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
			Fx_1.at(ai) += repulsion_weights.at(dj)*Cx.at(dj)*psi_2.at(ij);
			Fy_1.at(ai) += repulsion_weights.at(dj)*Cy.at(dj)*psi_2.at(ij);
			Fx_2.at(ai) += repulsion_weights.at(dj)*Cx.at(dj)*psi_1.at(ij);
			Fy_2.at(ai) += repulsion_weights.at(dj)*Cy.at(dj)*psi_1.at(ij);	
		}
		Fx_1.at(ai) *= Gf_1*psi_1.at(ai);	
		Fy_1.at(ai) *= Gf_1*psi_1.at(ai);
		Fx_2.at(ai) *= Gf_2*psi_2.at(ai);	
		Fy_2.at(ai) *= Gf_2*psi_2.at(ai);
	}
}

// Calculate the equilibrium velocities
void LBM::compute_equilibrium_velocities(Geometry& geom, Fluid& fluid_1, Fluid& fluid_2)
{
	// Note --- assumes the macroscopic density is already computed
	const double omega_1 = fluid_1.get_omega();
	const double inv_omega_1 = 1.0/omega_1;
	const std::vector<double>& rho_1 = fluid_1.get_rho();
	const std::vector<double>& f_dist_1 = fluid_1.get_f_dist();
	std::vector<double>& ux_1 = fluid_1.get_ux();	
	std::vector<double>& uy_1 = fluid_1.get_uy();
	std::vector<double>& u_eq_x_1 = fluid_1.get_u_eq_x();	
	std::vector<double>& u_eq_y_1 = fluid_1.get_u_eq_y();
	std::vector<double>& F_fr_x_1 = fluid_1.get_repulsive_force_x();
	std::vector<double>& F_fr_y_1 = fluid_1.get_repulsive_force_y();
	const std::vector<double>& Fs_x_1 = fluid_1.get_fluid_solid_force_x();
	const std::vector<double>& Fs_y_1 = fluid_1.get_fluid_solid_force_y();

	const double omega_2 = fluid_2.get_omega();
	const double inv_omega_2 = 1.0/omega_2;
	const std::vector<double>& rho_2 = fluid_2.get_rho();
	const std::vector<double>& f_dist_2 = fluid_2.get_f_dist();
	std::vector<double>& ux_2 = fluid_2.get_ux();	
	std::vector<double>& uy_2 = fluid_2.get_uy();
	std::vector<double>& u_eq_x_2 = fluid_2.get_u_eq_x();	
	std::vector<double>& u_eq_y_2 = fluid_2.get_u_eq_y();
	std::vector<double>& F_fr_x_2 = fluid_2.get_repulsive_force_x();
	std::vector<double>& F_fr_y_2 = fluid_2.get_repulsive_force_y();
	const std::vector<double>& Fs_x_2 = fluid_2.get_fluid_solid_force_x();
	const std::vector<double>& Fs_y_2 = fluid_2.get_fluid_solid_force_y();

	// For numeric comparisons
	const double tol = 1e-8;

	// Compute composite velocity
	std::fill(ux_1.begin(), ux_1.end(), 0.0);
	std::fill(uy_1.begin(), uy_1.end(), 0.0);
	std::fill(ux_2.begin(), ux_2.end(), 0.0);
	std::fill(uy_2.begin(), uy_2.end(), 0.0);

	for (size_t i=0; i<Ntot; ++i) {
		if (geom(i) == 1) {
			// Unweighted (by density) macroscopic velocity
			for (size_t j=0; j<Ndir; ++j) {
				ux_1.at(i) += f_dist_1.at(j*Ntot+i)*Cx.at(j); 
				uy_1.at(i) += f_dist_1.at(j*Ntot+i)*Cy.at(j);
				ux_2.at(i) += f_dist_2.at(j*Ntot+i)*Cx.at(j); 
				uy_2.at(i) += f_dist_2.at(j*Ntot+i)*Cy.at(j);
			}

			// Composite velocity
			temp_uc_x.at(i) = (ux_1.at(i)*omega_1+ux_2.at(i)*omega_2)/(rho_1.at(i)*omega_1+rho_2.at(i)*omega_2);  
			temp_uc_y.at(i) = (uy_1.at(i)*omega_1+uy_2.at(i)*omega_2)/(rho_1.at(i)*omega_1+rho_2.at(i)*omega_2);

			// Equilibrium velocities
			if (!equal_floats(rho_1.at(i), 0.0, tol)) {	
				u_eq_x_1.at(i) = temp_uc_x.at(i) + F_fr_x_1.at(i)*inv_omega_1/rho_1.at(i) + Fs_x_1.at(i)*inv_omega_1;
 				u_eq_y_1.at(i) = temp_uc_y.at(i) + F_fr_y_1.at(i)*inv_omega_1/rho_1.at(i) + Fs_y_1.at(i)*inv_omega_1;			
			}
			if (!equal_floats(rho_2.at(i), 0.0, tol)) {	
				u_eq_x_2.at(i) = temp_uc_x.at(i) + F_fr_x_2.at(i)*inv_omega_2/rho_2.at(i) + Fs_x_2.at(i)*inv_omega_2;
 				u_eq_y_2.at(i) = temp_uc_y.at(i) + F_fr_y_2.at(i)*inv_omega_2/rho_2.at(i) + Fs_y_2.at(i)*inv_omega_2;			
			}
		}
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

// Collision step for two fluids
void LBM::collide(Fluid& fluid_1, Fluid& fluid_2)
{
	// Equilibrium distributions and arrays
	fluid_1.compute_f_equilibrium();
	fluid_2.compute_f_equilibrium();

	std::vector<double>& f_dist_1 = fluid_1.get_f_dist();
	std::vector<double>& f_dist_2 = fluid_2.get_f_dist();

	const std::vector<double>& f_eq_dist_1 = fluid_1.get_f_eq_dist();
	double omega_1 = fluid_1.get_omega();
	const std::vector<double>& f_eq_dist_2 = fluid_2.get_f_eq_dist();
	double omega_2 = fluid_2.get_omega();

	// Collision
	for (size_t ai = 0; ai < Ntot; ++ai) {
		for (size_t dj = 0; dj < Ndir; ++dj) {						
			f_dist_1.at(ai + dj*Ntot) = (1.0 - omega_1)*f_dist_1.at(ai + dj*Ntot) + omega_1*f_eq_dist_1.at(ai + dj*Ntot);							
			f_dist_2.at(ai + dj*Ntot) = (1.0 - omega_2)*f_dist_2.at(ai + dj*Ntot) + omega_2*f_eq_dist_2.at(ai + dj*Ntot);
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

// Add an external volume force to a two species - two fluid system (gravity, pressure drop)
void LBM::add_volume_force(const Geometry& geom, Fluid& fluid_1, Fluid& fluid_2, const std::vector<double>& force)
{
	std::vector<double>& f_dist_1 = fluid_1.get_f_dist();
	std::vector<double>& f_dist_2 = fluid_2.get_f_dist();
	for (size_t ai = 0; ai < Ntot; ++ai) {
		if (geom(ai) == 1) {
			for (size_t dj = 0; dj < Ndir; ++dj) {						
				f_dist_1.at(ai + dj*Ntot) += force.at(dj);						
				f_dist_2.at(ai + dj*Ntot) += force.at(dj);
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

// Streaming step for a two fluid species and two phases
void LBM::stream(const Geometry& geom, Fluid& fluid_1, Fluid& fluid_2)
{
	std::vector<double>& f_dist_1 = fluid_1.get_f_dist();
	std::vector<double>& f_dist_2 = fluid_2.get_f_dist();

	int ist = 0, jst = 0;
	int xi = 0, yj =0, ijk_final = 0, bb_ijk_final = 0, ijk_ini = 0;
	// Stream with boundary conditions
	for (size_t ai = 0; ai < Ntot; ++ai) {
		xi = ai%Nx; 
		yj = ((ai-xi)/Nx)%Ny;
		// 1 - fluid node, 0 - solid
		if (geom(ai) == 1) {
			// Lattice direction 0
			temp_f_dist.at(ai) = f_dist_1.at(ai);
			temp_f_dist_spare.at(ai) = f_dist_2.at(ai);
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
					temp_f_dist.at(ijk_final) = f_dist_1.at(ijk_ini);
					temp_f_dist_spare.at(ijk_final) = f_dist_2.at(ijk_ini);
				} else {
					bb_ijk_final = static_cast<size_t>(bb_rules[dj-1]*Ntot + yj*Nx + xi);
					temp_f_dist.at(bb_ijk_final) = f_dist_1.at(ijk_ini);
					temp_f_dist_spare.at(bb_ijk_final) = f_dist_2.at(ijk_ini);
				}			
			}
		}
	}
	// Reassign and fill temp with 0s just in case
	std::swap(temp_f_dist, f_dist_1);
	std::fill(temp_f_dist.begin(), temp_f_dist.end(), 0.0);
	std::swap(temp_f_dist_spare, f_dist_2);
	std::fill(temp_f_dist_spare.begin(), temp_f_dist_spare.end(), 0.0);
}
