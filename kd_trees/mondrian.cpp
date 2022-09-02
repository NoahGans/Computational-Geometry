#include "kdTree.h"
#include "Point2D.h"
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




int  POINT_INIT_MODE = 0; 
int NB_INIT_CHOICES = 4; 


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
vector<GLfloat*> colorList;


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

/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);
vector<Point2D> initialize_points_random(int n);
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
void print_points();
void printPreorder(TreeNode* node);
void printMondrian(vector<double> xBounds, vector<double> yBounds, int depth, TreeNode* node);
void drawVerticleLine(double lowYBound, double highYBound, double l);



vector<Point2D> points ;
Kdtree * myTree;
int n;

int main(int argc, char** argv) {
  colorList.push_back(red);
  colorList.push_back(yellow);
  colorList.push_back(blue);
  colorList.push_back(white);
  colorList.push_back(green);
  colorList.push_back(cyan);
  colorList.push_back(magenta);

  //read number of points from users
  if (argc!=2) {
    printf("usage: mondrian <nbPoints>\n");
    exit(1); 
  }
  int n = atoi(argv[1]); 
  cout << n << "\n";
  printf("you entered n=%d\n", n);
  points = initialize_points_random(n);
  print_points();
  myTree = new Kdtree(points);
  
  printPreorder(myTree->root);
  //start the rendering 
  /* initialize GLUT  */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);
  cout << n << "\n";
  /* register callback functions */
  glutDisplayFunc(display);
  cout << n << "\n";
 
  /*glutKeyboardFunc(keypress);
*/
  /* init GL */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);   
  
  /* give control to event handler */
  glutMainLoop();
  return 0;
}





vector<Point2D> initialize_points_random(int n) {

   printf("initialize points random\n"); 
  //clear the vector just to be safe 
  points.clear(); 
  vector<Point2D> points;
  int i; 
  Point2D p; 
  for (i=0; i<n; i++) {
    p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
    p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    points.push_back(p); 
  }

  return points;
}


/* Given a binary tree, print its nodes in preorder*/
void printPreorder(TreeNode* node)
{
    if (node == NULL)
        return;
 
    cout << "Cow\n"; 
    /* first print data of node */
    cout << node->l << " ";

    /* then recur on left subtree */
    printPreorder(node->left);
 
    /* now recur on right subtree */
    printPreorder(node->right);
}



/*                                           */
void print_points() {
  
  int i; 
  printf("points: ");
  for (i=0; i< points.size(); i++) {
  }
  printf("\n");
  fflush(stdout);  //flush stdout, weird sync happens when using gl thread
}

/* ****************************** */
/* This is the main render function. We registered this function to be
   called by GL to render the window. 
 */
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

  draw_points();
  vector<double> Xbounds;
  Xbounds.push_back(0);
  Xbounds.push_back(WINDOWSIZE);
  vector<double> Ybounds;
  Ybounds.push_back(0);
  Ybounds.push_back(WINDOWSIZE);
  printMondrian(Xbounds, Ybounds, 0, myTree->root);

  /*draw_hull(); 

  /* execute the drawing commands */
  glFlush();
}




/* ****************************** */
/* draw the array of points stored in global variable points[] 
   each point is drawn as a small square   
*/
void draw_points(){

  const int R= 1;
  //draw polygon filled or line 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set drawing color 

  glColor3fv(yellow);   
  int i;
  for (i=0; i < points.size(); i++) {
    //draw a small square centered at (points[i].x, points[i].y)
    glBegin(GL_POLYGON);
    glVertex2f(points[i].x -R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y+R);
    glVertex2f(points[i].x -R,points[i].y+R);
    glEnd();
  }
} //draw_points 







