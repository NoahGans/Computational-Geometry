#ifndef __geom_h
#define __geom_h

#include <vector>

using namespace std; 

struct point2D {
    double x, y;
    bool operator==( const point2D& other) {
      return x == other.x && y == other.y;
    }
};

typedef struct _obstacle {
  vector<point2D> verticies;
} obstacle;

typedef struct _visGraph {
  vector< vector< int > > adjacencyList;
  vector<point2D> verticies;
} visGraph;

double distance(point2D a, point2D b);

void printPoint2D(point2D p);

void printObstacle(obstacle o);

void printVisibilityGraph(visGraph v);

int isVisible(point2D a, point2D b);

bool isValidPointInsert(point2D p, vector<obstacle> obstacles);

bool isValidAdditionToObstacle(obstacle o, vector<obstacle> obstacles, point2D s, point2D e);

visGraph constructVisGraph(vector<obstacle> obstacles, point2D start, point2D end);

#endif
