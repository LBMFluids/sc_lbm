#ifndef GEOMETRY_TESTS_H
#define GEOMETRY_TESTS_H

#include "../../include/geometry.h"
#include "../common/test_utils.h"

/*************************************************************** 
 * Common functions for testing Geometry class 
 ***************************************************************/

/** \brief Verifies basic Geometry object properties
 * 	\details Checks if dimensions in x and y are as specified
 * 		and if all entries are 0
 *  @param geom - Geometry object to check
 *  @param Nx - correct size in x
 *  @param Ny - correct size in y
 */
bool check_geom_obj(const Geometry&, const size_t, const size_t);



#endif
