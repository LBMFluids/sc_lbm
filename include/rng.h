#ifndef RNG_H
#define RNG_H

#include <random>

/***************************************************** 
 * class: RNG
 * 
 * Random number generator 
 * 
 *****************************************************/

class RNG
{
public:
    RNG() : gen(std::random_device()()) { } 

	/**
	 *	\brief Random number sampled from uniform distribution
	 *	@param dmin - minimum, inclusive
	 *	@param dmax - maximum, exclusive
	 */
    double get_random(const double dmin, const double dmax)
	{  
        std::uniform_real_distribution<double> dist(dmin, dmax);
        return dist(gen);
    }

private:
    std::mt19937 gen;
};

#endif
