#!/usr/bin/python2.7

# # # # # # # # # # # # # # # # # #
# Utility functions for Python code
# # # # # # # # # # # # # # # # # #

import sys
from colors import *

def msg(text, clr):
	''' Print text with color defined as clr and style font '''
	sys.stdout.write(clr)
	print text
	sys.stdout.write(RESET)

def test_pass(res, name):
	''' Print results of running a test '''
	if res:
		msg('  - ' +  name + ' test passed', GREEN)
	else:
		msg('  ' + name + ' test failed', MAGENTA)

