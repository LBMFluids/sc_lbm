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

# No force test suite
ut.msg('Tests without a driving force', RED)
subprocess.call([path_exe + 'lbm_tst_nof'], shell=True)

# Unidirectional force test suite
ut.msg('Tests with a unidirectional driving force', RED)
subprocess.call([path_exe + 'lbm_tst_uni'], shell=True)

# Multidirectional force test suite
ut.msg('Tests with a multidirectional driving force', RED)
subprocess.call([path_exe + 'lbm_tst_multi'], shell=True)

# Two-phase test suite - droplet immersed in a
#	continuous liquid in a semi-periodic domain with walls
ut.msg('Droplet immersed in a continuous liquid', RED)
subprocess.call([path_exe + 'lbm_tst_drop'], shell=True)

#ut.msg('Restart test', RED)
#subprocess.call([path_exe + 'lbm_rt'], shell=True)
