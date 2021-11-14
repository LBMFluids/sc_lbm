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
			xi = i;
			yi = j;
			// If a fluid node, fill out the value in all directions
			if (geom(xi,yi) == true) {
				for (size_t k=0; k<Ndir; ++k) {
					dir_i = k;
					yfi = dir_i*Nx*Ny + yi*Nx;
					ind_fi = yfi + i;
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
	for (size_t i=0; i<Nx*Ny; ++i) {
		for (size_t j=0; j<Ndir; ++j) {
			rho.at(i) += f_dist.at(j*Nx*Ny+i);
		}
	}				
}

// Compute macroscopic velocities
void Fluid::compute_velocities()
{
	compute_density();
	for (size_t i=0; i<Nx*Ny; ++i) {
		for (size_t j=0; j<Ndir; ++j) {
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
		if ((ind > 0) && (ind <= (Nx-1))) {
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
	std::vector<size_t> dims = {Nx,Ny,0};
	LbmIO lbm_io(fname, delim, single_file, dims);
	lbm_io.write_vector(temp_2D);
}

// Save a 3D variable to file
void Fluid::write_var(const std::vector<double>& variable, const std::string& fname, const bool is_3D) const
{
	std::vector<double> temp;
	for (size_t i=0; i<Ndir; ++i) {
		std::copy(variable.cbegin() + i*Nx*Ny, variable.cend(), temp.begin());
		write_var(temp, fname + "_" + std::to_string(i) + ".txt");
		temp.clear();	
	}
}

void Fluid::save_state(const std::string& frho, const std::string& fux,
                        const std::string& fuy, const int step)
{
	compute_density(); 
	write_var(rho, frho + "_" + std::to_string(step) + ".txt");
	compute_velocities(); 
	write_var(ux, fux + "_" + std::to_string(step) + ".txt");
	write_var(ux, fuy + "_" + std::to_string(step) + ".txt");
}











