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

#include "full_camera.h"

FullCamera::FullCamera() //ADD an implementation of when dont have the values, whats the default
{
//BEGIN_STAGE_ONE
//END_STAGE_ONE
}

//FullCamera::FullCamera(float f, Vertex& p_position, Vector& p_lookat, Vector& p_up)
FullCamera::FullCamera(float f, Vertex p_position, Vector p_lookat, Vector p_up)
{
    fov = f;
    position = p_position; //=Eye
    lookat = p_lookat;
    up = p_up;

    //calculating the coordinate system of the camera
    Vector numerator = position-lookat;
    float denominator = 1.0f/numerator.length();
    //float denominator = numerator.length();
    Vector w = numerator * denominator;

    cout <<"w="<< w.x<<", " << w.y<<", " << w.z<<", "<<endl;


}

void FullCamera::get_ray_offset(int p_x, int p_y, float p_ox, float p_oy, Ray& p_ray)
{
//BEGIN_STAGE_ONE
// END_STAGE_ONE
}

void FullCamera::get_ray_pixel(int p_x, int p_y, Ray &ray)
{
//BEGIN_STAGE_ONE
//END_STAGE_ONE
}

void FullCamera::render(Environment& env, FrameBuffer& fb)
{
//BEGIN_STAGE_ONE
//END_STAGE_ONE
}

