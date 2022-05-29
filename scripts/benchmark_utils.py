# # # # # # # # # # # # # # # # # # #
# Script with utility functions used
#	by benchmark cases
# # # # # # # # # # # # # # # # # # #

import numpy as np

def channel_flow_analytical(nx, ny, nwall, dPdL, i_dir, mu):
	''' Analytical solution	for laminar flow 
			between two parallel plates in i_dir direction 

			ncol, nrow - number of columns, rows
			nwall - wall thickness 
			dPdL - pressure drop
			i_dir - a string, flow direction, either 'x' or 'y' 
			mu - dynamic viscosity
			
	'''

	# axial - along the flow
	# lateral - perpendicular to the flow
	if i_dir == 'x':
		n_axial = nx
		n_lat = ny
	else:
		n_axial = ny
		n_lat = nx

	# Half width of the fluid domain
	channel_hw = (n_lat-2.0*nwall)/2.0

	# Maximum velocity (axial component)
	u_max = dPdL*(channel_hw*channel_hw)/(2.0*mu)
	# Average velocity (axial component)
	u_av = u_max*(2.0/3.0)
	
	# Full lateral profile
	grid_lat = np.arange(-channel_hw, channel_hw)
	grid_lat += 0.5
	u = u_max*(1.0-(grid_lat/channel_hw)*(grid_lat/channel_hw))

	# Boundaries
	add_bcs = np.zeros(nwall)
	u = np.concatenate((add_bcs, u))
	u = np.concatenate((u, add_bcs))
	grid_lat = np.concatenate((add_bcs, grid_lat))
	grid_lat = np.concatenate((grid_lat, add_bcs))
	
	return grid_lat, u

# Compute velocity magnitude for the entire domain 
def vel_mag(ux, uy):
	''' Compute velocity magnitude for the entire domain '''
	return np.transpose(np.sqrt(ux*ux + uy*uy))

# Mean value of array x with 0s excluded 
def ave(x): 
	''' Return mean of array x with 0s excluded '''
	return np.mean(x.ravel()[np.flatnonzero(x)])


