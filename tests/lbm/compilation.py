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
src_files += ' ' + path + 'lbm.cpp'
src_files += ' ' + path + 'io_operations/FileHandler.cpp'
src_files += ' ' + path + 'arrays/regular_array.cpp'
src_files += ' ' + path + 'utils.cpp'
tst_files = '../common/test_utils.cpp' + ' lbm_tests.cpp'

## No force test suite 
# Name of the executable
exe_name = 'lbm_tst_nof'
# Files needed only for this build
spec_files = 'no_force_tests.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Unidirectional force test suite
# Name of the executable
exe_name = 'lbm_tst_uni'
# Files needed only for this build
spec_files = 'uni_force_tests.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Multidirectional force test suite
# Name of the executable
exe_name = 'lbm_tst_multi'
# Files needed only for this build
spec_files = 'multi_force_tests.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Two-phase flows 
# Name of the executable
exe_name = 'lbm_tst_drop'
# Files needed only for this build
spec_files = 'droplet_tests.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

### The following code is compiled with maximum optimizations
## Reason: these are regression tests that run for quite a bit
#opt = '-O0'
### Two-phase flow - stationary droplet 
## Name of the executable
#exe_name = 'lbm_rt'
## Files needed only for this build
#spec_files = 'restart_tests.cpp '
#compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, tst_files, src_files])
#subprocess.call([compile_com], shell=True)
#subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)


