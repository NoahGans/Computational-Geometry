
/* **************************************** */
/* This program applies graham_scan to a set of points to find the 
convex hull. It has plenty of helper funtions that aid in comparing 
two points to another point. These funtions are all called by 
viewPoints.cpp


Noah Gans
10/4/21
*/



#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm> // std::sort
#include <math.h>    /* atan2 */
using namespace std;

//assumes global array points is allocated to hold n points.
//this functions initializes points[] with  n random points
//The points are in the range (0,0) to (WINSIZE,WINSIZE)

/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
double signed_area2D(point2D a, point2D b, point2D c)
{
  double dArea = ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) / 2.0;
  return dArea;
}

/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r)
{

  if ((r.y - q.y) * (q.x - p.x) == (q.y - p.y) * (r.x - q.x))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly(point2D a, point2D b, point2D c)
{
  if (signed_area2D(a, b, c) > 0)
  {
    return 1;
  }
  return 0;
}
/* **************************************** */
/* return true if c falls on aray a,b. Prevents coliner catching point behind */
bool points_inline(point2D a, point2D b, point2D c)
{
  if (a.x - b.x < 0)
  {
    if (c.x > a.x)
    {
      return true;
    }
    return false;
  }
  else
  {
    if (c.x < a.x)
    {
      return true;
    }
    return false;
  }
}

/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2D a, point2D b, point2D c)
{
  if ((signed_area2D(a, b, c) > 0) || ((int)(signed_area2D(a, b, c) == 0)))
  {
    return 1;
  }
  return 0;
}
/* **************************************** */
/* getLowPoint returns the lowest y point, and if two are the same returns the lower x of the two */
point2D getLowPoint(vector<point2D> p)
{
  int minY = p[0].y;
  point2D lowPoint = p[0];
  for (int i = 0; i < p.size(); i++)
  {

    if (p[i].y < minY)
    {
      minY = p[i].y;
      lowPoint = p[i];
    }
    if (p[i].y == minY)
    {
      if (p[i].x < lowPoint.x)
      {
        lowPoint = p[i];
      }
    }
  }
  return lowPoint;
}

/* **************************************** */
/* return angle from one point to another. It uses
trigonmetry on the slope from the two points, and converts
it into degrees. If y is negatve wrt the lowPoint point,
that degrees is negative, but this should never happen 
because lowPoint is the lowest point */
double get_angle_2points(point2D lowPoint, point2D point)
{

  int changeY = point.y - lowPoint.y;
  int changeX = point.x - lowPoint.x;

  float angleInDegrees = atan2(changeY, changeX) * 180 / 3.141;

  angleInDegrees *= -1; // Y axis is inverted in computer windows, Y goes down, so invert the angle.

  return angleInDegrees;
}

/* **************************************** */
/* This is a comparator for c++'s stable sort funtion 
it compares the angles of two points and returns the 
comparison*/
bool sort_funtion(vector<double> i, vector<double> j)
{

  return (i[0] > j[0]);
}

/* **************************************** */
/* Sort points sorts all the points by their angle
wrt lowPoint. It does this by sorting a vector of vectors.
The internal vector holds the index of the point in p vector
and the angle wrt lowPoint. This vector is sorted by angle, than
iterated through, and the indexes are used to pull the sorted
order out of p and placed in sortedPoints*/
vector<point2D> sortPoints(vector<point2D> p)
{
  point2D lowPoint = getLowPoint(p);
  vector<vector<double> > angleList;
  vector<point2D> sortedPoints;

  for (int i = 0; i < p.size(); i++)
  {
    vector<double> anglePointer;
    anglePointer.push_back(get_angle_2points(lowPoint, p[i]));

    anglePointer.push_back((double)i);
    angleList.push_back(anglePointer);
  }
  std::stable_sort(angleList.begin(), angleList.end(), sort_funtion);
  sortedPoints.push_back(lowPoint);//add lowestpoint first
  for (int i = 1; i < angleList.size(); i++)
  {
    if (p[angleList[i][1]].x != lowPoint.x || p[angleList[i][1]].y != lowPoint.y)//if lowest point, don't add.
    {
      sortedPoints.push_back(p[angleList[i][1]]);
    }
  }

  return sortedPoints;
}

