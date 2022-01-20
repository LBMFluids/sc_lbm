# # # # # # # # # # # # # # # # # #
# Module for testing correctness 
# of geometric properties
# # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../../scripts/'
sys.path.insert(0, py_src_path)

from utils import *
import numpy as np
import visualization as vis

# ------> This needs some comparison with expected number of nodes
class rectangle(object):
	''' Class for defining correctness of a rectangular object '''
	def __init__(self, dx, dy, x0, y0):
		self.dx = np.double(dx)
		self.dy = np.double(dy)
		self.x0 = np.double(x0)
		self.y0 = np.double(y0)
		self.name = 'rectangle'

	def tri_area(self, x1, y1, x2, y2, x3, y3):
		return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0)

	def correct(self, geom):
		# First check if built as expected
		# count solids then check all 0s in geom
		# if #solids != #0s then incorrect
		A_obj = self.dx*self.dy
		x0 = self.x0
		y0 = self.y0
		dx = self.dx
		dy = self.dy
		# Measures the sum of areas of each triangle created
		# between point ix, iy and the object edge
		# Ref: https://martin-thoma.com/how-to-check-if-a-point-is-inside-a-rectangle/
		count = 0 
		for iy, ix in np.ndindex(geom.shape):
			if np.int_(geom[iy,ix]) == 0:
				A1 = self.tri_area(x0-dx/2.0, y0-dy/2.0, x0-dx/2.0, y0+dy/2.0, ix, iy ) 
				A2 = self.tri_area(ix, iy, x0-dx/2.0, y0+dy/2.0, x0+dx/2.0, y0+dy/2.0)
				A3 = self.tri_area(ix, iy, x0+dx/2.0, y0-dy/2.0, x0+dx/2.0, y0+dy/2.0)
				A4 = self.tri_area(ix, iy, x0-dx/2.0, y0-dy/2.0, x0+dx/2.0, y0-dy/2.0)
				count += 1
				if not np.isclose((A1 + A2 + A3 + A4), A_obj):
					msg(self.name + ' - solid point outside the object area', MAGENTA)
					return False
		# Compare the node count to expected
		if (count == 0) and ((np.int_(self.dx) != 0) or (np.int_(self.dy) != 0)):
			msg('No ' + self.name + ' at all!', MAGENTA)
			return False		
		return True 

class square(rectangle):
	''' Class for defining correctness of a square object '''
	def __init__(self, ds, x0, y0):
		super(square, self).__init__(ds, ds, x0, y0)
		# Overwrite the name 
		self.name = 'square'

class ellipse(object):
	''' Class for defining correctness of an ellipse '''
	def __init__(self, dx, dy, x0, y0):
		self.dx = np.double(dx) 
		self.dy = np.double(dy)
		self.x0 = np.double(x0)
		self.y0 = np.double(y0)
		self.name = 'ellipse'
				
	def correct(self, geom):
		count = 0
		for iy, ix in np.ndindex(geom.shape):
			if np.int_(geom[iy,ix]) == 0:
				ir = (ix - self.x0)*(ix - self.x0)*self.dy*self.dy/4.0 + \
						(iy - self.y0)*(iy - self.y0)*self.dx*self.dx/4.0
				count += 1
				if ir > self.dx*self.dx*self.dy*self.dy/16.0:
					msg(self.name + ' - solid point outside the object area', MAGENTA)
					return False
		# Compare the node count to expected
		if (count == 0) and ((np.int_(self.dx) != 0) or (np.int_(self.dy) != 0)):
			msg('No ' + self.name + ' at all!', MAGENTA)
			return False
		return True

class circle(ellipse):
	''' Class for defining correctness of a circle object '''
	def __init__(self, D, x0, y0):
		super(circle, self).__init__(D, D, x0, y0)
		# Overwrite the name 
		self.name = 'circle'

