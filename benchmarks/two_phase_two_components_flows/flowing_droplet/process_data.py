# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for post-processing flowing droplet 
# # # # # # # # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../../../scripts/'
sys.path.insert(0, py_src_path)

import numpy as np
import visualization as vis
import matplotlib.pyplot as plt
import benchmark_utils as bu
import glob

# Case prefixes
dPdLs = ['_0.00000005_', '_0.0000001_', '_0.0000003_', '_0.0000004_']
Gs = ['-0.294', '-0.11']
cases = [x + y for x in dPdLs for y in Gs]

# Path to results
path = 'output/'

# Domain dimensions
Nx = 1272
Ny = 318

# Conversions
# How many cm is in one lu
lu2cm = 4.83e-5 
# How many seconds are in one ts
# This conversion can be found here
# https://doi.org/10.1016/j.compchemeng.2016.08.016
ts2s = 1000/6/1e-3*(0.483e-6)**2

# Dispersed and bulk densities
rho_bulk = 2.0
rho_disp = 0.06

# In the following numpy indexing, [0] is y and [1] is x 
for cname in cases:
	file_list = list(glob.iglob('output/*' + cname + '*.txt'))

	print(cname)
	print(file_list)

	# Should just be two of them
	for fname in file_list:
		if 'density' in fname:
			if ('droplet' in fname) and not ('bulk' in fname):
				rho_d = np.loadtxt(fname)
			elif 'bulk' in fname:
				rho_b = np.loadtxt(fname)
			else:
				raise ValueError(fname + ' should not be a part of this dataset')
		if not ('density' in fname):
			if not ('bulk' in fname):
				if ('ux' in fname): 
					ux_d = np.loadtxt(fname)
				if ('uy' in fname): 
					uy_d = np.loadtxt(fname)

	# Find the droplet 
	droplet_interior = np.where(rho_d >= rho_bulk)
	#print('Number of droplet interior nodes detected: ' + str(len(droplet_interior[0])))

	# Compute average velocity magnitude of the droplet
	u_d_ave = np.mean(bu.vel_mag(ux_d[droplet_interior], uy_d[droplet_interior])); 
	print('Average droplet velocity magnitude [lu/ts]: ' + str(u_d_ave))
	print('Average droplet velocity magnitude [cm/s]: ' + str(u_d_ave*lu2cm/ts2s))

	vis.plot_3D_flat(Nx, Ny, rho_d)

