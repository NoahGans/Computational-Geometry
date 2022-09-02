#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std; 


/* print point */
void printPoint2D(point2D p) {
  printf("(x=%f,y=%f) ", p.x, p.y);
}

/* print obstacle */
void printObstacle(obstacle o) {
  printf("[");
  for (point2D p : o.verticies) {
    printPoint2D(p);
    printf(", ");
  }
  printf("]");
}

/**
 * print visibility graph.
 * prints verticies, then prints the neighboors of each verticies (as well as
 * the index they are actually identified as, which is denoted by [index])
 */
void printVisibilityGraph(visGraph v) {
  printf("verticies: [");
  for (point2D p : v.verticies) {
    printPoint2D(p);
    printf(",");
  }
  printf("]\nadjacency list:\n");
  for(size_t i = 0; i < v.verticies.size(); i++) {
    printf("[%d]", static_cast<int>(i));
    printPoint2D(v.verticies[i]);
    printf(": ");
    for (int p_index : v.adjacencyList[i]) {
      printPoint2D(v.verticies[p_index]);
    }
    printf("\n");
  }
  printf("\n");
}

/* return euclidean distance between points a and b */
double distance(point2D a, point2D b) {
  // the distance formula
  return sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

/**
 * returns the signed area of triangle abc. The area is positive if c
 * is to the left of ab, and negative if c is to the right of ab
 */
int signedArea2D(point2D a, point2D b, point2D c) {
  return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r) {
  return (signedArea2D(p, q, r) == 0); 
}

/* return true if point c is between points a and b, else false */
bool between(point2D a, point2D b,  point2D c) {
  if (a.x != b.x) {
    // if not horizontal, compare x coord
    return ((a.x <= c.x) && (c.x <= b.x)) || ((a.x >= c.x) && (c.x >= b.x));
  } else { 
    // compare y coord
    return ((a.y <= c.y) && (c.y <= b.y)) || ((a.y >= c.y) && (c.y >= b.y)); 
  }
}

/* assign the coords of point p to point q */
void  assign2D(point2D p, point2D q) {
  p.x = q.x;
  p.y = q.y;
}

/* get midpoint between a and b, return as point */
point2D getMidPoint(point2D a, point2D b){
  point2D midPoint = { (a.x + b.x) / 2.0, (a.y + b.y) / 2.0 };
  return midPoint;
}

/* uses midpoint to get point incredibly close to point a */
point2D getProxy(point2D a, point2D midpoint){
  point2D proxy = { (a.x + ((midpoint.x - a.x) / 100)), (a.y + ((midpoint.y - a.y) / 100)) };
  printPoint2D(proxy);
  cout << "====================================================";
  return proxy;
}

/**
 * Helper for segSegInt when the segments ab and cd are parallel
 * returns character code for type of intersection
 *   'e' : The segments collinearly overlap, sharing a point; 'e' stands for 'edge.'
 *   'v' : An endpoint of one segment is on the other segment, but 'e' doesn't hold; 'v' stands for 'vertex.'
 *   '1' : The segments intersect properly (i.e., they share a point and neither 'v' nor 'e' holds); ' 1' stands for TRUE.
 *   '0' : The segments do not intersect (i.e., they share no points); '0' stands for FALSE.
 */
char parallelInt(point2D a, point2D b, point2D c, point2D d, point2D p) {
  // if they aren't collinear, do not intersect
  if (!collinear(a, b, c)) { return '0'; }

  // collinear, so point of intersection is determined (if exists)
  if (between(a, b, c)) {
    assign2D(p, c);
    return 'e';
  }
  
  if (between(a, b, d)) {
    assign2D(p, d);
    return 'e';
  } 
  
  if (between(c, d, a)) {
    assign2D(p, a);
    return 'e';
  }
  
  if (between(c, d, b)) {
    assign2D(p, b);
    return 'e';
  }
  // collinear and parallel but did not intersect
  return '0';
}

/**
 * Determines how ab and cd intsesect (if they do) when parallel, as well as calculate point of intersection. Helper for isVisible. 
 * returns character code for type of intersection
 *   'e' : The segments collinearly overlap, sharing a point; 'e' stands for 'edge.'
 *   'v' : An endpoint of one segment is on the other segment, but 'e' doesn't hold; 'v' stands for 'vertex.'
 *   '1' : The segments intersect properly (i.e., they share a point and neither 'v' nor 'e' holds); ' 1' stands for TRUE.
 *   '0' : The segments do not intersect (i.e., they share no points); '0' stands for FALSE.
 */
char segSegInt(point2D a, point2D b, point2D c, point2D d, point2D p) {
  // The two parameters of the parametric eqns
  double s, t;
  // Numerator and denominator of equations
  double num, denom;
  // Return char characterizing intersection
  char code = '?';

  denom = a.x * (double)(d.y - c.y) + b.x * (double)(c.y - d.y) + d.x * (double)(b.y - a.y) + c.x * (double)(a.y - b.y);

  //If denom is zero, then segments are parallel: handle separately
  if (denom == 0.0) {
    return parallelInt(a, b, c, d, p);
  }
  
  num = a.x * (double)(d.y - c.y) + c.x * (double)(a.y - d.y) + d.x * (double)(c.y - a.y);
  
  if ((num == 0) || (num == denom)) { code = 'v'; }
  s = num / denom;

  num = -(a.x * (double)(c.y - b.y) + b.x * (double)(a.y - c.y) + c.x * (double)(b.y - a.y));

  if ((num == 0) || (num == denom)) { code = 'v'; }
  t = num / denom;

  if ((0 < s) && (s < 1) && (0 < t) && (t < 1)) { code = '1'; }
  
  else if ((0 > s) || (s > 1) || (0 > t) || (t > 1)) { code = '0'; }

  p.x = a.x + s * (b.x - a.x);
  p.y = a.y + s * (b.y - a.y);

  return code;
}

/**
 * Takes a point q and checks if it is inside polygon P. Helper for isVisible
 * returns char code depending on type of interior
 *   'i' : q is strictly interior
 *   'o' : q is strictly exterior (outside)
 *   'e' : q in on an edge, but not an endpoint
 *   'v' : q is a vertex
 */
char inPoly(point2D q, obstacle P)
{
  // number of right edge/ray crossings
  int Rcross = 0;
  // number of left edge/ray crossings
  int Lcross = 0;
  // flags indicating the edge strads the x axis
  bool Rstrad, Lstrad;

  // create a copy of polygon the has point q as the origin of obstacle P
  vector<point2D> current_verticies;
  for (size_t i = 0; i < P.verticies.size(); i++) {
    point2D temp = {P.verticies[i].x - q.x, P.verticies[i].y - q.y};
    current_verticies.push_back(temp);
  }
  
  // size of vector as int
  int n = static_cast<int> (current_verticies.size());
  // For each edge e in the obstacle, see if crosses rays
  for (int i = 0; i < n; i++) {
    // First check if q = (0, 0) is a vertex
    if (current_verticies[i].x == 0 && current_verticies[i].y == 0) { return 'v' ; }
    // i1 is point index, like i
    // il = i—1 mod n (this accounts for wrap around from last point in obstacle back to begining)
    int i1 = (i + n - 1) % n;
	    
    // Check if e straddles x axis, with bias above/below
    Rstrad = ( current_verticies[i].y > 0 ) != ( current_verticies[i1].y > 0 );
    Lstrad = ( current_verticies[i].y < 0 ) != ( current_verticies[i1].y < 0 );
    if ( Rstrad || Lstrad ) {
      // stradels x axis. Compute intersection of edge with x axis
      int x_intersect = (current_verticies[i].x * current_verticies[i1].y - current_verticies[i1].x * current_verticies[i].y) / (double)(current_verticies[i1].y - current_verticies[i].y);
      if (Rstrad && x_intersect > 0) Rcross++;
      if (Lstrad && x_intersect < 0) Lcross++;
    } // end straddle computation
  } // end for
  
  //q on an edge if L/Rcross counts are not the same parity
  if ( ( Rcross % 2 ) != (Lcross % 2 ) ) { return 'e' ; }
  // q inside iff an odd number of crossings
  if ( (Rcross % 2) == 1 ) { return 'i' ; }
  // otherwise, q must be strictly outside the polygon
  else { return 'o' ; }
}

/**
 * Determine if a is visible from b (or vice vera), relative to all obstacles in a space
 * returns true iff a is visible from b, false otherwise
 */
bool isVisible(point2D a, point2D b, vector<obstacle> obstacles) {
  // iterate over the obstacles
  for (size_t obst_index = 0; obst_index < obstacles.size(); obst_index++) {
    // flag if need to check if line ab is in an obstacle. only need to check when both points are on the same obstacle
    int need_check_inside = 0;
    vector<point2D> current_verticies = obstacles[obst_index].verticies;
    size_t size = current_verticies.size();
    // iterate over the verticies of an obstacle
    for (size_t point_index = 0; point_index < size; point_index++) {
      point2D point_intersect;
      point2D p, q;
      // edges of obstacle are p and q, overlaps to begining of array to complete obstacle
      if (point_index != size - 1) {
        p = current_verticies[point_index];
	      q = current_verticies[point_index + 1];
      } 
      else {
        // overlap things
        p = current_verticies[size - 1];
        q = current_verticies[0];
      }

      // increment counter when a and/or b is in a specific obstacle
      if( (a == p) || (a == q) ){
        need_check_inside ++;
      } 
      if( (b == p) || (b == q) ){
        need_check_inside ++;
      }

      // check if edge ab intersects edge of obstacle pq
      char intersect_type = segSegInt(a, b, p, q, point_intersect);
      // b is not visible from a iff there is a proper intersection (collinear and on endpoint are considered visible)
      if (intersect_type == '1') {
        return 0;
      }
    }
    
    // if reached this point, line ab had no proper intersections with the polygons edges
    // if both a and b are on the obstacle, need to check if ab is in the interior of obstacle (which would mean not visible)
    // get midpoint of ab and check if that point lies properly with in the polygon
    // midpoint guaranteed to be inside if whole line is inside (and vice versa). If not, contradiction because would
    // have intersected somewhere 

    // counted 4 times because, if on obstacle points would be double counted
    if (need_check_inside == 4){
      //cout << "Calling in poly funtion\n\n\n";
      point2D midPoint = getMidPoint(a, b);
      //cout << midPoint.x << ", " << midPoint.y << "\n\n";
      //cout << inPoly(midPoint, obstacles[obst_index]) << "==\n";
      // b is not visible from a iff ab midpoint properly inside polygon
      if (inPoly(midPoint, obstacles[obst_index]) == 'i'){
        return 0;
      }
      point2D proxy = getProxy(a, midPoint);
      //cout << midPoint.x << ", " << midPoint.y << "\n\n";
      //cout << inPoly(midPoint, obstacles[obst_index]) << "==\n";
      // b is not visible from a iff ab midpoint properly inside polygon
      if (inPoly(proxy, obstacles[obst_index]) == 'i'){
        return 0;
      }

    }
  }
  // if reached this point, b is visible from a
  return 1;
}

/**
 * helper function to get all possible verticies.
 * includes all verticies of obstacles as well as start and end points.
 * NOTE THAT START IS ALWAYS FIRST AND END IS ALWAYS SECOND
 */
vector<point2D> getVerticies(vector<obstacle> obstacles, point2D start, point2D end){
  vector<point2D> verticies;
  // put start and end into vertex list first
	verticies.push_back(start);
	verticies.push_back(end);
  for (size_t i = 0; i < obstacles.size(); i++) {
    obstacle obsticleToObserve = obstacles[i];
    for (size_t j = 0; j < obsticleToObserve.verticies.size(); j++) {
      point2D pointToAdd = obsticleToObserve.verticies[j];
      verticies.push_back(pointToAdd);
    }
  }
  return verticies;
}

/**
 * construct visibility graph.
 * create and edge between u and v if u is visible from v and vice versa
 */
visGraph constructVisGraph(vector<obstacle> obstacles, point2D start, point2D end){
  visGraph toReturn;
	toReturn.verticies = getVerticies(obstacles, start, end);
  vector<vector<int> > vec(toReturn.verticies.size());
	toReturn.adjacencyList = vec;
  for (size_t i = 0; i < toReturn.verticies.size(); i++) {
    for (size_t j = 0; j < toReturn.verticies.size(); j++) {
      // if the verticies are not the same, check if visible and add to adjacency list if they are
      if (!(toReturn.verticies[i] == toReturn.verticies[j])) {
        if (isVisible(toReturn.verticies[i], toReturn.verticies[j], obstacles)) {
          toReturn.adjacencyList[i].push_back(j);
        }
      }
    }
  }
  return toReturn;
}

/**
 * check if point p is inside and obstacle.
 * return false if p is in an obstacle, true otherwise
 * used to validate user input
 */
bool isValidPointInsert(point2D p, vector<obstacle> obstacles) {
  for (obstacle o : obstacles) {
    // if inside an obstacle in ANY WAY, not a valid insert
    if (inPoly(p, o) != 'o') {
      return 0;
    }
  }
  return 1;
}

/**
 * checks if an edge intersects an obstacle in ANY way.
 * helper function for validating addition to obstacle.
 * similar to isVisible
 */
bool doesIntersect(vector<point2D> edge, vector<obstacle> obstacles) {
  point2D a = edge[0];
  point2D b = edge[1];
  // for each obstacle, check that no edges intersect ab in ANY way
  for (obstacle comp : obstacles) {
    int size = static_cast<int> (comp.verticies.size());
    // iterate over the verticies of an obstacle
    for (int point_index = 0; point_index < size; point_index++) {
      // allows for next point to wrap around
      int next_point = (point_index + size - 1) % size;
      point2D point_intersect;
      point2D p = comp.verticies[point_index];
      point2D q =comp.verticies[next_point];
      // skip if any points are the same
      if (a == p || a == q || b==p || b == q) {
        continue;
      }
      // return true if intersects in ANY way
      if (segSegInt(a, b, p, q, point_intersect) != '0') {
        return 1;
      }
    }
  }
  // if reached, no intersections
  return 0;
}

/**
 * check if an obstacle is an acceptable input.
 * used to validate user input. the user input is the last point in o, all other verticies
 * are assumed to ALREADY be valid.
 * valid is defined as not creating an overlapping edge and not containg the start
 * or end point in an object. User is also not allowed to place and obstacle inside another
 * obstacle, but this is assumed to be already validated.
 */
bool isValidAdditionToObstacle(obstacle o, vector<obstacle> obstacles, point2D s, point2D e) {
  // check that new obstacle will not have start or end point inside object
  if (inPoly(s, o) != 'o' || inPoly(e, o) != 'o') {
    return 0;
  }

  // if there is only one point in the object, no need to check for crossed edges
  if (o.verticies.size() == 1) {
    return 1;
  }

  size_t userInputIndex = o.verticies.size() - 1;
  // need to check the two new edges created by the user
  // this is the edge from the last node to the one before is, and the wraparound edge created
  vector<point2D> lastEdge = {o.verticies[userInputIndex - 1], o.verticies[userInputIndex]};
  vector<point2D> firstEdge = {o.verticies[userInputIndex], o.verticies[0]};
  
  // not valid if either edge intersects any obstacle
  if (doesIntersect(lastEdge, obstacles)) {
    return 0;
  }
  if (doesIntersect(firstEdge, obstacles)) {
    return 0;
  }
  // new obstacle is valid!
  return 1;
}

