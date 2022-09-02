


#include <iostream>
#include <vector>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>    
#include "kdTree.h"
#include "Point2D.h"


using namespace std;


vector<Point2D> sortPoints(vector<Point2D> points, char type);
Point2D medianSplit(vector<Point2D>* first, vector<Point2D>* second, vector<Point2D> points);
void printPoints(vector<Point2D> points);
void printBT(const std::string& prefix, const TreeNode* node, bool isLeft);




     //build the kd-tree
TreeNode* Kdtree::buildKdtree(vector<Point2D> xPoints, vector<Point2D> yPoints, int depth){
   cout << "=======Start========\n";
   cout << depth << "\n";
   vector<Point2D> P1;
   vector<Point2D> P2;
   double l;
   if (xPoints.size() == 1){
      cout << "Made it to a Leaf with points " << xPoints[0].x << ", " << xPoints[0].y << "\n";
      Point2D pointToAdd(xPoints[0].x, xPoints[0].y);
      TreeNode* leafNode = new TreeNode(pointToAdd, 'l', 0.0, NULL, NULL);
      cout << "Leaf done\n";
      return leafNode;
   }
   else if (depth % 2 == 0)
   {
      l = medianSplit(&P1, &P2, xPoints).x;
   }
   else{
      l = medianSplit(&P1, &P2, yPoints).y;
   }
   vector<Point2D> P1x = sortPoints(P1, 'x');
   vector<Point2D> P1y = sortPoints(P1, 'y');
   vector<Point2D> P2x = sortPoints(P2, 'x');
   vector<Point2D> P2y = sortPoints(P2, 'y');
   
   cout << "=======P1x========\n";
   printPoints(P1x);
   cout << "=======P1y========\n";
   printPoints(P1y);
   cout << "========P2x=======\n";
   printPoints(P2x);
   cout << "========P2y======\n";
   printPoints(P2y);
   cout << "========Calling Kids======\n";
   cout << "===============\n\n\n\n";
   
   TreeNode* leftChild = buildKdtree(P1x, P1y, depth + 1);
   TreeNode* rightChild = buildKdtree(P2x, P2y, depth + 1);
   cout << "========Returning to "<< depth << "======\n";
   Point2D pointToAdd(-999999.0, -999999.0);
   TreeNode* lineNode = new TreeNode(pointToAdd, 'b', l, leftChild, rightChild);
   cout << "===============\n\n\n\n";
   return lineNode;
         
}

Kdtree::Kdtree(vector<Point2D> points){
   vector<Point2D> xPoints = sortPoints(points, 'x');
   vector<Point2D> yPoints = sortPoints(points, 'y');
   root = buildKdtree(xPoints, yPoints, 0);
   printBT("", root, false);
   printPoints(points);

}



Point2D medianSplit(vector<Point2D>* first, vector<Point2D>* second, vector<Point2D> points){
   int medianIndex = points.size() / 2;
   
   for (int i = 0; i < points.size(); i++)
   {
      /* code */
      if(i < medianIndex){
         (*first).push_back(points[i]);
      }
      else{
         (*second).push_back(points[i]);
      }
   }
   return points[medianIndex];
}

void printBT(const std::string& prefix, const TreeNode* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << (*node).p.x << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), (*node).left, true);
        printBT( prefix + (isLeft ? "│   " : "    "), (*node).right, false);
    }
}

double fRand(double fMin, double fMax){
   double f = (double)rand() / RAND_MAX;
   return fMin + f * (fMax - fMin);
}

bool xSort(Point2D i,Point2D j) { 
   return (i.x < j.x); 
}

bool ySort(Point2D i,Point2D j) { 
   return (i.y < j.y); 
}

vector<Point2D> sortPoints(vector<Point2D> points, char type){
   if (type == 'x'){
      std::sort (points.begin(), points.end(), xSort);
   }
   else{
      std::sort (points.begin(), points.end(), ySort);
   }
   return points;
}

vector<Point2D> generateRandomPoints(int numPoints){
   vector<Point2D> points;
   srand (time(NULL));
   for (int i = 0; i < numPoints; i++)
   {  
      
      double x = fRand(0.0, 10.0);
      double y = fRand(0.0, 10.0);
      Point2D pointToAdd(x, y);
      points.push_back(pointToAdd);
      /* generate secret number between 1 and 10: */
      /* code */
   }
   
   return points;

}

void printPoints(vector<Point2D> points){
   for (int i = 0; i < points.size(); i++)
      {
         /* code */
         cout << points[i].x << ", " << points[i].y << "\n";
      }
}


/*
int main(int argc, char** argv) {
   cout << "Hello world" << "\n";
   vector<Point2D> points = generateRandomPoints(10);
   printPoints(points);
   cout << "++++++++++++++++++++\n";
   points = sortPoints(points, 'x');
   printPoints(points);
   cout << "++++++++++++++++++++\n";
   printPoints(points);
   Kdtree myTree(points);

}
*/