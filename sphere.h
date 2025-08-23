//
// Created by ageor on 17-Jul-25.
//


#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"
#include "hittable.h"
#include "vec3.h"
#include "interval.h"

class sphere : public hittable {
  private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
    public:
      sphere(const point3 & center, double radius, shared_ptr<material> mat)
        : center(center), radius(std::fmax(0,radius)), mat(mat) {}

      bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        double a = r.direction().length_squared();
        double h = dot(r.direction(), oc);
        double c = oc.length_squared() - radius * radius;

        double discriminant = h*h - a*c;
        if(discriminant < 0) return false;
        auto sqrt_d = sqrt(discriminant);

        //Finds the nearest root

       auto root = (h - sqrt_d) / a;
       if(!ray_t.surrounds(root)){
         root = (h + sqrt_d) / a;
         if(!ray_t.surrounds(root)) return false;
       }

       rec.t = root;
       rec.p = r.at(rec.t);
       vec3 outward_normal = (rec.p - center) / radius;
       rec.set_face_normal(r, outward_normal);
       rec.mat = mat;

        return true;
      }
};



#endif //SPHERE_H
