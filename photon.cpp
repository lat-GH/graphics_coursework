//
// Created by latma on 05/12/2022.
//
#include "photon.h"

Photon::Photon(){
    intensity =  Colour(1.0f, 1.0f, 1.0f); //start with full intensity and then slowly lose intensity as the photon travels through the scene???
    direction = Vector(0.0f, 0.0f, 0.0f);
    position = Vertex(0.0f, 0.0f, 0.0f);
}
Photon::Photon(Vertex pos){
    position = pos;
}

Photon::Photon(Colour col, Vector dir, Vertex pos){
    intensity =  col;
    direction = dir;
    position = pos;
}

//will be populated into the kd tree based on it's Vertex position
//kd tree needs to be able to index it's position
float Photon::operator[] (int i) const{

    if (i==0){return position.x;}
    else if (i==1){return position.y;}
    else if (i==2){return position.z;}
    else{
        cout<< "indexing "<< i <<"a part of Photon's position that doesnt exist?"<<endl;
        throw std::invalid_argument( "received invalid index" );
    }
}

bool Photon::operator == (Photon p) const{
    if(this->position.x == p.position.x && this->position.y == p.position.y && this->position.z == p.position.z ){
        return true;
    }
    else{ return false; }
}

//TODO check if need to overload 'Item' too -  "const Point& Item::point() const" ??