#!/usr/bin/python2.7

# Temporary script for compiling the tests

import subprocess, glob

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

