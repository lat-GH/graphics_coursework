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

#include "global_material.h"

#include <math.h>

Vector reflect(Vector incident, Vector Normal);

GlobalMaterial::GlobalMaterial(Environment* p_env, Colour p_reflect_weight, Colour p_refract_weight, float p_ior)
{
    reflect_weight = p_reflect_weight;
    refract_weight = p_refract_weight;
    ior = p_ior;
    environment = p_env;
}

//BEGIN_STAGE_TWO
//END_STAGE_TWO
//
//// reflection and recursion computation
//Colour GlobalMaterial::compute_once(Ray& viewer, Hit& hit, int recurse)
//{
//	Colour result;
//
//    Colour col;
//    Hit hit;
//
//    if (recurse == 0){
//        return col;
//    }
//    if (environment.raytrace()){
//
//    }
//
//    Ray reflection_ray;
//    reflection_ray.direction = reflect(ray.direction, hit.normal)
//
//
//
//
//	return result;
//}
//
//Colour reflection_raytrace(Ray &ray, int depth){
//
//
//}
//
//Vector reflect(Vector Incident, Vector Normal){
//    return Incident - 2.0f * (Normal.dot(Incident)) * Normal;
//}


Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
    Colour result;

    result.r=0.0f;
    result.g=0.0f;
    result.b=0.0f;

    return result;
}

