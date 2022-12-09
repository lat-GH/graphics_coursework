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

#include "sphere_object.h"

Scene::Scene()
{
	object_list = 0;
	light_list = 0;
}

void Scene::create_photonMap(){
    cout << "in create_photonMap"<< endl;
    Light *lights = light_list;
    while (lights != NULL){
        for( int i=0; i<numberOfPhotons; i++){
            Photon photon = Photon();
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
    //cout << "in photon_trace"<< endl;
    //cout << "p.intensity=" << p.intensity.r<< p.intensity.g << p.intensity.b << endl;
    //cout << "BEFORE p.direction=" << p.direction.x << p.direction.y << p.direction.z <<endl;


    // added a bias to the ray, so doesnt shoot from exact same position
    Ray incoming_ray = Ray(p.position + (1.000005f*p.direction),p.direction);
    //finds the closest thing that the photon will hit
    p.intersection = trace(incoming_ray);

    if(p.intersection != NULL){

        //updating the position of the photon to take the position of where it hits
        p.position = p.intersection->position;

        //need to create a new photon otherwise you are altering the orignal photon again and again
        Photon newPhoton = Photon();
        newPhoton.position = p.position;

        //using russain roullet to determine whether the current photon stored in the tree or not and its intensity
        //also determines the direction of the new photon

        bool absorbed = russian_roulette(p, newPhoton);

        //add a stopping case so only recurses 5 times
        if(!absorbed && num_bounces < 6){
            num_bounces ++;
            newPhoton.direct_photon = false;
            photon_trace(newPhoton, num_bounces);
            return;
        }
        else{
            //cout<< "absorbed"<<endl;
            return;
        }
    }
    //if no new intersection found, return p
    else{
        //cout<< "absorbed"<<endl;
        return;
    }

}

//decidieng the colour and whether or not to store the current photon
//then using that to determine the direction of the new photon
bool Scene::russian_roulette(Photon p, Photon &newP){
    float p_diffuseReflection = p.intersection->what->material->get_diffuseReflectionProbability(p);
    float p_specularReflection = p.intersection->what->material->get_specularReflectionProbability(p);

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    if (r )
    //diffusely reflected
    if(r < p_diffuseReflection){
        //setting the colour of the photon to use th diffuse value AND adding it to the map, because it leaves some colour behind
        p.intensity = p.intensity * p.intersection->what->material->get_diffuseColour(); //TODO this might not be the best method
        add_photoToTree(p);

        newP.intensity = p.intensity;
        //random reflection -- new direction is a random direction within a hemisphere
        newP.generate_randomSphereDirection();

        //want the direction of the new hit to move off the surface not through it
        if(newP.direction.dot(p.intersection->normal) < 0){
            newP.direction.negate();
        }
        return false;
    }

    //specularly reflected
    else if( r < p_specularReflection){
        //setting the colour of the photon to use th specular value BUT not adding it to the photon map;
        p.intensity = p.intensity * p.intersection->what->material->get_specularColour();
        newP.intensity = p.intensity;

        //specualr reflection --  use reflection equation
        //Incident - 2.0f * (Normal.dot(Incident)) * Normal;
        newP.direction = p.direction - 2.0f * (p.intersection->normal.dot(p.direction)) * p.intersection->normal;

        return false;
    }
    else{
        //photon has been absorbed
        //set the colour to be that of diffuse it has hit
        p.intensity = p.intersection->what->material->get_diffuseColour();
        add_photoToTree(p);
        return true;
    }
}

void Scene::add_photoToTree(Photon &p){
    //cout << "added to tree" << p.position.x << p.position.y << p.position.z<<  endl;
    kdTree.insert(p);

}
Photon Scene::get_nearestPhotons(Photon &p){
    return kdTree.nearest(p);
}
vector<Photon> Scene::get_n_nearestPhotons(Photon &p, int n){
    //hoping will only need to search based on the position, and wont care on the directio or the colour
    //so the arguement photon can be empty in the sens it only has a positional value
    return kdTree.nearest(p,n);

}
vector<Photon> Scene::get_radius_nearestPhotons(Photon &p, double n){ // TODO try updating this one to the one which uses the Coord radius, coord is a double
    //hoping will only need to search based on the position, and wont care on the directio or the colour
    //so the arguement photon can be empty in the sens it only has a positional value
    return kdTree.within(p,n);

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

    if(best_hit->what->get_ID() == 5){
        //cout << "Sphere" << endl;
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
      //will return you the ambient term
	  colour = colour + best_hit->what->material->compute_once(ray, *best_hit, recurse)*ambient_intensity; // this will be the global components such as ambient or reflect/refract

      //-----------photon mapping ----------------
      double radius = 0.1;
      Photon bestHit_photon = Photon(best_hit->position);

      //getting the n nearest photons to the photon at the position of the best hit
      vector<Photon> photons = get_radius_nearestPhotons(bestHit_photon, radius);

      unsigned int numPhotons = photons.size();

      Colour averageColour;
      if(numPhotons > 0){
          //the wieghting of the colour of the pixel should be based on its distamce from the middle pixel
          //when at the same position as the og photon wieght =1,  when at the distance of the radius wieght=0
          float colour_wieght;
          float radius_length = (bestHit_photon.position - photons[numPhotons-1].position).length();

          for(int i=0; i<numPhotons; i++){
              //only include photons from the object you have hit
              if(best_hit->what == photons[i].intersection->what){
                  //cout << "same object" << endl;
                  //calculates the distance from the pixel
                  float curr_radius = (bestHit_photon.position - photons[i].position).length();
                  //get the ratio of the distance away from the og photon vs the radius,
                  // then subtract it from 1 so 1 at center and 0 at edge
                  colour_wieght = 1 - curr_radius/radius_length;
                  averageColour += photons[i].intensity * colour_wieght;
              }
           }
          averageColour.r = averageColour.r/numPhotons;
          averageColour.g = averageColour.g/numPhotons;
          averageColour.b = averageColour.b/numPhotons;
      }

      colour = colour + averageColour;

      //--------------------------------------------

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

              //colour = colour + intensity * best_hit->what->material->compute_per_light(viewer, *best_hit, ldir); // this is the per light local contrib e.g. diffuse, specular

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
