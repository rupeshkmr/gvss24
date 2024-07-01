#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "color.h"



class Sphere {
  public:
    Sphere() {}

    Sphere(const point3& center, const double radius, const double Kd, const color& objColor) : center(center), radius(radius), Kd(Kd), objectColor(objColor) {}

    const point3& getCenter() const  { return center; }
    const double getRadius() const { return radius; }
    const double getKd() const { return Kd; }
    const color& getObjectColor() const  { return objectColor; }
    
    double hit_sphere(const ray& r) {
        vec3 oc = center - r.origin();
        double a = dot(r.direction(), r.direction());
        double b = - 2 * dot(r.direction(),oc);
        double c = dot(oc,oc) - radius * radius;
        double discriminant = b* b - 4 * a * c;
        if(discriminant>=0){
            return (-b-sqrt(discriminant)) / (2.0*a);
        }
        else
            return -1;
    }
  private:
    point3 center;
    double radius;
    color objectColor;
    double Kd;
};

#endif