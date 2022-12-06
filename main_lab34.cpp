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

//magick "C:\Users\latma\OneDrive\Documents\ComputerScience_FINALyr\Graphics\coursework-starter\cmake-build-debug\test.ppm" "C:\Users\latma\OneDrive\Documents\ComputerScience_FINALyr\Graphics\coursework-starter\cmake-build-debug\test.png"
//magick "C:\Users\latma\OneDrive\Documents\ComputerScience_FINALyr\Graphics\coursework-starter\cmake-build-debug\depth.ppm" "C:\Users\latma\OneDrive\Documents\ComputerScience_FINALyr\Graphics\coursework-starter\cmake-build-debug\depth.png"

//magick "C:\Users\latma\OneDrive\Documents\ComputerScience_FINALyr\Graphics\coursework-starter\cmake-build-release\test.ppm" "C:\Users\latma\OneDrive\Documents\ComputerScience_FINALyr\Graphics\coursework-starter\cmake-build-release\test.png"
//magick "C:\Users\latma\OneDrive\Documents\ComputerScience_FINALyr\Graphics\coursework-starter\cmake-build-release\depth.ppm" "C:\Users\latma\OneDrive\Documents\ComputerScience_FINALyr\Graphics\coursework-starter\cmake-build-release\depth.png"

