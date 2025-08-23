
#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "hittable.h"

class material {
    public:
    virtual ~material() = default;

    virtual bool scatter(const ray& in, const hit_record& rec, color& atenuation, ray& scattered) const {
        return false;
    }
};

class lambertian : public material {
private:
    color albedo;

    public:
    lambertian(const color& a) : albedo(a) {}

    bool scatter(const ray& in, const hit_record& rec, color& atenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if(scatter_direction.near_zero()) {
            scatter_direction = rec.normal; //catch unwanted scatter direction
        }
        scattered = ray(rec.p, scatter_direction);
        atenuation = albedo;
        return true;
    }
};

class metal : public material {
private:
    color albedo;
    double fuzz;
public:
    metal(const color& a, double fuzz) : albedo(a), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& in, const hit_record& rec, color& atenuation, ray& scattered) const override {
        auto reflected = reflect(in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (random_unit_vector() * fuzz);
        scattered = ray(rec.p, reflected);
        atenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

class dielectric : public material {
private:
    double refraction_index; // The ratio of the materials refractive index over the index of the surrounding area

    static double reflectance(double cosine, double refraction_index) { //I Don't understand how this works, but it doesn't matter
        //Schlick's approximation
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }

public:

    dielectric(double r) : refraction_index(r) {}

    bool scatter(const ray & in, const hit_record& rec, color& attenuation, ray& scattered) const override {

        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;
        vec3 unit_direction = unit_vector(in.direction());

        auto cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0);
        auto sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = (sin_theta * ri > 1.0);
        vec3 direction;

        if(cannot_refract || (reflectance(cos_theta, ri) > random_double())) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, ri);
        }

        scattered = ray(rec.p, direction);
        return true;
    }

};

#endif //MATERIAL_H
