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

string *splitStr_nItems(string str, int n);

PolyMesh::PolyMesh(char* file, bool smooth)
{
  cout << file << endl;

  string streamTxt;
  int lineNum = 1;

  ifstream fstream;
  fstream.open( file );

  bool facesFound = false;


  //getting the number of lines in the file
  if (fstream.is_open()){
      while (getline (fstream, streamTxt)) {

          //trying to find the vertex_count
          int posVertex = streamTxt.find("vertex");
          if (posVertex != -1){
            vertex_count = stoi(streamTxt.substr(posVertex+7,streamTxt.length()));
            //cout << "vertex_count=" << vertex_count << endl;
            //need to use malloc so that the memory persists outside of the scope of the if statement
            //verticies = (Vertex*) malloc(vertex_count * sizeof(Vertex)); //------ free this memory at the end?
            vertex = new Vertex[vertex_count];
          }
          //trying to find the triangle_count
          int posFace = streamTxt.find("face");
          if (posFace != -1){
              facesFound = true;
              triangle_count = stoi(streamTxt.substr(posFace+5,streamTxt.length()));
              //cout << "triangle_count=" << triangle_count << endl;
              //triangle = (TriangleIndex*) malloc(triangle_count * sizeof(TriangleIndex)); //------ free this memory at the end?
              triangle = new TriangleIndex[triangle_count];
          }

          //populating the vertex vector
          if(lineNum > 3 and lineNum <= vertex_count+3){
              //cout << "lineNum" << lineNum<<endl;
              string *V_str;
              //extracting an array of strings sperpated by spaces
              //changing the address of the pointer, not it's value. so dont need the star
              V_str = splitStr_nItems(streamTxt,3);
              //cout << "V_str[0]= " << V_str[0] <<endl;
              //creating a vertex and adding it to the array
              //lineNum-3 so starts at 0
              vertex[lineNum-4] = Vertex(stof(V_str[0]),stof(V_str[1]),stof(V_str[2]));
          }
          //populating the triangle vector
          if(facesFound){//dont want to try and populate the trainagles till it has found the faces element
              if(lineNum>vertex_count+3 and lineNum<=triangle_count+vertex_count+4){
                  //cout << "lineNum" << lineNum<<endl;

                  //want the triangle to start at index at 0
                  int i =lineNum-vertex_count-4;
                  //cout<<"i="<<i<<endl;

                  string *T_str;
                  T_str = splitStr_nItems(streamTxt,4);
                  //cout << "T_str[3]="<<T_str[3]<<endl;

                  triangle[i][0]=stoi(T_str[1]); // WHy does it wokr when run normal but not with the debugger ffs
                  triangle[i][1]=stoi(T_str[2]);
                  triangle[i][2]=stoi(T_str[3]);

                  //cout <<  " triangle[i][0]="<<triangle[i][0]<<endl;
              }
          }

          ++ lineNum;
      }
  }else cout << "Unable to open file";
  fstream.close( );

  //cout << "Number of lines " << lineNum-1<<endl;

//  //printing all of the verticies in the array
//  for(int i=0; i<vertex_count; i++){
//      cout<<vertex[i].x << ", " << vertex[i].y << ", " << vertex[i].z <<endl;
//  }
//  for(int i=0; i<triangle_count; i++){
//      cout<<triangle[i][0] << ", " <<triangle[i][1] << ", " <<triangle[i][2] <<endl;
//  }

  //BEGIN_STAGE_ONE
//END_STAGE_ONE
    next = 0;
}

string *splitStr_nItems(string str, int n){
    string *arr;
    arr = new string[n]; // might it not be working because you use the same arr? does it create a new one each time the function is called?
    for(int i=0; i<n; i++){
        int space_pos = str.find(" ");
        //get the substring from start up to the first space and store as an float into the array
        arr[i] = str.substr(0,space_pos);
        //cout << arr[i]<<endl;
        //removing the content just found from the string
        str = str.substr(space_pos+1,str.length());
    }
    //cout << "arr[0]= "<< arr[n-1] <<"|"<< endl;
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
{ //passing a pointer to a transform object
     //you only need to transform the verticies, because th traingles are linked by thier index in the array not value.
    for (int i=0; i<vertex_count; i++){
        //trans is a Transform which can be applied to a single vertex at a time
        trans.apply(vertex[i]);
    }


//BEGIN_STAGE_ONE
// END_STAGE_ONE
}