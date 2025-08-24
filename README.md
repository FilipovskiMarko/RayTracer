This is a Ray Tracer I built following the book Ray Tracing in one Weekend by Peter Shirley, Trevor David Black, Steve Hollasch. It helped me learn the fundamentals of ray tracing.

Currently this ray tracer has 3 materials: lambertian(just scatters light), metal(reflects) and dyelectric(refracts). It has a hittable list class where all the simulated objects are stored, currently it only supports adding spheres.
There is a camera class that supports defocus blur(depth of field), adjusting VFOV, resolution, camera positioning, samples per pixel(how many rays are sent out for each pixels color), max depth (maximum times a ray can bounce off a surface) and aspect ratio;

Currently it writes the image data into "image.ppm" file.

If you want to tinker around with the main and create your own ray traced images, you need to create your own hittable list object and add whatever you like to it, in the main file the list is titled world,
using world.add(make_shared<sphere>()); you can add your own spheres to the world. A sphere is created like this: make_shared<sphere>(point3(X,Y,Z) //center coordinates, radius, material).
A material is created like this: make_shared<material type>(color(R,G,B), //if metal you need to add a fuzz value after color, if dielectric you only need to input a refraction index);

With this you can add your own objects to the world, to generate the image you need to create a camera object and render the image. I reccomend using 16.0/9.0 aspect ratio, a lower resolution (<400 image width), 10 samples per pixel (default), and 10 max depth, so that your image is generated in a few seconds and not in 24 hours like the one below XD;



[skyView.tif](https://github.com/user-attachments/files/21956627/skyView.tif)


<img width="1920" height="1080" alt="skyView" src="https://github.com/user-attachments/assets/be6f686f-8f05-41cf-9f48-002fa4ac22a3" />


[1080pTopView.tif](https://github.com/user-attachments/files/21956636/1080pTopView.tif)


<img width="1920" height="1080" alt="1080pTopView" src="https://github.com/user-attachments/assets/66ec050a-8551-45a5-84a6-aad8daa5d05b" />
