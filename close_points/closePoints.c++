
#include <iostream>
#include <vector>
#include <assert.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <math.h>
#include <stdlib.h> /* strtol */
#include <stdlib.h> /* atoi */
#include <chrono>
using namespace std::chrono;
using namespace std; //Needed for funtions

// Strut given to us by professor, Holds the information for a 2D point
typedef struct _point2d
{
    double x, y;
} point2d;

/*
calculateDist is a simple funtion that caculates the distance between two 
points in a 2d plane. The distance returned has not been rooted to save
on time.

@args: double x1, double y1, double x2, double y2
@return: distance
*/
double calculateDist(double x1, double y1, double x2, double y2)
{
    double x_dist = x1 - x2;
    double y_dist = y1 - y2;
    double dist;

    dist = pow(x_dist, 2) + pow(y_dist, 2); //calculating Euclidean distance

    return dist;
}

/*
makePoints creates a list of points of size n which is an argument to the program. 
It returns a full list of point2d *. Each point has an x and y range of range which
is also an argument.

@args: n number of points, range range of x and y
@return: points list of points 

*/
point2d *makePoints(int n, int range)
{
    
    //radom seed based on time for variability
    srand(time(NULL));
    //a 1d array of 2D points
    point2d *points;
    //alocate size of n
    points = new point2d[n];
    for (int i = 0; i < n; i++)
    {
        //grid[i] is a vector*, that is, an array (of vectors); we allocate it
        int x = rand() % range;
        int y = rand() % range;
        point2d p = {x, y};
        points[i] = p;
    }
    return points;
}




/*
bruteForce uses the brute force solve method to find the two closest points.
It simpy iterates through all the points made by makePoints and for each point
caculates the distance to every other point besides itself. If the distance
caculated is lower than the current best lowest distance (lowestDist), the 
varrible is updated along with the two points that compose of that distance.
lowestDist is initally set to an extreamly high value so it is immidatly 
replaced. The funtion is given an adress to fill and update with the two
points that compose the shortest distance

@args: point2d *points, int n, point2d *pointOneAdress, point2d *pointTwoAdress
@return: shortest distance

*/
double bruteForce(point2d *points, int n, point2d *pointOneAdress, point2d *pointTwoAdress)
{
    double lowestDist = 1000000000000000000; //high starting value of lowestDist

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                double dist = calculateDist(points[i].x, points[i].y, points[j].x, points[j].y);
                if (dist < lowestDist)
                {
                    lowestDist = dist;
                    *pointOneAdress = points[i];
                    *pointTwoAdress = points[j];
                }
            }
        }
    }
    return lowestDist;
}

/*
printPoint is used to print the final points. It was mostly used
for debugging. It simply prints the x and y cords of a point
in a neat manner. 

@args: point2d point: point to print

*/
void printPoint(point2d point)
{
    
    cout << point.x;
    cout << " ";
    cout << point.y;
    cout << "\n";
}

/*
layGrid creates a grid of size k. K is determined in gridedSolve. It is
the root of n/2. The grid is a 2d array of vectors. Vectors, although
i'm not super familiar, allow for easy dynamic adding of data. Each vector
representing each cell of the grid will hold the points withing that cell. 
The funtion simply builds the 2d array, alocates the memory for this, and 
then iterates through it filling it with vectors. This grid is returned.

@args: int k
@return: grid
*/

vector<point2d> **layGrid(int k)
{
    vector<point2d> **grid;
    grid = new vector<point2d> *[k];
    for (int i = 0; i < k; i++)
    {
        //grid[i] is a vector*, that is, an array (of vectors); we allocate it
        grid[i] = new vector<point2d>[k];
    }
    return grid;
}

