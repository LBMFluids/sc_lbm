import subprocess

import sys
py_path = '../../scripts/'
sys.path.insert(0, py_path)

import utils as ut
from colors import *

py_version = 'python3'

#
# Compile and run all the tests from geometry category
#

# Compile
subprocess.call([py_version + ' compilation.py'], shell=True)

# Suite 1 
ut.msg('Constructors, destructors, and assignment operators', CYAN)
subprocess.call(['./r5_test'], shell=True)

# Suite 2 
ut.msg('Geometry functionality', CYAN)
subprocess.call([py_version + ' geom_test_driver.py'], shell=True)

