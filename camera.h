#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include <fstream>
#include "material.h"

#include "color.h"

class camera {
public:
    int image_width = 256;
    double aspect_ratio = 16.0 / 9.0;
    int samples_per_pixel = 10;
    int max_depth = 10; //Number of ray bounces before returning no color

    double vfov = 90; //Vertical view angle, field of view
    point3 lookfrom = point3(0,0,0); //The point where the camera is placed
    point3 lookat = point3(0,0,-1); //The point where it will be facing
    vec3 vup = vec3(0,1,0); // The direction that will be directly upwards to the camera

    double defocus_angle = 0.0; //
    double focus_distance = 10.0; //Distance from lookfrom to point of perfect focus

    void render(const hittable &world) {
        initialize();

        std::ofstream image_file("image.ppm");
        image_file<<"P3\n"<<image_width<<" "<<image_height<<"\n255\n";

        for(int j=0;j<image_height;j++) {
            std::clog<<"\rScanlines Remaining: "<<image_height - j<<" "<<std::flush;
            for(int i=0;i<image_width;i++){

                color pixel_color = color(0, 0, 0);
                for(int sample=0;sample<samples_per_pixel;sample++) {
                    ray r = get_ray(i,j);
                    pixel_color += ray_color(r,max_depth, world);
                }
                write_color(image_file,pixel_samples_scale * pixel_color);
            }
        }

    }


private:
    int image_height; // rendered image height
    point3 center; // location of camera
    point3 pixel00_loc; // location of pixel 0,0
    vec3 pixel_delta_u; // distance between pixels horizontaly to the right
    vec3 pixel_delta_v; // distance between pixels vertically down
    double pixel_samples_scale; // color scale factor for sum of pixels samples

    vec3 u,v,w; //Camera frame basis vectors;

    vec3 defocus_disk_u,defocus_disk_v; // Defocus disk horizontal and vertical radius


    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        //Determine viewport dimension
        //auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h * focus_distance;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        center = lookfrom;
        pixel_samples_scale = 1.0 / samples_per_pixel;

        w = unit_vector(lookfrom - lookat); // vector facing backwards to the camera lookat direction
        u = unit_vector(cross(vup,w)); // Right
        v = cross(w,u); // Up


        // Calculate the horizontal and vertical vectors of the viewport
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * (-v);

        // Horizontal and Vertical delta pixels
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        //Upper left corner and pixel
        auto viewport_upper_left = center - (focus_distance * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u+pixel_delta_v);

        //Calculate camera defocus disks basis vectors
        auto defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle/2));
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;
    }

    color ray_color(const ray& r,int depth, const hittable& world) const {
        if(depth <= 0) {
            return color(0, 0, 0);
        }
        hit_record rec;

        if(world.hit(r, interval(0.001,infinity), rec)) {
            //vec3 direction = rec.normal + random_unit_vector(); //True Lambertian Reflection
            //vec3 direction = random_on_hemisphere(rec.normal); //Random Reflection
            ray scattered;
            color atenuation;
            if(rec.mat->scatter(r,rec,atenuation,scattered)) {
                return atenuation * ray_color(scattered, depth - 1, world);
            }
            return color (0,0,0);
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);

        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i+offset.x())*pixel_delta_u) + ((j+offset.y())*pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }
    vec3 sample_square() const {
        return vec3(random_double()-0.5, random_double()-0.5, 0);
    }
    point3 defocus_disk_sample() const {
        //Returns a random point in the camera defocus disk
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
};


#endif //CAMERA_H
