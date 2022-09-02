#ifndef __sort_h
#define __sort_h

#include "Point2D.h"
#include <vector>

using namespace std; 
vector<Point2D> generateRandomPoints(int numPoints);





class TreeNode {
  public: 
    
    Point2D p; 
    char type; 
    double l;
    TreeNode *left, *right;  


    TreeNode(Point2D inputP, char kind, double Line, TreeNode* Left, TreeNode* Right){
      p = inputP;
      type = kind;
      l = Line;
      left = Left;
      right = Right;
    } 
};


class Kdtree {
  public: 
     TreeNode* root; 
     int count ; //number of leaves in the tree
     int height;  //height of the tree

     //build the kd-tree
     TreeNode* buildKdtree(vector<Point2D> xPoints, vector<Point2D> yPoints, int depth);
   
   Kdtree(vector<Point2D> points);

};

#endif
