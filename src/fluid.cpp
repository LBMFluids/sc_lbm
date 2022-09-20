#include "../include/fluid.h"

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
 * First direction (0) is the center, last (8th) is 
 * North-West.
 *
 * The order in arrays is row-major, for macroscopic 
 * 		[ row1 | row2 | ... | row_Ny ]
 * with row_i: [ col1 | col2 | ... | col_Nx] 
 *
 * and for distributions:
 *		[Direction 0][Direction 1] ... [Direction 9]
 * with each direction being
 * 		[ row1 | row2 | ... | row_Ny ]
 * and row structure as above
 *  
 * The user indexing starts with 0 and ends with N_x-1 or N_y-1,
 * so it is the same as the underlying C++ indexing. To request
 * the first node, ask for property(0,0) and last node for 
 * property(_Nx-1, _Ny-1). 
 *
 * First index is the x direction and second is the y. 
 * First index is the position within a row, second indicates
 * which row the user asks for.
 *
 ******************************************************/

// 
// Initialization
//

// Initialization of all simulation variables and zero densities 
void Fluid::zero_density_ini(const Geometry& geom)
{
	// Number of nodes in each direction
	Nx = geom.Nx();		
	Ny = geom.Ny();
	Ntot = Nx*Ny;
	// Zero initialize all the macroscopic and intermediate variables
	rho.resize(Nx*Ny, 0.0);
	ux.resize(Nx*Ny, 0.0);
	uy.resize(Nx*Ny, 0.0);
	u_eq_x.resize(Nx*Ny, 0.0);
	u_eq_y.resize(Nx*Ny, 0.0);
	F_repulsive_x.resize(Nx*Ny, 0.0);
	F_repulsive_y.resize(Nx*Ny, 0.0);
	// Zero-initialized distribution function
	f_dist.resize(Nx*Ny*Ndir, 0.0);
	// Zero-initialized equilibrium distribution function
	f_eq_dist.resize(Nx*Ny*Ndir, 0.0);
}

// Initialization of density distributions, density and velocity arrays
void Fluid::simple_ini(const Geometry& geom, const double rho_0, const bool mcmp)
{
	// Number of nodes in each direction
	Nx = geom.Nx();		
	Ny = geom.Ny();
	Ntot = Nx*Ny;
	const double rho_factor = static_cast<double>(Ndir);
	// Coordinates
	size_t xi = 0, yi = 0, ind_fi = 0, yfi = 0, dir_i = 0;
	// Just so they are of the right size
	rho.resize(Nx*Ny, 0.0);
	ux.resize(Nx*Ny, 0.0);
	uy.resize(Nx*Ny, 0.0);
	// Only for mcmp systems
	if (mcmp) {
		u_eq_x.resize(Nx*Ny, 0.0);
		u_eq_y.resize(Nx*Ny, 0.0);
		F_repulsive_x.resize(Nx*Ny, 0.0);
		F_repulsive_y.resize(Nx*Ny, 0.0);
	}
	// Zero-initialized distribution function
	f_dist.resize(Nx*Ny*Ndir, 0.0);
	for (size_t i=0; i<Nx; ++i) {
		for (size_t j=0; j<Ny; ++j) {
			xi = i;
			yi = j;
			// If a fluid node, fill out the value in all directions
			if (geom(xi,yi) == 1) {
				for (size_t k=0; k<Ndir; ++k) {
					dir_i = k;
					// Size of a row is Nx (and there are Ny rows)
					yfi = dir_i*Nx*Ny + yi*Nx;
					ind_fi = yfi + xi;
					f_dist.at(ind_fi) = rho_0/rho_factor;
				}			
			} 		
		}
	}
	// Zero-initialized equilibrium distribution function
	f_eq_dist.resize(Nx*Ny*Ndir, 0.0);
}

// Compute and store the force components stemming from interactions with solids
void Fluid::add_surface_forces(const std::vector<double> Fxs, const std::vector<double> Fys)
{
    std::transform(Fxs.cbegin(), Fxs.cend(), std::back_inserter(F_solid_x),
                   [this](double el) { return -1.0*this->Gsolid*el; });		
    std::transform(Fys.cbegin(), Fys.cend(), std::back_inserter(F_solid_y),
                   [this](double el) { return -1.0*this->Gsolid*el; });
}

