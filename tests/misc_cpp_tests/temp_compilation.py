#!/usr/bin/python2.7

# Temporary script for compiling the tests

import subprocess, glob, os

### Input 
# Path to the main directory
path = '../../src/'
# Compiler options
cx = 'g++'
std = '-std=c++11'
opt = '-O0'
# Common source files
src_files = path + 'geometry.cpp' + ' ' + path + 'misc_checks.cpp'
gobj_files = path + 'geom_object/rectangle.cpp'
tst_files = '../common/test_utils.cpp'

# utils.h tests
# Name of the executable
exe_name = 'utils_tests'
# Files needed only for this build
spec_files = 'utils_tests.cpp'
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files, gobj_files])
subprocess.call([compile_com], shell=True)

# FileHandler.h tests
# ------ Extra condition 
# Remove the custom mode test file if exists
f_app = './test_data/custom_mode.txt'
if os.path.exists(f_app): 
	os.remove(f_app)
# Name of the executable
exe_name = 'file_hdl_tests'
# Files needed only for this build
src_files += ' ' + path + 'io_operations/FileHandler.cpp'
spec_files = 'file_handler_tests.cpp'
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files, gobj_files])
subprocess.call([compile_com], shell=True)

# lbm_io.h tests
# Name of the executable
exe_name = 'lbm_io_tests'
# Files needed only for this build
src_files += ' ' + path + 'io_operations/lbm_io.cpp'
spec_files = 'lbm_io_tests.cpp'
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files, gobj_files])
subprocess.call([compile_com], shell=True)

