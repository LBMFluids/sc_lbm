#ifndef LBM_TESTS_H
#define LBM_TESTS_H

#include <string>
#include "../../include/lbm.h"

/** Compute macroscopic properties, save to files along wiht density distributions
 * @param geom - geometry object
 * @param fluid_1 - fluid which properties are being saved
 * @param fname - file name extension for the macroscopic properties
 * @param fname_f - file name extension for the the density distribution
 * @param fname_feq - file name extension for the equilibrium density distribution
 **/
void compute_and_write_values(Geometry& geom, Fluid& fluid_1, const std::string& fname,
						const std::string& fname_f, const std::string& fname_feq);

#endif
