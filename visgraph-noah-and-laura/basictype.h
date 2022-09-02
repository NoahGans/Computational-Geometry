#ifndef BASICTYPE_H
#define BASICTYPE_H


#include <stdio.h>

#include "geom.h"  //define point2D


/* This is the type for an element in the priority queue; When you use
 the priority queue, define this element to contain the data you need.
 In the e.g. below an elemType contains a point p and its distance

 There is no specific field that defines the priority. The priority of
 an element is implemented by a function getPriority(), which you have
 to implement.
*/

typedef struct {
  double dist; 
  point2D p;
  int index; 
  /* can add more stuff if needed */
} elemType; 


/* functions needed in priority queue */
float getPriority(elemType x);

void printElem(elemType x);


#endif
