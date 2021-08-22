import subprocess

import sys
py_path = '../../scripts/'
sys.path.insert(0, py_path)

import utils as ut
from colors import *

py_version = 'python3'

#
# Compile and run all the abm class specific tests
#

# Compile
subprocess.call([py_version + ' compilation.py'], shell=True)

# Test suite
ut.msg('Fluid class tests', CYAN)
subprocess.call(['./fl_test'], shell=True)

