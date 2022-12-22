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

/* CSG is an object that is built by Constructive Solid Geometry from two sub-objects.*/

using namespace std;

#include "csg_object.h"
#include "hit.h"

//void add_to_output(Hit* &output, Hit* &val);
void action(Hit* &output,bool keep, Hit* &what, bool &out);

CSG::CSG(CSG::Mode p_mode, Object* p_left, Object* p_right)
{
    chosenMode = p_mode;
    obj_A = p_left;
    obj_B = p_right;
}

//called in the trace() of the scene.cpp and should return an orderd list of hits
Hit* CSG::intersection(Ray ray)
{
	Hit* output = 0;
    switch(chosenMode){
        //difference
        case Mode::CSG_DIFF:
        {
            Difference(output, ray);
        }break;

        case Mode::CSG_UNION:
        {
            Union(output, ray);
        }break;
        case Mode::CSG_INTER:
        {
            Intersection(output, ray);
        }break;
    }

	return output;
}

// will perform the action of keeping or discarding intersections and produce the relevant resulting output of hits
void action(Hit* &output, bool keep, Hit* &AorB, bool &out){
    if(keep){
        Hit *AorB_single = AorB;
        //stepping along the list to be looking at the new value
        AorB = AorB->next;
        output = Hit::add_intoList(output, AorB_single);
    }
    else{
        Hit* temp = AorB;
        //stepping along the list to be looking at the new value
        AorB = AorB->next;
        delete temp;
    }

    //flip so if it was inside its now outside
    out = !out;

}


//Calculates the difference between A and B = A - B
void CSG::Difference(Hit* &output, Ray ray){
    Hit *A = obj_A->intersection(ray);
    Hit *B = obj_B->intersection(ray);

    //ASSUME the ray starts outside of both the objects
    bool A_outside = true;
    bool B_outside = true;

    //if there arent any B hits add all the A hits
    if (B == NULL  && A != NULL){
        //there are no more B hits, then add the remaining As
        while(A != NULL){
            Hit *A_single = A;
            A = A->next;
            output = Hit::add_intoList(output,A_single);
        }
        return;
    }
    else if (A == NULL){
        //get rid of any remaining B hits
        while(B != NULL)
        {
            Hit *temp = B;
            B = B->next;
            delete temp;
        }
        return;
    }

    //loop until run out of either As or Bs
    while (A != NULL && B!= NULL){

        if(A_outside && B_outside){
            if (A->t < B->t){
                action(output, true, A,A_outside);//keep A
            }
            else if (A->t > B->t){
                //discarding B is the same as not adding it to the output,
                //but also need to increment so looking at the next one
                action(output, false, B,B_outside);//discard B
            }
            else if (A->t == B->t){
                action(output, false, B,B_outside);//discard B
            }
        }

        else if(!A_outside && B_outside){
            if (A->t < B->t){
                action(output, true, A,A_outside);//keep A
            }
            else if (A->t > B->t){
                action(output, true, B,B_outside);//keep B
            }
            else if (A->t == B->t){
                action(output, true, A,A_outside);//keep A
            }
        }
        else if(!A_outside && !B_outside){
            if (A->t < B->t){
                action(output, false, A,A_outside);//discard A
            }
            else if (A->t > B->t){
                action(output, true, B,B_outside);//keep B
            }
            else if (A->t == B->t){
                action(output, false, A,A_outside);//discard A
            }
        }
        else if(A_outside && !B_outside){
            if (A->t < B->t){
                action(output, false, A,A_outside);//discard A
            }
            else if (A->t > B->t){
                action(output, false, B,B_outside);//discard B
            }
            else if (A->t == B->t){
                action(output, false, B,B_outside);//discard B
            }
        }
    }
    //there are no more B hits, then add the remaining As
    while(A != NULL){
        Hit *A_single = A;
        A = A->next;
        output = Hit::add_intoList(output,A_single);
    }
    //get rid of any remaining B hits
    while(B != NULL)
    {
        Hit *temp = B;
        B = B->next;
        delete temp;
    }


}

