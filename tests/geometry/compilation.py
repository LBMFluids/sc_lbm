# Script for compiling the tests

import subprocess, glob, os

### Input 
# Path to the main directory
path = '../../src/'
# Compiler options
cx = 'g++'
std = '-std=c++11'
opt = '-O0'
# Common source files
src_files = path + 'geometry.cpp' + ' ' + path + 'misc_checks.cpp '
src_files += path + 'geom_object/rectangle.cpp' + ' ' + path + 'geom_object/ellipse.cpp'
src_files += ' ' + path + 'io_operations/FileHandler.cpp'
src_files += ' ' + path + 'arrays/regular_array.cpp'
src_files += ' ' + path + 'utils.cpp'
tst_files = '../common/test_utils.cpp'

# Constructors etc. test
# Name of the executable
exe_name = 'r5_test'
# Files needed only for this build
spec_files = 'r5_test.cpp geometry_tests.cpp '
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)

# Geometry objects tests
# Name of the executable
exe_name = 'add_objects_test'
# Files needed only for this build
spec_files = 'add_objects_test.cpp '
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)

# Arrays of objects tests
# Name of the executable
exe_name = 'make_arrays_test'
# Files needed only for this build
spec_files = 'make_arrays_test.cpp '
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)

# Various other geometry tests
# Name of the executable
exe_name = 'misc_geom_tests'
# Remove the custom mode test file if exists
f_rm = 'test_data/rw_test_geom_out.txt'
if os.path.exists(f_rm): 
	os.remove(f_rm)
# Files needed only for this build
spec_files = 'misc_geom_tests.cpp '
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)
