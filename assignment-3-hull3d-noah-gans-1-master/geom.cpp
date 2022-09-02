#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <deque>

void printPoint(point3d p)
{
  cout << "Point at: " << p.x << ", " << p.y << ", " << p.z << "\n";
}
void strPoint(point3d p)
{
  cout << p.x << ", " << p.y << ", " << p.z;
}
void printEdge(point3d p, point3d q)
{
  cout << "Edge from point " << &p << " (" << p.x << ", " << p.y << ", " << p.z << ")<=====>" << &q << " (" << q.x << ", " << q.y << ", " << q.z << ")\n";
}
void printTriangle(triangle3d T){
  point3d a = *T.a;
  point3d b = *T.b;
  point3d c = *T.c;
  cout << "Triangle " << a.x << ", " << a.y << ", " << a.z << " -> " << b.x << ", " << b.y << ", " << b.z << " -> " << c.x << ", " << c.y << ", " << c.z;
}

void printEdge(edge e)
{
  cout << "edge: "
       << "[ "
       << "(" << (e.a)->x <<"," << (e.a)->y << "," <<  (e.a)->z << ") , "
       << "(" << (e.b)->x <<"," << (e.b)->y << "," <<  (e.b)->z << ")"  
       << "]"; 
}

void printLiveEdges(vector<edge> live_edges) {

  cout << "live edges: "; 
  for (int i = 0; i < live_edges.size(); i++)
    {
      /* code */
      printEdge(live_edges[i]);
      // cout << live_edges[i] << ", ";
    }
  cout << endl;
}


void printEdges(vector<point3d *> live_edges)
{
  for (int i = 0; i < live_edges.size(); i += 2)
  {
    /* code */

    point3d p = *live_edges[i];
    point3d q = *live_edges[i + 1];
    cout << "Edge from point " << live_edges[i] << " (" << p.x << ", " << p.y << ", " << p.z << ")<=====>" << live_edges[i + 1] << " (" << q.x << ", " << q.y << ", " << q.z << "), ";
  }
  cout << "\n";
}
vector<double> subtract(point3d a, point3d b)
{
  vector<double> submat;
  submat.push_back(a.x - b.x);
  submat.push_back(a.y - b.y);
  submat.push_back(a.z - b.z);
  return submat;
}

/* returns 6 times the signed volume of abcd. The volume is positive
   if d is behind abc,  negative if d is in front (i.e. same side
   as the normal) of abc, and 0 if abcd are coplanar.
 */
double signed_area3D(point3d a, point3d b, point3d c, point3d d)
{

  vector<double> vectorAB = subtract(a, b);
  vector<double> vectorBC = subtract(b, c);
  vector<double> vectorCD = subtract(c, d);
  return (vectorAB[0] * (vectorBC[1] * vectorCD[2] - vectorBC[2] * vectorCD[1]) -
          vectorBC[0] * (vectorAB[1] * vectorCD[2] - vectorAB[2] * vectorCD[1]) +
          vectorCD[0] * (vectorAB[1] * vectorBC[2] - vectorAB[2] * vectorBC[1]));
}

/* return 1 if points are on the same plane, and 0 otherwise */
int coplanar(point3d a, point3d b, point3d c, point3d d)
{
  if (signed_area3D(a, b, c, d) == 0)
  {
    return 1;
  }
  return 0;
}

/* return 1 if d is  strictly in front of abc; 0 otherwise */
int infront(point3d a, point3d b, point3d c, point3d d)
{
  if (signed_area3D(a, b, c, d) > 0)
  {
    return 1;
  }
  return 0;
}

bool equal(point3d a, point3d b)
{
  if (a.x == b.x && a.y == b.y && a.z == b.z)
  {
    return true;
  }
  return false;
}

point3d *get_next_point(point3d a, point3d b, vector<point3d>& p)
{

  //cout << "Next Point\n";
  //cout << "Edges Are +++++++++\n";
  //printPoint(a);
  //printPoint(b);
  //cout << "++++++++++++++\n";
  for (int i = 0; i < p.size(); i++)
  {
    //cout << i << " Considering ";
    //printPoint(p[i]);
    //cout << p[i] << "\n";
    for (int k = 0; k < p.size(); k++)
    {
      /* code */
      //cout << "             " << k << " ";
      //strPoint(p[k]);
      //cout << " is ";
      if (!equal(p[i], p[k]) && !equal(p[k], a) && !equal(p[k], b))
      {
        if (infront(a, b, p[i], p[k]) == 0)
        {
          //cout << "behind\n";
          break;
        }
        else{//cout << "in front of\n";
        }
      }
      else
      {
        //cout << "the same\n";
      }
      if (k == p.size() - 1)
      {
        //cout << "\n";
        //printPoint(p[i]);
        //cout << p[i] << "\n";
        return &(p[i]);
      }
    }
  }
  cout << "\n\n\n DANGER \n\n\n";
  assert(0); //should never be here 
  return NULL;
}











