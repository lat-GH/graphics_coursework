cmake_minimum_required(VERSION 3.23)
project(coursework_starter)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
add_executable(lab01 framebuffer.cpp framebuffer.h linedrawer.cpp linedrawer.h main_lab1.cpp)
add_executable(lab02 main_lab2.cpp framebuffer.cpp linedrawer.cpp polymesh_object.cpp hit.cpp teapot_smaller.ply)

#add_executable(coursework_starter
#        lab01/framebuffer.cpp
#        lab01/framebuffer.h
#        lab01/linedrawer.cpp
#        lab01/linedrawer.h
#        lab01/main_lab1.cpp
#        camera.h
#        colour.h
#        compound_material.cpp
#        compound_material.h
#        directional_light.cpp
#        directional_light.h
#        environment.h
#        falsecolour_material.cpp
#        falsecolour_material.h
#        full_camera.cpp
#        full_camera.h
#        hit.cpp
#        hit.h
#        light.h
#        main_lab2.cpp
#        main_lab34.cpp
#        material.h
#        object.h
#        phong_material.cpp
#        phong_material.h
#        plane_object.cpp
#        plane_object.h
#        polymesh_object.cpp
#        polymesh_object.h
#        ray.h
#        scene.cpp
#        scene.h
#        simple_camera.cpp
#        simple_camera.h
#        sphere_object.cpp
#        sphere_object.h
#        transform.h
#        vector.h
#        vertex.h)
