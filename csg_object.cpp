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

void add_to_output(Hit* &output, Hit* &val);
void action(Hit* &output,bool keep, Hit* &what, bool &out);

CSG::CSG(CSG::Mode p_mode, Object* p_left, Object* p_right)
{
    chosenMode = p_mode;
    obj_A = p_left;
    obj_B = p_right;
	//next = (Object*)0; TODO wtf does this mean???
}

//called in the trace() of the scene.cpp and should return an orderd list of hits
Hit* CSG::intersection(Ray ray)
{
	Hit* output = 0;
    switch(chosenMode){
        //difference
        case Mode::CSG_DIFF:
        {
            //cout<<"diff case"<<endl;
            Difference(output, ray);
        }break;

//        case Mode::CSG_UNION:
//        {
//
//        }break;
//        case Mode::CSG_INTER:
//        {
//
//        }break;
    }

	return output;
}

//Calculates the difference between A and B = A - B
void CSG::Difference(Hit* &output, Ray ray){
    Hit *A_hits = obj_A->intersection(ray);
    Hit *B_hits = obj_B->intersection(ray);

    //ASSUME the ray starts outside of both the objects
    //TODO set these base on thier entering values
    bool A_outside = true;
    bool B_outside = true;

    Hit *A = A_hits;
    Hit *B = B_hits;
    //if there arent any hits dont do anything
    if (A == NULL || B == NULL ){
        return;
    }

    while (A->next != 0 || B->next != 0){
        if(A_outside && B_outside){
            if (A->t < B->t){
                action(output, true, A,A_outside);//keep A
            }
            if (A->t > B->t){
                //discarding B is the same as not adding it to the output,
                //but also need to increment so looking at the next one
                action(output, false, B,B_outside);//discard B
            }
            if (A->t == B->t){
                action(output, false, B,B_outside);//discard B
            }
        }

        else if(!A_outside && B_outside){
            if (A->t < B->t){
                action(output, true, A,A_outside);//keep A
            }
            if (A->t > B->t){
                action(output, true, B,B_outside);//keep B
            }
            if (A->t == B->t){
                action(output, true, A,A_outside);//keep A
            }
        }
        else if(!A_outside && !B_outside){
            if (A->t < B->t){
                action(output, false, A,A_outside);//discard A
            }
            if (A->t > B->t){
                action(output, true, B,B_outside);//keep B
            }
            if (A->t == B->t){
                action(output, false, A,A_outside);//discard A
            }
        }
        else if(A_outside && !B_outside){
            if (A->t < B->t){
                action(output, false, A,A_outside);//discard A
            }
            if (A->t > B->t){
                action(output, false, B,B_outside);//discard B
            }
            if (A->t == B->t){
                action(output, false, B,B_outside);//discard B
            }
        }
    }

    //there are no more B hits, then add the remaining As
    if(B->next == 0 ){
        while(A->next != 0){
            add_to_output(output, A);
            A = A->next;
        }
    }


}

void add_to_output(Hit* &output, Hit* &val){
    Hit *stepper = output;
    if(output != 0){
        while(stepper->next != 0){
            stepper = stepper->next;
        }
        stepper->next = val;

    }else{
        output = val;
    }
}



void action(Hit* &output,bool keep, Hit* &what, bool &out){
    if(keep){
        add_to_output(output, what);
    }
    what = what->next;
    out = !out;

}

//            if (A->t < B->t){
//                add_to_output(output, A);
//                //stepping along to now look at the next value of A
//                A = A->next;
//                A_outside = !A_outside;
//            }
//            if (A->t > B->t){
//                //discarding B is the same as not adding it to the output,
//                //but also need to increment so looking at the next one
//                B = B->next;
//                B_outside = !B_outside;
//            }
//            if (A->t == B->t){
//                B = B->next;
//                B_outside = !B_outside;
//            }
//        }


void CSG::apply_transform(Transform& transform)
{
//BEGIN_STAGE_TWO
//END_STAGE_TWO
}

