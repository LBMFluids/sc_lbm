import subprocess, os

import sys
py_path = '../scripts/'
sys.path.insert(0, py_path)

import utils as ut
from colors import *

py_version = 'python3'

#
# Compile and run all the tests 
#

# Bar delimiting the test suites
nSim = 10

print('\n'*2)

# Testing utilities
ut.msg('- '*nSim + 'TESTING UTILS TESTS' + ' -'*nSim, REVERSE+RED)
os.chdir('common/')
subprocess.call(['make && ./tu_test'], shell=True)
os.chdir('../')

# Misc
print('\n'*2)
ut.msg('- '*nSim + 'MISCELANEOUS TESTS' + ' -'*nSim, REVERSE+RED)
os.chdir('misc_cpp_tests/')
subprocess.call([py_version + ' run_misc_tests.py'], shell=True)
os.chdir('../')

# Geometry class
print('\n'*2)
ut.msg('- '*nSim + 'GEOMETRY CLASS TESTS' + ' -'*nSim, REVERSE+RED)
os.chdir('geometry/')
subprocess.call([py_version + ' run_geometry_tests.py'], shell=True)
os.chdir('../')