// Initialize vectors and parameters for repulsive interactions
void Fluid::initialize_fluid_repulsion(const double Gf)
{
	Gfluid_repulsion = Gf;	
	F_repulsive_x.resize(Nx*Ny, 0.0);
	F_repulsive_y.resize(Nx*Ny, 0.0);
}

//
// Macroscopic properties
// 

// Compute macroscopic density
void Fluid::compute_density()
{
	std::fill(rho.begin(), rho.end(), 0.0);
	for (size_t i=0; i<Ntot; ++i) {
		for (size_t j=0; j<Ndir; ++j) {
			rho.at(i) += f_dist.at(j*Ntot+i);
		}
	}
}

// Compute macroscopic velocities
void Fluid::compute_velocities(const Geometry& geom)
{
	std::fill(ux.begin(), ux.end(), 0.0);
	std::fill(uy.begin(), uy.end(), 0.0);
	for (size_t i=0; i<Ntot; ++i) {
		if (geom(i) == 1) {
			for (size_t j=0; j<Ndir; ++j) {
				ux.at(i) += f_dist.at(j*Ntot+i)*Cx.at(j); 
				uy.at(i) += f_dist.at(j*Ntot+i)*Cy.at(j);
			}
			ux.at(i) /= rho.at(i);
			uy.at(i) /= rho.at(i);
		}
	}	
}

// Compute density and x and y velocity components
void Fluid::compute_macroscopic(const Geometry& geom)
{
	compute_density();
	compute_velocities(geom);
}

// Compute the equilibrium distribution function
void Fluid::compute_f_equilibrium(const Geometry& geom)
{
	double rt0 = 0.0, rt1 = 0.0, rt2 = 0.0;
	double ueqxij = 0.0, ueqyij = 0.0, uxsq = 0.0, uysq = 0.0, uxuy5 = 0.0;
	double uxuy6 = 0.0, uxuy7 = 0.0, uxuy8 = 0.0, usq = 0.0;

	compute_macroscopic(geom);
	for (size_t ai = 0; ai < Ntot; ++ai) {

		rt0 = wrt0*rho.at(ai);
		rt1 = wrt1*rho.at(ai);
	 	rt2 = wrt2*rho.at(ai);

		ueqxij =  ux.at(ai);
      	ueqyij =  uy.at(ai);
	    uxsq   =  ueqxij * ueqxij;
		uysq   =  ueqyij * ueqyij;
		uxuy5  =  ueqxij +  ueqyij;
		uxuy6  = -ueqxij +  ueqyij;
		uxuy7  = -ueqxij - ueqyij;
		uxuy8  =  ueqxij - ueqyij;
		usq    =  uxsq + uysq;

		f_eq_dist.at(ai) = rt0*(1.0 - feq3*usq);
		f_eq_dist.at(ai + Ntot) = rt1*(1.0 + feq1*ueqxij + feq2*uxsq - feq3*usq);
		f_eq_dist.at(ai + 2*Ntot) = rt1*(1.0 + feq1*ueqyij + feq2*uysq - feq3*usq);
		f_eq_dist.at(ai + 3*Ntot) = rt1*(1.0 - feq1*ueqxij + feq2*uxsq - feq3*usq);
		f_eq_dist.at(ai + 4*Ntot) = rt1*(1.0 - feq1*ueqyij + feq2*uysq - feq3*usq);
		f_eq_dist.at(ai + 5*Ntot) = rt2*(1.0 + feq1*uxuy5 + feq2*uxuy5*uxuy5 - feq3*usq);
		f_eq_dist.at(ai + 6*Ntot) = rt2*(1.0 + feq1*uxuy6 + feq2*uxuy6*uxuy6 - feq3*usq);
		f_eq_dist.at(ai + 7*Ntot) = rt2*(1.0 + feq1*uxuy7 + feq2*uxuy7*uxuy7 - feq3*usq);
		f_eq_dist.at(ai + 8*Ntot) = rt2*(1.0 + feq1*uxuy8 + feq2*uxuy8*uxuy8 - feq3*usq);
	}			
}