triangle3d find_extreme_triangle(vector<point3d>& points)
{
  //initialize lowpoint to point[0]
  point3d *lowPointAdress = &points[0];
  double lowZ = points[0].z;
  
  for (int i = 0; i < points.size(); i++)
    {
      /* code */
      if (points[i].z < lowZ)
	{
	  /* code */
	  lowPointAdress = &points[i];
	  lowZ = points[i].z;
	}
    }
  point3d proxyPoint = {lowPointAdress->x + 1, lowPointAdress->y, lowPointAdress->z};
  point3d *actualSecondPoint = get_next_point(*lowPointAdress, proxyPoint, points);
  point3d *thridPoint = get_next_point(*lowPointAdress, *actualSecondPoint, points);
  triangle3d baseT = {lowPointAdress, actualSecondPoint, thridPoint};
  return baseT;
}







bool pointAlreadyOnHull(point3d p, vector<point3d *> hullPoints)
{
  for (int i = 0; i < hullPoints.size(); i++)
  {
    /* code */
    if (equal(p, *hullPoints[i]))
    {
      return true;
    }
  }
  return false;
}



bool check_if_not_in_live_edges(point3d a, point3d b, vector<edge> live_edges){
  for (int i = 0; i < live_edges.size(); i++)
  {
    if (equal(a, *(live_edges[i].a)) && equal(b, *(live_edges[i].b)))
    {
      cout << "In Live Edges, do not add 1\n";
      return false;
    }
    if (equal(b, *(live_edges[i].a)) && equal(a, *(live_edges[i].b)))
    {
      cout << "In Live Edges, do not add 2\n";
      return false;
      
    } 
  }
  return true;
}




int clean_edges(vector<edge> live_edges, point3d a, point3d b) {
  for (int i = 0; i < live_edges.size(); i++)
  {
    if (equal(a, *(live_edges[i].a)) && equal(b, *(live_edges[i].b)))
    {
      return i;
    }
    if (equal(b, *(live_edges[i].a)) && equal(a, *(live_edges[i].b)))
    {
      return i;
    } 
  }

  cout << "edge to delete not found \n";
  return -1;
}




vector<edge> add_inital_edges(triangle3d baseT, vector<edge> live_edges){
  edge ab = {baseT.a, baseT.b}; 
  edge bc =  {baseT.b, baseT.c};
  edge ca =  {baseT.c, baseT.a};
  live_edges.push_back(ab);
  live_edges.push_back(bc);
  live_edges.push_back(ca);
  return live_edges;
}

