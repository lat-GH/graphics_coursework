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

// classes that contain our lights, all derived from Light
#include "directional_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "phong_material.h"
#include "falsecolour_material.h"
#include "global_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"


using namespace std;

// you will find it useful during development/debugging to create multiple functions that fill out the scene.
void build_scene(Scene& scene)
{
	// The following transform allows 4D homogeneous coordinates to be transformed.It moves the supplied teapot model to somewhere visible.
	Transform * transform = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, -2.7f,
			0.0f, 1.0f, 0.0f, 5.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

	//  Read in the teapot model.
	//PolyMesh* pm = new PolyMesh("teapot_smaller.ply", false);
    PolyMesh* pm = new PolyMesh((char *) "teapot_smaller.ply", true);

	pm->apply_transform(*transform);

	Sphere* sphere = new Sphere(Vertex(0.2f, 0.0f, 1.5f), 0.6f);//-1 0 2 r=0.4
    //Sphere* skydome = new Sphere(Vertex(0.0f, 0.0f, 0.0f), 50);
    Plane* background = new Plane(0.0f, 0.0f, -1.0f, 10.0f);
    Plane* background02 = new Plane(-1.0f, 0.0f, 0.0f, 3.0f);
    Plane* background03 = new Plane(0.0f, 1.0f, 0.0f, 4.0f);


	DirectionalLight* dl = new DirectionalLight(Vector(1.0f, -1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 0.0f));
    //DirectionalLight* dl = new DirectionalLight(Vector(-1.0f, -1.0f, -1.0f), Colour(1.0f, 1.0f, 1.0f, 0.0f));
    //1 -1 1
	scene.add_light(dl);

	Phong* redPhong = new Phong(Colour(0.2f, 0.0f, 0.0f), Colour(0.4f, 0.0f, 0.0f), Colour(0.4f, 0.4f, 0.4f), 40.f);
	Phong* bluePhong = new Phong(Colour(0.01f, 0.01f, 0.2f), Colour(0.0f, 0.0f, 1.0f), Colour(0.5f, 0.5f, 0.5f), 40.f);
    Phong* greenPhong = new Phong(Colour(0.0f, 0.2f, 0.0f), Colour(0.0f, 1.0f, 0.0f), Colour(0.5f, 0.5f, 0.5f), 40.f);
    Phong* purplePhong = new Phong(Colour(0.2f, 0.0f, 0.2f), Colour(1.0f, 0.0f, 1.0f), Colour(0.5f, 0.5f, 0.5f), 40.f);
    FalseColour* rainbow = new FalseColour();
    GlobalMaterial* globalMat = new GlobalMaterial(&scene, Colour(0.8f, 0.8f, 0.8f), Colour(0.8f, 0.8f, 0.8f), 1.5);

    //pm->set_material(redPhong);
    //pm->set_material(globalMat);// TODO why does my whole scene break when i apply the globalmat to the teapot
	//scene.add_object(pm);

	sphere->set_material(globalMat);
    //sphere->set_material(redPhong);
	scene.add_object(sphere);

    //skydome->set_material(redPhong);
    //scene.add_object(skydome);
    background->set_material(bluePhong);
    background02->set_material(greenPhong);
    background03->set_material(purplePhong);
    scene.add_object(background);
    scene.add_object(background02);
    scene.add_object(background03);
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
	
	// Declare a camera
	//Camera *camera = new SimpleCamera(0.5f);
    //postion = 0,0.1,-1
    //good test = pos=8,0,15 look=0,0,8
    Camera* camera = new FullCamera(350.0f, Vertex(0.0f, 0.1f, -1.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, -1.0f, 0.0f)); //standard
    //Camera* camera = new FullCamera(350.0f, Vertex(8.0f, 0.1f, 15.0f), Vector(0.0f, 0.0f, 8.0f), Vector(0.0f, -1.0f, 0.0f)); //good test for multiple intersections
    //Camera* camera = new FullCamera(350.0f, Vertex(0.0f, 1.0f, -1.0f), Vector(0.0f, -1.0f, 1.0f), Vector(0.0f, -1.0f, 0.0f)); //good reflection test
	
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
