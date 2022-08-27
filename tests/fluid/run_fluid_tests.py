import subprocess

import sys
py_path = '../../scripts/'
sys.path.insert(0, py_path)

import utils as ut
from colors import *

py_version = 'python3'

# Directory with executables
path_exe = '../../executables/'

#
# Compile and run all the Fluid class specific tests
#

# Compile
subprocess.call([py_version + ' compilation.py'], shell=True)

# SCSP 
ut.msg('Fluid class tests', CYAN)
subprocess.call([path_exe + 'fl_test'], shell=True)

# MCMP
ut.msg('Fluid class tests - two fluids', CYAN)
subprocess.call([path_exe + 'mcmp_fl_test'], shell=True)

