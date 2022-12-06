//
// Created by latma on 05/12/2022.
//

#include "photon_map.h"
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;

PhotonMap::PhotonMap(Scene s){
    scene = s;
    create_map(scene);
}

void PhotonMap::create_map(Scene scene){
    Light *lights = scene.light_list;
    while (lights->next != NULL){
        for( int i=0; i<numberOfPhotons; i++){ //TODO or should you be looping based on a value determined by the light?
            Photon photon = Photon(); //TODO make sure this persists in the kdtree
            //generates a photon based on the type of light in a random direction
            lights->generate_photon(photon);
            photon = photon_trace(photon);
            add_photoToTree(photon);
        }
        lights = lights->next;
    }

}

Photon PhotonMap::photon_trace(Photon &p){
    Ray incoming_ray = Ray(p.position,p.direction);
    //finds the closest thing that the photon will hit
    Hit* intersection = scene.trace(incoming_ray); //TODO do you need to add a bias to avoid colliding with the same surface over again?

    if(intersection != NULL){
        //calculating the colour based on the location of the hit, in a given material? multiply it with the given colour value of the phot
        //ASSUMPTION you dont want to recurse to find the colour??? //TODO find out how reflective surfaces work in photonmapping
        p.intensity = p.intensity * intersection->what->material->compute_once(incoming_ray, *intersection, 1); //----not sure should have the * ?????

        //updating the position of the photon to take the position of where it hits
        p.position = intersection->position;
        // you need to store the photon at each intersection
        add_photoToTree(p);

        //TODO photon_trace(); ??????

        bool absorbed = russian_roulette(p,intersection);
        if(!absorbed){
            p.intensity += photon_trace(p).intensity;
            return p;
        }
        else{return p;}
    }
    //if no new intersection found, return p
    else{return p;}

}
//?????????????this method shouldnt be altering the colour of the photon????????just generating a new direction????????
bool PhotonMap::russian_roulette(Photon &p, Hit* &h){
//    float p_diffuseReflection = get_diffuseProbability(h->what->material);
//    float p_specularReflection = get_specularProbability(h->what->material);
//    10%absorbed 40%diffuse 50%specualr
    float p_absorbed = 0.1;

    default_random_engine gen;
    uniform_real_distribution<float> distribution(0.0,1.0);
    float r = distribution(gen);

//  if(r<p_diffuseReflection)
    if(r<p_absorbed){//10% absorbed
        if(r<0.5){//40% diffuse
            //random reflection -- new direction is a random direction within a hemisphere
            default_random_engine gen02;
            uniform_real_distribution<float> distribution(-1.0,1.0);
            p.direction.x = distribution(gen02);
            p.direction.y = distribution(gen02);
            p.direction.z = distribution(gen02);

            //want the direction of the new hit to move off the surface not through it
            if(p.direction.dot(h->normal) < 0){
                p.direction.negate();
            }
            return false;
        }
        else{//50% specular
            //specualr reflection --  use reflection equation
            //Incident - 2.0f * (Normal.dot(Incident)) * Normal;
            p.direction = p.direction - 2.0f * (h->normal.dot(p.direction)) * h->normal;

            return false;
        }

    }else{
        //photon has been absorbed
        return true;
    }

}
void PhotonMap::add_photoToTree(Photon &p){
    kdTree.insert(p);

}
Photon PhotonMap::get_nearestPhotons(Photon &p){
    return kdTree.nearest(p);
}