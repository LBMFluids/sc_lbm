#ifndef LBM_TESTS_H
#define LBM_TESTS_H

#include <string>
#include <map>
#include "../../include/lbm.h"

/***************************************************** 
 *
 * Test utilities for the LBM class test suites
 *
 *****************************************************/

/** 
 * Run simulations with an option for full data collection
 *
 * @param gfile - file with the geometry 
 * @param rho_ini - initial density 
 * @param vol_force - volume force 
 * @param path - directory with all files and data
 * @param fname_ini - file name extension for the initial values
 * @param fname_col - file name extension for the values after the collision step
 * @param fname_force - file name extension for the values after the force addition step
 * @param fname_stream - file name extension for the values after the streaming step
 *
 **/
void run_and_collect_all(const std::string& gfile, const double rho_ini, 
			const std::vector<double>& vol_force, const std::string& path,
			const std::string& fname_ini, const std::string& fname_col,
			const std::string& fname_force, const std::string& fname_stream);

/** 
 * Run simulations with an option for collecting only initial and final data (as full)
 *
 * @param gfile - file with the geometry 
 * @param rho_ini - initial density 
 * @param max_iter - max number of iterations to perform
 * @param vol_force - volume force 
 * @param path - directory with all files and data
 * @param fname_ini - file name extension for the initial values
 * @param fname_stream - file name extension for the values after the streaming step
 *
 **/
void run_and_collect(const std::string& gfile, const double rho_ini, 
			const int max_iter, const std::vector<double>& vol_force, 
			const std::string& path, const std::string& fname_ini, 
			const std::string& fname_stream);

/** 
 * One step run and very detailed check for two-phase flows
 *
 * @param parameters - map with parameters needed for the simulation 
 * @param gfile - file with the geometry 
 * @param path - directory with all files and data
 * @param bulk_prefix - file name extension for the droplet  
 * @param droplet_prefix - file name extension for the fluid 
 * @param vol_force - volume force 
 *
 **/
void run_and_collect(std::map<std::string, double>& parameters,  
			const std::string& gfile, const std::string& path, 
			const std::string& bulk_prefix, const std::string& droplet_prefix,
			const std::vector<double>& vol_force);

/** 
 * Compute macroscopic properties, save to files along with density distributions
 *
 * @param geom - geometry object
 * @param fluid_1 - fluid which properties are being saved
 * @param fname - file name extension for the macroscopic properties
 * @param fname_f - file name extension for the the density distribution
 * @param fname_feq - file name extension for the equilibrium density distribution
 * @param path - common path to all the files
 **/
void compute_and_write_values(Geometry& geom, Fluid& fluid_1, const std::string& fname,
						const std::string& fname_f, const std::string& fname_feq, const std::string& path);

/** 
 * Compute macroscopic properties, save to files along with density distributions - wo the equilibrium distribution
 *
 * @param geom - geometry object
 * @param fluid_1 - fluid which properties are being saved
 * @param fname - file name extension for the macroscopic properties
 * @param fname_f - file name extension for the the density distribution
 * @param path - common path to all the files
 **/
void compute_and_write_values(Geometry& geom, Fluid& fluid_1, const std::string& fname,
						const std::string& fname_f, const std::string& path);


/** 
 * Compute macroscopic properties, save to files - main variables in multiphase flow
 *
 * @param geom - geometry object
 * @param fluid_1 - fluid which properties are being saved
 * @param fname - file name extension for the macroscopic properties
 * @param fname_f - file name extension for the the density distribution
 * @param fname_feq - file name extension for the equilibrium density distribution
 * @param path - common path to all the files
 **/
void compute_and_write_values_multiphase(Geometry& geom, Fluid& fluid_1, const std::string& fname,
						const std::string& fname_f, const std::string& fname_feq, 
						const std::string& path);

/**
 * Compare C++ generated file and the correct solution (from MATLAB) 
 * 
 * @param file_list - each string is one file template to check 
 * @param path - common path to all the files
 **/
bool compare_with_correct(const std::vector<std::string>&& file_list,
 							const std::string& path);

/**
 * Compare C++ generated file and the correct solution (from MATLAB) with special name for density distribution and no equilibrium density distribution 
 * 
 * @param file_list - each string is one file template to check 
 * @param path - common path to all the files
 * @param f_extension - special name extension for density distribution
 **/
bool compare_with_correct(const std::vector<std::string>&& file_list,
 							const std::string& path, const std::string& f_extension);

/**
 * Perform the actual comparisong between files  
 *
 * @param fname - each string is one file template to check 
 * @param path - common path to all the files
 * @param file_extension - ending of the file along with .txt 
 * @param prop_name - name of the property in plural to print
 **/
bool check_macroscopic(const std::string& fname, const std::string& path, 
				const std::string& file_extension, const std::string& prop_name);

/**
 * Perform the actual comparisong between files - version for distributions  
 *
 * @param fname - each string is one file template to check 
 * @param path - common path to all the files
 * @param file_extension - ending of the file along with .txt 
 * @param prop_name - name of the property in plural to print
 **/
bool check_distributions(const std::string& fname, const std::string& path, 
				const std::string& file_extension, const std::string& prop_name);

#endif
