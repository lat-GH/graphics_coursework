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

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


#include "polymesh_object.h"

using namespace std;

float *strLine_to_Vertex(string str, float *arr);

PolyMesh::PolyMesh(char* file, bool smooth)
{
  cout << file << endl;

  string streamTxt;
  int lineNum = 0;
  // ---------------------------------YOU ARE HERE ------- create a vector that will store all of the vertecies and all the lines https://cplusplus.com/reference/vector/vector/

  ifstream fstream;
  fstream.open( file );

  //define the pointer, then assign how large the memory is using malloc
  Vertex *verticies;

  //getting the number of lines in the file
  if (fstream.is_open()){
      while (getline (fstream, streamTxt)) {

          //trying to find the vertex_count
          int posVertex = streamTxt.find("vertex");
          if (posVertex != -1){
            vertex_count = stoi(streamTxt.substr(posVertex+7,streamTxt.length()));
            cout << "vertex_count=" << vertex_count << endl;
            //need to use malloc so that the memory persists outside of the scope of the if statement
            verticies = (Vertex*) malloc(vertex_count * sizeof(Vertex));
          }
          //trying to find the triangle_count
          int posFace = streamTxt.find("face");
          if (posFace != -1){
              triangle_count = stoi(streamTxt.substr(posFace+5,streamTxt.length()));
              cout << "triangle_count=" << triangle_count << endl;
          }

          //populating the vertex vector
          if(3<= lineNum and lineNum <= vertex_count+2){
              float *V_str;
              float arr[3];
              //changing the address of the pointer, not it's value. so dont need the star
              V_str = strLine_to_Vertex(streamTxt,arr);
              //cout << "V_str[0]= " << V_str[0] <<endl;
              //creating a vertex and adding it to the array
              verticies[lineNum-3] = Vertex(V_str[0],V_str[1],V_str[2]);
          }
          ++ lineNum;
      }
  }else cout << "Unable to open file";
  fstream.close( );

  cout << "Number of lines " << lineNum<<endl;

  //printing all of the verticies in the array
  for(int i=0; i<vertex_count; i++){
      cout<<verticies[i].x<<verticies[i].y<<verticies[i].z<<endl;
  }
  //BEGIN_STAGE_ONE
//END_STAGE_ONE
    next = 0;
}

float *strLine_to_Vertex(string str, float *arr){
    for(int i=0; i<3; i++){
        int space_pos = str.find(" ");
        //get the substring from start up to the first space and store as an float into the array
        arr[i] = stof(str.substr(0,space_pos));
        //cout << arr[i]<<endl;
        //removing the number just found from the string
        str = str.substr(space_pos+1,str.length());
    }
    //cout << "arr[0]= "<< arr[0] << endl;
    //return &arr[0]; // == arr
    return arr;
}



Hit* PolyMesh::intersection(Ray ray)
{
    Hit* hits = 0;
//BEGIN_STAGE_ONE
//END_STAGE_ONE
    return hits;
}

void PolyMesh::apply_transform(Transform& trans)
{
//BEGIN_STAGE_ONE
// END_STAGE_ONE
}