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

  //Vertex verticies[vertex_count];
  vector<Vertex> verticies;

  //getting the number of lines in the file
  if (fstream.is_open()){
      while (getline (fstream, streamTxt)) {

          //trying to find the vertex_count
          int posVertex = streamTxt.find("vertex");
          if (posVertex != -1){
            //cout << "found vertext at " << posVertex << endl;
            vertex_count = stoi(streamTxt.substr(posVertex+7,streamTxt.length()));
            cout << "vertex_count=" << vertex_count << endl;
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
              cout << "V_str[0]= " << V_str[0] <<endl;
              //verticies[lineNum-3] = Vertex(V_str[0],V_str[1],V_str[2]);
              //cout << streamTxt;
          }


          ++ lineNum;
      }
  }else cout << "Unable to open file";
  fstream.close( );

  cout << "Number of lines " << lineNum<<endl;

  for(Vector i: verticies){
      cout<<i.x<<endl;
  }
  //BEGIN_STAGE_ONE
//END_STAGE_ONE
    next = 0;
}

float *strLine_to_Vertex(string str, float *arr){
    //cout << str<<endl;
    //float arr[3];
    for(int i=0; i<3; i++){
        int space_pos = str.find(" ");
        //cout<<"i="<<i<<" "<<str.substr(0,space_pos)<<"|"<<endl;
        arr[i] = stof(str.substr(0,space_pos));
        cout << arr[i]<<endl;
        str = str.substr(space_pos+1,str.length());
        //cout << "new str" << str << endl;
    }
    cout << "arr[0]= "<< arr[0] << endl;
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