# # # # # # # # # # # # # # # # # # # # # # # # 
# Script for computing contact angles  
#	for different liquid-solid interactions
# For now contact angles are computed visually
# This script generates figures
# # # # # # # # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../../../scripts/'
sys.path.insert(0, py_src_path)

import numpy as np
import visualization as vis
import matplotlib.pyplot as plt
import glob

# Case prefixes
cases = ['-0.4', '-0.3', '-0.2', '-0.1', '0.1', '0.2', '0.3', '0.4']

# Path to results
path = 'output/'

# Domain dimensions
Nx = 400
Ny = 306

# Dispersed and bulk densities
rho_bulk = 2.0
rho_disp = 0.06

# In the following numpy indexing, [0] is y and [1] is x 
for cname in cases:
	file_list = list(glob.iglob('output/*_' + cname + '*.txt'))
	print(file_list)

	# Should just be two of them
	for fname in file_list:
		if 'droplet' in fname:
			rho_d = np.loadtxt(fname)
		elif 'bulk' in fname:
			rho_b = np.loadtxt(fname)
		else:
			raise ValueError(fname + ' should not be a part of this dataset')

	# Find the interface
	interface = np.where((rho_b - rho_d <= 0) & (rho_d <= rho_bulk) & (rho_b > rho_disp))

	# Find the minimum left point on the interface w.r.t solid
	ind_left_min = np.argmin(interface[0])
	y_left_min = interface[0][ind_left_min]
	x_left_min = interface[1][np.argwhere(interface[0] == y_left_min)]  

	# Node row above the minimum
	y_above_left_min = y_left_min + 1
	x_above_left_min = interface[1][np.argwhere(interface[0] == y_above_left_min)]

	# Base interpolation points
	# L - node closer to the origin, R - node further from the origin
	# 1 - second row from solid object, 2 - first row above solid object
	# no number - interpolated base point (halfway between solid object node and fluid node)

	xL1 = np.amin(x_above_left_min) 
	xL2 = np.amin(x_left_min)
	xR1 = np.amax(x_above_left_min) 
	xR2 = np.amax(x_left_min)

	yL1 = np.amin(y_above_left_min) 
	yL2 = np.amin(y_left_min)
	yR1 = np.amax(y_above_left_min) 
	yR2 = np.amax(y_left_min)

	yL = np.amin(interface[0]) - 0.5 
	yR = np.amin(interface[0]) - 0.5;

	if xL1 == xL2:
		xL1 = xL1 - 1
	
	if xR1 == xR2:
		xR1 = xR1 - 1

	# Linear interpolation
	slopeL = (yL1-yL2)/(xL1-xL2)
	slopeR = (yR1-yR2)/(xR1-xR2)

	interL = yL2-slopeL*xL2
	interR = yR2-slopeR*xR2

	# Base x coordinates
	xL = 1/slopeL*(yL-interL)
	xR = 1/slopeR*(yR-interR)

	# Contact angle - computation 	
	# Length of droplet base
	L = xR-xL
 	# Height of the droplet
	H = np.amax(interface[0])-np.amin(interface[0])+0.5  

	# Radius of the droplet
	R = (4*H*H+L*L)/(8*H) 

	# Contact angle
	theta_rad = np.arctan([L/(2*(R-H))])
	theta = np.degrees(theta_rad)

	if theta[0] < 0:
		theta[0] += 180
		
	print(theta[0])

	vis.plot_3D_flat(Nx, Ny, rho_d)