void drawPolygon(vector<double> xBounds, vector<double> yBounds){
  
  double x = rand()/static_cast<double>(RAND_MAX+1); 

   // [0,1[ * (max - min) + min is in [min,max[
  int that = 0 + static_cast<int>( x * (0 - 6) );
  cout << "Random num = " << that << "\n";

  glColor3fv(colorList[that]); 
  cout << "top Left : " << xBounds[0] << ", " << yBounds[1] << "\n";
  cout << "bottom right : " << xBounds[1] << ", " << yBounds[0] << "\n";

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
  glBegin(GL_POLYGON);
  glVertex2f(xBounds[0], yBounds[1]);
  glVertex2f(xBounds[0], yBounds[0]); 
  glVertex2f(xBounds[1], yBounds[0]);
  glVertex2f(xBounds[1], yBounds[1]);  
  glEnd();
  glColor3fv(black); 
  glLineWidth(2);

  glBegin(GL_LINES);
  glVertex2f(xBounds[0], yBounds[1]);
  glVertex2f(xBounds[0], yBounds[0]);
  glVertex2f(xBounds[0], yBounds[0]);
  glVertex2f(xBounds[1], yBounds[0]);
  glVertex2f(xBounds[1], yBounds[0]);
  glVertex2f(xBounds[1], yBounds[1]);
  glVertex2f(xBounds[1], yBounds[1]);
  glVertex2f(xBounds[0], yBounds[1]);

  glEnd();



}


/* ****************************** */
/* Render the hull; the points on the hull are expected to be in
   boundary order (either ccw or cw). To render the hull we'll draw
   lines between consecutive points */
void printMondrian(vector<double> xBounds, vector<double> yBounds, int depth, TreeNode* node){
  cout << "\n\nLoop ++++++++++++\n";
  vector<double> leftChildXbounds;
  leftChildXbounds.push_back(xBounds[0]);
  leftChildXbounds.push_back(xBounds[1]);
  vector<double> leftChildYbounds;
  leftChildYbounds.push_back(yBounds[0]);
  leftChildYbounds.push_back(yBounds[1]);
  vector<double> rightChildXbounds;
  rightChildXbounds.push_back(xBounds[0]);
  rightChildXbounds.push_back(xBounds[1]);
  vector<double> rightChildYbounds; 
  rightChildYbounds.push_back(yBounds[0]);
  rightChildYbounds.push_back(yBounds[1]);
  
  cout << "Parent Bounds x: " << xBounds[0] << '-' << xBounds[1] << "\n";
  cout << "Parent Bounds y: " << yBounds[0] << '-' << yBounds[1] << "\n";

  if(node->right == NULL && node->left == NULL){
    cout << "reached Leaf \n ";
    drawPolygon(xBounds, yBounds);
    cout << "Return \n\n";
    return;
  }
  if (depth % 2 == 0)
  {
    cout << "Even Split\n";
    leftChildXbounds[1] = node->l;
    rightChildXbounds[0] = node->l;
  }
  else{
    cout << "odd Split\n";
    leftChildYbounds[1] = node->l;
    rightChildYbounds[0] = node->l;
  }
    cout << "Next Call\n";
    //left
    cout << "Calling Left child with\n";
    cout << "Left Child Bounds x: " << leftChildXbounds[0] << '-' << leftChildXbounds[1] << "\n";
    cout << "Left Child Bounds y: " << leftChildYbounds[0] << '-' << leftChildYbounds[1] << "\n";
    printMondrian(leftChildXbounds, leftChildYbounds, depth - 1, node->left);
    //right
    cout << "Calling Right child with\n";
    cout << "right Child Bounds x: " << rightChildXbounds[0] << '-' << rightChildXbounds[1] << "\n";
    cout << "right Child Bounds y: " << rightChildYbounds[0] << '-' << rightChildYbounds[1] << "\n";

    printMondrian(rightChildXbounds, rightChildYbounds, depth - 1, node->right);
   
   
   
  //  TreeNode* leftChild = buildKdtree(P1x, P1y, depth + 1);
  //  TreeNode* rightChild = buildKdtree(P2x, P2y, depth + 1);
  //  cout << "========Returning to "<< depth << "======\n";
  //  Point2D pointToAdd(-999999.0, -999999.0);
  //  TreeNode* lineNode = new TreeNode(pointToAdd, 'b', l, leftChild, rightChild);
  //  cout << "===============\n\n\n\n";
  //  return lineNode;
         


    //draw a line from the last point to the first point 
    //fill in code here 

    
  //if (hull not empty)
}




void keypress(unsigned char key, int x, int y) {
  switch(key) {
  case 'q':
    exit(0);
    break;

  
    //redraw
    glutPostRedisplay();

  } //switch (key)

}//keypress



