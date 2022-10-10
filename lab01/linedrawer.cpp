/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

/* This is the code you will need to replace for Lab 1.
 *
 * It contains two simple implementations that loop over the longest axis adding the gradient to the position on the other axis at each step.
 * The objective is for you to remove the use of floating point variables and work entirely with integers.
 * You should use Bresenhams Line Algorithm to achieve this.
 */

#include <iostream>
#include "linedrawer.h"
#include "math.h"
//ASSUMING s=starting e=ending so sx is the starting x point

//OLD x
//int draw_x_line(FrameBuffer *fb, int sx, int sy, int ex, int ey)
//{
//  int dir = 1;
//  if (sx > ex)
//  {
//    dir = -1;
//  }
//
//  int   x     = sx;
//  float y     = (float)sy;
//  float slope = ((float)ey-(float)sy)/((float)ex-(float)sx);
//        slope = slope * dir;
//
//  while (x != ex)
//  {
//    fb->plotPixel(x, (int)y, 1.0f, 0.0f, 0.0f);
//
//    y += slope;
//
//    x += dir;
//  }
//
//  return 0;
//}
//NEW x
int draw_x_line(FrameBuffer *fb, int xo, int yo, int xl, int yl)
//iterating through all the points around in the center circle o and the outter of the circle l
{

    int dirx = 1;
    if (xo > xl) //if the outter point is larger than the inner point then you want to decrese to draw Right to Left
    {
        dirx = -1;
    }
    int wy = yo;
    int x = xo;

    int dy = abs(yl - yo);
    int dx = abs(xl - xo);
    int fy = dy/2;

    int diry = 1;
    if (yo > yl){
        diry = -1;
    }

    while (x != xl){
        fb->plotPixel(x, (int)wy, 1.0f, 1.0f, 0.0f);
        x += dirx;

        fy += dy; // same as stepping by m

        if(fy > dx){
            wy += diry;
            fy -= dx;
        }
    }

    return 0;
}
//OLD y
int draw_y_line(FrameBuffer *fb, int sx, int sy, int ex, int ey)
{
  int dir = 1;
  if (sy > ey)
  {
    dir = -1;
  }

  int   y     = sy;
  float x     = (float)sx;
  float slope = ((float)ex-(float)sx)/((float)ey-(float)sy);
        slope = slope * dir;

  while (y != ey)
  {
    fb->plotPixel((int)x, y, 1.0f, 1.0f, 1.0f);

    x += slope;

    y += dir;
  }

  return 0;
}
//NEW
//int draw_y_line(FrameBuffer *fb, int xo, int yo, int xl, int yl)
//{
//    int dir = 1; //increment
//    if (yo > yl)
//    {
//        dir = -1;
//    }
//    int wx = xo;
//    int y = yo;
//
//    int dx = xl - xo;
//    int dy = yl - yo;
//    int fx = dx/2;
//
//    while (y <= yl){
//        fb->plotPixel(y, (int)wx, 1.0f, 1.0f, 1.0f);
//        y += dir;
//
//        fx += dx; // same as stepping by m
//
//        if(fx > dy){
//            wx += 1;
//            fx -= dy;
//        }
//    }
//
//    return 0;
//}


int draw_line(FrameBuffer *fb, int sx, int sy, int ex, int ey)
{
//if the coordinates are exactly the same
    if ((sx == ex) && (sy==ey))
  {
    return fb->plotPixel(sx, sy, 1.0f, 1.0f, 1.0f);
    
  } // if the magnitude of the x is bigger than y
    // if x is the longest axis
    else if (((ex-sx)* (ex-sx)) >= ((ey-sy)* (ey-sy)))
  {
    return draw_x_line(fb, sx, sy, ex, ey);
    
  } //if y is the longest axis
    else
  {
    return draw_y_line(fb, sx, sy, ex, ey);
  }
}
