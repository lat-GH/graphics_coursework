//
// Created by latma on 03/12/2022.
//
//
//#ifndef COURSEWORK_STARTER_POINT_LIGHT_H
//#define COURSEWORK_STARTER_POINT_LIGHT_H
//
//#endif //COURSEWORK_STARTER_POINT_LIGHT_H

#pragma once
#include "light.h"

class PointLight : public Light {
public:

    Vertex position;
    Colour colour;
    Vector look_at;

    PointLight();
    PointLight(Vertex pos, Colour col, Vector look);

    bool get_direction(Vertex &surface, Vector &dir);
    void get_intensity(Vertex &surface, Colour &intensity);
    float get_distanceToLight(Vertex &hit_position);

};