/* This is the top level for the program you need to create for lab three and four.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// these are core raytracing classes
#include "framebuffer.h"
#include "scene.h"

// classes that contain our objects to be rendered, all derived from Object
#include "polymesh_object.h"
#include "sphere_object.h"
#include "plane_object.h"
#include "quadratic_object.h"
#include "csg_object.h"

// classes that contain our lights, all derived from Light
#include "directional_light.h"
#include "point_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "phong_material.h"
#include "falsecolour_material.h"
#include "global_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"
#include "photon_map.h"


using namespace std;

// you will find it useful during development/debugging to create multiple functions that fill out the scene.
void build_scene(Scene& scene)
{
	// The following transform allows 4D homogeneous coordinates to be transformed.It moves the supplied teapot model to somewhere visible.
	Transform * transform = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, -2.7f,
			0.0f, 1.0f, 0.0f, 5.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
    //transform02 just moves it up a little
    Transform * transform02 = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
                                          0.0f, 1.0f, 0.0f, 1.4f,
                                          0.0f, 0.0f, 1.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f);

    PolyMesh* pm = new PolyMesh((char *) "teapot_smaller.ply", true);
	pm->apply_transform(*transform);

	Sphere* sphere = new Sphere(Vertex(0.0f, 0.0f, 0.0f), 0.6f);//-1 0 2 r=0.4
    Sphere* sphere02 = new Sphere(Vertex(1.0f, 0.1f, 1.5f), 0.6f);//-1 0 2 r=0.4

    Plane* background = new Plane(0.0f, 0.0f, -1.0f, 10.0f);
    Plane* background_behind = new Plane(0.0f, 0.0f, 1.0f, 10.0f);
    Plane* background_RHS = new Plane(-1.0f, 0.0f, 0.0f, 5.0f);
    Plane* background_LHS = new Plane(1.0f, 0.0f, 0.0f, 5.0f);
    Plane* background_Bottom = new Plane(0.0f, 1.0f, 0.0f, 5.0f);
    Plane* background_Top= new Plane(0.0f, -1.0f, 0.0f, 5.0f);

    //Quadratic* quad_Obj = new Quadratic(1.0f,0,0,0,4.0f,0,0,1.0f,0,-1); //cone
    Quadratic* quad_Obj = new Quadratic(1.0f,0,0,0,-1.0f,0,0,1.0f,0,0); //cone
    //Quadratic* quad_Obj = new Quadratic(4.0f,0,0,0,4.0f,0,0,0.0f,0,-1); //cylinder
    //quad_Obj->apply_transform(*transform02); //step through to check if the values get changed

    CSG::Mode csg_mode = CSG::CSG_DIFF;
    //CSG::Mode csg_mode = CSG::CSG_UNION;
    //CSG::Mode csg_mode = CSG::CSG_INTER;
    CSG* csg_object = new CSG(csg_mode,sphere, sphere02);
    //csg_object->apply_transform(*transform02);

	//DirectionalLight* light = new DirectionalLight(Vector(1.0f, -1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 0.0f)); //1 -1 1
    Vector lightDirection = Vector(3.0f, 4.0f, -5.0f);//1, 4. -5
    PointLight* light = new PointLight(Vertex (lightDirection), Colour(1.0f, 1.0f, 1.0f, 1.0f), lightDirection, 0.5);
    //Light* light = new Light();

	scene.add_light(light);

	Phong* redPhong = new Phong(Colour(0.2f, 0.0f, 0.0f), Colour(0.4f, 0.0f, 0.0f), Colour(0.4f, 0.4f, 0.4f), 40.f);
	Phong* bluePhong = new Phong(Colour(0.01f, 0.01f, 0.2f), Colour(0.0f, 0.0f, 1.0f), Colour(0.5f, 0.5f, 0.5f), 40.f);
    Phong* greenPhong = new Phong(Colour(0.0f, 0.2f, 0.0f), Colour(0.0f, 0.5f, 0.0f), Colour(0.5f, 0.5f, 0.5f), 40.f);
    Phong* purplePhong = new Phong(Colour(0.2f, 0.0f, 0.2f), Colour(1.0f, 0.0f, 1.0f), Colour(0.5f, 0.5f, 0.5f), 40.f);
    FalseColour* rainbow = new FalseColour();
    GlobalMaterial* globalMat_reflect = new GlobalMaterial(&scene, Colour(0.9f, 0.9f, 0.9f), Colour(0.0f, 0.0f, 0.0f), 1.0f);
    GlobalMaterial* globalMat_refract = new GlobalMaterial(&scene, Colour(0.9f, 0.9f, 0.9f), Colour(0.9f, 0.9f, 0.9f), 1.5);


    pm->set_material(redPhong);
    //pm->set_material(globalMat);
	//scene.add_object(pm);

    //sphere->set_material(globalMat_reflect);
    //sphere->set_material(globalMat_refract);
    sphere->set_material(greenPhong);
	scene.add_object(sphere);

    sphere02->set_material(redPhong);
    scene.add_object(sphere02);

    background->set_material(bluePhong);
    background_RHS->set_material(greenPhong);
    background_LHS->set_material(greenPhong);
    background_Bottom->set_material(purplePhong);
    background_Top->set_material(purplePhong);
    background_behind->set_material(rainbow);
    scene.add_object(background);
    scene.add_object(background_behind);
    scene.add_object(background_RHS);
    scene.add_object(background_LHS);
    scene.add_object(background_Bottom);
    scene.add_object(background_Top);



    quad_Obj->set_material(redPhong);
    //scene.add_object(quad_Obj);

    //scene.add_object(csg_object);
}


// This is the entry point function to the program.
int main(int argc, char *argv[])
{
	int width = 512;
	int height = 512;
	// Create a framebuffer
	FrameBuffer* fb = new FrameBuffer(width, height);
	
	// Create a scene
	Scene scene;
	
	// Setup the scene
	build_scene(scene);

    //photon mapping
    PhotonMap photon_map = *new PhotonMap(scene);
	
	// Declare a camera
	//Camera *camera = new SimpleCamera(0.5f);

    //Camera* camera = new FullCamera(350.0f, Vertex(0.0f, 0.1f, -1.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, -1.0f, 0.0f)); //standard
    //Camera* camera = new FullCamera(350.0f, Vertex(8.0f, 0.1f, 15.0f), Vector(0.0f, 0.0f, 8.0f), Vector(0.0f, -1.0f, 0.0f)); //good test for multiple intersections
    //Camera* camera = new FullCamera(350.0f, Vertex(0.0f, 1.0f, -1.0f), Vector(0.0f, -1.0f, 1.0f), Vector(0.0f, -1.0f, 0.0f)); //good reflection test
    //---------box scene camera ------------
    Camera* camera = new FullCamera(350.0f, Vertex(0.0f, 0.1f, -3.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, -1.0f, 0.0f)); //standard

    //--------------CSG cameras------------------------
    //Camera* camera = new FullCamera(350.0f, Vertex(-10.0f, 0.0f, -2.0f), Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, -1.0f, 0.0f));//epsiloiod
    //Camera* camera = new FullCamera(350.0f, Vertex(10.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, -1.0f, 0.0f));// good for cone


	// Camera generates rays for each pixel in the framebuffer and records colour + depth.
	camera->render(scene,*fb);
	
	// Output the framebuffer colour and depth as two images
	fb->writeRGBFile((char *)"test.ppm");
	fb->writeDepthFile((char *)"depth.ppm");
	
	cerr << "\nDone.\n" << flush;
	
	// a debug check to make sure we don't leak hits.
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}

