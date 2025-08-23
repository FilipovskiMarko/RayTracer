#include <chrono>
#include <fstream>
#include "hittable.h"
#include "sphere.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "interval.h"
#include "camera.h"

int main() {
    //World
    hittable_list world;
    auto material_ground = make_shared<lambertian>(color(0.65,0.65,0.0));
    world.add(make_shared<sphere>(point3(0,-1000,-1),1000,material_ground)); //Ground

    //Random spheres

    for (int i = -11; i < 11; i++) {
        for (int j = -11; j < 11; j++) {
            auto choose_mat = random_double();
            point3 center(i + 0.9 * random_double(), 0.2, j + 0.9 * random_double());

            //if the center is far enough from 4 in the x scale, the sphere is created
            if ((center - point3(4,0.2,0)).length() > 0.9) {
                shared_ptr<material> sphere_mat;

                if (choose_mat < 0.7) {
                    auto albedo = color::random() * color::random();
                    sphere_mat = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_mat));
                } else if (choose_mat < 0.9) {
                    auto albedo = color::random() * color::random();
                    auto fuzz = random_double(0, 0.5);
                    sphere_mat = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_mat));
                } else {
                    sphere_mat = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center,0.2, sphere_mat));
                }
            }
        }
    }

    auto glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(-4,1,0), 1, glass));

    auto diffuse = make_shared<lambertian>(color(29.0/255, 168.0/255, 147.0/255));
    world.add(make_shared<sphere>(point3(0, 1, 0), 1, diffuse));

    auto metalic = make_shared<metal>(color(29.0/255, 168.0/255, 147.0/255), 0.25);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1, metalic));



    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1920;
    cam.samples_per_pixel = 500.0;
    cam.max_depth = 50;


    cam.vfov = 75;
    cam.lookfrom = point3(0,5.5,0);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,0,1);

    cam.defocus_angle = 0.6;
    cam.focus_distance = 3;

    cam.render(world);

}
