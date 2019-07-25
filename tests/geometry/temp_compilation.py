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

# Geometry objects tests
# Name of the executable
exe_name = 'add_objects_test'
# Files needed only for this build
spec_files = 'add_objects_test.cpp' + path + '/geom_objects/rectangle.cpp' 
compile_com = cx + std + opt + '-o' + exe_name + src_files + spec_files

