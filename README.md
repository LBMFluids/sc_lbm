# sc_lbm
Shan and Chen 2D lattice Boltzmann method in C++

Author: [Agnieszka Truszkowska](https://scholar.google.com/citations?user=jP83uecAAAAJ&hl=en), Ph.D., assistant professor at the University of Alabama in Huntsville

The code has full functionality to simulate single component - single phase flow as well as two-component - two-phase flows. User manual and a documentation are under way. Automatic documentation can be generated using [Doxygen](https://doxygen.nl/). `benchmarks` directory is a good starting resource.

The users are encourages to report any issues or bugs. Improvement ideas and extensions are welcome!

Contact information is available [here](https://www.uah.edu/eng/faculty-staff/agnieszka-truszkowska).

- - - 

## Important change

The data used in tests and benchmarks is currently being migrated to:

https://github.com/LBMFluids/lbm_data

This will reduce the size of this repository but it will also make the build process longer by several steps. 

The new approach will create symbolic links to the data in the data repo. The old directories will not be removed by a pull, but they will no longer be tracked by GitHub. The symlinks will be named in the same way as their directory predecessors to minimize the changes in the code. These changes will not break anything, they are introduced now to allow them to happen early in the release process. The size of the repo was a concern and a testing limitation so this was an important item to be addressed.

### Installation with new data location

1. Clone or update this repository:

  https://github.com/LBMFluids/lbm_data
 
2. Insert the path to the data repository in 1 in the ```data_path``` variable 

https://github.com/LBMFluids/sc_lbm/blob/d10122ac70f01eb519c0c64f66383a10185634b1/config_data.py#L25

The path must be absolute.

3. Run the ```confing.py``` script 

4. Run the ```data_config.py``` script


### Installation without new data location

1. Run only the ```confing.py``` script

2. If tests or benchmark data is needed manually copy the up-to-date data from 

https://github.com/LBMFluids/lbm_data

to corresponding directories in this repository. Directories are named in the same way to allow easier usage.





