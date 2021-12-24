# # # # # # # # # # # # # # # # # #
#
# Module with visualization tools 
#
# # # # # # # # # # # # # # # # # #

import matplotlib.pyplot as plt
import numpy as np

def spy_plot(data, Nr, Ncol):
	''' Plot sparsity pattern of 
			each sublist in data in a NrxNcol
			subplot composition; plots the data
			in columns, then progresses to rows i.e.
			Row 1: data 1, data 2, data 3;
    		Row 2: data 4, data 5, data 6 '''
	
	fig, axs = plt.subplots(Nr, Ncol)
	if Nr == 1:
		# This is separated so there is no indexing
		# error when accessing axs
		for ind, subset in enumerate(data):
			ax = axs[ind]
			ax.spy(subset, markersize=5)	
	else:
		ind = 0
		for ir in range(0, Nr):
			for ic in range(0, Ncol):
				ax = ax[ir,ic]
				ax.spy(data[ind], markersoze=5)
				ind += 1

	plt.show()


