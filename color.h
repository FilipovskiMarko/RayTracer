//
// Created by ageor on 16-Jul-25.
//

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

//color alias for vector
using color = vec3;

inline double linear_to_gamma(double linear_component) {
    if(linear_component > 0.0) {
        return std::sqrt(linear_component);
    }
    return 0.0;
}

void write_color(std::ostream &out, const color& pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

  static const interval intensity(0,0.999);

  int ir = int(intensity.clamp(r) * 255.999);
  int ig = int(intensity.clamp(g) * 255.999);
  int ib = int(intensity.clamp(b) * 255.999);


  out << ir << " " << ig << " " << ib << "\n";
}

#endif //COLOR_H
