/****************************************************************************
*
* krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
*
* I've put a lot of time and effort into this code. For the last decade
* it's been used to introduce hundreds of students at multiple universities
* to raytracing. It forms the basis of your coursework but you are free
* to continue using/developing forever more. However, I ask that you don't
* share the code or your derivitive versions publicly. In order to continue
* to be used for coursework and in particular assessment it's important that
* versions containing solutions are not searchable on the web or easy to
* download.
*
* If you want to show off your programming ability, instead of releasing
* the code, consider generating an incredible image and explaining how you
* produced it.
*/

#pragma once

#include "colour.h"
#include "ray.h"
#include "object.h"
#include "light.h"
#include "hit.h"
#include "environment.h"

#include "kd-master/src/tree.h"
#include "kd-master/src/core.h"
using namespace KD;

// Scene is a class that is used to build a scene database of objects
// and lights and then trace a ray through it.

class Scene: public Environment {
public:

  Object *object_list;
  Light *light_list;

  //storing the photon mapp in the KD tree
  typedef KD::Core<3, Photon> CORE;
  Tree<CORE> kdTree = Tree<CORE>(Photon(Vector(-900,-900,-900)),Photon(Vector(900,900,900)));


  float ambient_intensity = 0.9f;
  //int numberOfPhotons = 10000;
  int numberOfPhotons = 500;

  Scene();

  // Filter the list of returned hits to the closest +ve
  Hit* select_first(Hit* list);

  // Trace a ray through the scene and find the closest if any object
  // intersection in front of the ray.
  Hit *trace(Ray ray);
	
  // Trace a ray through the scene and return its colour. This function
  // is the one that should recurse down the reflection/refraction tree within a material
  //void raytrace(Ray ray, int recurse, Colour& colour, float& depth);
  void raytrace(Ray ray, int recurse, Colour &colour, float &depth);

  // raytrace a shadow ray.
  bool shadowtrace(Ray, float limit);

  void add_object(Object *obj);
  void add_light(Light *light);

  void generateShadowRay(Ray &shadowRay, Hit *hit, Vector lit_dir);



  //------------photon map--------------
  void create_photonMap();
  void photon_trace(Photon &p, int num_bounces);
  bool russian_roulette(Photon p, Photon &newP);
  void add_photoToTree(Photon &p);
  Photon get_nearestPhotons(Photon &p);
  vector<Photon> get_n_nearestPhotons(Photon &p, int n);
};
