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

// Hit is a class to store and maniplulate information about an intersection
// between a ray and an object.

#include "hit.h"

Hit* Hit::free_pool = (Hit *)0;

int Hit::allocated = 0;
int Hit::pool_size = 0;

//overleaoded the new keyword, to create its own little heap of hit objects that it uses.
//I think this helps to make sure that it doesnt create more than 100 hits?
void* Hit::operator new(size_t size)
{
	allocated += 1;

	if (free_pool == 0)
	{
		Hit* pool = new Hit[100];
		free_pool = pool;

		for (int i = 0; i < 99; i += 1)
		{
			pool[i].next = &pool[i+1];
		}
		pool[99].next = 0;

		pool_size += 100;
	}

	Hit* next = free_pool;
    //moving along the free pool to use the next one in the cache
	free_pool = free_pool->next;
    //making sure the next of the free_pool is =0
	next->next = 0;
    //returns the pointer to the remaining pool of hits from the cache
	return next;
}

void Hit::operator delete(void* p)
{
	allocated -= 1;
	((Hit*)p)->next = free_pool;
	free_pool = (Hit*)p;
}

Hit* Hit::add_intoList(Hit* &head, Hit* &newHit){
        Hit *hits = head;
    //check the hit you want to add exists
    if (newHit != 0)
    {
        //check this hit isnt empty
        if ( hits != 0)
        {
            Hit* step = head;
            Hit* prev = 0;
            //starts at the head and steps through the list
            while (step != 0)
            {
                //check if the new one you want to add is less than - if so should be added in before hand
                if (newHit->t < step->t)
                {
                    // if the new hit is in front of the current step, it inserts before it.
                    newHit->next = step;
                    if (prev != 0)
                    {
                        //prev will keep track of the hit before step, so that newHit can be inserted linked in (not creatign a new head, and loing all teh old list)
                        prev->next = newHit;
                    }
                    else
                    {
                        //if there is nothing previous, it becomes the new head of the list
                        hits = newHit;
                    }
                    break;
                }
                prev = step;
                step = step->next;
            }

            if (step == 0)
            {
                //if get to end of the list will isert it at the end
                prev->next = newHit;
                //make sure dont keep the rest of the old hits
                newHit->next = 0;
            }
        }
        else
        {
            //no entries in the list yet
            newHit->next = 0;
            hits = newHit;
        }

        //applys whether a hit point as entering or exiting based on thier order in the hitlist
        Hit* temp = hits;
        bool entering = true;
        while (temp != 0)
        {
            temp->entering = entering;
            entering = !entering;
            temp = temp->next;
        }
    }

    return hits;
}

ostream& operator<<(ostream& os, const Hit& h)
{
  os << "Hit{" <<",[" << h.position.x << "," << h.position.y << "," << h.position.z << "],[" << h.normal.x << "," << h.normal.y << "," << h.normal.z << "]}\n";
  return os;
}
