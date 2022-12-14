/* hull 3D

   Initializes a set of points in 3D and renders them (allowing to
   translate/rotate when user presses l/r/u/d/x/X,y/Y,z/Z).

   Needs to compute the convex hull. 

   OpenGL 1.x
   Laura Toma
*/

#include "geom.h"
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
//this allows this code to compile both on apple and linux platforms
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <vector>

using namespace std; 



/* global variables */


//the array of n points; note: this variable needs to be global
//because it needs to be rendered
vector<point3d>  points;

int n;  //desired number of points 


//the convex hull, stored as a list. note: this variable needs to be
//global because it needs to be rendered
vector<triangle3d>  hull; 


const int WINDOWSIZE = 500; 

//we predefine some colors for convenience 
GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};
// from
// https://www.opengl.org/discussion_boards/showthread.php/132502-Color-tables
//
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



//whenever the user rotates and translates the scene, we update these
//global translation and rotation
GLfloat pos[3] = {0,0,0};
GLfloat theta[3] = {0,0,0};

// draw polygons line or filled.  
GLint fillmode = 0; 

/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);
void initialize_points_random();
void initialize_points_sphere();
void print_points();
void print_hull();
void draw_points(); 
void draw_hull(); 
void draw_xy_rect(GLfloat z, GLfloat* col); 
void draw_xz_rect(GLfloat y, GLfloat* col); 
void draw_yz_rect(GLfloat x, GLfloat* col); 
void cube(GLfloat side); 
void filledcube(GLfloat side); 
void draw_axes(); 




int main(int argc, char** argv) {

  //read number of points from users
  
  
  if (argc!=2) {
    printf("usage: hull3d <nbPoints>\n");
    exit(1); 
  }
  n = atoi(argv[1]); 

  printf("you entered n=%d\n", n);

  //assert(n>0); 

  initialize_points_random();
  //initialize_points_sphere();
  cout << "Points are the following: \n";
  print_points();
  cout << "Starting Gift Wrapping..... \n";
  hull = hull_giftwrap(points); 
  cout << "Coow";
  



  //print_hull();
  

  




  
  /* OPEN GL STUFF */
    /* open a window and initialize GLUT stuff */
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);

  glutInitWindowPosition(100,100);

  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display); 
  glutKeyboardFunc(keypress);

  /* OpenGL init */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);  
  //when depth test is enabled, GL determines which objects are in
  //front/behind and renders them correctly
  glEnable(GL_DEPTH_TEST); 

  /* setup the camera (i.e. the projection transformation) */ 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1 /* aspect */, 1, 10.0); /* the frustrum is from z=-1 to z=-10 */
  /* camera is at (0,0,0) looking along negative y axis */

  //initialize the translation to bring the points in the view frustrum which is [-1, -10]
  pos[2] = -3;


  /* start the event handler */
  glutMainLoop();

  return 0;
}


void print_points(){

  for (int i=0; i<n; i++) {
    cout << "" << (points[i]).x << ", " << (points[i]).y << ", " << (points[i]).z << "\n";
  }

}

void print_hull(){
  for (int i = 0; i < hull.size(); i++)
  {
    /* code */
    cout << "Triangle " << i << ": (" << (*hull[i].a).x << ", " << (*hull[i].a).y << ", " << (*hull[i].a).z << ") ===== ("<< (*hull[i].b).x << ", " << (*hull[i].b).y << ", " << (*hull[i].b).z << ") ===== ("<< (*hull[i].c).x << ", " << (*hull[i].c).y << ", " << (*hull[i].c).z << ")\n";
  }

}

/* this function is called whenever the window needs to be rendered */
void display(void) {
  
  //clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  //clear all modeling transformations 
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
  
  /* The default GL window is x=[-1,1], y= [-1,1] with the origin in
     the center.  The view frustrum was set up from z=-1 to z=-10. The
     camera is at (0,0,0) looking along negative z axis.
  */ 
  
  /*  First we translate and rotate our local reference system with the
      user transformation. pos[] represents the cumulative translation
      entered by the user, and theta[] the cumulative rotation entered
      by the user */
  glTranslatef(pos[0], pos[1], pos[2]);  
  glRotatef(theta[0], 1,0,0); //rotate theta[0] around x-axis, etc 
  glRotatef(theta[1], 0,1,0);
  glRotatef(theta[2], 0,0,1);
  
  /* We translated the local reference system where we want it to be;
     now we draw the objects in the local reference system.  */
 
  //print_points();
  //print_hull();

  draw_points();
  draw_hull(); 
  
  //don't need to draw a cube but I found it nice for perspective 
  cube(1); 


  glFlush();
}



