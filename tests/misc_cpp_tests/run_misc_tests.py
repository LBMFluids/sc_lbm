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
# Compile and run all the tests from misc cpp category
#

# Compile
subprocess.call([py_version + ' compilation.py'], shell=True)

# Utilities 
ut.msg('Utility functions in utils.h', CYAN)
subprocess.call([path_exe + 'utils_tests'], shell=True)

# FileHandler class
ut.msg('FileHandler class', CYAN)
subprocess.call([path_exe + 'file_hdl_tests'], shell=True)

# LbmIO class
ut.msg('LbmIO class', CYAN)
subprocess.call([path_exe + 'lbm_io_tests'], shell=True)