class walls:
	''' Class for defining correctness of single pair of walls '''
	def __init__(self, dh, direction):
		# dh - thickness, orthogonal to direction
		# shape[0] - y, shape[1] - x
		self.dh = np.int_(dh)
		self.direction = direction
	def correct(self, geom):
		# Set the correct expected number of wall (0) nodes
		# Needed explicitly because of a separate setting 
		# for dh = whole width/lenght
		if self.direction == 'x':
			if self.dh != geom.shape[0]:
				expected_count = 2*self.dh*geom.shape[1]
			else:
				expected_count = geom.size
		if self.direction == 'y':
			if self.dh != geom.shape[1]:
				expected_count = 2*self.dh*geom.shape[0]
			else:
				expected_count = geom.size
		# Count wall nodes and check if they are within bounds
		count = 0
		for iy, ix in np.ndindex(geom.shape):
			if np.int_(geom[iy,ix]) == 0:
				if self.direction == 'x':
					if not ((iy < self.dh) or (iy > geom.shape[0]-self.dh-1)):
						msg('x wall thicker than requested', MAGENTA)
						return False
				else:
					if not ((ix < self.dh) or (ix > geom.shape[1]-self.dh-1)):
						msg('y wall thicker than requested', MAGENTA)
						return False
				count += 1
		# Compare the wall node count to expected
		if (count == 0) and (np.int_(self.dh) != 0):
			msg('No wall at all!', MAGENTA)
			return False		
		if count != expected_count:
			msg('Incomplete ' + self.direction + ' walls', MAGENTA)
			return False		
		return True

class obj_array:
	''' Class for determining correctness of an array 
			of objects '''
	def __init__(self, **kwargs):
		# --- > follow the sme convention in file nmes
		# then just extrct the dt nd make kwargs on the go
		# File with the object
		self.obj_file = kwargs['obj_file']
		# Object dimensions
		self.obj_x = kwargs['x']
		self.obj_y = kwargs['y']
		# Domain dimensions
		self.arr_x = kwargs['arr_x']
		self.arr_y = kwargs['arr_y']
		# Array bounds
		self.x0 = kwargs['x0']
		self.xf = kwargs['xf']
		self.y0 = kwargs['y0']
		self.yf = kwargs['yf']
		# Displacements and number of objects
		if kwargs['Nx'] == None:
			self.dx = kwargs['dx']
			self.dy = kwargs['dy']
			# Expected number of objects in x and y
			self.Nx = int((self.xf-self.x0+1)/(self.dx + self.obj_x)) + 1
			self.Ny = int((self.yf-self.y0+1)/(self.dy + self.obj_y)) + 1
		else:
			self.Nx = kwargs['Nx']
			self.Ny = kwargs['Ny']
			# Expected displacements 
			self.dx = int((self.xf-self.x0+1)/(self.Nx - 1)) - self.obj_x
			self.dy = int((self.yf-self.y0+1)/(self.Ny - 1)) - self.obj_y

	def correct(self, geom):
		# Load a single object that was replicated
		# and find its' bounds
		obj = np.loadtxt(self.obj_file)
		bounds = np.nonzero(obj == 0)	
		obj_x0 = np.min(bounds[1])
		obj_xf = np.max(bounds[1])
		obj_y0 = np.min(bounds[0])
		obj_yf = np.max(bounds[0])
		#
		# Recreates the array and compares
		# directly, something better in the future
		x_shift = obj_xf - obj_x0 + self.dx + 1
		y_shift = obj_yf - obj_y0 + self.dy + 1
		
		new_geom = np.ones((self.arr_y, self.arr_x))

		for iy, ix in zip(bounds[0], bounds[1]):
			for row in range(0,int(self.Nx)):
				for col in range(0,int(self.Ny)):
					xs = int(ix + row*x_shift)
					ys = int(iy + col*y_shift)
					new_geom[ys,xs] = 0 

		if np.array_equal(new_geom, geom):
			return True
		else:
			# Plot
			vis.spy_plot([new_geom, geom], 1, 2)
			msg('Object array not equal to expected', MAGENTA)
			return False

def empty(geom):
	''' Checks if geom is empty - no solid nodes '''
	if np.count_nonzero(geom) != geom.size: 
		msg('Geom not empty! Solids (0s) present', MAGENTA)
		return False
	return True


