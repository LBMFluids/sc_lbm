import subprocess

import sys
py_path = '../scripts/'
sys.path.insert(0, py_path)

import utils as ut
from colors import *

py_version = 'python3'

# Directory with executables
path_exe = '../executables/'

#
# Compile and run all the benchmarks
#

# Compile
subprocess.call([py_version + ' compilation.py'], shell=True)

# General message
ut.msg('LBM class tests', CYAN)

# Laminar flow in x direction with walls in y 
ut.msg('Laminar flow in x direction', RED)
subprocess.call([path_exe + 'lam_x'], shell=True)

# Laminar flow in y direction with walls in x 
ut.msg('Laminar flow in y direction', RED)
subprocess.call([path_exe + 'lam_y'], shell=True)

# Flow past a step (in x) 
ut.msg('Flow past a step (wall)', RED)
subprocess.call([path_exe + 'stp'], shell=True)

