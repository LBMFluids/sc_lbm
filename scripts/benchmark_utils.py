# # # # # # # # # # # # # # # # # # #
# Script with utility functions used
#	by benchmark cases
# # # # # # # # # # # # # # # # # # #

import numpy as np

def channel_flow_analytical(nx, ny, dPdL, i_dir, mu):
	''' Analytical solution	for laminar flow 
			between two parallel plates in i_dir direction 

			ncol, nrow - number of columns, rows
			dPdL - pressure drop
			i_dir - a string, flow direction, either 'x' or 'y' 
			mu - dynamic viscosity
	
		Note: wall thickness is assumed to be 1 lu
		
	'''

	# axial - along the flow
	# lateral - perpendicular to the flow
	if i_dir == 'x':
		n_axial = nx
		n_lat = ny
	else:
		n_axial = ny
		n_lat = nx

	# Wall thickness
	nwall = 1.0

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
	u = np.insert(u, 0, 0)
	u = np.insert(u, u.size, 0)
	grid_lat = np.insert(grid_lat, 0, grid_lat[0]-1)
	grid_lat = np.insert(grid_lat, grid_lat.size, grid_lat[-1]+1)

	return grid_lat, u

def vel_mag(ux, uy):
	''' Compute velocity magnitude for the entire domain '''
	return np.transpose(np.sqrt(ux*ux + uy*uy))

def ave(x): 
	''' Return mean of array x with 0s excluded '''
	return np.mean(x.ravel()[np.flatnonzero(x)])

def compare_with_matlab(path, fname, tres):
	''' Compare sequence of results with matlab files 

		path - path to results without the /
		fname - file tag
		tres - timestamp on the macroscopic results from C++		

	'''

	mname = path + '/' + 'matlab_' + fname
	cppname = path + '/' + fname

	# Density
	if not load_and_compare(cppname, mname, 'density', tres):
		return False 
	# Velocities 
	# ux
	if not load_and_compare(cppname, mname, 'ux', tres):
		return False
	# uy
	if not load_and_compare(cppname, mname, 'uy', tres):
		return False

	# Density distribution functions
	for ri in range(0,9):
		if not load_and_compare(cppname, mname, 'f_dist_' + str(ri)):
			return False

	return True

def load_and_compare(cppname, mname, var, tres = ''):
	''' Load data from cppname and mname that store values 
			of variable var and compare them for equality;
			optionally transpose the loaded cpp matrix '''

	if not tres:
		cppsol = np.loadtxt(cppname + '_' + var + '.txt')
	else:
		cppsol = np.loadtxt(cppname + '_' + var + '_' + tres + '.txt')
	msol = np.loadtxt(mname + '_' + var + '.txt')

	if not np.allclose(msol, cppsol, 1e-5, 1e-4):
		print('Cpp', var, 'not equal to matlab one') 
		return False 
	return True
