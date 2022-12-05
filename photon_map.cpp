//
// Created by latma on 05/12/2022.
//

#include "photon_map.h"

PhotonMap::PhotonMap(Scene s){
    scene = s;
    create_map(scene);
}

void PhotonMap::create_map(Scene scene){
    Light *lights = scene.light_list;
    while (lights->next != NULL){
        Photon photon = Photon();
        //generates a photon based on the type of light in a random direction
        lights->generate_photon(photon);
        photon = photon_trace(photon);
        add_photoToTree(photon);

        lights = lights->next;
    }

}

Photon PhotonMap::photon_trace(Photon &p){

}
bool PhotonMap::russian_roulette(Photon &p, Hit &h){

}
void PhotonMap::add_photoToTree(Photon &p){

}
Photon* PhotonMap::get_nearestPhotons(Photon &p){

}