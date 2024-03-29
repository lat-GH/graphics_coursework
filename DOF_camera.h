//
// Created by latma on 16/12/2022.
//

#ifndef COURSEWORK_STARTER_DOP_CAMERA_H
#define COURSEWORK_STARTER_DOP_CAMERA_H

#endif //COURSEWORK_STARTER_DOP_CAMERA_H

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

// Full camera allows a camera to be placed in space with a lookat and up direction
// as well as the field of view. It loops over the pixels in a framebuffer and computes
// a ray that is then passed to the environment.

//#ifndef
//#define

#include "camera.h"

class DOFCamera: public Camera{
public:
    int width;
    int height;
    float fov;
    Vertex position;
    Vector lookat;
    Vector up;
    Vector right;

    float focal_length;
    float aperture_size;
    int num_of_samples;


    Vector v; //cameraUp
    Vector w; //lookBehind;
    Vector u; //lookRight;



    DOFCamera();

    //FullCamera(float p_f, Vertex& p_position, Vector& p_lookat, Vector &p_up);
    DOFCamera(float p_f, Vertex p_position, Vector p_lookat, Vector p_up, float p_focalL, float p_aperture, int samples);

    void calculate_secondaryRay(Vertex curr_orig, Vertex destination_point, Ray &result);

    void get_ray_pixel(int p_x, int p_y, Ray &ray);

    void render(Environment& env, FrameBuffer& fb);
};

//#endif

