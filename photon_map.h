//
// Created by latma on 05/12/2022.
//

#ifndef COURSEWORK_STARTER_PHOTON_MAP_H
#define COURSEWORK_STARTER_PHOTON_MAP_H

#include <iostream>

#include "colour.h"
#include "vector.h"
#include "vertex.h"
#include "photon.h"
#include photonMap_scene.h
#include "hit.h"
#include "light.h"
#include "kd-master/src/tree.h"
#include "kd-master/src/core.h"

using namespace std;
using namespace KD;

class PhotonMap {
public:

    int numberOfPhotons = 10000;

    typedef KD::Core<3, Photon> CORE;
    Tree<CORE> kdTree = Tree<CORE>(Photon(),Photon());

    PhotonMap(Scene s); //---------------------------------why are they not yellow, not being recognised?


    void create_map(Scene s);
    void photon_trace(Photon &p, Scene &scene);
    bool russian_roulette(Photon &p, Hit* &h);
    void add_photoToTree(Photon &p);
    Photon get_nearestPhotons(Photon &p);
    vector<Photon> get_n_nearestPhotons(Photon &p, int n);
};

#endif //COURSEWORK_STARTER_PHOTON_MAP_H
