# Remove files from tracking so they don't get deleted in the 
# local repositories

# First run git status > all_files.log
# The run this code in the directory where all_files.log resides

import subprocess

file_list = 'all_files.log' 

with open(file_list, 'r') as fin:
	for line in fin:
		if ('deleted' in line) and ('.txt' in line):
			# Assumes all files with .txt extension 
			# to be deleted should be untracked
			temp = line.strip().split()
			# Last entry is the path to the file
			subprocess.call(['git update-index --skip-worktree ' + temp[-1]], shell=True)
			
			
