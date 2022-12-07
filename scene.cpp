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

#include "scene.h"

#include <bits/stdc++.h>

Scene::Scene()
{
	object_list = 0;
	light_list = 0;
}

void Scene::create_photonMap(){
    Light *lights = light_list;
    while (lights->next != NULL){
        for( int i=0; i<numberOfPhotons; i++){ //TODO or should you be looping based on a value determined by the light?
            Photon photon = Photon(); //TODO make sure this persists in the kdtree
            //generates a photon based on the type of light in a random direction
            lights->generate_photon(photon);
            int bounces = 0;
            photon_trace(photon,bounces);
        }
        lights = lights->next;
    }
}

//Photon PhotonMap::photon_trace(Photon &p){ //it not updating the colour of the photon on the unwind?
void Scene::photon_trace(Photon &p, int num_bounces){
    Ray incoming_ray = Ray(p.position,p.direction);
    //finds the closest thing that the photon will hit
    p.intersection = trace(incoming_ray); //?????? do you need to add a bias to avoid colliding with the same surface over again?

    if(p.intersection != NULL){
        //calculating the colour based on the location of the hit, in a given material? multiply it with the given colour value of the photon
        p.intensity = p.intensity * p.intersection->what->material->compute_once(incoming_ray, *p.intersection, 1); //----not sure should have the * ?????

        //updating the position of the photon to take the position of where it hits
        p.position = p.intersection->position;
        // you need to store the photon at each intersection
        add_photoToTree(p);

        //TODO add shadow_trace(); using intersection->next
        //need to create a new photon otherwise you are altering the orignal photon again and again
        Photon newPhoton = Photon();
        bool absorbed = russian_roulette(p, newPhoton);

        //add a stopping case so only recurses 5times
        if(!absorbed && num_bounces < 6){
            num_bounces ++;
            newPhoton.direct_photon = false;
            photon_trace(newPhoton, num_bounces);
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
bool Scene::russian_roulette(Photon p, Photon &newP){
    float p_diffuseReflection = p.intersection->what->material->get_diffuseReflectionProbability(p);
    float p_specularReflection = p.intersection->what->material->get_specularReflectionProbability(p);

    //TODO - test that both probabilites add up to 1

    //is the probability that it gets absorbed - the same for all materials
    float p_absorbed = 0.1;

    default_random_engine gen;
    uniform_real_distribution<float> distribution(0.0,1.0);
    float r = distribution(gen);

    if(r < p_absorbed){//10% absorbed
        if(r < p_diffuseReflection){

            //random reflection -- new direction is a random direction within a hemisphere
            default_random_engine gen02;
            uniform_real_distribution<float> distribution(-1.0,1.0);
            newP.direction.x = distribution(gen02);
            newP.direction.y = distribution(gen02);
            newP.direction.z = distribution(gen02);

            //want the direction of the new hit to move off the surface not through it
            if(newP.direction.dot(p.intersection->normal) < 0){
                newP.direction.negate();
            }
            return false;
        }
        else{
            //specualr reflection --  use reflection equation
            //Incident - 2.0f * (Normal.dot(Incident)) * Normal;
            newP.direction = newP.direction - 2.0f * (p.intersection->normal.dot(newP.direction)) * p.intersection->normal;

            return false;
        }

    }else{
        //photon has been absorbed
        //and the absorbed photon has already been added to the kd tree, outisde of russian roulette
        return true;
    }

}

void Scene::add_photoToTree(Photon &p){
    kdTree.insert(p);

}
Photon Scene::get_nearestPhotons(Photon &p){
    return kdTree.nearest(p);
}
vector<Photon> Scene::get_n_nearestPhotons(Photon &p, int n){ // TODO try updating this one to the one which uses the Coord radius, coord is a double
    //hoping will only need to search based on the position, and wont care on the directio or the colour
    //so the arguement photon can be empty in the sens it only has a positional value
    return kdTree.nearest(p,n);

}

bool Scene::shadowtrace(Ray ray, float limit)
{
	Object *objects = this->object_list;

	while (objects != 0)
	{
		Hit *hit = this->select_first(objects->intersection(ray));

		if (hit != 0)
		{
          //testing if hit the same surface with the 0.0000, because it would be super close,
          //and dont want it to go past the position of the light
		  if ((hit->t > 0.00000001f) &&( hit->t < limit))
		    {
			  delete hit;
		      return true;
		    }
		  delete hit;
		}

		objects = objects->next;
	}

	return false;
}

//works out the hit on the object that is closest to the camera = best_hit
Hit *Scene::trace(Ray ray)
{
	Hit *best_hit = 0;

	Object *objects = this->object_list;

	while (objects != 0)
	{
		Hit *hit = this->select_first(objects->intersection(ray));

		if (hit != 0)
		{
			if (best_hit == 0)
			{
				best_hit = hit;

			}//if the new hit found is a closer distance from the camera than the best hit
            else if (hit->t < best_hit->t)
			{
				delete best_hit;
				best_hit = hit;
			}
			else
			{
				delete hit;
			}
		}

		objects = objects->next;
	}

	return best_hit;
}

Hit* Scene::select_first(Hit* list)
{
	Hit* result = 0;

	while (list != 0)
	{
		if (list->t >= 0.0f)
		{
			result = list;
			list = list->next;
			break;
		}

		Hit* temp = list;
		list = list->next;
		delete temp;
	}

	while (list != 0)
	{
		Hit* temp = list;
		list = list->next;
		delete temp;
	}

	return result;
}

void Scene::generateShadowRay(Ray &shadowRay, Hit *hit, Vector lit_dir){
    //need to move it up along the normal by a small amount so that it doesnt inersect again
    float bias = 1.000005f;
    Vector shadow_bias = Vector(bias, bias, bias);
    //multiplying the position by the direction towards the light, i guess that means it would lift it off the surface a little?
    shadowRay.position = hit->position + (-lit_dir * shadow_bias);

    //when you negate it get the shadow ray going out towards the light source
    shadowRay.direction = -lit_dir;

}

void Scene::raytrace(Ray ray, int recurse, Colour &colour, float &depth)
{
  Object *objects = object_list;
  Light *lights = light_list;
	
  // a default colour if we hit nothing.
  colour.r = 0.0f;
  colour.g = 0.0f;
  colour.b = 0.0f;
  colour.a = 0.0f;
  depth = 0.0f;

  // first step, find the closest primitive
  Hit *best_hit = this->trace(ray);

  // if we found a primitive then compute the colour we should see
  if (best_hit != 0)
  {
	  depth = best_hit->t;
      //working out the colour for the best hit
	  colour = colour + best_hit->what->material->compute_once(ray, *best_hit, recurse)*ambient_intensity; // this will be the global components such as ambient or reflect/refract

      //-------photon mapping--------

      int photon_sample = 10;
      Photon bestHit_photon = Photon(best_hit->position);
      //getting the n nearest photons to the photon at the position of the best hit
      vector<Photon> photons = get_n_nearestPhotons(bestHit_photon, photon_sample); //TODO - check that this method can find the photon you are looking for

      unsigned int numPhotons = photons.size();
      if (numPhotons > 0){
          Colour colour_average = Colour();
          //looping through all the nearest photons found, and averaging thier colour results to calculate the colour of the photon at the position og tht hit
          for(int i=0; i<numPhotons; i++){
              colour_average += photons[i].intensity;
          }
          colour_average = colour_average* (1/numPhotons);

          colour += colour_average;
      }



      //--------------------------------

	  // next, compute the light contribution for each light in the scene.
	  Light* light = light_list;
	  while (light != (Light*)0)
	  {
		  Vector viewer;
		  Vector ldir;

          //why is the viewer the negative to the position?
          //because want vector going towards the eye, from the intersection (not dir from eye to intersect)
		  //viewer = -best_hit->position;
          viewer = ray.position - best_hit->position;
		  viewer.normalise();

		  bool lit;
          //assigns the direction of the light to ldir
		  lit = light->get_direction(best_hit->position, ldir);

		  if (ldir.dot(best_hit->normal) > 0)
		  {
			  lit = false;//light is facing wrong way.
		  }

          // Put the shadow check here, if lit==true and in shadow, set lit=false
          if(lit){
              bool inShadow;
              Ray shadowRay;
              //create a shadow ray between the intersection point and the light direction
              generateShadowRay(shadowRay, best_hit, ldir);
              //get the max distance that it should trace the shadow ray till
              int shadowRayLimit = light->get_distanceToLight(best_hit->position);
              //int shadowRayLimit = 1000;
              //trace the shadow ray to see if it hits an object casting a shadow
              inShadow = shadowtrace(shadowRay, shadowRayLimit);
              //if shadow ray intersects with an object then the pixel is not lit
              if (inShadow){
                  lit = false;
              }

          }

		  if (lit)
		  {
			  Colour intensity;
			  
			  light->get_intensity(best_hit->position, intensity);
			  
			  colour = colour + intensity * best_hit->what->material->compute_per_light(viewer, *best_hit, ldir); // this is the per light local contrib e.g. diffuse, specular

		  }

		  light = light->next;
	  }

	  delete best_hit;
  } else
	{
		colour.r = 0.0f;
		colour.g = 0.0f;
		colour.b = 0.0f;
		colour.a = 1.0f;
	}
}

void Scene::add_object(Object *obj)
{
  obj->next = this->object_list;
  this->object_list = obj;
}

void Scene::add_light(Light *light)
{
  light->next = this->light_list;
  this->light_list = light;
}