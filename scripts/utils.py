#!/usr/bin/python2.7

# # # # # # # # # # # # # # # # # #
# Utility functions for Python code
# # # # # # # # # # # # # # # # # #

import sys, os
from colors import *

def msg(text, clr):
	''' Print text with color defined as clr and style font '''
	print(clr + text + RESET)

def test_pass(res, name):
	''' Print results of running a test '''
	if res:
		msg('  - ' +  name + ' test passed', GREEN)
	else:
		msg('  ' + name + ' test failed', MAGENTA)

def float_equality(num1, num2, tol):
	''' Compare two floating point numbers for approximate equality '''
	
	max_num = max([1.0, abs(num1), abs(num2)])
	return abs(num1 - num2) <= tol*max_num
		




