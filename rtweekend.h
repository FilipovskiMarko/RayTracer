#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>

// C++ Strings
using std::shared_ptr;
using std::make_shared;

// Constants
inline const double infinity = std::numeric_limits<double>::infinity();
inline const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}
inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}



#endif //RTWEEKEND_H
