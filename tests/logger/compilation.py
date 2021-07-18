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
src_files = path + 'logger.cpp'
src_files += ' ' + path + 'io_operations/FileHandler.cpp'
tst_files = '../common/test_utils.cpp'

### Test 
# Name of the executable
exe_name = 'log_tests'
# Files needed only for this build
spec_files = 'logger_tests.cpp'
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)


