# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for comparison and visualization
#	of results for laminar flow in x direction
#	in a 2D channel with a rectangular step
# # # # # # # # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../../scripts/'
sys.path.insert(0, py_src_path)

import utils as ut
import benchmark_utils as bu
import numpy as np
import visualization as vis

#
# Input
#

path = 'output'
fname = 'step_results'
gfile = 'domain_with_step.txt'
res_time = '30000'

# Dimensions
Nx = 200
Ny = 100

#
# Load the data for post-processing and visualization
#

cppname = path + '/' + fname
ux = np.loadtxt(cppname + '_ux_30000.txt')
uy = np.loadtxt(cppname + '_uy_30000.txt')
geom = np.loadtxt(path + '/' + gfile) 

#
# Comparison - matlab files
#

if not bu.compare_with_matlab(path, fname, res_time):
	print('Cpp and MATLAB solutions do not match')

#
# Visualization 
#

# Surface plot
vis.plot_3D_flat(Nx, Ny, uy) 

# Streamlines
vis.plot_velocity_field(Nx, Ny, ux, uy, geom)