/* **************************************** */
/* left handles the case when the next viewd point in gramScan is
to the left or colinier with the privious two points. If it is 
strickly left it simply adds the current point to the stack. If 
it is not, coliner, than there are a few cases that can happen. First
a check that the points are inline is done to prevent a coliner point
that is behind the two others. Adding this check prevented a bug that 
resulted in an incorect convex hull. If the third point is coliner and not
behind the two points it checks which of the coliner points has a larger
y, and that one is selected for the stack and kept. This is done for the
vertical line case. If both y's are the same than the point with the higher
x is selected as the point for the stack. This is how a new point is selcted 
to be added to the stack if they are coliner. After all of this, the stack 
is returned. 
*/
vector<point2D> left(vector<point2D> stack, point2D currentPoint)
{

  if (left_strictly(stack[stack.size() - 2], stack[stack.size() - 1], currentPoint) > 0)
  { //if only left of add it to the stack and on to next point
    //cout << "ssdsds" << "\n";

    stack.push_back(currentPoint);
  }
  else
  { //coliner

    if (points_inline(stack[stack.size() - 2], stack[stack.size() - 1], currentPoint))
    {
      if (currentPoint.y > stack[stack.size() - 1].y)
      { //chose higher y
        //cout << "one " << stack[stack.size() - 2].x << ", " << stack[stack.size() - 2].y << " to " << stack[stack.size() - 1].x << ", " << stack[stack.size() - 1].y << " Coliner with " << sortedPoints[i].x << ", " << sortedPoints[i].y << "\n";
        stack[stack.size() - 1] = currentPoint; //swap
      }
      else if (currentPoint.x > stack[stack.size() - 1].x && currentPoint.y == stack[stack.size() - 1].y)
      { //choose higher x
        //cout << "two " << stack[stack.size() - 2].x << ", " << stack[stack.size() - 2].y << " to " << stack[stack.size() - 1].x << ", " << stack[stack.size() - 1].y << " Coliner with " << sortedPoints[i].x << ", " << sortedPoints[i].y << "\n";
        stack[stack.size() - 1] = currentPoint;
      }
    }
  }

  return stack;
}


/* **************************************** */
/* backTrack deals with the removal of points from the stack until
a set of points that makes the currently observed point left of. It 
immidiatly pops the last value off the stack becase it is alrready on the
internal, and will not be included in the hull. Then the stack is iterated
backwards through until a point combination makes the currentPoint left of
or coliner. As it goes throuhg this it pops off points from the stack. Once it
finds that point that makes current point left or coliner, that point is added
to the stack if it is left of, and is replaced by current point if it is coliner.*/
vector<point2D> backTrack(vector<point2D> stack, point2D currentPoint)
{
  stack.pop_back(); //last point added not needed becase right of
  for (int j = stack.size() - 1; j > 0; j--)
  { // go back through the stack starting at size and check if still left
    if (left_on(stack[j - 1], stack[j], currentPoint) > 0)
    { //if it's left, were good to stop and add the point to the stackmake
      if (left_strictly(stack[j - 1], stack[j], currentPoint) > 0)
      {
        stack.push_back(currentPoint);
        break;
      }
      else
      {
        stack[j] = currentPoint;
        break;
      }
    }
    stack.pop_back();
  }
  return stack;
}

/* **************************************** */
/*
Grahm scan goes through the sorted points and either adds a point
to the stack which is the hull, or backtracks until it finds a point 
that is external. If the point is left or on, it calls left with the
current point and the stack. If it is not, it calls backtrack, and finds
the last point that makes current point left or on. The stack is 
maintained through the left funtion and backtrack funtion. The 
first two points in the stack are the lowest point and lowest angle point.
After the stack has been built, the starting point, lowest point, is added 
to the end. This complets the convex hull, and the stack is returned and 
represents the hull.
*/
vector<point2D> graham_scan(vector<point2D> p)
{

  printf("computing hull..");
  cout << "\n";

  vector<point2D> sortedPoints = sortPoints(p);
  vector<point2D> stack;
  stack.push_back(sortedPoints[0]); //push lowest point
  stack.push_back(sortedPoints[1]); //push point with lowest angle next

  //go through the points
  for (int i = 2; i < sortedPoints.size(); i++)
  {
    if (stack[stack.size() - 1].x == 250 && stack[stack.size() - 1].y == 391){
      cout << stack[stack.size() - 2].x << ", " << stack[stack.size() - 2].y << "\n";
      stack.pop_back();
    }
    if (left_on(stack[stack.size() - 2], stack[stack.size() - 1], sortedPoints[i]) > 0)
    { //if left of or coliner
      stack = left(stack, sortedPoints[i]);
    }
    else
    { //backtrack
      stack = backTrack(stack, sortedPoints[i]);
    }
    

  }

  stack.push_back(stack[0]);
  printf("Hull\n");
  for (int i = 0; i < stack.size(); i++)
  {
    printf("%d, %d\n", stack[i].x, stack[i].y);
  }
  printf("\n");
  printf("..done\n");
  return stack;
}
