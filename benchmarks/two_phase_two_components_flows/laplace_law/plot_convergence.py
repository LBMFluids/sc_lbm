# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for computing the surface tension
#	using Laplace law as a function of time
#	from a single dataset rather than a fit
#
# Eventually the surface tension should 
#	reach a roughly constant value - this 
#	script is meant to be used for a
#	convergence study only 
#
# The data for this simulation need to
#	be generated to avoid increasing the 
#	size of the repo. To do so, run the
#	R = 30 case in the cpp file.
#
# # # # # # # # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../../../scripts/'
sys.path.insert(0, py_src_path)

import numpy as np
import visualization as vis
import matplotlib.pyplot as plt
import glob

# Path to results
path = 'output/'

# Domain dimensions
Nx = 400
Ny = 400

# Strength of fluid-fluid repulsive interactions
G12 = 0.9

# Points for pressure collection
p_in = [200, 200]
p_out = [370, 370]

# Dispersed and bulk densities
rho_bulk = 1.9
rho_disp = 0.06

# Final radiuses
R_final = []
# Final pressure differences
p_final = []
# Times
t_steps = []

# Collect the pressure difference and the radius
file_list = [x for x in glob.iglob(path + '*_30_' + '*.txt') if 'final' not in x]
file_list.sort(key=lambda x: int(x.split('_')[-1].split('.')[0]))

for ind in range(0, len(file_list)-1, 2):
	# Need both densities (assuming they are consecutive in the file list)
	fname_1 = file_list[ind]
	fname_2 = file_list[ind + 1]

	# Load the correct file for each fluid
	if 'droplet' in fname_1:
		rho_d = np.loadtxt(fname_1)
		rho_b = np.loadtxt(fname_2)
	else:
		rho_d = np.loadtxt(fname_2)
		rho_b = np.loadtxt(fname_1)

	# Pressure
	p_tot = 1.0/3.0*(rho_b + rho_d) + G12/3.0*rho_b*rho_d 
	p_final.append(p_tot[p_in[0], p_in[1]] - p_tot[p_out[0], p_out[1]])
	
	# Radius (approximate)
	select_indices = np.where((rho_d - rho_b < 0) & (rho_b <= rho_bulk) & (rho_b > rho_disp))
	Rx = (np.amax(select_indices[0]) - np.amin(select_indices[0]))/2.0	
	Ry = (np.amax(select_indices[1]) - np.amin(select_indices[1]))/2.0
	R_final.append(0.5*(Rx + Ry)) 

	# Extract time
	t_steps.append(float(fname_1.split('_')[-1].split('.')[0]))

# Convert to numpy arrays
R_final = np.asarray(R_final)
p_final = np.asarray(p_final)
t_steps = np.asarray(t_steps)

# Plot
plt.plot(t_steps, p_final*R_final, c = 'green')
plt.xlabel('t')
plt.ylabel('Surface tension')
plt.show()


