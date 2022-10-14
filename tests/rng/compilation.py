import subprocess, glob, os

#
# Input 
#

# Path to executables 
path_exe = '../../executables/'

# Compiler options
cx = 'g++'
std = '-std=c++11'
opt = '-O0'
tst_files = '../common/test_utils.cpp'
other = '-Wall'

# Correctness of generated random distribution 
# Name of the executable
exe_name = 'rng_test'
# Files needed only for this build
spec_files = 'rng_tests.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, tst_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)


