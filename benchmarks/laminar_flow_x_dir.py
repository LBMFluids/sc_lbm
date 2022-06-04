# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for comparison and visualization
#	of results for laminar flow in x direction
#	in a 2D channel
# # # # # # # # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../scripts/'
sys.path.insert(0, py_src_path)

import utils as ut
import benchmark_utils as bu
import numpy as np
import visualization as vis

#
# Input
#

path = 'output'
fname = 'y_walls_results'
res_time = '30000'
axial_vel_file = path + '/' + fname + '_ux_' + res_time + '.txt'
Nx = 10 
Ny = 50
Hw = 1
dPdL = 1e-4
idir = 'x'
rho = 2.0
mu = 1.0/6.0*rho 

#
# Comparison - analytical solution
#

num_sol = np.transpose(np.loadtxt(axial_vel_file))
grid, an_sol = bu.channel_flow_analytical(Nx, Ny, dPdL, idir, mu)

for ui in num_sol:
	if not np.allclose(an_sol, ui, 1e-5, 1e-4):
		print('Numerical solution not equal to analytical')

#
# Comparison - matlab files
#

if not bu.compare_with_matlab(path, fname, res_time):
	print('Cpp and MATLAB solutions do not match')

#
# Visualization 
#

# 2D comparison with the analytical solution
vis.plot_2D_with_an(grid, num_sol[0], an_sol)

# Surface plot
vis.plot_3D_flat(Ny, Nx, num_sol) 
