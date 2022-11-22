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
    //cout<< "recurse"<<recurse << endl;
    if (recurse == 0){
        //cout << "end of recurse" << endl;
        return result;
    }

    //get the reflection ratio using the frensel term
    float reflectionTerm;
    fresnel(viewer.direction, hit.normal, 1, ior, reflectionTerm);
    //cout << "reflectionTerm= " <<reflectionTerm<< endl;
    //bias used to determine the distance from the surface to start the ray
    Vector bias = 1.000005f *  hit.normal ;

    //checking for total internal reflection??? ------------- WHY do I need to do this inside and outside of the refract_ray?
    if ( reflectionTerm < 1){
        Vector refraction_dir;
        //cout<<"reflectionTerm < 1"<<endl;

        if(refract_ray(viewer.direction,hit.normal, ior, refraction_dir)){
            //cout << "refraction_dir=" << refraction_dir.x << refraction_dir.y << refraction_dir.z << endl;
            Ray refraction_ray;
            refraction_ray.direction = refraction_dir;
            refraction_ray.direction.normalise();

            //cout << "refraction_dir=" << refraction_dir.x << refraction_dir.y << refraction_dir.z << endl;
            //cout << "entering = "<< hit.entering  << "hit.normal.dot(refraction_dir) " << hit.normal.dot(refraction_dir)<< endl; the dot of normal and refraction ray is always -ve, menaing they are always facing opposite directions as they should be?

            if(hit.entering){
                //want the new ray inside the material to start below the normal
                refraction_ray.position = hit.position - bias;
            }
            else{
                refraction_ray.position = hit.position + bias;
            }

            environment->raytrace(refraction_ray, recurse-1, result_refraction, hit.t);
        }
    }


    Ray reflection_ray;
    reflection_ray.direction = reflect(viewer.direction, hit.normal);
    reflection_ray.direction.normalise();
    reflection_ray.position = hit.position + 1.000005f*reflection_ray.direction; //want to start the reflection ray just above the surface

    environment->raytrace(reflection_ray, recurse-1, result_reflection, hit.t);
    //float kr = 0.8f;
    //result += result * kr;
    //result +=  result_reflection*reflectionTerm;
    result +=  result_reflection*reflectionTerm + result_refraction*(1-reflectionTerm);


	return result;
}


Vector reflect(Vector Incident, Vector Normal){
    return Incident - 2.0f * (Normal.dot(Incident)) * Normal;
}

bool going_in(float IdotN, float ior){
    if(IdotN > 0){ //going out
        //ior/1 = ior
        //return ior;
        return false;
    }
    else{//going in
        //return 1/ior;
        return true;
    }
}

void GlobalMaterial::fresnel(Vector& Incident, Vector& Normal, float etai, float etat, float& reflectionTerm){
    float cosIncident = Incident.dot(Normal);
    //clamping the value of cosIncident to the range of -1 - 1
    if (cosIncident > 1){cosIncident = 1;}
    else if(cosIncident < -1){cosIncident = -1;}

    float refracIndx_I;
    float refracIndx_T;
    //will flip the direction of the ratio based on the value of the cosIncident
    //if(going_in(cosIncident, etai)){
    if(cosIncident < 0){ //going into the material
        refracIndx_I = etai;//1.0f;
        refracIndx_T = etat;//ior;
    }
    else{
        //coming out of the material
        refracIndx_I = etat;//ior;
        refracIndx_T = etai; //1.0f;
    }
    float snellsRatio = refracIndx_I/refracIndx_T;
    //cout << "etai" << etai << "etat" << etat <<endl;
    //cout <<"refracIndx_I "<<refracIndx_I << "refracIndx_T "<< refracIndx_T << "snellsRatio<"<< snellsRatio<< endl;

    //checking for total interal reflection
    float sinTransmitted = snellsRatio * sqrtf(max(0.f, 1 - cosIncident*cosIncident)); //-----check this maths is correct
    //cout << "cosIncident = "<< cosIncident << "^^2" << cosIncident*cosIncident << "1-" << 1 - cosIncident*cosIncident<< endl;
    //cout << "sinTransmitted = "<< sinTransmitted<<endl;
    if (sinTransmitted >= 1){
        reflectionTerm = 1;
        //return 1;
    }
    else{
        float cosTransmitted = sqrtf(max(0.f, 1 - sinTransmitted*sinTransmitted));
        cosIncident = fabsf(cosIncident);
        float wavelenght_S = ((refracIndx_T*cosIncident) - (refracIndx_I*cosTransmitted) / (refracIndx_T*cosIncident) + (refracIndx_I*cosTransmitted));
        float wavelenght_P = ((refracIndx_I*cosIncident) - (refracIndx_T*cosTransmitted) / (refracIndx_I*cosIncident) + (refracIndx_T*cosTransmitted));

        reflectionTerm = (wavelenght_S*wavelenght_S + wavelenght_P*wavelenght_P)/2;
        //return (wavelenght_S*wavelenght_S + wavelenght_P*wavelenght_P)/2;
    }

}

bool GlobalMaterial::refract_ray(Vector& Incident, Vector& Normal, float ior, Vector& refract_ray){
    float cosIncident = Incident.dot(Normal);
    //clamping the value of cosIncident to the range of -1 - 1
    if (cosIncident > 1){cosIncident = 1;}
    else if(cosIncident < -1){cosIncident = -1;}

    float refracIndx_I = 1, refracIndx_T = ior;
    //making a copy so can manipulate it
    Vector N = Normal;

    //when entering
    //need the cos of the angle to be +ve
    if(cosIncident < 0) { cosIncident = -cosIncident;}
    //when exiting
    else{
        //swapping the values of snells ratio
        swap(refracIndx_I,refracIndx_T);
        N = -N;
    }
    float snellsRatio = refracIndx_I/refracIndx_T;
    float insideSqrt = 1 - snellsRatio*snellsRatio * (1 - cosIncident*cosIncident);

    if(insideSqrt<0){return false;} //total internal refelction and therefore no refraction ray returned
    else{
        refract_ray = snellsRatio * Incident + (snellsRatio * cosIncident - sqrtf(insideSqrt)) * N;
        return true;
    }

}


Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
    Colour result;

    result.r=0.0f;
    result.g=0.0f;
    result.b=0.0f;

    return result;
}