/*
assignPointsToGrid places the points generated into their appropriate cells.
This is done through a constant caculation. cellLength is used as a dividend 
on the x and y values to compute a cell number. This point is then pushed
into the vector from the determined cell location. Two conditionals
catch if the cord is the range of the data. For example if range is 10, our 
cell length is 3.33, and our x value is 10, the found cell x cord is
3. This will segfault becase there is no 3 index in the grid. In this 
instance the x and y grid cords are reduced by one.

@args: int range, vector<point2d> **grid, point2d point, double cellLength


*/
void assignPointsToGrid(int range, vector<point2d> **grid, point2d point, double cellLength)
{
    //last point subject to exceding grid
    //printPoint(point);
    int xGridPos = point.x / cellLength;

    if (point.x == range)
    {
        xGridPos -= 1;
    }
    int yGridPos = point.y / cellLength;
    //last point subject to exceding grid
    if (point.y == range)
    {
        yGridPos -= 1;
    }
    grid[xGridPos][yGridPos].push_back(point);
}
/*
getBestWithinDist is the first funtion called when determining the shortest
distance for a cell. It only caculates the shortest distance within a single
cell. It funtions by running brute force for the points in a cell. It has 
the exact same logic as bruteForce. It has adresses for bestDist
and the points corospoding to the best distance. This allows global 
acsses to this information.

@param nt x, int y, vector<point2d> **grid, float k, double *bestDist, point2d *pointOneAdress, point2d *pointTwoAdress

*/
void getBestWithinDist(int x, int y, vector<point2d> **grid, float k, double *bestDist, point2d *pointOneAdress, point2d *pointTwoAdress)
{
    for (int i = 0; i < grid[x][y].size(); i++)
    {
        for (int j = 0; j < grid[x][y].size(); j++)
        {
            if (i != j)
            {
                double dist = calculateDist(grid[x][y][i].x, grid[x][y][i].y, grid[x][y][j].x, grid[x][y][j].y);
                if (dist < *bestDist)
                {
                    *bestDist = dist;
                    *pointOneAdress = grid[x][y][i];
                    *pointTwoAdress = grid[x][y][j];
                }
            }
        }
    }
}

