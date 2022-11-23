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

GlobalMaterial::GlobalMaterial(Scene* p_env, Colour p_reflect_weight, Colour p_refract_weight, float p_ior)
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


    return result + result_refraction;



    //get the reflection ratio using the frensel term
    float fresnel_reflection;
    fresnel(viewer.direction, hit.normal, 1, ior, fresnel_reflection);


    Ray reflection_ray;
    reflection_ray.direction = reflect(viewer.direction, hit.normal);
    reflection_ray.direction.normalise();
    reflection_ray.position = hit.position + 1.000005f*reflection_ray.direction; //want to start the reflection ray just above the surface

    environment->raytrace(reflection_ray, recurse-1, result_reflection, hit.t);
    //cout << "result_reflection = "<<result_reflection.r << result_reflection.g << result_reflection.b << endl;
    //float kr = 0.8f;
    //result += result * kr;
    //result +=  result_reflection*0.8f;
    result += result_refraction*1;//*(1-reflectionTerm);
    cout << "result = " << result.r << result.g << result.b << endl;
    //result +=  result_reflection*reflectionTerm + result_refraction*(1-reflectionTerm);

	return result;
}


Vector reflect(Vector Incident, Vector Normal){
    return Incident - 2.0f * (Normal.dot(Incident)) * Normal;
}

void GlobalMaterial::fresnel(Vector& viewer, Vector& Normal, float etai, float etat, float& reflectionTerm){
    Vector Incident = viewer;//making a copy so not to edit the viewer value
    Incident.negate();
    float cosIncident = Incident.dot(Normal);
    //cout << "frenel cosIncident" << cosIncident << endl;
    //clamping the value of cosIncident to the range of -1 - 1
    if (cosIncident > 1){cosIncident = 1;}
    else if(cosIncident < -1){cosIncident = -1;}

    float refracIndx_air;
    float refracIndx_glass;
    //will flip the direction of the ratio based on the value of the cosIncident
    //if(going_in(cosIncident, etai)){
    if(cosIncident > 0){ //going into the material
        refracIndx_air = etai;//1.0f;
        refracIndx_glass = etat;//ior;
    }
    else{
        //cout << "frenel cosIncident" << cosIncident << endl;
        //cout << "frensel coming out"<<endl;
        //coming out of the material
        refracIndx_air = etat;//ior;
        refracIndx_glass = etai; //1.0f;
    }
    float snellsRatio = refracIndx_glass/refracIndx_air;
    //float snellsRatio = refracIndx_air/refracIndx_glass; // when used this way around it breaks everything
    //cout << "snellsRatio"<<snellsRatio <<endl;

    //checking for total interal reflection
    float sinTransmitted = snellsRatio * sqrtf(max(0.f, 1 - cosIncident*cosIncident)); //-----check this maths is correct
    //cout << "cosIncident = "<< cosIncident << "^^2" << cosIncident*cosIncident << "1-" << 1 - cosIncident*cosIncident<< endl;
    //cout << "sinTransmitted = "<< sinTransmitted<<endl; // always a +ve 0.somtheing number
    if (sinTransmitted >= 1){
        //cout << "frens Internal Reflection"<<endl;
        reflectionTerm = 1;
        //return 1;
    }
    else{
        //cout << "frens Refracted" << endl;
        float cosTransmitted = sqrtf(max(0.f, 1 - sinTransmitted*sinTransmitted));
        cosIncident = fabsf(cosIncident);
        float wavelenght_S = ((refracIndx_glass*cosIncident) - (refracIndx_air*cosTransmitted) / (refracIndx_glass*cosIncident) + (refracIndx_air*cosTransmitted));
        float wavelenght_P = ((refracIndx_air*cosIncident) - (refracIndx_glass*cosTransmitted) / (refracIndx_air*cosIncident) + (refracIndx_glass*cosTransmitted));

        reflectionTerm = (wavelenght_S*wavelenght_S + wavelenght_P*wavelenght_P)/2;
        //return (wavelenght_S*wavelenght_S + wavelenght_P*wavelenght_P)/2;
    }

}

bool GlobalMaterial::refract_ray(Vector& viewer, Vector& Normal, float snellsRatio, Vector& refract_ray){

    Vector Incident = viewer;
//    Incident.negate();

//    float cosIncident = Incident.dot(Normal);
    float cosIncident = Normal.dot(-viewer);

    float cosTransmitted_squared = 1 - (1/pow(snellsRatio,2)) * (1-pow(cosIncident,2));
    //dont want to sqrt a -ve num
    if(cosTransmitted_squared < 0){
        return false;
    }
    float cosTransmitted = sqrtf( cosTransmitted_squared );
    refract_ray = (1/snellsRatio) * viewer - (cosTransmitted - ((1 / snellsRatio) * cosIncident)) * Normal;
    return true;

//    Vector Incident = viewer;
//    Incident.negate();
//
//    float cosIncident = Incident.dot(Normal);
//    //cout << "refract cosIncident" << cosIncident << endl;
//    //clamping the value of cosIncident to the range of -1 - 1
//    if (cosIncident > 1){cosIncident = 1;}
//    else if(cosIncident < -1){cosIncident = -1;}
//
//    float refracIndx_air = 1, refracIndx_glass = ior;
//    //    cout << refracIndx_I << refracIndx_T << endl;
//    //making a copy so can manipulate it
//    Vector N = Normal;
//
//    //when entering
//    //need the cos of the angle to be +ve when entering
//    if(cosIncident > 0) { cosIncident = -cosIncident;}
//    //when exiting
//    else{
//        //cout << "refract coming out"<<endl;
//        //swapping the values of snells ratio
//        //swap(refracIndx_I,refracIndx_T);
//        refracIndx_air = ior;
//        refracIndx_glass = 1;
//        //cout << refracIndx_air << refracIndx_glass << endl;
//        N = -N;
//    }
//    //scratch pixel's refraction equation
//    float snellsRatio = refracIndx_glass/refracIndx_air;
//    //float snellsRatio = refracIndx_air/refracIndx_glass;// when use this way round it breaks everything
//    //cout << "snellsRatio"<<snellsRatio <<endl;
//    float insideSqrt = 1 - snellsRatio*snellsRatio * (1 - cosIncident*cosIncident);
//    //cout << "insideSqrt" << insideSqrt <<endl;
//    if(insideSqrt<0){
////        cout << "refrca Internal Reflection"<<endl; //----------this never gets called BUT is it because we catch the total internal reflection case in the frenel term
//        return false;
//    } //total internal refelction and therefore no refraction ray returned
//    else{
//        //cout << "refrac Refraction" <<endl;
//        //Incident.negate();
//        refract_ray = snellsRatio * Incident + (snellsRatio * cosIncident - sqrtf(insideSqrt)) * N;
//        return true;
//    }

}


Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
    Colour result;

    result.r=0.0f;
    result.g=0.0f;
    result.b=0.0f;

    return result;
}

