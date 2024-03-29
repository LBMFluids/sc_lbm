# Script for compiling the benchmarks 

import subprocess, glob, os

### Input 
# Path to the main directory
path = '../../src/'
# Path to executables 
path_exe = '../../executables/'
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

## Laminar flow in x direction 
# Name of the executable
exe_name = 'lam_x'
# Files needed only for this build
spec_files = 'laminar_flow_x_dir.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Laminar flow in y direction 
# Name of the executable
exe_name = 'lam_y'
# Files needed only for this build
spec_files = 'laminar_flow_y_dir.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Flow past a step (wall) 
# Name of the executable
exe_name = 'stp'
# Files needed only for this build
spec_files = 'flow_over_a_step.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Flow past a cylinder 
# Name of the executable
exe_name = 'fpc'
# Files needed only for this build
spec_files = 'flow_past_cylinder.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Flow past an array of ellipses in x direction
# Name of the executable
exe_name = 'xarre'
# Files needed only for this build
spec_files = 'x_flow_array_ellipses.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Flow past a staggered array of circles in x direction
# Name of the executable
exe_name = 'xarrc'
# Files needed only for this build
spec_files = 'x_flow_array_staggered_circles.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Flow past an array of squares in y direction
# Name of the executable
exe_name = 'yarrs'
# Files needed only for this build
spec_files = 'y_flow_array_squares.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Flow past a staggered array of rectangles in y direction
# Name of the executable
exe_name = 'yarrr'
# Files needed only for this build
spec_files = 'y_flow_staggered_array_rectangles.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

