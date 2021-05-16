# # # # # # # # # # # # # # # # # #
# Driver for misc geometry tests
# # # # # # # # # # # # # # # # # #

import numpy as np
import subprocess, sys, os, glob

py_src_path = '../../scripts/'
sys.path.insert(0, py_src_path)

from utils import * 
from geom_test import *

py_version = 'python3'

#
# Common part
#

# Path to files used by tests
fpath = "test_data/"

# Colors signifying outcome of the test
# Best if the same as C++
fail_clr = " "
pass_clr = " "
# Forms the file with array settings to be tested
subprocess.call([py_version + ' arrays_to_test.py'], shell=True)

#
# Remove files
#

# Test 2
x_wall_files = glob.glob(fpath + 'x_wall_dH_' + '*.txt')
for file_rm in x_wall_files:
	if os.path.exists(file_rm):
		os.remove(file_rm)
# Test 3
y_wall_files = glob.glob(fpath + 'y_wall_dH_' + '*.txt')
for file_rm in y_wall_files:
	if os.path.exists(file_rm):
		os.remove(file_rm)
# Test 4
rectangle_files = glob.glob(fpath + 'rectangle_' + '*.txt')
for file_rm in rectangle_files:
	if os.path.exists(file_rm):
		os.remove(file_rm)

# Test 5
square_files = glob.glob(fpath + 'square_' + '*.txt')
for file_rm in square_files:
	if os.path.exists(file_rm):
		os.remove(file_rm)

# Test 6
ellipse_files = glob.glob(fpath + 'ellipse_' + '*.txt')
for file_rm in ellipse_files:
	if os.path.exists(file_rm):
		os.remove(file_rm)

# Test 7
circle_files = glob.glob(fpath + 'circle_' + '*.txt')
for file_rm in circle_files:
	if os.path.exists(file_rm):
		os.remove(file_rm)

# Test 8
array_type_files = glob.glob(fpath + 'array_type_*')
for file_rm in array_type_files:
	if os.path.exists(file_rm):
		os.remove(file_rm)

#
# Run executables
#

# For tests 1 - 3
subprocess.call(['./misc_geom_tests'], shell=True)
# For tests 4 - 7
subprocess.call( ['./add_objects_test'], shell=True)
# For tests 8 
subprocess.call(['./make_arrays_test'], shell=True)

#
# Python tests
#

# Test 1 - read/write test
# Load and check if  both files are equal
frd = fpath + 'rw_test_geom.txt'
fwrt = fpath + 'rw_test_geom_out.txt'
geom_1 = np.loadtxt(frd)
geom_2 = np.loadtxt(fwrt)
 #test_pass(np.array_equal(geom_1, geom_2), 'Read/write')

# Test 2 - x walls
for fxwall in x_wall_files:
	# Thickness
	dh = fxwall.split('_')[-1].split('.')[0]
	# Test object
	wall_x = walls(dh, 'x')
	geom = np.loadtxt(fxwall)
	#test_pass(wall_x.correct(geom), 'x wall test, wall thickness ' + str(dh))

# Test 3 - y walls
for fywall in y_wall_files:
	# Thickness
	dh = fywall.split('_')[-1].split('.')[0]
	# Test object
	wall_y = walls(dh, 'y')
	geom = np.loadtxt(fywall)
	#test_pass(wall_y.correct(geom), 'y wall test, wall thickness ' + str(dh))

# Test 4 - rectangles
for frec in rectangle_files:
	# Rectangle properties
	props = [int(s) for s in frec.split('_') if s.isdigit()]
	props.append(frec.split('_')[-1].split('.')[0])
	# Test object
	rec = rectangle(props[0], props[1], props[2], props[3])
	geom = np.loadtxt(frec)
	#test_pass(rec.correct(geom), 'Rectangle test for ' + frec)

# Test 5 - squares 
for fsqr in square_files:
	# Square properties
	props = [int(s) for s in fsqr.split('_') if s.isdigit()]
	props.append(fsqr.split('_')[-1].split('.')[0])
	# Test object
	sqr = square(props[0], props[1], props[2])
	geom = np.loadtxt(fsqr)
	#test_pass(sqr.correct(geom), 'Square test for ' + fsqr)

# Test 6 - ellipses 
for fell in ellipse_files:
	# Square properties
	props = [int(s) for s in fell.split('_') if s.isdigit()]
	props.append(fell.split('_')[-1].split('.')[0])
	# Test object
	ell = ellipse(props[0], props[1], props[2], props[3])
	geom = np.loadtxt(fell)
	#test_pass(ell.correct(geom), 'Ellipse test for ' + fell)

# Test 7 - circle 
for fcirc in circle_files:
	# Square properties
	props = [int(s) for s in fcirc.split('_') if s.isdigit()]
	props.append(fcirc.split('_')[-1].split('.')[0])
	# Test object
	circ = circle(props[0], props[1], props[2])
	geom = np.loadtxt(fcirc)
	#test_pass(circ.correct(geom), 'Circle test for ' + fcirc)

# Test 8 - array
# Array and object properties in 
# order of appearance in the filename
arr_props = ['arr_x', 'arr_y', 'x', 'y', 'xc', 'yc']
arr_props += ['x0', 'xf', 'y0', 'yf', 'dx', 'dy', 'Nx', 'Ny']

for farr in array_type_files:

	# Create argument list 
	if '.object' in farr:
		continue
	arr_props_args = {key:np.int32(value) for (key,value) in zip(arr_props, farr.strip().split()[1:]) if key not in ['xc', 'yc']}
	arr_props_args['obj_file'] = farr + '.object'
	if arr_props_args['Nx'] == 0:
		arr_props_args['Nx'] = None

	# Make and test an array 		
	num_array = obj_array(**arr_props_args)
	geom = np.loadtxt(farr, dtype=np.int64)
	test_pass(num_array.correct(geom), 'Array test for ' + farr)
	
	





