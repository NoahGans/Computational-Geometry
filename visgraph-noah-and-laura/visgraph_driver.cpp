/* 
Driver for motion planning with a visibility graph.
Given a predefined list of obstacles (or user inputed obstacles) calculates the shortest
path between two end points using dijkstras algorithm

@author Laura Friel
@author Noah Gans
@version December 14th, 2021

*/

#include <assert.h>
// this is the file with the definition of a pqueue element
#include "basictype.h"
#include "geom.h"
// to compile on both apple and unix platform
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <inttypes.h>
#include <iostream>
#include <math.h>
#include <limits>
#include "pqheap.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std; 

//pre-defined colors for convenience 
GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};
/* from https://www.opengl.org/discussion_boards/showthread.php/132502-Color-tables  */
GLfloat brown[3] = { 0.647059, 0.164706, 0.164706}; 
GLfloat DarkBrown[3] = { 0.36, 0.25, 0.20}; 
GLfloat DarkTan[3] = { 0.59, 0.41, 0.31};
GLfloat Maroon[3]= { 0.556863, 0.137255, 0.419608}; 
GLfloat DarkWood[3] = { 0.52, 0.37, 0.26}; 
GLfloat  Copper[3] = { 0.72,  0.45,  0.20};
GLfloat green1[3] = {.5, 1, 0.5};
GLfloat green2[3] = {0.0, .8, 0.0};
GLfloat green3[3] = {0.0, .5, 0.0};
GLfloat ForestGreen[3] = { 0.137255, 0.556863, 0.137255};
GLfloat MediumForestGreen[3] = { 0.419608 , 0.556863 , 0.137255}; 
GLfloat LimeGreen[3] ={ 0.196078,  0.8 , 0.196078}; 
GLfloat Orange[3] = { 1, .5, 0}; 
GLfloat Silver[3] = { 0.90, 0.91, 0.98};
GLfloat Wheat[3] = { 0.847059 , 0.847059, 0.74902}; 


/* Constants */
const int WINDOWSIZE = 500; 
const double DOUBLE_INF = std::numeric_limits<double>::max();
const int START_INDEX = 0;
const int END_INDEX = 1;

/* global variables */
// starting point of path
point2D s;
// end point of path
point2D e;
//obstacles to be rendered in window
vector<obstacle>  obstacles;
//visibility visabilityGraph to be rendered
visGraph  visabilityGraph; 
//shortest path to be rendered
vector<point2D> shortestPath;

// global variables used for user insertion of obstacles and starting points
bool isEditingObstacles = false;
int tempObstacleIndex = -1;
bool isEditingStart = false;
bool isEditingEnd = false;

//********************************************************************/
/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
/* rendering functions */
void drawObstacles(); 
void drawEndPoints();
void drawShortestPath();
void drawVisibilityGraph(); 
void drawShortestPath(); 

void initializeBasicScene();
obstacle initializeSquare(point2D center, double sideLength);
void dijkstra();
void clearVisibilityGraph();
void clearEverything();

/* basic initialization of scene with several squares and a start and end point */
void initializeBasicScene() {
  // // really simple initializer
  // s = { 2, 4 };
  // e = { 24, 12 };
  // obstacle square = initializeSquare({ 50,50 }, 100);
  // obstacles.push_back(square);

  // obstacle square = initializeSquare({ 13, 8 }, 4);
  // //obstacle square2 = initializeSquare({ 300, 299}, 100);
  // obstacles.push_back(square);
  // //obstacles.push_back(square2);

  // degen case because improper intersect but through two polygons
  s = { 20, 300 };
  e = { 450, 50 };
  obstacle square = initializeSquare({ 50,50 }, 88);
  obstacle square2 = initializeSquare({ 300, 250}, 75);
  obstacle square3 = initializeSquare({ 200, 100}, 45);
  obstacle square4 = initializeSquare({ 100, 200}, 100);
  obstacles.push_back(square);
  obstacles.push_back(square2);
  obstacles.push_back(square3);
  obstacles.push_back(square4);
}

/**
 * initializes a square object.
 * parameters are the bottom left location of the square and its side length, and
 * it is build counter clock wise.
 */
obstacle initializeSquare(point2D bottom_left, double sideLength) {
  obstacle square;
  point2D top_left = { bottom_left.x, bottom_left.y + sideLength };
  point2D top_right = { bottom_left.x + sideLength, bottom_left.y + sideLength };
  point2D bottom_right = { bottom_left.x + sideLength, bottom_left.y };
  square.verticies.push_back(bottom_left);
  square.verticies.push_back(bottom_right);
  square.verticies.push_back(top_right);
  square.verticies.push_back(top_left);
  return square;
}

int  main(int argc, char** argv) {
  initializeBasicScene();
  visabilityGraph = constructVisGraph(obstacles, s, e);
  dijkstra();
  
  //start the rendering 
  /* initialize GLUT  */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display);
  glutKeyboardFunc(keypress);
  glutMouseFunc(mouse);

  /* init GL */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);   
  
  /* give control to event handler */
  glutMainLoop();

  // clear all our vectors so not memory leaks
  clearEverything();
  return 1;
}

