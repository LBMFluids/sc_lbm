#
# Sets up most of the components needed to use this software
#	The exception is the data needed for running the tests
#	and benchmarks. The user should refer to the README for 
# 	complete information. 
#

import subprocess, os
from pathlib import Path

# Make a directory to store executables (if it doesn't already exists)
exe_dir = 'executables'
if not os.path.isdir(exe_dir):
	subprocess.call(['mkdir ' + exe_dir], shell=True)

# Make directories needed for running tests if not present
# Directories not to consider
exclude_dirs = ['common', 'logger', 'raw_ptr_geometry']
os.chdir('tests/')
test_dir = 'test_data'
cur_path = Path('./')
dir_list = [f for f in cur_path.iterdir() if (f.is_dir() and (not f.name in exclude_dirs))] 

for dir_i in dir_list:
	path_i = Path(dir_i.name + '/test_data')
	if (not path_i.exists()) or	(not path_i.is_dir()):
		print('Creating ' + dir_i.name + '/' + path_i.name)
		subprocess.call(['mkdir ' + dir_i.name + '/' + path_i.name], shell=True)

# Make directories for benchmarking
os.chdir('../benchmarks/')
make_dirs = ['output', 'two_phase_two_components_flows/contact_angles/output', 
 		'two_phase_two_components_flows/flowing_droplet/output',
		'two_phase_two_components_flows/laplace_law/output',
		'two_phase_two_components_flows/phase_separation/output']

for dir_i in make_dirs:
	path_i = Path(dir_i)
	if (not path_i.exists()) or	(not path_i.is_dir()):
		print('Creating ' + path_i.name)
		subprocess.call(['mkdir ' + path_i.name], shell=True)

os.chdir('../')


