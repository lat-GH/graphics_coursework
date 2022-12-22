//
// Created by latma on 05/12/2022.
//
//
#ifndef COURSEWORK_STARTER_PHOTON_H
#define COURSEWORK_STARTER_PHOTON_H
//

//#pragma once
#include <iostream>

#include "colour.h"
#include "vector.h"
#include "vertex.h"
#include "hit.h"

using namespace std;

class Photon {
public:
    Colour intensity; // is this the same as colour?
    Vector direction;
    Vertex position;
    Hit* intersection;
    bool direct_photon = true;
    bool shadow_photon = false;

    //constructors
    Photon();
    Photon(Vertex pos);
    Photon(Colour col);
    Photon(Colour col, Vector dir, Vertex pos);

    //will be populated into the kd tree based on it's Vertex position
    //kd tree needs to be able to index it's position
    float operator[] (int i) const;
    bool operator == (Photon p) const;

    void generate_randomSphereDirection();

};

#endif //COURSEWORK_STARTER_PHOTON_H