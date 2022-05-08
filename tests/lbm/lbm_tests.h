#ifndef LBM_TESTS_H
#define LBM_TESTS_H

#include <string>
#include "../../include/lbm.h"

/** 
 * Compute macroscopic properties, save to files along wiht density distributions
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
 * Compare C++ generated file and the correct solution (from MATLAB) 
 * 
 * @param file_list - each string is one file template to check 
 * @param path - common path to all the files
 **/
bool compare_with_correct(const std::vector<std::string>&& file_list,
 							const std::string& path);

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
