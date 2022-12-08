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

 // A simple Phong based lighting model.

#include "phong_material.h"

#include <math.h>

Phong::Phong(Colour p_ambient, Colour p_diffuse, Colour p_specular, float p_power)
{
    ambient_coeff = p_ambient;
    diffuse_coeff = p_diffuse;
    specular_coeff = p_specular;
    powerOfn = p_power;

}

// The compute_once() method supplies the ambient term.
Colour Phong::compute_once(Ray& viewer, Hit& hit, int recurse)
{
	Colour result;
    result = ambient_coeff;
	return result;
}

// The compute_per_light() method supplies the diffuse and specular terms.
Colour Phong::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
	Colour result;
    Vector N = hit.normal;
    Vector L = -ldir; //ldir is direction of light, BUT L is direction towards the light?
    Vector R = ldir - 2.0f *(ldir.dot(N)) * N;
    R.normalise();

    Vector V = viewer;

    result = (diffuse_coeff * (N.dot(L))) + (specular_coeff * (pow(R.dot(V),powerOfn)));
	return result;
}

Colour  Phong::get_diffuseColour(){
    return diffuse_coeff;
}
Colour  Phong::get_specularColour(){
    return specular_coeff;

}
//TODO attempt a new method of generating the ratio between them
//Colour Phong::diffuse_specular_ratio(){
//    float r = diffuse_coeff.r/ specular_coeff.r;
//    float g = diffuse_coeff.g/ specular_coeff.g;
//    float b = diffuse_coeff.b/ specular_coeff.b;
//
//    //-------------
//    float numerator = diffuse_coeff.r + diffuse_coeff.g +diffuse_coeff.b;
//    float denominator = specular_coeff.r + specular_coeff.g +specular_coeff.b;
//
//
//}

float Phong::get_diffuseReflectionProbability(const Photon &p){
    float average = (diffuse_coeff.r + diffuse_coeff.g +diffuse_coeff.b)/3;
    return average;
}

float Phong::get_specularReflectionProbability(const Photon &p){
    float average = (specular_coeff.r + specular_coeff.g +specular_coeff.b)/3;
    return average;
}


