#ifndef PLANE_H
#define PLANE_H

#include "vec3.h"
#include "color.h"



class Plane {
  public:
    Plane() {}

    Plane(const point3& center, const vec3& Normal, const double Kd, const color& objColor) : center(center), Normal(Normal), Kd(Kd), objectColor(objColor) {}

    const point3& getCenter() const  { return center; }
    const vec3& getNormal() const { return Normal; }
    const double getKd() const { return Kd; }
    const color& getObjectColor() const  { return objectColor; }
    
    double hit_plane(const ray& r) {
        double t;
        t = dot(Normal, center - r.origin()) / (dot(Normal, r.direction()));
        if(t>0){
            return t;
        }
        else
            return -1;
    }
  private:
    point3 center;
    vec3 Normal;
    color objectColor;
    double Kd;
};

#endif