vector<point3d*> add_inital_hull_points(triangle3d baseT, vector<point3d*> hullPoints){
  hullPoints.push_back(baseT.a);
  hullPoints.push_back(baseT.b);
  hullPoints.push_back(baseT.c);
  return hullPoints;



}
/* compute and return the convex hull of the points */
vector<triangle3d> hull_giftwrap(vector<point3d>& points)
{
  vector<edge> live_edges;// list of live edges
  vector<point3d*> hullPoints;// list of points on hull
  vector<triangle3d> result;//hull triangles

  // find extreme face
  triangle3d baseT = find_extreme_triangle(points);
  live_edges = add_inital_edges(baseT, live_edges);
  hullPoints = add_inital_hull_points(baseT, hullPoints);

  printLiveEdges(live_edges); 
  
  
  int sizeOfEdges = live_edges.size();
  //note: sizeOfEdges will increase inside the loop 
  cout << "\n\n\n\n\n\n\n\n++++++++++++++++++++++++++++++++++++++\n\nFinding hull Points \nInitilization is complete\n\n++++++++++++++++++++++++++++++++++++++ ";
  int count = 0;
 
  while(live_edges.size() >= 3){
    //cout << "\n\n=================\n\nNext Loop, moving to next edge \n\n=================\n\n";
    cout << "\n\n";
    cout << "Live Edges: ";
    printLiveEdges(live_edges);
    cout << "Edge Observed is ";
    edge e = live_edges.back();
    live_edges.pop_back();
    printEdge(e);
    
    cout << "\n"; 
    point3d *a = e.a, *b = e.b;
    //if ((*b).z == 362 && count < 193){
      //count = 193;
      //return result;
    //}
    point3d *nextPoint = get_next_point(*b, *a, points);
    cout << "Point found from edge ==>   ";
    strPoint(*nextPoint);
    cout << "\n";
    if (pointAlreadyOnHull(*nextPoint, hullPoints))
    {
      cout << "On Hull and checking how many to add\n";
      if ((!check_if_not_in_live_edges(*nextPoint, *a, live_edges)) && (!check_if_not_in_live_edges(*b, *nextPoint, live_edges))){
        cout << "Both possible edges are already on live edges, and therfore both should be deleted.\n";
        int edge_skiped = clean_edges(live_edges, *a, *nextPoint);
        cout << "Deleting ";
        strPoint(*a);
        cout << " --> ";
        strPoint(*nextPoint);
        cout << "\n";
        live_edges.erase(live_edges.begin() + edge_skiped);
        edge_skiped = clean_edges(live_edges, *b, *nextPoint);
        cout << "Deleting ";
        strPoint(*nextPoint);
        cout << " --> ";
        strPoint(*b);
        cout << "\n";
        live_edges.erase(live_edges.begin() + edge_skiped);
        cout << "\n\n\n";
        printLiveEdges(live_edges);

      }
      
      else if((check_if_not_in_live_edges(*nextPoint, *a, live_edges)) && (check_if_not_in_live_edges(*b, *nextPoint, live_edges))){
        cout << "Point on hull but possible edges are not live: Add both\n";
        edge e1 = {nextPoint, b};
        edge e2 = {a, nextPoint};
        live_edges.push_back(e2);
        live_edges.push_back(e1);
        
      }
      
      // case one one edge is new and must be added other not
      //case two, both are new and both must be added
      else{
        int del_a_edge_index = -2;
        int del_b_edge_index = -2;
        if (check_if_not_in_live_edges(*b, *nextPoint, live_edges))
        // add the edge
        {
          cout << "B to new point is not in live edges list, possible to add\n";
          edge e1 = {nextPoint, b};
          strPoint(*nextPoint);
          cout << " --> ";
          strPoint(*b);
          cout << "\n";
          cout << "Adding b -> new point\n";
          live_edges.push_back(e1);
          del_a_edge_index = clean_edges(live_edges, *a, *nextPoint);
          cout << "edge a index " << del_a_edge_index << "\n";
        }
        if (check_if_not_in_live_edges(*nextPoint, *a, live_edges))
        {
          cout << "new point to a is not in live edges list, possible to add\n";
          edge e2 = {a, nextPoint};
          //cout << "Adding newPoint -> a\n";
          strPoint(*a);
          cout << " --> ";
          strPoint(*nextPoint);
          cout << "\n";
          live_edges.push_back(e2);
          del_b_edge_index = clean_edges(live_edges, *b, *nextPoint);
          cout << "edge b index " << del_b_edge_index << "\n";
        }
          //Now make deletions
        if(del_a_edge_index >= 0){
          cout << "Edge being removed is: ==============" << del_a_edge_index << " ";
          printEdge(live_edges[del_a_edge_index]);
          cout << "\n";
          live_edges.erase(live_edges.begin() + del_a_edge_index);
        }
        if(del_b_edge_index >= 0){
          cout << "Edge being removed is: ==============" << del_b_edge_index << " ";
          printEdge(live_edges[del_b_edge_index]);
          cout << "\n";
          live_edges.erase(live_edges.begin() + del_b_edge_index);
        }
          
        
      }
      
    }
    else
    {
      /* code */
      cout << "Not on Hull and adding both\n";
      //cout << nextPoint << "\n";
      edge e1 = {a, nextPoint}; 
      live_edges.push_back(e1);
      edge e2 = {nextPoint, b}; 
      live_edges.push_back(e2);
    }
    triangle3d addT = {a, nextPoint, b};
    //cout << "\n";
    //printTriangle(addT);
    //cout << "\n";
    //cout << "Adding point to hull";
    hullPoints.push_back(nextPoint);
    //printPoint(*nextPoint);
    result.push_back(addT);
    count += 1;
    if(count == 200){
      cout << "force quit\n\n\n\n\n\n\n";
      return result;
    }

  }
  
  return result;
  
  
}

