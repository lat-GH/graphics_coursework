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

Scene::Scene()
{
	object_list = 0;
	light_list = 0;
}

bool Scene::shadowtrace(Ray ray, float limit)
{
	Object *objects = this->object_list;

	while (objects != 0)
	{
		Hit *hit = this->select_first(objects->intersection(ray));

		if (hit != 0)
		{
          //testing if hit the same surface with the 0.0000, because it would be super close
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

Hit* Scene::select_first(Hit* list) //-----------YOU ARE HERE ----- trying to work out how this works ;')
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
              int shadowRayLimit = 1000;
              //create a shadow ray between the intersection point and the light direction
              generateShadowRay(shadowRay, best_hit, ldir);
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