/* this function is called whenever  key is pressed */
void keypress(unsigned char key, int x, int y) {

  switch(key) {

  case 'i': 
    //re-initialize 
    initialize_points_sphere(); 
    //re-compute th ehull
    hull = hull_giftwrap(points); 
    cout << "Key press\n";
    glutPostRedisplay(); 
    break; 
    
    //ROTATIONS 
  case 'x':
    theta[0] += 5.0; 
    glutPostRedisplay();
    break;
  case 'y':
    theta[1] += 5.0;
    glutPostRedisplay();
    break;
  case 'z':
    theta[2] += 5.0;
    glutPostRedisplay();
    break;
  case 'X':
    theta[0] -= 5.0; 
    glutPostRedisplay();
    break;
  case 'Y':
    theta[1] -= 5.0; 
    glutPostRedisplay();
    break;
  case 'Z':
    theta[2] -= 5.0; 
    glutPostRedisplay();
    break;
    
    //TRANSLATIONS 
    //backward (zoom out)
  case 'b':
    pos[2] -= 0.1; 
    glutPostRedisplay();
    break;
    //forward (zoom in)
  case 'f':
    pos[2] += 0.1; 
    //glTranslatef(0,0, 0.5);
    glutPostRedisplay();
    break;
    //down 
  case 'd': 
     pos[1] -= 0.1; 
    //glTranslatef(0,0.5,0);
    glutPostRedisplay();
    break;
    //up
  case 'u': 
    pos[1] += 0.1; 
    //glTranslatef(0,-0.5,0);
    glutPostRedisplay();
    break;
    //left 
  case 'l':
    pos[0] -= 0.1; 
    glutPostRedisplay();
    break;
    //right
  case 'r':
    pos[0] += 0.1; 
    glutPostRedisplay();
    break;

    //fillmode 
  case 'c': 
    fillmode = !fillmode; 
     glutPostRedisplay();
    break;

  case 'q':
    exit(0);
    break;
  } 
}//keypress










/* initialize the array of points stored in global variable points[]
   with random points in the range x= [0,WINDOWSIZE],
   y=[0,WINDOWSIZE], z=[0,WINDOWSIZE] */
void initialize_points_random() {
  
  //clear the vector just to be safe 
  points.clear(); 

  int i; 
  for (i=0; i<n; i++) {
    point3d p;
    p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
    p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    p.z=  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    points.push_back(p); 
  }
}



/* initialize points on a sphere 
   spherical coordinates: 
   x = r sin PHI cos THETA
   y = r sin PHI sin THETA 
   z = r cos PHI
   
*/
void initialize_points_sphere() {

  //clear the vector just to be safe 
  points.clear(); 

  int m = sqrt(n); 
  double phi, theta, incr = 2* M_PI / m; 
  int rad = .4 * WINDOWSIZE; 

  int i, j, x, y, z;
  point3d p;
  for (i=0; i<m; i++) {
    phi = i*incr; 
    for (j=0; j<m; j++) {
      theta = j * incr;
      
      x = rad * sin(phi) * cos(theta);
      y = rad * sin(phi) * sin(theta);
      z = rad * cos(phi); 

      p.x = x; p.y=y; p.z=z;
      //all the points are in [-rad,rad]
      //shift them to [0,2*rad]
      p.x = (p.x+rad);
      p.y = (p.y+ rad);
      p.z = (p.z+ rad);
   
      
    } //for j 
  }//for i 
  printf("done initializing points on sphere. total %d points\n", points.size());
}





/* x is a value in [0,WINDOWSIZE]: it is mapped to [-1,1] */
GLfloat windowtoscreen(GLfloat x) {
  return (-1 + 2*x/WINDOWSIZE); 
}




/* ****************************** */
/* Draw the array of points stored in global variable points[].  Each
   point is drawn as a small cube.

   NOTE: The points are in the range x=[0, WINDOWSIZE], y=[0,
   WINDOWSIZE], z=[0, WINDOWSIZE] and they must be mapped back into x=[-1,1],
   y=[-1, 1], z=[-1,1]
  */
