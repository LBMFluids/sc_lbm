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
# Compile and run all the LBM class specific tests
#

# Compile
subprocess.call([py_version + ' compilation.py'], shell=True)

# General message
ut.msg('LBM class tests', CYAN)

# Test suite
subprocess.call([path_exe + 'lbm_tst_nof'], shell=True)

