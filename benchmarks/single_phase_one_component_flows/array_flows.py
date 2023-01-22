# # # # # # # # # # # # # # # # # # # # # # # # #
# Script for comparison and visualization
#	of results for laminar flow ini a 2D channel 
# 	with an array of objects (x and y direction)
# # # # # # # # # # # # # # # # # # # # # # # # #

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

# ---> Result files
fname = 'array_of_ellipses_x_walls_results'
#fname = 'staggered_array_of_circles_x_walls_results'
#fname = 'array_of_squares_y_walls_results'
#fname = 'staggered_array_of_rectangles_y_walls_results'

# ---> File with geometry
gfile = 'array_of_ellipses_x_walls.txt'
#gfile = 'staggered_array_of_circles_x_walls.txt'
#gfile = 'array_of_squares_y_walls.txt'
#gfile = 'staggered_array_of_rectangles_y_walls.txt'

# ---> Other settings
res_time = '30000'

# ---> Dimensions
# For x flows
Nx = 200
Ny = 100
# For y flows
#Nx = 100
#Ny = 200

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
