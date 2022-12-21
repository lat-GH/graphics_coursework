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
Colour GlobalMaterial::compute_once(Ray& viewer, Hit& hit, int recurse)
{
	Colour result;
    Colour result_reflection;
    Colour result_refraction;

    //how far to shoot the ray into the scene
    float raytrace_depth = 9999999.0;
    float snellRatio;

    if (recurse <= 0){
        return result;
    }

    //calculating the value of the snells ratio that you pass into the refract()
    if (hit.entering){
        snellRatio = ior/1;
    }
    else{snellRatio = 1/ior;}

    Vector refraction_dir;
    if(refract_ray(viewer.direction,hit.normal, snellRatio, refraction_dir))
    {
        Ray refraction_ray;
        refraction_ray.direction = refraction_dir;
        refraction_ray.direction.normalise();
        //need to move slightly ahead along the ray
        refraction_ray.position = hit.position + 0.00001 * refraction_ray.direction;
        environment->raytrace(refraction_ray, recurse-1, result_refraction, raytrace_depth);
    }

    Ray reflection_ray;
    reflection_ray.direction = reflect(viewer.direction, hit.normal);
    reflection_ray.position = hit.position + 1.000005f*reflection_ray.direction; //want to start the reflection ray just above the surface
    reflection_ray.direction.normalise();
    environment->raytrace(reflection_ray, recurse-1, result_reflection, hit.t);
//    if(reflect_weight.r == 0.0f &&  reflect_weight.g == 0.0 && reflect_weight.b == 0.0){
//        result +=  reflect_weight*result_reflection;
//        return result;
//    }

    //get the reflection ratio using the frensel term
    float fresnel_reflection;
    if(refract_weight.r == 0.0f &&  refract_weight.g == 0.0 && refract_weight.b == 0.0){
        fresnel_reflection = 1;
    }else{
        fresnel(viewer.direction, hit.normal, snellRatio, fresnel_reflection);
    }

    result +=  reflect_weight*result_reflection*fresnel_reflection + refract_weight*result_refraction*(1-fresnel_reflection);
    return result;
}


Vector reflect(Vector Incident, Vector Normal){
    return Incident - 2.0f * (Normal.dot(Incident)) * Normal; //original
    //return -Incident - 2.0f * (Normal.dot(-Incident)) * Normal;
}

//void GlobalMaterial::fresnel(Vector& viewer, Vector& Normal, float etai, float etat, float& reflectionTerm){
void GlobalMaterial::fresnel(Vector& viewer, Vector& Normal, float snellsRatio, float& fresnel_reflection){
    float cosIncident = Normal.dot(-viewer);
    float cosTransmitted_squared = 1 - (1/pow(snellsRatio,2)) * (1-pow(cosIncident,2));
    if (cosTransmitted_squared < 0){
        fresnel_reflection = 0;
        return;
    }
    float cosTransmitted = sqrtf( cosTransmitted_squared );
    float Rx = ((snellsRatio*cosIncident) - cosTransmitted) / ((snellsRatio*cosIncident) + cosTransmitted); //TODO whyy is cosIncident == cosTransmitted
    float Ry = (cosIncident - (snellsRatio*cosTransmitted)) / (cosIncident + (snellsRatio*cosTransmitted));

    fresnel_reflection = (pow(Rx,2) + pow(Ry,2)) / 2;

}


bool GlobalMaterial::refract_ray(Vector& viewer, Vector& Normal, float snellsRatio, Vector& refract_ray){
    //calculating the cos of the angle between the normal and the Incident ( I = -viewer)
    float cosIncident = Normal.dot(-viewer);
    float cosTransmitted_squared = 1 - (1/pow(snellsRatio,2)) * (1-pow(cosIncident,2));
    //dont want to sqrt a -ve number
    if(cosTransmitted_squared < 0){
        return false;
    }
    float cosTransmitted = sqrtf( cosTransmitted_squared );
    refract_ray = (1/snellsRatio) * viewer - (cosTransmitted - ((1 / snellsRatio) * cosIncident)) * Normal;
    return true;
}


Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
    Colour result;

//    result.r=0.0f;
//    result.g=0.0f;
//    result.b=0.0f;
    //------------------------not sure why... but this makes it look a lil nicer?
    result.r=0.005f;
    result.g=0.05f;
    result.b=0.05f;

    return result;
}

Colour GlobalMaterial::get_diffuseColour(){
    return Colour(0.0f, 0.0f, 0.0f);

}
Colour GlobalMaterial::get_specularColour(){
    return Colour(1.0f, 1.0f, 1.0f);

}

float  GlobalMaterial::get_diffuseReflectionProbability(const Photon &p){

    return 1- get_specularReflectionProbability(p);

}
float  GlobalMaterial::get_specularReflectionProbability(const Photon &p){
    float snellRatio;
    //calculating the value of the snells ratio that you pass into the refract()
    if (p.intersection->entering){
        snellRatio = ior/1;
    }
    else{snellRatio = 1/ior;}

    float reflection_probability;
    Vector viewer = p.direction;
    fresnel(viewer, p.intersection->normal, snellRatio, reflection_probability);
    return reflection_probability;

}

