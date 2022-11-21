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
Vector get_TriangleNormal(Vector vA, Vector vB, Vector vC);
void setup_triangleNormals_and_vertexNormals();

PolyMesh::PolyMesh(char* file, bool smooth)
{

  smoothing = smooth;

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


  //printing all of the verticies in the array
//  for(int i=0; i<vertex_count; i++){
//      cout<<vertex[i].x << ", " << vertex[i].y << ", " << vertex[i].z <<endl;
//  }
//  for(int i=0; i<triangle_count; i++){
//      cout<<triangle[i][0] << ", " <<triangle[i][1] << ", " <<triangle[i][2] <<endl;
//  }


    //calculating all the normals for the triangles, and calculating each of the vertex normals
    triangle_normals = new Vector[triangle_count];
    vertex_normals = new Vector[vertex_count];

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
    vDir.cross(vAC, vN); // storing result in vN
    //short for determinate (to help solve using Cramers rules)
    float det = vAB.dot(vN);

    //if the determinate is netgative, then the traingle is back facing - you are hitting it from behind
    //if the determinate is close to 0(even if not equal), the ray misses the triangle
    if(det>EPSILON) return false; // > gives outside facing, < give inside facing

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

void PolyMesh::setup_triangleNormals_and_vertexNormals(){
    for(int i=0; i<triangle_count; i++){
        //getting the verticies of the triangle
        Vector A(vertex[triangle[i][0]]);
        Vector B(vertex[triangle[i][1]]);
        Vector C(vertex[triangle[i][2]]);
        //calculating and assigning the value of the normal of the curr triangle
        triangle_normals[i] = get_TriangleNormal(A,B,C);

        //appeneing the value of the newly calcualted tNormal to all of the verticies that are used by this triangle
        vertex_normals[triangle[i][0]] = triangle_normals[i] + vertex_normals[triangle[i][0]]; //vertex A
        //normal @ vertex A = Normal of curr triangle attached to this vertex + all the normals of the triangles youve already calculated that relate to this vertex
        vertex_normals[triangle[i][1]] = triangle_normals[i] + vertex_normals[triangle[i][1]]; // B
        vertex_normals[triangle[i][2]] = triangle_normals[i] + vertex_normals[triangle[i][2]]; // C

    }

    //averageing the vertex normals (currenlty a sum of 3 normals)
    for(int i=0; i<vertex_count; i++){
        vertex_normals[i] = vertex_normals[i] * Vector(1/3.0f, 1/3.0f, 1/3.0f);
        vertex_normals[i].normalise();
    }

}

Vector get_TriangleNormal(Vector vA, Vector vB, Vector vC){
    Vector vAB = vB - vA;
    Vector vAC = vC - vA;
    Vector N;
    //N = AB x AC to get the normal?
    vAB.cross(vAC,N);
    N.normalise();
    return N;
}

Hit* PolyMesh::intersection(Ray ray)
{
    //dont want to define the normals till have the verticies have been transformed
    //TODO transform these verticies in the transform method
    setup_triangleNormals_and_vertexNormals();

    Hit *hits = 0;

    int hitCounter = 0;

    for(int i=0; i<triangle_count; i++){
        //initialsing the Barycetric coords (scalar values)
        float u=0.0f, v=0.0f, t = 0.0f;
        //getting the verticies of the triangle
        Vector A(vertex[triangle[i][0]]);
        Vector B(vertex[triangle[i][1]]);
        Vector C(vertex[triangle[i][2]]);

        //cout<<"A.x=" << A.x <<endl; // indegubber says A.x = 1.38136995, BUT it prints it out only 1.38137 WHYYYYY????????????

        bool intersect = MollerTrumbore_algorithm(ray.position, ray.direction, A,B,C, t, u, v);

        if (intersect == true){
            hitCounter ++;

            //create a new hit instance
            Hit *newHit = new Hit;

            newHit->t = t;
            //P=position of the intersection = wA + uB + vC = O + t*D
            //Vertex P = (1-u-v)*A + u*B + v*C; // SHOULD be equal
            Vertex P = ray.position + t*ray.direction;
            newHit->position = P;
            //the object its hitting is the current polymesh?
            newHit->what = this;


            if (smoothing){
                //(1-u-v)*A + u*B + v*C; using barycentric coords to interpolate the values of the vertex normal to calculate the intersection normal
                newHit->normal = (1-u-v)*vertex_normals[triangle[i][0]] + u*vertex_normals[triangle[i][1]] + v*vertex_normals[triangle[i][2]];
                //newHit->normal = vertex_normals[triangle[i][0]];
            }
            else{
                newHit->normal = triangle_normals[i];
            }
            //TODO test how it looks without forcing the normal to face the direction of the camera
//            //return the smallest t and +ve
//            //need to test if the normal is facing towards the camera
//            float dotProduct = newHit->normal.dot(ray.direction);
//            //if the nagle between is +ve then they are facing in the same direction, but you want it to be facing in the opposite direction to face the camera, so negate
//            if (dotProduct>0){
//                newHit->normal.negate();
//            }

              //they all seem to be facing the wrong way lets just try negating them
              newHit->normal.negate();


            //check if entering or exiting
            if(ray.direction.dot(newHit->normal) < 0){ //if angle between is negavtive then ray is outside == entering
                newHit->entering = true;
            }else{
                //cout << "ray is exiting!! and the hit count =" << hitCounter <<endl;
                newHit->entering = false;
            }

            if(hits==0){
               hits = newHit;
            }
            else{
                //if the current hit is bigger than the new hit and its positive then return the smaller of the hits
                if(hits->t > newHit->t && newHit->t > 0){
                    hits = newHit;
                }
            }

        }
    }

    return hits;
}

void PolyMesh::apply_transform(Transform& trans)
{ //passing a pointer to a transform object
     //you only need to transform the verticies, because th traingles are linked by thier index in the array not value.
    for (int i=0; i<vertex_count; i++){
        //trans is a Transform which can be applied to a single vertex at a time
        trans.apply(vertex[i]);
    }

}