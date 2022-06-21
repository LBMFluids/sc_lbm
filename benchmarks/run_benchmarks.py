import subprocess

import sys
py_path = '../scripts/'
sys.path.insert(0, py_path)

import utils as ut
from colors import *

py_version = 'python3'

# Directory with executables
path_exe = '../executables/'

#
# Compile and run all the benchmarks
#

# Compile
subprocess.call([py_version + ' compilation.py'], shell=True)

# General message
ut.msg('Benchmarks for single phase flow', CYAN)

## Laminar flow in x direction with walls in y 
#ut.msg('Laminar flow in x direction', RED)
#subprocess.call([path_exe + 'lam_x'], shell=True)
#
## Laminar flow in y direction with walls in x 
#ut.msg('Laminar flow in y direction', RED)
#subprocess.call([path_exe + 'lam_y'], shell=True)

# Flow past a step (in x) 
#ut.msg('Flow past a step (wall)', RED)
#subprocess.call([path_exe + 'stp'], shell=True)

# Flow past a cylinder (in x) - low Re
#dPdL = 1e-6
#fname = 'cylinder_slow_results'
#ut.msg('Flow past a cylinder - low Re', RED)
#subprocess.call([path_exe + 'fpc ' + str(dPdL) + ' ' + fname], shell=True)
#
## Flow past a cylinder (in x) - medium Re
#dPdL = 1e-5
#fname = 'cylinder_medium_results'
#ut.msg('Flow past a cylinder - medium Re', RED)
#subprocess.call([path_exe + 'fpc ' + str(dPdL) + ' ' + fname], shell=True)
#
## Flow past a cylinder (in x) - medium Re
#dPdL = '5e-5'
#fname = 'von_karman_vortex_street'
#ut.msg('Flow past a cylinder - medium Re', RED)
#subprocess.call([path_exe + 'fpc ' + str(dPdL) + ' ' + fname], shell=True)

## Flow past an array of ellipses in x direction
#ut.msg('Flow in x direction past an array of ellipses', RED)
#subprocess.call([path_exe + 'xarre'], shell=True)

## Flow past a staggered array of circles in x direction
#ut.msg('Flow in x direction past a staggered array of circles', RED)
#subprocess.call([path_exe + 'xarrc'], shell=True)

## Flow past an array of squares in y direction
#ut.msg('Flow in y direction past an array of squares', RED)
#subprocess.call([path_exe + 'yarrs'], shell=True)

## Flow past a staggered array of rectangles in y direction
ut.msg('Flow in y direction past staggered array of rectangles', RED)
subprocess.call([path_exe + 'yarrr'], shell=True)