/**
 * run dijkstras algorithm on visibility graph to find shortest path from
 * start node to end node
 */
void dijkstra() {
  // for below vectors, all indecies represent node ids
  // dist vector records distance from source to current node
  vector<double> dist;
  // done records if a node has been fully explored
  vector<bool> done;
  // record of what led to shortest distance at a particular node
  vector<int> prev;

  // initialize vectors
  // distance vector is infinity for all nodes except start
  // done vector is false for all nodes
  // prev is false for all nodes
  dist.push_back(0);
  dist.push_back(DOUBLE_INF);
  done.push_back(false);
  done.push_back(false);
  prev.push_back(-1);
  prev.push_back(-1);
  for (size_t i = 2; i < visabilityGraph.verticies.size(); i++) {
    dist.push_back(DOUBLE_INF);
    done.push_back(false);
    prev.push_back(-1);
  }

  //initialize PQ
  PQueue* pq;
  elemType minElt;
  pq = PQ_initialize();
  // insert start
  PQ_insert(pq, { dist[START_INDEX], s, START_INDEX }); 
  
  bool isFound = false;
  while (!PQ_isEmpty(pq) && !isFound) {
    assert(PQ_extractMin(pq, &minElt));
    if (!done[minElt.index]) {
      for (int neighboor : visabilityGraph.adjacencyList[minElt.index]) {
        // distance between minElt off PQ and neighboor
        double distBtwnNeighboor = distance(minElt.p, visabilityGraph.verticies[neighboor]) + dist[minElt.index];
        if (dist[neighboor] > distBtwnNeighboor) {
          // new distance shorter than previously found, so record!
          dist[neighboor] = distBtwnNeighboor;
          prev[neighboor] = minElt.index; 
          PQ_insert(pq, { dist[neighboor], visabilityGraph.verticies[neighboor], neighboor });
        }
      }
      done[minElt.index] = true;
      // once target fully explore, can stop
      if (minElt.p == e) {
        isFound = true;
      }
    }
  }
  //destroy the pqueue 
  PQ_delete(pq);

  // shortest path recovered backwards, starting at end node
  shortestPath.push_back(visabilityGraph.verticies[END_INDEX]);
    int finger = END_INDEX;
    if (prev[finger] != -1) {
      while (finger != 0) {
        shortestPath.push_back(visabilityGraph.verticies[prev[finger]]);
        finger = prev[finger];
      }
    }
}

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT);
  //clear all modeling transformations
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); 


  /* The default GL window is [-1,1]x[-1,1]x[-1,1] with the origin in
     the center. The camera is at (0,0,0) looking down negative
     z-axis.  

     The points are in the range (0,0) to (WINSIZE,WINSIZE), so they
     need to be mapped to [-1,1]x [-1,1] */
  //then scale the points to [0,2]x[0,2]
  glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);  
  //first translate the points to [-WINDOWSIZE/2, WINDOWSIZE/2]
  glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0); 
  drawEndPoints();
  drawVisibilityGraph();
  drawObstacles();
  drawShortestPath();

  /* execute the drawing commands */
  glFlush();
}

/* draw both start and end points as small squares */
void drawEndPoints(){
  // the size of the square (here 1 pixle)
  const int R= 1;
  //draw polygon filled or line 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set drawing color 
  glColor3fv(cyan);   
  glBegin(GL_POLYGON);
  glVertex2f(s.x -R, s.y-R);
  glVertex2f(s.x +R, s.y-R);
  glVertex2f(s.x +R, s.y+R);
  glVertex2f(s.x -R, s.y+R);
  glEnd();

  //set drawing color
  glColor3fv(yellow);   
  glBegin(GL_POLYGON);
  glVertex2f(e.x -R, e.y-R);
  glVertex2f(e.x +R, e.y-R);
  glVertex2f(e.x +R, e.y+R);
  glVertex2f(e.x -R, e.y+R);
  glEnd();

} //drawEndPoints 

/* draw each obstacle in the list of obstacles as polygons. not filled in */
void drawObstacles(){
  //set color 
  glColor3fv(red);
  for (obstacle o : obstacles) {
    vector<point2D> verticies = o.verticies;
    int size = static_cast<int> (verticies.size());
    for (int i = 0; i < size; i++) {
      // wrap around so end connected to begining
      int next_point = (i + size - 1) % size;
      //draw a line fromcrt to next 
      glBegin(GL_LINES);
      glVertex2f(verticies[i].x, verticies[i].y); 
      glVertex2f(verticies[next_point].x, verticies[next_point].y); 
      glEnd();
    }
  }
}

/** 
 * Render the shortest path; To render, iterate over points in list and connect
 * lines between points
 */
void drawShortestPath(){
  glColor3fv(green);
  for (size_t i = 1; i < shortestPath.size(); i++) {
    glBegin(GL_LINES);
    glVertex2f(shortestPath[i].x, shortestPath[i].y); 
    glVertex2f(shortestPath[i - 1].x, shortestPath[i - 1].y); 
    glEnd();
  }
  glEnd();
}

