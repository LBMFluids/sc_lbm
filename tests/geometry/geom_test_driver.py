#!/usr/bin/python2.7

# # # # # # # # # # # # # # # # # #
# Driver for misc geometry tests
# # # # # # # # # # # # # # # # # #

import numpy as np
import subprocess, sys, os, glob

py_src_path = '../../python_tools/'
sys.path.insert(0, py_src_path)

from utils import * 
from geom_test import *

#
# Common part
#

# Path to files used by tests
fpath = "./test_data/"

# Colors signifying outcome of the test
# Best if the same as C++
fail_clr = " "
pass_clr = " "
# Compile the code
# Compiles all tests in geom section for now
subprocess.call("./temp_compilation.py", shell=True)

# Remove files
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

# Run executables
# For tests 1 - 3
subprocess.call("./misc_geom_tests", shell=True)

# Test 1 - read/write test
# Load and check if  both files are equal
frd = fpath + 'rw_test_geom.txt'
fwrt = fpath + 'rw_test_geom_out.txt'
geom_1 = np.loadtxt(frd)
geom_2 = np.loadtxt(fwrt)
test_pass(np.array_equal(geom_1, geom_2), 'Read/write')

# Test 2 - x walls
# Compare with expected for each file
for fxwall in x_wall_files:
	# Thickness
	dh = fxwall.split('_')[-1].split('.')[0]
	# Test object
	wall_x = walls(dh, 'x')
	geom = np.loadtxt(fxwall)
	test_pass(wall_x.correct(geom), 'x wall test, wall thickness ' + str(dh))

# Test 3 - y walls
# Compare with expected for each file
for fywall in y_wall_files:
	# Thickness
	dh = fywall.split('_')[-1].split('.')[0]
	# Test object
	wall_y = walls(dh, 'y')
	geom = np.loadtxt(fywall)
	test_pass(wall_y.correct(geom), 'y wall test, wall thickness ' + str(dh))

# Test 4 - add nodes

# Test 5 - remove nodes - do them in C++ no need for python
