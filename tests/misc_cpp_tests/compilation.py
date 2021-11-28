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
# Common source files
src_files = path + 'misc_checks.cpp'
src_files += ' ' + path + 'io_operations/FileHandler.cpp'
tst_files = '../common/test_utils.cpp'

### Test suite 1
# utils.h tests
# Name of the executable
exe_name = 'utils_tests'
# Files needed only for this build
spec_files = 'utils_tests.cpp'
# Print containers
verbose = '-DVERBOSE'
# or overwrite and don't
verbose = '-UVERBOSE'
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files, verbose])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

### Test suite 2
# FileHandler.h tests
# ------ Extra condition 
# Remove the custom mode test file if exists
f_app = './test_data/custom_mode.txt'
if os.path.exists(f_app): 
	os.remove(f_app)
# Name of the executable
exe_name = 'file_hdl_tests'
# Files needed only for this build
spec_files = 'file_handler_tests.cpp'
compile_com = ' '.join([cx, std, opt, '-o', exe_name, spec_files, tst_files, src_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)

### Test suite 3
# lbm_io.h tests
# Remove these files if exist
f_path = './test_data/'
files_rm = ['wr_bool.txt', 'wr_int.txt', 'wr_string.txt', 'wr_double.txt']
files_rm = [f_path + x for x in files_rm]
files_rm.append('./dflt_lbm_io_file.txt')
for frm in files_rm:
	if os.path.exists(frm): 
		os.remove(frm)
# Name of the executable
exe_name = 'lbm_io_tests'
# Files needed only for this build
spec_files = 'lbm_io_tests.cpp'
compile_com = ' '.join([cx, std, opt, '-o', exe_name, tst_files, src_files, spec_files])
subprocess.call([compile_com], shell=True)
subprocess.call(['mv ' + exe_name + ' ' + path_exe], shell=True)
