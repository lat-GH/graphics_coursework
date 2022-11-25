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

#include "quadratic_object.h"
#include <math.h>

using namespace std;

Quadratic::Quadratic(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j)
{
    A = a;
    B = b;
    C = c;
    D = d;
    E = e;
    F = f;
    G = g;
    H = h;
    I = i;
    J = j;
  //next = (Object *)0; //TODO find out what is suppossed to be used for
}

Hit *Quadratic::intersection(Ray ray)
{
    Vector Pos = ray.position;
    Vector dir = ray.direction;
    float Px = Pos.x, Py = Pos.y, Pz = Pos.z;
    float Dx = dir.x, Dy = dir.y, Dz = dir.z;

    float A_quad = (A*pow(Dx,2)) + (2*B*Dx*Dy) + (2*C*Dx*Dz) + (E*pow(Dy,2)) + (2*F*Dy*Dz) + (G*pow(Dz,2));
    float B_quad  = 2*( (A*Px*Dx) + (B *(Px*Dy+Dx*Py)) + (C *(Px*Dz+Dx*Pz)) + (D*Dx) + (E*Py*Dy) + (F *(Py*Dz+Dy*Pz)) + (G*Dy) + (H*Pz*Dz) + (I*Dz));
    float C_qaud = (A*pow(Px,2)) + (2*B*Px*Py) + (2*C*Px*Pz) + (2*D*Px) + (E*pow(Py,2)) + (2*F*Py*Pz) + (2*G*Py) + (H*pow(Pz,2)) + (2*I*Pz) + J;

    float discriminant = pow(B_quad,2) - (4*A_quad*C_qaud);
    //case when no intersections
    if (discriminant < 0){
        Hit *hits = 0; // TODO move this outiside the if statement
        return hits;
    }
    //case when only one intersection, ray it tangent to the surface
    if(A_quad == 0){
        Hit *newHit = new Hit;

        newHit->t =  -C_qaud/B_quad;
        newHit->position = ray.position + newHit->t*ray.direction;
        newHit->what = this;

        newHit->normal = calcNormal(newHit->position);

        newHit->entering = false; //TODO check this might not be the correct way to assign it...?
        newHit->next = 0;

        return newHit;
    }
    //case when have 2 intersections
    else{
        Hit *hit_in = new Hit;
        Hit *hit_out = new Hit;

        //2 intersctions therefore 2 hit values
        float T_in = (-B_quad - sqrtf(discriminant)) / (2*A_quad);
        float T_out = (-B_quad + sqrtf(discriminant)) / (2*A_quad);

        hit_in->t = T_in;
        hit_out->t = T_out;

        hit_in->position = ray.position + hit_in->t*ray.direction;
        hit_out->position = ray.position + hit_out->t*ray.direction;

        hit_in->normal = calcNormal(hit_in->position);
        hit_out->normal = calcNormal(hit_out->position);

        hit_in->what = this;
        hit_out->what = this;

        hit_in->entering = true;
        hit_out->entering = false;

        hit_in->next = hit_out;
        hit_out->next = 0;
        return hit_in;

    }

}

Vector Quadratic::calcNormal(Vector P){
    float Nx = 2* (A*P.x + B*P.y + C*P.z + D);
    float Ny = 2* (B*P.x + E*P.y + F*P.z + G);
    float Nz = 2* (C*P.x + F*P.y + H*P.z + I);

    return Vector(Nx, Ny, Nz);
}


void Quadratic::apply_transform(Transform& trans)
{
    //TODO work out how to assign the values from the matrix transformation to the  A,B,C,D,E,F,G,H,I,J;
    Transform Q = Transform(A,B,C,D,B,E,F,G,C,F,H,I,D,G,I,J);
    //Transform Q = Transform(&A,&B,&C,&D,&B,&E,&F,&G,&C,&F,&H,&I,&D,&G,&I,&J);
    Transform transpose = trans.transpose();
    Transform result = transpose * Q * trans;
    //Vector v = Vector();
    //result.apply(v);



}

