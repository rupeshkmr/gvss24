#include "../src/color.h"
#include "../src/ray.h"
#include "../src/vec3.h"
#include "../src/utility_functions.hpp"
#include "../src/sphere.hpp"
#include <iostream>
#include <stdlib.h>
int spheres = 2;
int planes = 2;
int samples_per_pixel = 50; 
Sphere sphereObjects[2];
// Plane planeObjects[planes];

// vec3 random_vec3() {
//         return vec3(random_double(), random_double(), random_double());
//     }

// vec3 random_vec3(double min, double max) {
//     return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
// }

vec3 sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}



void hit_objects(const ray& r) {
    double t = 9999.0;
    double temp_t;
    int hitIdx = 0;
    bool hit = false;
    hit_record.t = -1.0;
    for(int i=0; i<spheres; i++) {
        temp_t = sphereObjects[i].hit_sphere(r);
        if(temp_t >0 && temp_t < t) {
            t = temp_t;
            hitIdx = i;
            hit = true;
        }
    }
    if(hit){
        hit_record.t = t;
        hit_record.hitIdx = hitIdx; 
    }   
}

// Coloring by outward normal

color ray_color(const ray& r) {
    vec3  unit_direction = unit_vector(r.direction());

    hit_objects(r);
    
    if(hit_record.t > 0.0) {
        vec3 N = unit_vector(r.at(hit_record.t) - sphereObjects[hit_record.hitIdx].getCenter()); // also we can use r.at(t)
        return 0.5 * color(N.x() + 1,N.y() + 1,N.z() + 1);
    }    

    float a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0- a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7,1.0);
}
color ray_color(const ray& r, int depth) {
    if(depth == 0)
        return color(0,0,0);
    vec3  unit_direction = unit_vector(r.direction());
    
    hit_objects(r);
    if(hit_record.t > 0.0) {
        vec3 N = unit_vector(r.at(hit_record.t) - sphereObjects[hit_record.hitIdx].getCenter()); // also we can use r.at(t)
        if(dot(N, r.direction()) < 0.0) {
            vec3 direction = random_on_hemisphere(N);
            ray new_ray(r.at(hit_record.t), unit_vector(direction));
            return 0.5 * ray_color(new_ray, depth-1);
        }
    }

    float a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0- a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7,1.0);
}
int main() {
    // Init hit_record
    hit_record.t = -1.0;

    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (double(image_width) / image_height);
    point3 camera_center = point3(0,0,0);

    // Compute Vu and Vv
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);
    
    // Compute du and dv
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of upper left pixel
    point3 viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u / 2 - viewport_v /2;
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Add objects
    sphereObjects[0] = Sphere(point3(0,0,-1), 0.5, 0.8, color(0.8f, 0.8f, 0.8f));
    sphereObjects[1] = Sphere(point3(0,-100.5,-1), 100.0, 0.4, color(0.4f,0.6f,0.6f));


    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            // anti aliasing
            color pixel_color(0,0,0);   
            for(int sample = 0; sample < samples_per_pixel; sample++){
                vec3 offset = sample_square();
                vec3 pixel_sample = pixel00_loc
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);
                vec3 ray_direction = pixel_sample - camera_center;
                ray r(camera_center, ray_direction);
                pixel_color += ray_color(r, 50);
                // color pixel_color = ray_color(r);
            }

            write_color(std::cout, pixel_color/(double)samples_per_pixel);
        }
    }

    std::clog << "\rDone.                 \n";
}