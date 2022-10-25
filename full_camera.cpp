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

}

void FullCamera::get_ray_offset(int p_x, int p_y, float p_ox, float p_oy, Ray& p_ray)
{
//BEGIN_STAGE_ONE
// END_STAGE_ONE
}

Vector *calculate_wuv(Vertex position, Vector lookat, Vector up){
    Vector *wuv;
    wuv = new Vector[3];

    //calculating the coordinate system of the camera
    Vector w_numerator = position-lookat;
    float w_denominator = 1.0f/w_numerator.length();
    Vector w = w_numerator * w_denominator;
    //cout <<"w="<< w.x<<", " << w.y<<", " << w.z<<", "<<endl;

    Vector u_numerator;
    up.cross(w,u_numerator); //you pass in the vector you want to store the result into
    float u_denominator = 1.0f/u_numerator.length();
    Vector u = u_numerator * u_denominator;
    //cout <<"u="<< u.x<<", " << u.y<<", " << u.z<<", "<<endl;

    Vector v;
    w.cross(u,v);
    //cout <<"v="<< v.x<<", " << v.y<<", " << v.z<<", "<<endl;

    wuv[0]=w;
    wuv[1]=u;
    wuv[2]=v;

    //cout <<"INSIDE wuv[1].x= " <<wuv[1].x<<endl;
    return wuv;
}


void FullCamera::get_ray_pixel(int p_x, int p_y, Ray &ray)
{
    //ray needs a position and direction
    //position = eye of camera
    ray.position.x = position.x;
    ray.position.y = position.y;
    ray.position.z = position.z;
    ray.position.w = 1.0f;

    //direction
    //calculating the pixels coordinate, based on the cameras coordinate system
    float s = 1.0f; //TODO find out what this values means, and what it should be set to inorder to see the image
    float xu = s*(p_x - (width)/2);
    float yv = s*(p_y - (height/2));

    fov = 0.5f; //NOTICED THAT----- Hit Pool = 100 if fov>1.25

    Vector D = u*(xu) + v*(yv) + w*(fov);//TODO how does changing the value of the fov affect the image?
    //cout <<"D="<< D.x<<", " << D.y<<", " << D.z<<", "<<endl;

    D.normalise();
    ray.direction.x = D.x;
    ray.direction.y = D.y;
    ray.direction.z = D.z;

    //ray.direction.normalise();
}

void FullCamera::render(Environment& env, FrameBuffer& fb)
{
    width = fb.width;
    height = fb.height;

    // calculating the camera's coordinate system
    Vector *wuv;
    //calculate the camera coordinate system
    wuv = calculate_wuv(position,lookat,up);

    //cout <<"OUT"<<endl;
    //cout <<"OUTSIDE wuv[0].x= " <<wuv[0].x<< endl;

    w =wuv[0];
    u =wuv[1];
    v =wuv[2];

    //looping through the pixels of the 2d image
    for (int y = 0; y < height; y += 1)
    {
        for (int x = 0; x < width; x += 1)
        {
            //rays have a position and a direction
            Ray ray;
            //for each pixel in the image calculate the corresponding ray
            //position is equivalent to the eye of the camera
            //direction is calculated...
            get_ray_pixel(x, y, ray);

            //rgba values
            Colour colour;
            float depth;

            env.raytrace(ray, 5, colour, depth);

            fb.plotPixel(x, y, colour.r, colour.g, colour.b);
            fb.plotDepth(x, y, depth);
        }

        cerr << "#" << flush;
    }
}

