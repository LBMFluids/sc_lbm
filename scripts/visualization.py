# # # # # # # # # # # # # # # # # #
#
# Module with visualization tools 
#
# # # # # # # # # # # # # # # # # #

import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np

def spy_plot(data, Nr, Ncol):
	''' Plot sparsity pattern of 
			each sublist in data in a NrxNcol
			subplot composition; plots the data
			in columns, then progresses to rows i.e.
			Row 1: data 1, data 2, data 3;
    		Row 2: data 4, data 5, data 6 '''
	
	fig, axs = plt.subplots(Nr, Ncol)
	if Nr == 1 and Ncol > 1:
		# This is separated so there is no indexing
		# error when accessing axs
		for ind, subset in enumerate(data):
			ax = axs[ind]
			ax.spy(subset, markersize=5)
	elif Nr == 1 and Ncol == 1:
		# Just one plot
		axs.spy(data[0], markersize=5)
	else:
		ind = 0
		for ir in range(0, Nr):
			for ic in range(0, Ncol):
				ax = ax[ir,ic]
				ax.spy(data[ind], markersoze=5)
				ind += 1

	plt.show()

def plot_2D_with_an(x, y_num, y_an):
	''' Plot of numerical and analytical solutions '''

	plt.plot(x, y_num, 'b', label = 'Numerical solution')
	plt.plot(x, y_an, 'ro', label = 'Analytical solution')
	plt.xlabel('Channel width', fontsize = 18)
	plt.ylabel('Velocity', fontsize = 18)
	plt.legend(loc = 2)
	plt.grid()
	plt.show()

def plot_3D_flat(nrow, ncol, sol):
	''' Plot a 3D surface plot, initially shown as flat 2D surface ''' 	

	fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

	# Grid
	X = np.arange(0, nrow)
	Y = np.arange(0, ncol)
	X, Y = np.meshgrid(X, Y)

	# Plot
	surf = ax.plot_surface(X, Y, sol, cmap=cm.jet,
                       linewidth=0, antialiased=False)

	# Add a color bar which maps values to colors.
	fig.colorbar(surf, shrink=0.5, aspect=5)

	ax.view_init(90, 90)
	plt.show()

def plot_velocity_field(nrow, ncol, ux, uy, geom):
	''' Plot a velocity field as streamlines '''

	# Grid
	X = np.arange(0, nrow)
	Y = np.arange(0, ncol)
	X, Y = np.meshgrid(X, Y)

	# Velocity field components
	U = uy
	V = ux 

	# Mask - solid objects
	mask = np.logical_not(geom)
	U = np.ma.array(U, mask=mask)
	V = np.ma.array(V, mask=mask)

	# Varying color along a streamline
	fig = plt.figure(figsize=(5,5))
	ax = plt.subplot()
	strm = ax.streamplot(X, Y, U, V, linewidth=2, color='b')

	ax.imshow(~mask, alpha=0.5, cmap='gray', aspect='auto')
	ax.set_aspect('equal')
	ax.invert_yaxis()

	plt.show()