// Compute the equilibrium distribution function in a multicomponent - multiphase system
void Fluid::compute_f_equilibrium()
{
	double rt0 = 0.0, rt1 = 0.0, rt2 = 0.0;
	double ueqxij = 0.0, ueqyij = 0.0, uxsq = 0.0, uysq = 0.0, uxuy5 = 0.0;
	double uxuy6 = 0.0, uxuy7 = 0.0, uxuy8 = 0.0, usq = 0.0;

	for (size_t ai = 0; ai < Ntot; ++ai) {

		rt0 = wrt0*rho.at(ai);
		rt1 = wrt1*rho.at(ai);
	 	rt2 = wrt2*rho.at(ai);

		ueqxij =  u_eq_x.at(ai);
      	ueqyij =  u_eq_y.at(ai);
	    uxsq   =  ueqxij * ueqxij;
		uysq   =  ueqyij * ueqyij;
		uxuy5  =  ueqxij +  ueqyij;
		uxuy6  = -ueqxij +  ueqyij;
		uxuy7  = -ueqxij - ueqyij;
		uxuy8  =  ueqxij - ueqyij;
		usq    =  uxsq + uysq;

		f_eq_dist.at(ai) = rt0*(1.0 - feq3*usq);
		f_eq_dist.at(ai + Ntot) = rt1*(1.0 + feq1*ueqxij + feq2*uxsq - feq3*usq);
		f_eq_dist.at(ai + 2*Ntot) = rt1*(1.0 + feq1*ueqyij + feq2*uysq - feq3*usq);
		f_eq_dist.at(ai + 3*Ntot) = rt1*(1.0 - feq1*ueqxij + feq2*uxsq - feq3*usq);
		f_eq_dist.at(ai + 4*Ntot) = rt1*(1.0 - feq1*ueqyij + feq2*uysq - feq3*usq);
		f_eq_dist.at(ai + 5*Ntot) = rt2*(1.0 + feq1*uxuy5 + feq2*uxuy5*uxuy5 - feq3*usq);
		f_eq_dist.at(ai + 6*Ntot) = rt2*(1.0 + feq1*uxuy6 + feq2*uxuy6*uxuy6 - feq3*usq);
		f_eq_dist.at(ai + 7*Ntot) = rt2*(1.0 + feq1*uxuy7 + feq2*uxuy7*uxuy7 - feq3*usq);
		f_eq_dist.at(ai + 8*Ntot) = rt2*(1.0 + feq1*uxuy8 + feq2*uxuy8*uxuy8 - feq3*usq);
	}			
}

//
// I/O
//

// Save a 2D variable to file
void Fluid::write_var(const std::vector<double>& variable, const std::string& fname) const
{
	// Convert to a 2D vector - outer - rows (0 to Ny-1), inner - columns (0 to Nx-1)
	std::vector<std::vector<double>> temp_2D;
	std::vector<double> one_row;
	one_row.resize(Nx, -1.0);
	size_t ind = 0;
	for (size_t i=0; i<Nx*Ny; ++i) {
		// First element of the first row
		if (i == 0) {
			one_row.at(ind++) = variable.at(i);
			continue;
		}
		// Last row
		if (i == Nx*Ny-1) {
			one_row.at(Nx-1) = variable.at(i);
			temp_2D.push_back(one_row);
			continue;
		}
		if ((ind > 0) && (ind < Nx)) {
			// All but the first element of any row
			one_row.at(ind++) = variable.at(i); 
		} else {
			// First element from second row on
			ind = 0;
			temp_2D.push_back(one_row);
			std::fill(one_row.begin(), one_row.end(), -1.0);
			one_row.at(ind++) = variable.at(i);
		}
	}
	// Write to file
	std::string delim{" "};
	bool single_file = true; 
	std::vector<size_t> dims = {Ny,Nx,0};
	LbmIO lbm_io(fname, delim, single_file, dims);
	lbm_io.write_vector(temp_2D);
}

// Save a 3D variable to file
void Fluid::write_var(const std::vector<double>& variable, const std::string& fname, const bool is_3D) const
{
	std::vector<double> temp(Nx*Ny, -1.0);
	for (size_t i=0; i<Ndir; ++i) {
		std::copy(variable.cbegin() + i*Nx*Ny, variable.cbegin() + (i+1)*Nx*Ny, temp.begin());
		write_var(temp, fname + "_" + std::to_string(i) + ".txt");
	}
}

void Fluid::save_state(const std::string& frho, const std::string& fux,
                        const std::string& fuy, const int step, const Geometry& geom)
{
	compute_density(); 
	write_var(rho, frho + "_" + std::to_string(step) + ".txt");
	compute_velocities(geom); 
	write_var(ux, fux + "_" + std::to_string(step) + ".txt");
	write_var(uy, fuy + "_" + std::to_string(step) + ".txt");
}



