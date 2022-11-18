#!/bin/bash
#SBATCH --qos=small
#SBATCH --time=60:00:00
#SBATCH --nodes=1
#SBATCH --mail-user=at0175@uah.edu

python3 run_droplet_flow.py 