void draw_points(){

  const int R= 1;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set color 
  glColor3fv(yellow);   
  
  int i;
  for (i=0; i < points.size(); i++) {
    
    //draw a small filled cube centered at (points[i].x, points[i].y,
    //points[i].z)
    
    //first save local coordinate system 
    glPushMatrix(); 
    //translate our local coordinate system to the point that we wan to draw
    glTranslatef(windowtoscreen((points[i]).x),
		 windowtoscreen((points[i]).y), 
		 windowtoscreen((points[i]).z));
    //now our origin is at the point: draw the cube 
    filledcube(.01); 
    
    //move local system back to where we were
    glPopMatrix(); 
  } //for 

}//draw_points




/* ****************************** */
/* draw the list of points stored in global variable hull[].

   NOTE: the points are in the range x=[0, WINDOWSIZE], y=[0,
   WINDOWSIZE], z=[0, WINDOWSIZE] and they must be mapped back into
   x=[-1,1], y=[-1, 1] */
void draw_hull(){ 
  //print_points();
  //print_hull();
  for (int i = 0; i < hull.size(); i ++)
  {
    //cout << (*hull[i].a).x << "/////////;;\n"; 
    float ax = windowtoscreen( hull[i].a->x);
    float ay = windowtoscreen( hull[i].a->y);
    float az = windowtoscreen( hull[i].a->z);
    float bx = windowtoscreen( hull[i].b->x);
    float by = windowtoscreen( hull[i].b->y);
    float bz = windowtoscreen( hull[i].b->z);
    float cx = windowtoscreen( hull[i].c->x);
    float cy = windowtoscreen( hull[i].c->y);
    float cz = windowtoscreen( hull[i].c->z);
    //cout << ax << "\n"; 
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glColor3fv(white); //use for white (see through) faces
    //Use following for colorful faces
    GLfloat r = (GLfloat) rand() / RAND_MAX;
    GLfloat g = (GLfloat) rand() / RAND_MAX;
    GLfloat b = (GLfloat) rand() / RAND_MAX;
    GLfloat color[3] = {r, g, b};
    glColor3fv(color);
    glBegin(GL_TRIANGLES);
    glVertex3f(ax, ay, az); //v1
    glVertex3f(bx, by, bz);//v2
    glVertex3f(cx, cy, cz);//v3
    glEnd();
   
   
  }
} 







//draw a square x=[-side,side] x y=[-side,side] at depth z
void draw_xy_rect(GLfloat z, GLfloat side, GLfloat* col) {

  glColor3fv(col);
  glBegin(GL_POLYGON);
  glVertex3f(-side,-side, z);
  glVertex3f(-side,side, z);
  glVertex3f(side,side, z);
  glVertex3f(side,-side, z);
  glEnd();
}


//draw a square y=[-side,side] x z=[-side,side] at given x
void draw_yz_rect(GLfloat x, GLfloat side, GLfloat* col) {
  
  glColor3fv(col);
  glBegin(GL_POLYGON);
  glVertex3f(x,-side, side);
  glVertex3f(x,side, side);
  glVertex3f(x,side, -side);
  glVertex3f(x,-side, -side);
  glEnd();
}


//draw a square x=[-side,side] x z=[-side,side] at given y
void draw_xz_rect(GLfloat y, GLfloat side, GLfloat* col) {

  glColor3fv(col);
  glBegin(GL_POLYGON);
  glVertex3f(-side,y, side);
  glVertex3f(-side,y, -side);
  glVertex3f(side,y, -side);
  glVertex3f(side,y, side);
  glEnd();
}

//draw a cube 
void cube(GLfloat side) {
  GLfloat f = side, b = -side;
 
  if (fillmode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }


  /* back face  BLUE*/
  draw_xy_rect(b,side, blue);
 /* front face  RED*/
  draw_xy_rect(f,side, red);
  /* side faces  GREEN*/
  draw_yz_rect(b, side, green);
  draw_yz_rect(f, side, green);
  //up, down faces missing to be able to see inside 

  /* middle z=0 face CYAN*/
  draw_xy_rect(0, side, cyan);
  /* middle x=0 face WHITE*/
  draw_yz_rect(0,side, gray);
  /* middle y=0 face  pink*/
  draw_xz_rect(0, side, magenta);
}



//draw a filled cube  [-side,side]^3
void filledcube(GLfloat side) {
  
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  /* back, front faces */
  draw_xy_rect(-side,side, yellow);
  draw_xy_rect(side,side, yellow);
  
  /* left, right faces*/
  draw_yz_rect(-side, side, yellow);
  draw_yz_rect(side, side, yellow);
  
  /* up, down  faces  */
  draw_xz_rect(side,side, yellow);
  draw_xz_rect(-side,side, yellow);
}
