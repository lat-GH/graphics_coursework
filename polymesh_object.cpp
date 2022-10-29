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
            //need to use malloc so that the memory persists outside of the scope of the if statement
            vertex = new Vertex[vertex_count];
          }
          //trying to find the triangle_count
          int posFace = streamTxt.find("face");
          if (posFace != -1){
              facesFound = true;
              triangle_count = stoi(streamTxt.substr(posFace+5,streamTxt.length()));
              //triangle = (TriangleIndex*) malloc(triangle_count * sizeof(TriangleIndex)); //------ free this memory at the end?
              triangle = new TriangleIndex[triangle_count];
          }

          //populating the vertex vector
          if(lineNum > 3 and lineNum <= vertex_count+3){
              string *V_str;
              //extracting an array of strings sperpated by spaces
              //changing the address of the pointer, not it's value. so dont need the star
              V_str = splitStr_nItems(streamTxt,3);
              //creating a vertex and adding it to the array
              //lineNum-3 so starts at 0
              vertex[lineNum-4] = Vertex(stof(V_str[0]),stof(V_str[1]),stof(V_str[2]));
          }
          //populating the triangle vector
          if(facesFound){//dont want to try and populate the trainagles till it has found the faces element
              if(lineNum>vertex_count+3 and lineNum<=triangle_count+vertex_count+4){
                  //want the triangle to start at index at 0
                  int i =lineNum-vertex_count-4;

                  string *T_str;
                  T_str = splitStr_nItems(streamTxt,4);

                  triangle[i][0]=stoi(T_str[1]);
                  triangle[i][1]=stoi(T_str[2]);
                  triangle[i][2]=stoi(T_str[3]);

              }
          }

          ++ lineNum;
      }
  }else cout << "Unable to open file";
  fstream.close( );


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
        //removing the content just found from the string
        str = str.substr(space_pos+1,str.length());
    }
    //return &arr[0]; // == arr
    return arr;
}

bool MollerTrumbore_algorithm(Vector &vOrig, Vector &vDir, const Vector &vA, const Vector &vB, const Vector &vC, float &t, float &u, float &v){
    const float EPSILON = 0.0000001;

    //set up the Vectors for the u,v Barycentric coordinates
    Vector vAB = vB - vA;
    Vector vAC = vC - vA;
    //calculating the normal of the ray and the trainagle face for testing
    Vector vN;
    vDir.cross(vAC, vN); // storing result in vN //TODO make sure its not altering the value of vDir
    //short for determinate (to help solve using Cramers rules)
    float det = vAB.dot(vN);

    //if the determinate is netgative, then the traingle is back facing - you are hitting it from behind
    //if the determinate is close to 0(even if not equal), the ray misses the triangle
    if(det<EPSILON) return false;

    //ray and triangle are parallel if the det (angle between the vN and vAB) is close to 0
    if(fabs(det) < EPSILON) return false;

    float invDet = 1/det;

    Vector vOA = vOrig - vA;
    //calculating the barycentric coord u
    u = vOA.dot(vN)*invDet;
    //barycentric states that u + v + w = 1 therefore u must be 0<u<1
    if (u<0 || u>1) return false;

    Vector vOAxvAB;
    vOA.cross(vAB,vOAxvAB);
    v = vDir.dot(vOAxvAB)*invDet;
    if (v<0 || u+v>1) return false;

    // t = distance along the ray at which it intersetcs the triangle
    t = vAC.dot(vOAxvAB)*invDet;

    return true; // the ray does intersect with the triangle
}

Hit* PolyMesh::intersection(Ray ray) //------------YOU ARE HERE------- try to work out where this gets run
{
    int i=0; //triangle counter
    //Vector A(vertex[triangle[i][0]].x,vertex[triangle[i][0]].y, vertex[triangle[i][0]].z);
    Vector A(vertex[triangle[i][0]]); //TODO check you can use this method to convert from a Vertex to a Vector
    Vector B(vertex[triangle[i][1]]);
    Vector C(vertex[triangle[i][2]]);

    //initialsing the Barycetric coords (scalar values)
    float u, v, t;

    //Test
    bool intersect = MollerTrumbore_algorithm(ray.position, ray.direction, A,B,C,  u, v, t);


    Hit* hits = 0;//TODO find out what hits are and how is best to populate them
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