#!/usr/bin/python2.7

# # # # # # # # # # # # # # # # # #
# Generates properties of arrays
# for testing
# # # # # # # # # # # # # # # # # #

# Name of the file with settings for testing
fname = './test_data/array_tests_input.txt'

# Object types
objects = ['ellipse', 'rectangle']
eq_objects = ['circle', 'square']

# Setup properties
set_props = [('200', '100'), ('500', '700')]

# Object properties - for all cases
obj_props = [('5', '7', '10', '15')]
eq_obj_props = [('5', '5', '10', '12')]

# Array properties
arr_props = [('5', '190', '3', '70'),('5', '490', '3', '670')]

# Distance and numbers combinations - for all cases
dist_num = [('10', '20', '0', '0'), ('0', '0', '20', '5'), ('0', '0', '10', '5')]

with open(fname, 'w') as fin:
	# Rectangles and ellipses
	for obj in objects:
		for setp, arrp in zip(set_props, arr_props):
			for objp in obj_props:
				for dn in dist_num:
					entry = ' '.join([obj, setp[0], setp[1], objp[0], objp[1], objp[2], objp[3]]) + ' '
					entry += ' '.join([arrp[0], arrp[1], arrp[2], arrp[3]]) + ' '
					entry += ' '.join([dn[0], dn[1], dn[2], dn[3]])
					entry += '\n'
					fin.write(entry)

	# Circles and squares
	for obj in eq_objects:
		for setp, arrp in zip(set_props, arr_props):
			for objp in obj_props:
				for dn in dist_num:
					entry = ' '.join([obj, setp[0], setp[1], objp[0], objp[0], objp[2], objp[3]]) + ' '
					entry += ' '.join([arrp[0], arrp[1], arrp[2], arrp[3]]) + ' '
					entry += ' '.join([dn[0], dn[1], dn[2], dn[3]])
					entry += '\n'
					fin.write(entry)
