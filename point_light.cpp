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
    look_at = look;

}
// should return true if the surface is in front of the light
// and false if it's behind and not illuminated.
//gets the vector between the surface and the light point
bool PointLight::get_direction(Vertex &surface, Vector &dir)
{
    dir = surface - position;
    dir.normalise();

    return true; //TODO is there a case when you can use the look_at var to dertmine if the surface is in front or behind?
}

void PointLight::get_intensity(Vertex &surface, Colour &level)
{
    Vector dir = surface - position;
    float radius_squared = dir.len_sqr();//TODO make sure this gets the right value, and its not crazy big
    float intensity = 0.5;//TODO play around to find how this value affects
    level =  (colour * intensity) * (1/ (4 * M_PI * radius_squared)); //colour doesnt have divsion operator, so multiply by 1 over

}

float PointLight::get_distanceToLight(Vertex &hit_position)
{
    Vector dir = hit_position - position;
    return dir.length();
}