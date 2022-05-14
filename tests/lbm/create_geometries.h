#ifndef CREATE_GEOMETRIES_H
#define CREATE_GEOMETRIES_H

//
// Creates and saves hardcoed geometries for all tests
//

#include <string>
#include "../../include/geometry.h"

/// No solid nodes
void make_empty();

/// A wall spanning the x direction (and growing in y) 
void make_x_walls();

/// A wall spanning the y direction (and growing in x)
void make_y_walls();

/// A staggered array no walls
void make_array_no_walls();

/// A staggered array with x walls
void make_array_x_walls();

/// A staggered array with y walls
void make_array_y_walls();

#endif
