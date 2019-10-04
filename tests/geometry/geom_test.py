# # # # # # # # # # # # # # # # # #
# Module for testing correctness 
# of geometric properties
# # # # # # # # # # # # # # # # # #

import sys
py_src_path = '../../python_tools/'
sys.path.insert(0, py_src_path)

from utils import *
import numpy as np

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

class ellipse:
	''' Class for defining correctness of an elliptical object '''
	def __init__(self, dx, dy, x0, y0):
		self.dx = dx	
		self.dy = dy
		self.x0 = x0
		self.y0 = y0
				
	def correct(self, geom):
		for ix, iy in np.ndindex(geom.shape):
			if geom[ix,iy] == 0:
				ir = (ix - self.x0)*(ix - self.x0)*self.dy*self.dy + \
				        (iy - self.y0)*(iy - self.y0)*self.dx*self.dx
				if ir > self.dx*self.dx*self.dy*self.dy:
					msg('Ellipse - solid point outside the object area', MAGENTA)
					return False
		return True

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
	# Array of objects has to be represented as 0 points
	# instead of whole objects (object correctness is checked elsewhere)
	def __init__(self, a, b, x0, xf, y0, yf, dx, dy):
		self.a = a
		self.b = b
		self.x0 = x0
		self.xf = xf
		self.y0 = y0
		self.yf = yf
		self.dx = dx
		self.dy = dy
		# Expected number of objects in x and y
		self.Nx = (xf-x0-a-dx)/(a+dx) + 2;
		self.Ny = (yf-y0-b-dy)/(b+dy) + 2;

	def correct(self, geom):
		if (geom.size - np.count_nonzero(geom)) != self.Nx*self.Ny: 
			msg('Wrong number of objects present', MAGENTA)			
			return False
		yi = self.y0
		for ig in range(0, self.Ny):
			xi = self.x0
			for jg in range(0, self.Nx):
				if geom[yi,xi] != 0:
					msg('Object center at a wrong place', MAGENTA)			
					return False
				xi += self.a + self.dx
			yi += self.b + self.dy
		return True

class stag_array(obj_array):
	''' Class for determining correctness of an array 
			of objects '''
	# Array of objects has to be represented as 0 points
	# instead of whole objects (object correctness is checked elsewhere)
	def __init__(self, a, b, x0, xf, y0, yf, dx, dy, alpha):
		obj_array.__init__(self, a, b, x0, xf, y0, yf, dx, dy)
		self.alpha = alpha
		# Expected number and initial position of objects in x and y with staggering
		self.alpha_rad = alpha*np.pi/180.0;
		self.beta_rad = (180.0-2.0*alpha)*np.pi/180.0;
		self.dx_alpha = np.sin(self.alpha_rad)/np.sin(self.beta_rad)*(dx + a);
		self.dxs = int((dx+a)/2)
		self.dys = int(np.sqrt(self.dx_alpha*self.dx_alpha - self.dxs*self.dxs));
		self.xs0 = x0 + self.dxs
		self.xsf = xf - self.dxs
		self.Ny = int(yf-y0-b-(self.dys-b))/(b+(self.dys-b)) + 2;
		self.Nstg = int((self.xsf-self.xs0-a-dx)/(a+dx)) + 2

	def correct(self, geom):
		count = 0
		yi = self.y0
		for ig in range(0, self.Ny):
			xi = self.xs0 if ig%2 else self.x0
			for jg in range(0, self.Nstg if ig%2 else self.Nx):
				count += 1
				if geom[yi,xi] != 0:
					msg('Object center at a wrong place', MAGENTA)			
					return False
				xi += self.a + self.dx
			yi += self.dys
		if (geom.size - np.count_nonzero(geom)) != count:
			msg('Wrong number of objects present', MAGENTA)			
			return False
		return True

def empty(geom):
	''' Checks if geom is empty - no solid nodes '''
	if np.count_nonzero(geom) != geom.size: 
		msg('Geom not empty! Solids (0s) present', MAGENTA)
		return False
	return True


