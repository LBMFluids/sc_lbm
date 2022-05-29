# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for comparison and visualization
#	of results for laminar flow in y direction
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

axial_vel_file = 'output/x_walls_results_uy_3000.txt'
Nx = 50 
Ny = 10
Hw = 3
dPdL = 1e-5/6
idir = 'y'
mu = 1.0/6.0 

#
# Comparison - analytical solution
#

num_sol = np.loadtxt(axial_vel_file)
grid, an_sol = bu.channel_flow_analytical(Nx, Ny, Hw, dPdL, idir, mu)

print(num_sol[0])
print(an_sol)

for ui in num_sol:
	if not np.allclose(an_sol, ui):
		print('Numerical solution not equal to analytical')

#
# Comparison - matlab files
#

#
# Visualization 
#
