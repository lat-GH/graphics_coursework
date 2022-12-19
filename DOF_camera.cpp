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

#include "DOF_camera.h"
#include <bits/stdc++.h>

DOFCamera::DOFCamera() //ADD an implementation of when dont have the values, whats the default
{
//BEGIN_STAGE_ONE
//END_STAGE_ONE
}

//FullCamera::FullCamera(float f, Vertex& p_position, Vector& p_lookat, Vector& p_up)
DOFCamera:: DOFCamera(float p_f, Vertex p_position, Vector p_lookat, Vector p_up, float p_focalL, float p_aperture, int p_samples)
{
    fov = p_f;
    position = p_position; //=Eye
    lookat = p_lookat;
    up = p_up;

    //calculating the coordinate system of the camera
    w = position-lookat;
    w.normalise();

    up.cross(w,u);
    u.normalise();

    w.cross(u,v);

    focal_length = p_focalL;
    aperture_size = p_aperture;
    num_of_samples = p_samples;



}

void DOFCamera::calculate_secondaryRay(Vertex curr_orig, Vertex destination_point, Ray result)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    //can't generate random numbers -ve numbers, so generate over larger +ve range and subtract later
    std::uniform_real_distribution<float> dis(0.0, 2*aperture_size);

    float x,y,z;
    //ASSUMPTION apature size is the radius (not the diameter)
    x = dis(gen) - aperture_size;
    y = dis(gen) - aperture_size;
    z = dis(gen) - aperture_size; //do you move the camera in all directiosn or is it just a plane?

    result.position = curr_orig + Vertex(x,y,z);
    result.direction = destination_point - result.position;

}

void DOFCamera::get_ray_pixel(int p_x, int p_y, Ray &ray)
{
    //ray needs a position and direction
    //position = eye of camera
    ray.position.x = position.x;
    ray.position.y = position.y;
    ray.position.z = position.z;
    ray.position.w = 1.0f;

    //direction
    //calculating the pixels coordinate, based on the cameras coordinate system
    float s = 1.0f; //--------------find out what this values means, and what it should be set to inorder to see the image
    float xu = s*(p_x - (width)/2);
    float yv = s*(p_y - (height/2));

    // fov = 256.0f; //fov needs to be bigger than 0.5 in order to see it, increasing the distance from the camera, makes stuff in the scene look bigger in the image plane
    Vector D = u*(xu) + v*(yv) - w*(fov);
    //cout <<"D="<< D.x<<", " << D.y<<", " << D.z<<", "<<endl;

    D.normalise();
    ray.direction.x = D.x;
    ray.direction.y = D.y;
    ray.direction.z = D.z;

}

void DOFCamera::render(Environment& env, FrameBuffer& fb)
{
    width = fb.width;
    height = fb.height;

    //looping through the pixels of the 2d image plane
    for (int y = 0; y < height; y += 1)
    {
        for (int x = 0; x < width; x += 1)
        {
            //cout << x << "," << y << endl;

            //rays have a position and a direction
            Ray primary_ray;
            //for each pixel in the image calculate the corresponding ray
            //position is equivalent to the eye of the camera
            //direction is calculated...
            get_ray_pixel(x, y, primary_ray);

            //rgba values
            Colour final_colour;
            float recurssion_depth;

            //calculate the focal point using the primary ray
            Vertex focal_point = primary_ray.position + primary_ray.direction*focal_length;

            for(int i=0; i< num_of_samples; i++){
                Ray secondary_ray;
                Colour temp_colour;
                //calculate a new eye origin, in a random direction, within the aperture distance
                calculate_secondaryRay(primary_ray.position, focal_point, secondary_ray);
                env.raytrace(secondary_ray, 5, temp_colour, recurssion_depth);
                final_colour += temp_colour;
            }

            //calculate an average colour from all the samples
            final_colour = final_colour / num_of_samples;

            fb.plotPixel(x, y, final_colour.r, final_colour.g, final_colour.b);
            fb.plotDepth(x, y, recurssion_depth);
        }

        cerr << "#" << flush;
    }
}

//
// Created by latma on 16/12/2022.
//
