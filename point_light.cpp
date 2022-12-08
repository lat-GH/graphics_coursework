//
// Created by latma on 03/12/2022.
//
#include "point_light.h"


using namespace std;

PointLight::PointLight()
{
    Light();
}

PointLight::PointLight(Vertex pos, Colour col, Vector look, float n)
{
    Light();
    position = pos;
    colour = col;
    look.negate();// negating the look so that it is in direction from the light to the origin, not the direction of the origin to the light
    look_at = look;
    look_at.normalise();
    power = n;

}
// should return true if the surface is in front of the light
// and false if it's behind and not illuminated.
//gets the vector from the light source to the surface
bool PointLight::get_direction(Vertex &surface, Vector &dir)
{
    dir = surface - position;
    dir.normalise();
    if(dir.dot(look_at) < 0){return false;} //if facing in opposite directions then the direction is facing behind the light
    return true;
}

void PointLight::get_intensity(Vertex &surface, Colour &level)
{
    Vector dir = surface - position;
    dir.normalise();
    level = colour * (pow((dir.dot(look_at)), power));
//    float radius_squared = dir.len_sqr();
//    float intensity = 0.5;
//    level =  (colour * intensity) * (1/ (4 * M_PI * radius_squared)); //colour doesnt have divsion operator, so multiply by 1 over

}

float PointLight::get_distanceToLight(Vertex &hit_position)
{
    Vector dir = hit_position - position;
    return dir.length();
}
void PointLight::generate_photon(Photon &p){
    //has the same starting position as the light source
    p.position.x = position.x;
    p.position.y = position.y;
    p.position.z = position.z;

    p.generate_randomSphereDirection();

    //TODO might want to make it such that the photons are shot out in the direction of the hemisphere instead of out in all directions
    //add a dot product and negate agaisnt the look_at


}