/*
This function accounts for the other distance calculations for a given cell. 
Points could be very close together, but located just on the other side of a 
cell division. Thus, the distance between points within the observed cell to 
all other points in surrounding cells must be calculated. This is done by
iterating through the points in the observed cell, and for each point calculating 
the distance to each point in every other cell. Time can be saved by only 
looking at cells that are to the bottom right, middle right, top right, and
top of the observed cell. The other adjacent cells do not need to have distance
calculated to them because they will either be outside of the grid when the
observed cell is on the far left, or these distances will have already been
calculated in previous cell calculations. This is a small dynamic programming
time save. 

Some conditionals are inline codded becase it's esier for refrencing

@args int x, int y, vector<point2d> **grid, float k, double *bestDist, point2d *pointOneAdress, point2d *pointTwoAdress




*/
void getBestAdjacentDist(int x, int y, vector<point2d> **grid, float k, double *bestDist, point2d *pointOneAdress, point2d *pointTwoAdress)
{
    for (int i = 0; i < grid[x][y].size(); i++) //go through all points in observed cell
    {
        for (int j = x; j < x + 2; j++) //go through x points up to one greater(adjacent x cells)
        {
            if (j < k) //don't refrence outside of grid
            {
                for (int m = y - 1; m < y + 2; m++) //go through y points up to one greater(adjacent y cells)
                                                    //Starts with y-1 for bottom right cell
                {
                    if (!(j == x && m < y)) //excludes cell right bellow observed cell
                    {
                        if (m < k && m >= 0) //makes sure not refrenceing cell outside y bounds
                        {
                            if ((m != y) || (x != j)) //if cell is not observed cell

                            {
                                for (int l = 0; l < grid[j][m].size(); l++) //go through points in adjacent cell
                                {
                                    double dist = calculateDist(grid[x][y][i].x, grid[x][y][i].y, grid[j][m][l].x, grid[j][m][l].y);
                                    if (dist < *bestDist)
                                    {
                                        *bestDist = dist;
                                        *pointOneAdress = grid[x][y][i];
                                        *pointTwoAdress = grid[x][y][j];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
getGridBestDist gets the shortest distance from a cell. It first calculates
the best distance within the observed cell with getBestWithinDist. Then
it looks at potential points crossing over cell lines with getBestAdjacentDist.


@ args int x, int y, vector<point2d> **grid, float k, double *bestDist, point2d *pointOneAdress, point2d *pointTwoAdress

*/
void getGridBestDist(int x, int y, vector<point2d> **grid, float k, double *bestDist, point2d *pointOneAdress, point2d *pointTwoAdress)
{
    getBestWithinDist(x, y, grid, k, bestDist, pointOneAdress, pointTwoAdress);
    getBestAdjacentDist(x, y, grid, k, bestDist, pointOneAdress, pointTwoAdress);
}

/*

gridedSolve solves the closest point problem using a grid method. It places all
the points into grids, and then finds the distance of points only within each
grid and adjacent ones. This spatially limits the problem to running in n time.
Having spatial problems allows for spatial messures, a grid, to be be used to 
reduce the problem size. In an ideal world there would be two points in each 
grid that way there would be constant time for each grid. So, in order to have
2 points in each cell we need cells = n/2. Cells is the square of k, so 
k = root(n/2). After this, the points need to be bucketed. The cell point cordinate
thresholds are determined by dividning the range by k. This gives the length
of each cell on our cordinate plane. After the points are placed into their cells
the grid is itrated through and the best distance is caculated for each one.
At the end the best distance overall is reuturned, and the pointAdresses will
be filled.


@ args int x, int y, vector<point2d> **grid, float k, double *bestDist, point2d *pointOneAdress, point2d *pointTwoAdress

*/

double gridedSolve(point2d *points, int n, int range, point2d *pointOneAdress, point2d *pointTwoAdress)
{

    float k = lround(sqrt(n / 2));

    float cellLength = range / k;

    vector<point2d> **grid = layGrid(k);
    for (int i = 0; i < n; i++)
    {
        assignPointsToGrid(range, grid, points[i], cellLength);
    }

    double bestDist = 1000000000000; // Best dist is set super high so it's replaced

    for (int x = 0; x < k; x++)
    {
        for (int y = 0; y < k; y++)
        {
            getGridBestDist(x, y, grid, k, &bestDist, pointOneAdress, pointTwoAdress);
        }
    }
    delete[] grid; //clear grid memeory
    return bestDist;
}

/*
Main runs all the funtions makes the points runs the two
solve methods, and produces the results. 


*/
int main(int argc, char **argv)
{

    int n = atoi(argv[1]);

    int range = atoi(argv[2]);
    point2d *points = makePoints(n, range);

    point2d brutePointOne;
    point2d brutePointTwo;
    point2d gridPointOne;
    point2d gridPointTwo;
    auto start = high_resolution_clock::now();
    double gridDist = gridedSolve(points, n, range, &gridPointOne, &gridPointTwo);
    auto stop = high_resolution_clock::now();
    auto gridDuration = duration_cast<microseconds>(stop - start);
    auto startForce = high_resolution_clock::now();
    double forceDist = bruteForce(points, n, &brutePointOne, &brutePointTwo);
    auto stopForce = high_resolution_clock::now();
    auto forceDuration = duration_cast<microseconds>(stopForce - startForce);

    delete[] points; //clear points space
    cout << "n = " << n << "\n" << "Range = " << range << "\n" << "\n";
    cout << "Brute Force Approch" << "\n" << "Distance: " << forceDist << "\n";
    cout << "Point One: ";
    printPoint(brutePointOne);
    cout << "Point Two: ";
    printPoint(brutePointTwo);
    cout << "Brute Time: " << forceDuration.count() << " Microseconds" << "\n";
    cout << "\n";
    cout << "Grid Approch" << "\n" << "Distance: " << gridDist << "\n";
    cout << "Point One: ";
    printPoint(gridPointOne);
    cout << "Point Two: ";
    printPoint(gridPointTwo);
    cout << "Grid Time: " << gridDuration.count() << " Microseconds" << "\n";

}