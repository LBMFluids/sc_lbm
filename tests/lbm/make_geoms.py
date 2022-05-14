import subprocess, glob, os

import sys
py_path = '../../scripts/'
sys.path.insert(0, py_path)

# Directory with executables
path_exe = '../../executables/'

#
# Compile and run the code that creates the geometries 
#

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
src_files += ' ' + path + 'io_operations/FileHandler.cpp'
src_files += ' ' + path + 'arrays/regular_array.cpp'
src_files += ' ' + path + 'utils.cpp'

## Compile 
# Name of the executable
exe_name = 'mk_geoms'
# Files needed only for this build
spec_files = 'create_geometries.cpp '
compile_com = ' '.join([cx, std, opt, other, '-o', exe_name, spec_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

## Run 
subprocess.call([path_exe + exe_name], shell=True)
