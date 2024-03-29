# Script for compiling the benchmarks 

import subprocess, glob, os

### Input 
# Path to the main directory
path = '../../../src/'
# Path to executables 
path_exe = '../../../executables/'
# Compiler options
cx = 'g++'
std = '-std=c++11'
opt = '-O3'
other = '-Wall'

# Common source files
src_files = path + 'geometry.cpp' + ' ' + path + 'misc_checks.cpp '
src_files += path + 'geom_object/rectangle.cpp' + ' ' + path + 'geom_object/ellipse.cpp'
src_files += ' ' + path + 'fluid.cpp'
src_files += ' ' + path + 'lbm.cpp'
src_files += ' ' + path + 'io_operations/FileHandler.cpp'
src_files += ' ' + path + 'arrays/regular_array.cpp'
src_files += ' ' + path + 'utils.cpp'

# Name of the executable
exe_name = 'chdrop'
# Files needed only for this build
spec_files = 'droplet_in_a_channel.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)


