#
# Configures the data needed for running the tests and benchmarks
#
# Requirement: updated version of the lbm_data repository
# 	https://github.com/LBMFluids/lbm_data	
#
# The user needs to provide the paths to the above repo on their 
#	computer, specifically, to the data_for_sc_lbm directory.
#
# This script creates symlinks with the same name as data directories
#	in the old version. If these directories are present, they are
#	first moved to a new directory called "old_#" where # stands
#	for their original name. This configuration is made optional
# 	so that the users can supply the data on their own in case this
#	is what they need.
#

import os

#
# User input
#

# Location of the data directory (absolute)
# 	--- With a / at the end
data_path = # Absolute path goes here 

#
# Configuration 
#

def link_data_dirs(data_to_link, inner_name):
	''' 
		Replace data directories named
			inner_name in each directory
			in data_to_link with a symbolic 
			link to the corresponding mirror
			directory in lbm_data.

		Moves any existing data to inner_name_old

		data_to_link have paths relative from
			the point of running this script
	'''

	# Root directory
	root_dir = os.getcwd()
	
	# Create symbolic links 
	for dir_i in data_to_link:
		print('Processing ', dir_i)
		print(os.getcwd())
		os.chdir(dir_i)
	
		# Skip if already a symlink
		if os.path.islink(inner_name):
			# Go back
			os.chdir(root_dir)
			continue
	
		# Rename if the directory exits
		if os.path.isdir(inner_name):
			print('Renaming ', inner_name)
			os.rename(inner_name, 'old_' + inner_name)
	
		# Link to correct directory in lbm_data
		print('Linking ', dir_i)
		os.symlink(data_path + dir_i + inner_name + '/', inner_name)
	
		# Go back
		os.chdir(root_dir)

# # # Tests
# Name of the data directory in each test
inner_name = 'test_data'
# All the test directories to configure
test_data_to_link = ['tests/rng/', 'tests/misc_cpp_tests/', 'tests/geometry/',
						'tests/fluid/', 'tests/lbm/']
link_data_dirs(test_data_to_link, inner_name)

# # # Benchmarks
# Name of the data directory in each benchmark directory
inner_name = 'output'
# All the benchmark directories to configure
benchmark_data_to_link = ['benchmarks/single_phase_one_component_flows/',
							'benchmarks/two_phase_two_components_flows/contact_angles/',
							'benchmarks/two_phase_two_components_flows/flowing_droplet/',
							'benchmarks/two_phase_two_components_flows/laplace_law/',
							'benchmarks/two_phase_two_components_flows/phase_separation/']
link_data_dirs(benchmark_data_to_link, inner_name)
