//
// Created by latma on 03/12/2022.
//
#include "point_light.h"

PointLight::PointLight()
{
    Light();
}

PointLight::PointLight(Vertex pos, Colour col, Vector look)
{
    Light();
    position = pos;
    colour = col;
    look.negate();// negating the look so that it is in direction from the light to the origin, not the direction of the origin to the light
    look_at = look;
    look_at.normalise();

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
    float n = 0.5;
    level = colour * (pow((dir.dot(look_at)), n));
//    float radius_squared = dir.len_sqr();
//    float intensity = 0.5;
//    level =  (colour * intensity) * (1/ (4 * M_PI * radius_squared)); //colour doesnt have divsion operator, so multiply by 1 over

}

float PointLight::get_distanceToLight(Vertex &hit_position)
{
    Vector dir = hit_position - position;
    return dir.length();
}