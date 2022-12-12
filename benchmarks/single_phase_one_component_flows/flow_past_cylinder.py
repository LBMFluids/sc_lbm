# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for comparison and visualization
#	of results for laminar flow in x direction
#	in a 2D channel with cylinder
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


# ------- Choose a simulation

# Creeping flow
#fname_1 = 'cylinder_slow_results'
# Stable vortex formation
#fname_1 = 'cylinder_medium_results'
# Von Karman vortex street 
fname_1 = 'von_karman_vortex_street'

# Other settings
gfile = 'domain_with_cylinder.txt'
res_time = '30000'

# Dimensions
Nx = 600
Ny = 300

#
# Load the data for post-processing and visualization
#

cppname = path + '/' + fname_1
ux = np.loadtxt(cppname + '_ux_30000.txt')
uy = np.loadtxt(cppname + '_uy_30000.txt')
geom = np.loadtxt(path + '/' + gfile) 

#
# Comparison - matlab files
#

if not bu.compare_with_matlab(path, fname_1, res_time):
	print('Cpp and MATLAB solutions do not match')

#
# Visualization 
#

# Surface plot
vis.plot_3D_flat(Nx, Ny, uy) 

# Streamlines
vis.plot_velocity_field(Nx, Ny, ux, uy, geom)