void CSG::Union(Hit* &output, Ray ray){

    Hit *A = obj_A->intersection(ray);
    Hit *B = obj_B->intersection(ray);

    //ASSUME the ray starts outside of both the objects
    bool A_outside = true;
    bool B_outside = true;

    //if there arent any hits dont do anything
    if (A == NULL && B == NULL ){
        return;
    }
    else if( A == NULL && B!= NULL){
        //if there are no more As add the remaining Bs
        while(B != NULL)
        {
            Hit *B_single = B;
            B = B->next;
            output = Hit::add_intoList(output, B_single);
        }
    }
    else if( A != NULL && B == NULL){
        //there are no more B hits, then add the remaining As
        while(A != NULL){
            Hit *A_single = A;
            A = A->next;
            output = Hit::add_intoList(output,A_single);
        }
    }

    //loop until run out of either As or Bs
    while (A != NULL && B!= NULL){

        if(A_outside && B_outside){
            if (A->t < B->t){
                action(output, true, A,A_outside);//keep A
            }
            else if (A->t > B->t){
                action(output, true, B,B_outside);//keep B
            }
            else if (A->t == B->t){
                action(output, true, A,A_outside);//keep A
            }
        }

        else if(!A_outside && B_outside){
            if (A->t < B->t){
                action(output, false, A,A_outside);//discard A
            }
            else if (A->t > B->t){
                action(output, false, B,B_outside);//discard B
            }
            else if (A->t == B->t){
                action(output, false, B,B_outside);//discard B
            }
        }
        else if(!A_outside && !B_outside){
            if (A->t < B->t){
                action(output, false, A,A_outside);//discard A
            }
            else if (A->t > B->t){
                action(output, false, B,B_outside);//discard B
            }
            else if (A->t == B->t){
                action(output, true, A,A_outside);//keep A
            }
        }
        else if(A_outside && !B_outside){
            if (A->t < B->t){
                action(output, true, B,B_outside);//keep B
            }
            else if (A->t > B->t){
                action(output, true, A,A_outside);//keep A
            }
            else if (A->t == B->t){
                action(output, false, A,A_outside);//discard A
            }
        }
    }

    //there are no more B hits, then add the remaining As
    while(A != NULL){
        Hit *A_single = A;
        A = A->next;
        output = Hit::add_intoList(output,A_single);
    }
    //if there are no more As add the remaining Bs
    while(B != NULL)
    {
        Hit *B_single = B;
        B = B->next;
        output = Hit::add_intoList(output, B_single);
    }

}

void CSG::Intersection(Hit *&output, Ray ray) {
    Hit *A = obj_A->intersection(ray);
    Hit *B = obj_B->intersection(ray);

    //ASSUME the ray starts outside of both the objects
    bool A_outside = true;
    bool B_outside = true;

    //if it doesnt hit both, dont want any
    if (A == NULL || B == NULL ){
        while(A != NULL){
            Hit *temp = A;
            A = A->next;
            delete temp;
        }
        //get rid of any remaining B hits
        while(B != NULL)
        {
            Hit *temp = B;
            B = B->next;
            delete temp;
        }
        return;
    }

    //loop until run out of either As or Bs
    while (A != NULL && B!= NULL){

        if(A_outside && B_outside){
            if (A->t < B->t){
                action(output, false, A,A_outside);//discard A
            }
            else if (A->t > B->t){
                //discarding B is the same as not adding it to the output,
                //but also need to increment so looking at the next one
                action(output, false, B,B_outside);//discard B
            }
            else if (A->t == B->t){
                action(output, true, A,A_outside);//keep A
            }
        }

        else if(!A_outside && B_outside){
            if (A->t < B->t){
                action(output, false, A,A_outside);//discard A
            }
            else if (A->t > B->t){
                action(output, true, B,B_outside);//keep B
            }
            else if (A->t == B->t){
                action(output, false, A,A_outside);//discard A
            }
        }
        else if(!A_outside && !B_outside){
            if (A->t < B->t){
                action(output, true, A,A_outside);//keep A
            }
            else if (A->t > B->t){
                action(output, true, B,B_outside);//keep B
            }
            else if (A->t == B->t){
                action(output, true, A,A_outside);//keep A
            }
        }
        else if(A_outside && !B_outside){
            if (A->t < B->t){
                action(output, true, A,A_outside);//keep A
            }
            else if (A->t > B->t){
                action(output, true, B,B_outside);//keep B
            }
            else if (A->t == B->t){
                action(output, false, B,B_outside);//discard B
            }
        }
    }

    //get rid of any remainign As
    while(A != NULL){
        Hit *temp = A;
        A = A->next;
        delete temp;
    }
    //get rid of any remaining B hits
    while(B != NULL)
    {
        Hit *temp = B;
        B = B->next;
        delete temp;
    }
}



void CSG::apply_transform(Transform& transform)
{
    obj_A->apply_transform(transform);
    obj_B->apply_transform(transform);
}

