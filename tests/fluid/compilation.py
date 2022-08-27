# Script for compiling the tests

import subprocess, glob, os

### Input 
# Path to the main directory
path = '../../src/'
# Path to executables 
path_exe = '../../executables/'
# Compiler options
cx = 'g++'
std = '-std=c++11'
opt = '-O0'
other = '-Wall'
# Common source files
src_files = path + 'geometry.cpp' + ' ' + path + 'misc_checks.cpp '
src_files += path + 'geom_object/rectangle.cpp' + ' ' + path + 'geom_object/ellipse.cpp'
src_files += ' ' + path + 'fluid.cpp'
src_files += ' ' + path + 'io_operations/FileHandler.cpp'
src_files += ' ' + path + 'arrays/regular_array.cpp'
src_files += ' ' + path + 'utils.cpp'
tst_files = '../common/test_utils.cpp'

## Test suite for single component - single phase 
# Name of the executable
exe_name = 'fl_test'
# Files needed only for this build
spec_files = 'fluid_test_main.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Test suite for two component - two phase 
# Name of the executable
exe_name = 'mcmp_fl_test'
# Files needed only for this build
spec_files = 'two_fluids_test_main.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)


