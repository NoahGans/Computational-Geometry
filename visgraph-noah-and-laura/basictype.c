#include "basictype.h"



/* This function is needed in the priority queue to extract the
   priority of an element.  

   In our case the priority is the distance. 
*/

float getPriority(elemType x) {
  return x.dist;
}


//prints an element
void printElem(elemType x) {
   printf("[");
  printPoint2D(x.p);
  printf(", dist=%.2f]\n", x.dist); 
} 
