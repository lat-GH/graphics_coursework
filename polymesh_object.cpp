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


#include "polymesh_object.h"

using namespace std;

PolyMesh::PolyMesh(char* file, bool smooth)
{
  //cout << file << endl;

  string streamTxt;
  int numLines = 0;

  ifstream fstream;
  fstream.open( file );

  //getting the number of lines in the file
  if (fstream.is_open()){
      while (getline (fstream, streamTxt)) {
          ++ numLines;
      }
  }
  fstream.close( );
  cout << "Number of lines " << numLines<<endl;

  //creating the array based on the file
  string plyFile[numLines];

  fstream.open( file );
  if (fstream.is_open()){
      int i = 0;
        cout << "opened file"<< endl;
        while (getline (fstream, streamTxt)) {
            //cout << streamTxt << endl;
            plyFile[i]=streamTxt;
            ++i;
        }
    }else cout << "Unable to open file";
    fstream.close( );

    //printing out the array
    for (int i = 0; i < numLines; i++){
        cout <<plyFile[i];
    }

    cout << "line1" << plyFile[1]; //YOU ARE HER --- extract out the number of the vertecies


//BEGIN_STAGE_ONE
//END_STAGE_ONE
    next = 0;
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