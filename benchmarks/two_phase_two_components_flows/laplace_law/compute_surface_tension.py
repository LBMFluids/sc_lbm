# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for computing the surface tension
#	using Laplace law 
#
# Note - there is something off with visual
# 	inspection of values but the figures 
#	are left for overall check
#
# As is, the results are in close agreement
#	with the literature for this case:
# 		Huang, Haibo, et al. 
# 		Physical Review E 76.6 (2007): 066701
# # # # # # # # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../../../scripts/'
sys.path.insert(0, py_src_path)

import numpy as np
import visualization as vis
import matplotlib.pyplot as plt
import glob

# Case prefixes
cases = ['80', '70', '60', '50', '40', '30', '20']

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

# Collect the pressure difference and the radius
for cname in cases:
	file_list = list(glob.iglob('output/*_' + cname + '_*final.txt'))
	print(file_list)

	# Should just be two of them
	for fname in file_list:
		if 'droplet' in fname:
			rho_d = np.loadtxt(fname)
		elif 'bulk' in fname:
			rho_b = np.loadtxt(fname)
		else:
			raise ValueError(fname + ' should not be a part of this dataset')

	# Pressure
	p_tot = 1.0/3.0*(rho_b + rho_d) + G12/3.0*rho_b*rho_d 
	p_final.append(p_tot[p_in[0], p_in[1]] - p_tot[p_out[0], p_out[1]])
	
	# Radius (approximate)
	select_indices = np.where(np.logical_and(rho_d - rho_b < 0, rho_b <= rho_bulk, rho_d > rho_disp))
	Rx = (np.amax(select_indices[0]) - np.amin(select_indices[0]))/2.0	
	Ry = (np.amax(select_indices[1]) - np.amin(select_indices[1]))/2.0
	R_final.append(0.5*(Rx + Ry)) 

	# And a plot for each fluid
#	vis.plot_3D_flat(Nx, Ny, rho_b)
#	vis.plot_3D_flat(Nx, Ny, rho_d)
#	vis.plot_3D_flat(Nx, Ny, p_tot)

# Convert to numpy arrays
R_final = np.asarray(R_final)
p_final = np.asarray(p_final)

# Fit and plot
fit_params = np.polyfit(1/R_final, p_final, 1)
print('Fit results (surface tension is first):')
print(fit_params)

plt.scatter(1/R_final, p_final, c = 'blue')
plt.plot(1/R_final, fit_params[1] + fit_params[0]/R_final, c = 'black')
plt.text(.6, .94, 'Surface tension: {0:.2f}'.format(round(fit_params[0],2)), bbox={'facecolor':'w','pad':5},
         ha='right', va='top', transform=plt.gca().transAxes )
plt.xlabel('1/R')
plt.xlabel('Pressure difference')
plt.show()

print('Values of radiuses for inspection: ')
print(R_final)	
