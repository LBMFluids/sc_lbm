import subprocess

import sys
py_path = '../../../scripts/'
sys.path.insert(0, py_path)

import utils as ut
from colors import *

py_version = 'python3'

# Directory with executables
path_exe = '../../../executables/'

#
# Compile and run all the Laplace law simulations
#

# Compile
subprocess.call([py_version + ' compilation.py'], shell = True)

# Run
subprocess.call([path_exe + 'llaw'], shell = True)


