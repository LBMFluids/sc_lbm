# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for analysis and visualization
#	of phase separation simulations
# # # # # # # # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../../../scripts/'
sys.path.insert(0, py_src_path)

import numpy as np
import visualization as vis
import glob

# Case prefixes
cases = ['small', 'medium', 'high']

# Path to results
path = 'output/'

# Domain dimensions
Nx = 200
Ny = 200

# Check density values in every final file 
# Expected minimum value for each fluid is about 0.06,
# maximum is about 2.0, and the largest sum should not be more 
# than about 2.0-2.1. The smallest sum is now ~1.4 which seems fine

# Load each file using numpy, then check
for cname in cases:
	file_list = list(glob.iglob('output/' + cname + '*final.txt'))
	print(file_list)
	# Should just be two of them
	rho_1 = np.loadtxt(file_list[0])
	print(np.amin(rho_1))
	print(np.amax(rho_1))
	rho_2 = np.loadtxt(file_list[1])
	print(np.amin(rho_2))
	print(np.amax(rho_2))
	# Now the sum
	print(np.amin(rho_1 + rho_2))
	print(np.amax(rho_1 + rho_2))
	
	# And a plot for each fluid
	vis.plot_3D_flat(Nx, Ny, rho_1)
	vis.plot_3D_flat(Nx, Ny, rho_2)

# Now visualize each case
# Just for fluid 2 - change if needed 
for cname in cases:
	file_list = [x for x in glob.iglob(path + cname + '*fluid_2*.txt') if 'final' not in x]
	file_list.sort(key=lambda x: int(x.split('_')[-1].split('.')[0]))

	# Surface plots
	for fname in file_list:
		print(fname)
		rho = np.loadtxt(fname)
		vis.plot_3D_flat(Nx, Ny, rho) 