/**
 * Render the visibility graphs; To render the visibility graph we'll draw
 * lines based on the adjacency list
 */
void drawVisibilityGraph(){
  glColor3fv(blue);
  for (size_t i = 0; i < visabilityGraph.adjacencyList.size(); i++) {
    vector<int> adj = visabilityGraph.adjacencyList[i];
    for (int point_index : adj) {
      //draw a line fromcrt to next 
      glBegin(GL_LINES);
      glVertex2f(visabilityGraph.verticies[i].x, visabilityGraph.verticies[i].y); 
      glVertex2f(visabilityGraph.verticies[point_index].x, visabilityGraph.verticies[point_index].y); 
      glEnd();
    }
  }
}

/**
 * what to do on certain key presses.
 *   'q' : quits display
 *   'i' : edit mode (to start inserting new obstacles)
 *   'o' : save an obstacle
 *   'd' : recalculate visibility graph and run dijkstras
 *   's' : edit start point
 *   'e' : edit end point
 */
void keypress(unsigned char key, int x, int y) {
  switch(key) {
    // quit the display
    case 'q': {
      exit(0);
      break;
    }
    // go into edit mode
    // all visuals cleared except for start and end points
    case 'i': {
      clearEverything();
      // this will be true until 'd' is pressed
      isEditingObstacles = true;
      // add a temporary first obstacle to empty obstacles list
      obstacle tempObstacle;
      obstacles.push_back(tempObstacle);
      //redraw
      glutPostRedisplay();
    }
    // add an new empty obstacle to obstacle list and keep track of obstacle being
    // edited. not that with will happen when 'i' is pressed too
    case 'o': {
      obstacle tempObstacle;
      obstacles.push_back(tempObstacle);
      tempObstacleIndex++;
      break;
    }
    // create visibility graph and find shortest path (if not already done)
    case 'd': {
      // end editing mode and reset variables for future use
      isEditingObstacles = false;
      // reset location of obstacle being edited
      tempObstacleIndex = -1;
      if (visabilityGraph.verticies.size() == 0) {
        visabilityGraph = constructVisGraph(obstacles, s, e);
        dijkstra();
        //redraw
        glutPostRedisplay();
      }
      break;
    }
    // change to start point edit mode (will not exit this mode untill valid point reset)
    case 's': {
      clearVisibilityGraph();
      shortestPath.clear();
      //redraw
      glutPostRedisplay();
      isEditingStart = true;
      break;
    }
    // change to end point edit mode (will not exit this mode untill valid point reset)
    case 'e': {
      clearVisibilityGraph();
      shortestPath.clear();
      //redraw
      glutPostRedisplay();
      isEditingEnd = true;
      break;
    }
  } //switch (key)
}//keypress

/**
 * what to do on mouse clicks.
 * depending on edit mode (defined by global boolean variables):
 *   editing obstacles : check that new point is valid and rerender
 *   editing start : check that point is valid, end edit start point when is
 *   editing end : check that point is valid, end edit end point when is
 *   no edit : ignore
 */
void mouse(int button, int state, int x, int y) {
  // left click
  if (button == GLUT_LEFT_BUTTON) {
    // specifically on down click
    if (state == GLUT_DOWN) {
      // get coordinates of the click on display
      // modify y coord because inverted in window
      point2D click = { x * 1.0, WINDOWSIZE - (y * 1.0) };

      // all obstacle and point edits must be valid (meaning they don't lie in an obstacle)
      // stop if the mouse click is not in a valid region
      if (!isValidPointInsert(click, obstacles)) {
        return;
      }
      // mouse click was not in an obstacle!

      // check if in any editing modes
      if (isEditingObstacles) {
        // add latest mouse click to obstacle
        obstacle tempObstacle = obstacles[tempObstacleIndex];
        tempObstacle.verticies.push_back(click);
        if (!isValidAdditionToObstacle(tempObstacle, obstacles, s, e)) {
          // remove latest mouse click from obstacle if not valid
          tempObstacle.verticies.erase(tempObstacle.verticies.begin() + tempObstacle.verticies.size() - 1);
        }
        obstacles[tempObstacleIndex] = tempObstacle;
        //redraw
        glutPostRedisplay();
      // if in either start or end, end edit mode once reset
      } else if (isEditingStart) {
          s = click;
          isEditingStart = false;
          //redraw
          glutPostRedisplay();
      } else if (isEditingEnd) {
          e = click;
          isEditingEnd = false;
          //redraw
          glutPostRedisplay();
      }
    }
  }
}

/* clear vectors so no memory leaks */
void clearVisibilityGraph() {
  visabilityGraph.verticies.clear();
  for (vector<int> adj : visabilityGraph.adjacencyList) {
    adj.clear();
  }
  visabilityGraph.adjacencyList.clear();
}

/* clear vectors so no memory leaks */
void clearEverything() {
  obstacles.clear();
  clearVisibilityGraph();
  shortestPath.clear();
}

