//
// Created by latma on 05/12/2022.
//

#include "photon_map.h"
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;

PhotonMap::PhotonMap(Scene s){
    //scene = s;
    create_map(s);
}

void PhotonMap::create_map(Scene scene){
    Light *lights = scene.light_list;
    while (lights->next != NULL){
        for( int i=0; i<numberOfPhotons; i++){ //TODO or should you be looping based on a value determined by the light?
            Photon photon = Photon(); //TODO make sure this persists in the kdtree
            //generates a photon based on the type of light in a random direction
            lights->generate_photon(photon);
            photon_trace(photon,scene);
            add_photoToTree(photon);
        }
        lights = lights->next;
    }

}

//Photon PhotonMap::photon_trace(Photon &p){ //it not updating the colour of the photon on the unwind?
void PhotonMap::photon_trace(Photon &p, Scene &scene){
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

        //TODO add shadow_trace(); using intersection->next
        //!!!!!!!!!!!!  you need to create a new photon otherwise you are altering the orignal photon again and again
        bool absorbed = russian_roulette(p,intersection);
        if(!absorbed){
            p.direct_photon = false;
            photon_trace(p,scene);
            return;
        }
        else{
            return;
        }
    }
    //if no new intersection found, return p
    else{
        return;
    }

}
//shouldnt be affecting the colour just generating a new directiion?????
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
            //absorbed so add to photon map - dont add to photon map if specularly reflected??????
            add_photoToTree(p);

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
        add_photoToTree(p);
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
vector<Photon> PhotonMap::get_n_nearestPhotons(Photon &p, int n){
    //hoping will only need to search based on the position, and wont care on the directio or the colour
    //so the arguement photon can be empty in the sens it only has a positional value
    return kdTree.nearest(p,n);

}